// NP Game Developer


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Algo/RandomShuffle.h"
#include "Developer/DerivedDataCache/Public/DerivedDataCacheInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

FFoundBeamTarget UAuraBeamSpell::FindPrimaryTarget(const FVector StartLocation, const FVector EndLocation,
	const float SphereRadius, TSubclassOf<UInterface> ConformToInterface, float TraceExtend, bool DrawDebug, 
	float DrawDebugDuration)
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
			DrawDebugDuration, 
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
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			DrawDebugDuration
		);
		
		// If hit
		if (HitResult.bBlockingHit)
		{
			// Does hit actor implement specific interface to be considered as target
			if (HitResult.GetActor()->GetClass()->ImplementsInterface(ConformToInterface))
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

int32 UAuraBeamSpell::GetNumChainingTargets()
{
	if (NumChainingTargets.IsValid())
	{
		return NumChainingTargets.AsInteger(GetAbilityLevel());
	}
	
	return 0;
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
