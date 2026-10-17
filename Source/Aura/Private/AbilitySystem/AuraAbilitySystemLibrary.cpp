// NP Game Developer


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Algo/RandomShuffle.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "Subsystem/AuraGameInstanceSubsystem.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHUD = Cast<AAuraHUD>(PC->GetHUD());
		if (OutAuraHUD)
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet = AS;
			return true;
		}
	}
	
	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* ClassInfo= GetCharacterClassInfo(WorldContextObject);
	if (ClassInfo == nullptr) return;
	
	// Get character class info data asset
	FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

	// Use for source object in gameplay effect context handle
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	// Apply primary attribute gameplay effect to Ability System Component
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultInfo.PrimaryAttributes,
		Level,
		UAuraAbilitySystemLibrary::MakeEffectContextHandle(ASC, AvatarActor)
	);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	// Apply secondary attribute gameplay effect to Ability System Component
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(
		ClassInfo->SecondaryAttributes,
		Level,
		UAuraAbilitySystemLibrary::MakeEffectContextHandle(ASC, AvatarActor)
	);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	// Apply vital attribute gameplay effect to Ability System Component
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(
		ClassInfo->VitalAttributes,
		Level,
		UAuraAbilitySystemLibrary::MakeEffectContextHandle(ASC, AvatarActor)
	);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* ClassInfo= GetCharacterClassInfo(WorldContextObject);
	if (ClassInfo == nullptr) return;
	
	for (auto Ability : ClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	
	const FCharacterClassDefaultInfo& ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : ClassDefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(
				AbilityClass, 
				ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor())
			);
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContextObject)
	);
	
	if (AuraGameMode == nullptr) return nullptr;

	return AuraGameMode->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContextObject)	
	);
	
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	const UAuraGameInstanceSubsystem* SubSystem = GameInstance->GetSubsystem<UAuraGameInstanceSubsystem>();
	
	// if game mode exist, only on server side
	if (AuraGameMode)
	{
		return AuraGameMode->AbilityInfo;
	}
	
	// If on client side, we use UAuraGameInstanceSubsystem to access AbilityInfo
	return SubSystem->AbilityInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (AuraEffectContext->GetDamageType().IsValid())
		{
			return *AuraEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

float UAuraAbilitySystemLibrary::GetKnockForceMagnitude(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetKnockbackForceMagnitude();
	}
	return 0.f;
}

FVector UAuraAbilitySystemLibrary::GetKnockForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlocked)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bIsBlocked);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsCritical)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bIsCritical);
	}
}

void UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsSuccessfulDebuff)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffDamage(InDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffDuration(InDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDamageType(MakeShared<FGameplayTag>(InDamageType));
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InDeathImpulse)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetKnockbackForceMagnitude(FGameplayEffectContextHandle& EffectContextHandle,
	const float InKnockbackForceMagnitude)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetKnockbackForceMagnitude(InKnockbackForceMagnitude);
	}
}

void UAuraAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InKnockbackForce)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetKnockbackForce(InKnockbackForce);
	}
}

bool UAuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetIsRadialDamage();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UAuraAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsRadialDamage)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InInnerRadius)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InOuterRadius)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	FVector InRadialDamageOrigin)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOrigin(InRadialDamageOrigin);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const float Radius,
                                                           const FVector& SphereOrigin)
{
	/*
	 * Create a collision query params
	 */
	FCollisionQueryParams SphereQueryParams;
	SphereQueryParams.AddIgnoredActors(ActorsToIgnore);
	
	/*
	 * Create a sphere collision radius and find all overlapped actors
	 * then add them to output actors
	 */
	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// Find all overlapped actors
		World->OverlapMultiByObjectType(
			Overlaps, SphereOrigin, FQuat::Identity, 
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), 
			FCollisionShape::MakeSphere(Radius),
			SphereQueryParams
		);
		
		/*
		 * Only add the actors that has ICombatInterface implemented and
		 * the actor is alive
		 */
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && 
				!ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(
					ICombatInterface::Execute_GetAvatar(Overlap.GetActor())
				);
			}
			
		}
	}
}

void UAuraAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors,
	TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.IsEmpty()) return;
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}
	
	// Make a copy
	TArray<AActor*> ActorsToCheck = Actors;
	
	// Array sort comparison: https://www.google.com/search?q=unreal+array+sort+heap+sort+stable+sort&sxsrf=APpeQnvYHIaahVE6B14DYjhnIfHYHMGLWg%3A1785213276104
	// Sort actors by distance 
	ActorsToCheck.HeapSort(
		[Origin](AActor& A, AActor& B)
		{
			const double ADistance = (A.GetActorLocation() - Origin).Length();
			const double BDistance = (B.GetActorLocation() - Origin).Length();
			return ADistance < BDistance;
		}
	);
	
	// Number of actors in array to select
	int32 Size = FMath::Min(ActorsToCheck.Num(), MaxTargets);
	
	// Append those selected actors
	OutClosestTargets.Append(ActorsToCheck.GetData(), Size);
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool Friends = (FirstActor->ActorHasTag("Player") && SecondActor->ActorHasTag("Player")) || 
		(FirstActor->ActorHasTag("Enemy") && SecondActor->ActorHasTag("Enemy"));
	return !Friends;
}

void UAuraAbilitySystemLibrary::RandomPointsInRadiusOnGround(const UObject* WorldContextObject,
	TArray<FVector>& OutPoints, TArray<AActor*>& ActorToIgnore, int32 NumPoints, const float Radius,
	const FVector& SphereOrigin, const float RadiusForPoint, float TraceHalfSize, bool bRandomIfSinglePoint, bool bShuffle)
{
	OutPoints.Empty();
	
	/*
	 * Find world
	 */
	UWorld* World = GEngine->GetWorldFromContextObject(
		WorldContextObject,
		EGetWorldErrorMode::LogAndReturnNull
	);
	if (World == nullptr) return;
	
	// Make all character within radius to be ignored
	GetLivePlayersWithinRadius(
		WorldContextObject, 
		ActorToIgnore, 
		TArray<AActor*>(), 
		Radius, 
		SphereOrigin
	);
	
	/*
	 * Use Algorithm to generate number of points
	 */
	TArray<FVector2D> AcceptedPoints;
	for (int32 i = 0; i < NumPoints; i++)
	{
		FVector2D Point = MitchellBestCandidateAlgo(Radius, RadiusForPoint, AcceptedPoints);
		AcceptedPoints.Add(Point);
	}
	
	for (int32 i = 0; i < NumPoints; i++)
	{
		// Get the point
		FVector2D Point2D = AcceptedPoints[i];
		
		// Find out the location of point from center point
		FVector RandomPoint = SphereOrigin + FVector(Point2D, SphereOrigin.Z);
		
		/*
		 * Set random XY to sphere origin's XY if do not do random when there is
		 * only 1 point
		 */
		if (!bRandomIfSinglePoint && NumPoints == 1)
		{
			RandomPoint.X = SphereOrigin.X;
			RandomPoint.Y = SphereOrigin.Y;
		}
		
		/*
		 * Prepare parameters for tracing
		 */
		FVector TraceStart(RandomPoint.X, RandomPoint.Y, RandomPoint.Z + TraceHalfSize);
		FVector TraceEnd(RandomPoint.X, RandomPoint.Y, RandomPoint.Z - TraceHalfSize);
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(ActorToIgnore);
		
		// Do tracing
		World->LineTraceSingleByProfile(
			HitResult, 
			TraceStart, 
			TraceEnd,
			FName("BlockAll"),
			CollisionQueryParams
		);
		
		// If it hit
		if (HitResult.bBlockingHit)
		{
			RandomPoint.Z = HitResult.ImpactPoint.Z;
			OutPoints.Add(RandomPoint);
		}
	}
	
	if (bShuffle)
	{
		Algo::RandomShuffle(OutPoints);
	}
}

FVector2D UAuraAbilitySystemLibrary::RandomPointInRadius(float Radius)
{
	/*
	 * Reference:
	 * https://www.google.com/search?q=algorithm+for+random+locations+in+circle+radius+evenlly&ie=UTF-8
	 */
	float V1 = FMath::RandRange(0.f, 1.f);
	float V2 = FMath::RandRange(0.f, 1.f);
	float R = Radius * FMath::Sqrt(V1);
	float Theta = FMath::DegreesToRadians(2 * 180.f * V2);
	
	return FVector2D (R * FMath::Cos(Theta), R * FMath::Sin(Theta));
}

FVector2D UAuraAbilitySystemLibrary::MitchellBestCandidateAlgo(float MaxRadius, float MinDistBetween2Points,
                                                               const TArray<FVector2D>& AcceptedPointsHistory, int32 MaxAttempts)
{
	// Best distance found
	float BestDist = 0.f;
	
	// Bool to indicate the best candidate is found or not
	bool bBestCandidateFound = false;
	
	// Best candidate point
	FVector2D BestCandidate = RandomPointInRadius(MaxRadius);
	
	// For each attempt / candidate
	for (int32 i = 0; i < MaxAttempts; i++)
	{
		// Potential candidate
		FVector2D Candidate = RandomPointInRadius(MaxRadius);
		
		// Return candidate if no considered points
		if (AcceptedPointsHistory.Num() == 0)
		{
			return Candidate;
		}
		
		/*
		 * Find the distance to the closest existing point
		 */
		float MinDistToExisting = FLT_MAX;
		for (const FVector2D& AcceptedPoint : AcceptedPointsHistory)
		{
			float Dist = (Candidate - AcceptedPoint).Length();
			if (Dist < MinDistToExisting)
			{
				MinDistToExisting = Dist;
			}
		}
		
		/*
		 * Check if this candidate is valid and better than previous candidates
		 */
		if (MinDistToExisting >= MinDistBetween2Points)
		{
			if (MinDistToExisting > BestDist)
			{
				// Stored the closest distance for next iteration
				BestDist = MinDistToExisting;
				bBestCandidateFound = true;
				BestCandidate = Candidate;
			}
		}
	}
	
	// Return best candidate if found
	if (bBestCandidateFound)
	{
		return BestCandidate;
	}
	
	/*
	 * At this point no best candidate found so 
	 * BestCandidate is random generated point
	 */
	return BestCandidate;
}


FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams DamageEffectParams)
{
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	// Make an effect context for the gameplay effect
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	
	/*
	 * Set knockback and death impulse
	 */
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);
	
	/*
	 * Set radial damage
	 */
	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);
	
	/*
	 * Make a gameplay effect
	 */
	FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
		DamageEffectParams.DamageGameplayEffectClass,
		DamageEffectParams.AbilityLevel,
		EffectContextHandle
	);
	
	/*
	 * Add debuff related values to the gameplay effect
	 * for later to be retrieved
	 */
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle,
		DamageEffectParams.DamageType,
		DamageEffectParams.BaseDamage
	);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle,
		Debuff_Chance,
		DamageEffectParams.DebuffChance
	);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle,
		Debuff_Damage,
		DamageEffectParams.DebuffDamage
	);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle,
		Debuff_Duration,
		DamageEffectParams.DebuffDuration
	);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle,
		Debuff_Frequency,
		DamageEffectParams.DebuffFrequency
	);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	
	return EffectContextHandle;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpaceRotators(const FVector& ForwardVector, const FVector& Axis, const float Spread, const int32 NumRotators)
{
	TArray<FRotator> Rotators;
	const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-Spread / 2.f, FVector::UpVector);
	
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(ForwardVector.Rotation());
	}
	
	return Rotators;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlyRotateVectors(const FVector& ForwardVector, const FVector& Axis, const float Spread, const int32 NumVectors)
{
	TArray<FVector> Vectors;
	const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-Spread / 2.f, FVector::UpVector);
	
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(ForwardVector);
	}
	
	return Vectors;
}

int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
                                                             ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;
	
	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);
	
	return static_cast<int32>(XPReward);
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::MakeEffectContextHandle(UAbilitySystemComponent* ASC,
                                                                                const UObject* SourceObject)
{
	check(ASC);
	
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	if (SourceObject != nullptr)
	{
		ContextHandle.AddSourceObject(SourceObject);
	}
	return ContextHandle;
}


