// NP Game Developer


#include "AbilitySystem/AbilityTasks/LocalSpellDecalUnderMouse.h"

#include "Actors/Decal/SpellDecal.h"
#include "Aura/Aura.h"
#include "Player/AuraPlayerController.h"

ULocalSpellDecalUnderMouse* ULocalSpellDecalUnderMouse::CreateLocalSpellDecalUnderMouse(UGameplayAbility* OwningAbility,
                                                                        TSubclassOf<ASpellDecal>SpellDecalClass,
                                                                        float InSpellDecalRadius,
                                                                        float InSpellDecalHeight)
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
