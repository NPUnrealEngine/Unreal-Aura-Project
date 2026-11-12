// NP Game Developer


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Algo/RandomShuffle.h"
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
	const float SphereRadius, TSubclassOf<UInterface> HasInterfaceClass, float TraceExtend, bool DrawDebug, 
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
			if (HitResult.GetActor()->GetClass()->ImplementsInterface(HasInterfaceClass))
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

void UAuraBeamSpell::FindSecondaryTargets(const FVector CenterLocation, TArray<AActor*>& ActorToIgnored,
	TSubclassOf<UInterface> HasInterfaceClass, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, 
	TArray<AActor*>& OutFoundTargets, UClass* ActorClassToFilter, bool DrawDebug, float DrawDebugDuration)
{
	OutFoundTargets.Empty();
	
	// Get number of chainable targets
	int32 NumTargets = NumChainingTargets.AsInteger(GetAbilityLevel());
	
	// If no chainable targets then return
	if (NumTargets <= 0) return;
	
	// Get range/radius for finding secondary targets
	float Radius = SecondaryBeamRange.GetValueAtLevel(GetAbilityLevel()) * 100.f;
	
	// Add default value ECC_Pawn to ObjectTypes
	ObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	// Array for storing sphere trace results
	TArray<AActor*> FoundActors;
	
	// Add our owner character to be ignored during sphere tracing
	ActorToIgnored.AddUnique(OwnerCharacter);
	
	// Do Tracing
	bool bTargetFound = UKismetSystemLibrary::SphereOverlapActors(
		GetAvatarActorFromActorInfo(),
		CenterLocation,
		Radius,
		ObjectTypes,
		ActorClassToFilter,
		ActorToIgnored,
		FoundActors
	);
	
	if (DrawDebug)
	{
		UKismetSystemLibrary::DrawDebugCircle(
			GetAvatarActorFromActorInfo(),
			CenterLocation,
			Radius,
			20,
			FLinearColor::Yellow,
			DrawDebugDuration,
			2.f,
			FVector::YAxisVector,
			FVector::XAxisVector
		);
	}
	
	// If no object found then return
	if (!bTargetFound) return;
	
	/*
	 * Filter out all actors that isn't implemented specific interface to be considered as target,
	 * otherwise add actor to output targets
	 * 
	 */
	for (auto It = FoundActors.CreateIterator(); It; ++It)
	{
		if (!(*It)->GetClass()->ImplementsInterface(HasInterfaceClass))
		{
			It.RemoveCurrent();
		}
		else
		{
			OutFoundTargets.AddUnique(*It);
		}
	}
	
	/*
	 * Resize array to match number of chainable targets
	 */
	while (OutFoundTargets.Num() > NumTargets)
	{
		// Shuffle array
		Algo::RandomShuffle(OutFoundTargets);
		
		// Randomly pick element index to be removed
		int32 RandomIndex = FMath::RandRange(0, OutFoundTargets.Num() - 1);
		
		// RemoveAtSwap is much faster because it moves the last element 
		// into the gap instead of shifting all subsequent elements down.
		// Use TargetArray.RemoveAt(RandomIndex) if order matters.
		OutFoundTargets.RemoveAtSwap(RandomIndex);
	}
	
	// Draw debug if necessary
	if (DrawDebug)
	{
		for (AActor* Target : OutFoundTargets)
		{
			UKismetSystemLibrary::DrawDebugSphere(
				GetAvatarActorFromActorInfo(),
				Target->GetActorLocation(),
				50.f,
				12,
				FLinearColor::Green,
				DrawDebugDuration,
				2.f
			);
		}
	}
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
