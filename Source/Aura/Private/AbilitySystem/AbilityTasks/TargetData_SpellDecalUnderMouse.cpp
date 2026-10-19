// NP Game Developer


#include "AbilitySystem/AbilityTasks/TargetData_SpellDecalUnderMouse.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actors/Decal/SpellDecal.h"
#include "Aura/Aura.h"
#include "Input/AuraInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"


UTargetData_SpellDecalUnderMouse* UTargetData_SpellDecalUnderMouse::CreateSpellDecalUnderMouse(
	UGameplayAbility* OwningAbility, TSubclassOf<ASpellDecal> SpellDecalClass, UInputAction* InConfirmAction,
	UInputAction* InCancelAction, float InSpellDecalRadius, float InSpellDecalHeight)
{
	// Create a task
	UTargetData_SpellDecalUnderMouse* MyTask = NewAbilityTask<UTargetData_SpellDecalUnderMouse>(OwningAbility);
	
	/*
	 * Set up the task
	 */
	if (OwningAbility->IsLocallyControlled())
	{
		// Create the decal from the given decal class 
		MyTask->DecalActor = OwningAbility->GetWorld()->SpawnActor<ASpellDecal>(SpellDecalClass);
		
		// Confirm action
		MyTask->ConfirmAction = InConfirmAction;
	
		// Cancel action
		MyTask->CancelAction = InCancelAction;
		
		// Get aura player controller
		if (APawn* Pawn = Cast<APawn>(OwningAbility->GetAvatarActorFromActorInfo()))
		{
			MyTask->AuraPlayerController = Cast<AAuraPlayerController>(Pawn->GetController());
		}
		
		// Make sure this task is ticking
		MyTask->bTickingTask = true;
	}
	
	// Decal radius
	MyTask->SpellDecalRadius = InSpellDecalRadius;
	
	// Decal height
	MyTask->SpellDecalHeight = InSpellDecalHeight;
	
	return MyTask;
}

void UTargetData_SpellDecalUnderMouse::BindInputActions()
{
	/*
	 * Bind confirm and cancel action and then
	 * store their handle id which can be used to unbind later
	 */
	if (UAuraInputComponent* AuraInputComponent = Cast<UAuraInputComponent>(AuraPlayerController->InputComponent))
	{
		if (AuraPlayerController.IsValid() && ConfirmAction != nullptr)
		{
			uint32 BindingHandle = AuraInputComponent->BindAction(
				ConfirmAction,
				ETriggerEvent::Started,
				this,
				&UTargetData_SpellDecalUnderMouse::OnConfirm
			).GetHandle();
			
			InputActionBindingHandles.Add(BindingHandle);
		}
		if (AuraPlayerController.IsValid() && CancelAction != nullptr)
		{
			uint32 BindingHandle = AuraInputComponent->BindAction(
				CancelAction,
				ETriggerEvent::Started,
				this,
				&UTargetData_SpellDecalUnderMouse::OnCancel
			).GetHandle();
			
			InputActionBindingHandles.Add(BindingHandle);
		}
	}
}

void UTargetData_SpellDecalUnderMouse::RemoveBindingInputActions()
{
	/*
	 * Unable confirm and cancel action
	 */
	if (UAuraInputComponent* AuraInputComponent = Cast<UAuraInputComponent>(AuraPlayerController->InputComponent))
	{
		for (uint32 BindingHandle : InputActionBindingHandles)
		{
			AuraInputComponent->RemoveActionBindingForHandle(BindingHandle);
		}
	}
}

void UTargetData_SpellDecalUnderMouse::Activate()
{
	Super::Activate();
	
	/*
	 * Client:
	 *	- Bind input actions
	 *	- Setup decal size
	 *	
	 * Server:
	 *	- Make sure we do not tick task
	 *	- Hide and destroy decal if necessary
	 *  - Wait for client to send target data
	 */
	
	if (IsLocallyControlled())
	{
		BindInputActions();
	
		/*
		 * Set spell decal size
		 */
		if (DecalActor)
		{
			FSpellDecalSize DSize = FSpellDecalSize(
				FVector2D(SpellDecalRadius, SpellDecalRadius),
				SpellDecalHeight
			);
		
			DecalActor->SetDecalSize(DSize);
		}
	} 
	else
	{
		bTickingTask = false;
		if (DecalActor)
		{
			DecalActor->SetHidden(true);
			DecalActor->Destroy();
			DecalActor = nullptr;
		}
		WaitForTargetHitData();
	}
}

void UTargetData_SpellDecalUnderMouse::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	/*
	 * Update decal position only on client 
	 */
	
	if (!IsLocallyControlled()) return;
	
	/*
	 * Find out impact location from cursor straight line trace
	 * then update decal's location
	 */
	if (DecalActor&& AuraPlayerController.IsValid())
	{
		FHitResult CursorHitResult;
		AuraPlayerController->GetHitResultUnderCursor(ECC_Decal, false, CursorHitResult);
		if (CursorHitResult.bBlockingHit)
		{
			DecalActor->SetActorLocation(CursorHitResult.ImpactPoint);
		}
	}
}

void UTargetData_SpellDecalUnderMouse::OnDestroy(bool bInOwnerFinished)
{
	/*
	 * Clean up
	 */
	
	if (DecalActor)
	{
		DecalActor->Destroy();
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void UTargetData_SpellDecalUnderMouse::GetMouseCursorHitResult(FHitResult& HitResult)
{
	/*
	 * Set no blocking hit if cancel action was pressed
	 */
	if (bIsCancelled)
	{
		HitResult.bBlockingHit = false;
		return;
	}
	
	/*
	 * Confirm action was pressed then do tracing
	 */
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	PC->GetHitResultUnderCursor(ECC_Target, false, HitResult);
}

void UTargetData_SpellDecalUnderMouse::OnTargetHitDataArrived(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	Super::OnTargetHitDataArrived(DataHandle, ActivationTag);
	
	if (ShouldBroadcastAbilityTaskDelegates() && DataHandle.IsValid(0))
	{
		// Convert to FGameplayAbilityTargetData_SingleTargetHit
		if (const FGameplayAbilityTargetData_SingleTargetHit* HitData = static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(DataHandle.Get(0)))
		{
			/*
			 * Use bBlockingHit to determine if client was confirm or cancel
			 * 
			 * 
			 * If there is blocking hit then confirm action was pressed on client and had hit result,
			 * otherwise cancel action was pressed on client.
			 * 
			 * Case where client pressed confirm action and had no hit result then we consider it as 
			 * cancel action pressed since there is no hit result.
			 */
			if (HitData->GetHitResult()->bBlockingHit)
			{
				OnConfirmPressed.Broadcast(DataHandle);
			}
			else
			{
				OnCancelPressed.Broadcast();
			}
		}
		
	}
}

void UTargetData_SpellDecalUnderMouse::OnConfirm(const FInputActionValue& InputActionValue)
{
	// Set to false mean player had confirm action pressed
	bIsCancelled = false;
	
	// Hide decal actor
	if (DecalActor)
	{
		DecalActor->SetHidden(true);
	}
	
	// Remove all binding input actions to prevent it been triggered again 
	RemoveBindingInputActions();
	
	// Send data to server
	FGameplayAbilityTargetDataHandle& DataHandle = SendMouseCursorData();
	
	// Broadcast delegate if necessary
	if (OnConfirmPressed.IsBound() && ShouldBroadcastAbilityTaskDelegates())
	{
		OnConfirmPressed.Broadcast(DataHandle);
	}
}

void UTargetData_SpellDecalUnderMouse::OnCancel(const FInputActionValue& InputActionValue)
{
	// Set to true mean player had cancel action pressed
	bIsCancelled = true;
	
	// Hide decal actor
	if (DecalActor)
	{
		DecalActor->SetHidden(true);
	}
	
	// Remove all binding input actions to prevent it been triggered again 
	RemoveBindingInputActions();
	
	// Send data to server even there is no hit result or blocking hit
	// Server will determine by itself
	SendMouseCursorData();
	
	// Broadcast delegate if necessary
	if (OnCancelPressed.IsBound() && ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancelPressed.Broadcast();
	}
}

