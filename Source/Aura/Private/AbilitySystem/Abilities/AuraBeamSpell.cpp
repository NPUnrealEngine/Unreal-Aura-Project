// NP Game Developer


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Developer/DerivedDataCache/Public/DerivedDataCacheInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	}
	
}

void UAuraBeamSpell::TraceTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		USkeletalMeshComponent* WeaponComponent = ICombatInterface::Execute_GetWeapon(OwnerCharacter);
		const FVector SocketLocation = WeaponComponent->GetSocketLocation(FName("TipSocket"));
		
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingle(
			OwnerCharacter,
			SocketLocation,
			BeamTargetLocation,
			10.f,
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true
		);
		if (HitResult.bBlockingHit)
		{
			MouseHitLocation = HitResult.ImpactPoint;
			MouseHitActor = HitResult.GetActor();
		}
	}
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamSpell::PrimaryTargetDied);
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation()
	);
	
	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	//int32 NumAdditionalTargets = 5;
	
	UAuraAbilitySystemLibrary::GetClosestTargets(
		NumAdditionalTargets,
		OverlappingActors,
		OutAdditionalTargets,
		MouseHitActor->GetActorLocation()
	);

	for (AActor* Target : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamSpell::AdditionalTargetDied);
			}
		}
	}
}

FFoundBeamTarget UAuraBeamSpell::FindPrimaryTarget(const FVector StartLocation, const FVector EndLocation,
	const float SphereRadius, TSubclassOf<UInterface> TargetInterfaceClass, float TraceExtend, bool DrawDebug)
{
	check(OwnerCharacter);
	
	// Convert max beam range from meter to pixel
	float MaxBeamRange = PrimaryBeamRange.GetValueAtLevel(GetAbilityLevel()) * 100.f;
	
	// Determine target is in beam range
	float bInBeamRange = (EndLocation - StartLocation).Size() <= MaxBeamRange;
	
	// Draw debug circle with beam range
	if (DrawDebug){
		UKismetSystemLibrary::DrawDebugCircle(
			GetAvatarActorFromActorInfo(), 
			StartLocation, 
			MaxBeamRange, 
			20, 
			FLinearColor::Yellow, 
			5.f, 
			2.f, 
			FVector::YAxisVector, 
			FVector::XAxisVector
		);
	}
	
	if (OwnerCharacter->Implements<UCombatInterface>() && bInBeamRange)
	{
		/*
		 * Find new end location and do tracing
		 */
		
		// Direction with length
		FVector DirectionMagnitude = (EndLocation - StartLocation).GetSafeNormal() * ((EndLocation - StartLocation).Size() + TraceExtend);
		FVector NewEndLocation = StartLocation + DirectionMagnitude;
		
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingle(
			OwnerCharacter,
			StartLocation,
			NewEndLocation,
			SphereRadius,
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnore,
			DrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			HitResult,
			true
		);
		
		// If hit
		if (HitResult.bBlockingHit)
		{
			// Does hit actor implement specific interface to be considered as target
			if (HitResult.GetActor()->GetClass()->ImplementsInterface(TargetInterfaceClass))
			{
				return FFoundBeamTarget(
					HitResult.bBlockingHit, 
					HitResult.ImpactPoint, 
					HitResult.GetActor()
				);
			}
		}
	}
	
	return FFoundBeamTarget(
		false, 
		FVector::ZeroVector, 
		nullptr
	);
}

void UAuraBeamSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo)
	{
		OwnerPlayerController = ActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
