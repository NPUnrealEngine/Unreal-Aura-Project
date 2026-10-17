// NP Game Developer


#include "AbilitySystem/AbilityTasks/LocalSpellDecalUnderMouse.h"

#include "Actors/Decal/SpellDecal.h"
#include "Aura/Aura.h"
#include "Input/AuraInputComponent.h"
#include "Player/AuraPlayerController.h"

ULocalSpellDecalUnderMouse* ULocalSpellDecalUnderMouse::CreateLocalSpellDecalUnderMouse(UGameplayAbility* OwningAbility,
                                                                        TSubclassOf<ASpellDecal>SpellDecalClass,
                                                                        float InSpellDecalRadius,
                                                                        float InSpellDecalHeight,
                                                                        UInputAction* InConfirmAction, 
                                                                        UInputAction* InCancelAction,
                                                                        bool bInEndTaskOnConfirmOrCancelPressed)
{
	// Create a task
	ULocalSpellDecalUnderMouse* MyTask = NewAbilityTask<ULocalSpellDecalUnderMouse>(OwningAbility);
	
	/*
	 * Set up the task
	 */
	
	// Create the decal from the given decal class 
	MyTask->DecalActor = OwningAbility->GetWorld()->SpawnActor<ASpellDecal>(SpellDecalClass);
	
	// Decal radius
	MyTask->SpellDecalRadius = InSpellDecalRadius;
	
	// Decal height
	MyTask->SpellDecalHeight = InSpellDecalHeight;
	
	MyTask->ConfirmAction = InConfirmAction;
	MyTask->CancelAction = InCancelAction;
	
	MyTask->bEndTaskOnConfirmOrCancelPressed = bInEndTaskOnConfirmOrCancelPressed;
	
	// Get aura player controller
	if (APawn* Pawn = Cast<APawn>(OwningAbility->GetAvatarActorFromActorInfo()))
	{
		MyTask->AuraPlayerController = Cast<AAuraPlayerController>(Pawn->GetController());
	}
	
	// Make sure this task is ticking
	MyTask->bTickingTask = true;
	
	return MyTask;
}

void ULocalSpellDecalUnderMouse::Activate()
{
	Super::Activate();
	
	/*
	 * Bind confirm and cancel action
	 */
	if (UAuraInputComponent* AuraInputComponent = Cast<UAuraInputComponent>(AuraPlayerController->InputComponent))
	{
		if (AuraPlayerController.IsValid() && ConfirmAction != nullptr)
		{
			AuraInputComponent->BindAction(
				ConfirmAction,
				ETriggerEvent::Triggered,
				this,
				&ULocalSpellDecalUnderMouse::OnConfirm
			);
		}
		if (AuraPlayerController.IsValid() && CancelAction != nullptr)
		{
			AuraInputComponent->BindAction(
				CancelAction,
				ETriggerEvent::Triggered,
				this,
				&ULocalSpellDecalUnderMouse::OnCancel
			);
		}
	}
	
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

void ULocalSpellDecalUnderMouse::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
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

void ULocalSpellDecalUnderMouse::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	
	if (DecalActor)
	{
		DecalActor->Destroy();
	}
}

void ULocalSpellDecalUnderMouse::OnConfirm(const FInputActionValue& InputActionValue)
{
	if (OnConfirmPressed.IsBound())
	{
		OnConfirmPressed.Broadcast();
	}
	if (bEndTaskOnConfirmOrCancelPressed)
	{
		EndTask();
	}
}

void ULocalSpellDecalUnderMouse::OnCancel(const FInputActionValue& InputActionValue)
{
	if (OnCancelPressed.IsBound())
	{
		OnCancelPressed.Broadcast();
	}
	if (bEndTaskOnConfirmOrCancelPressed)
	{
		EndTask();
	}
}
