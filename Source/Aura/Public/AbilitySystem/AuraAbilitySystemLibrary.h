// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilityInfo;
class AAuraHUD;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	 * UI
	 */
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(blueprintPure, Category="AuraAbilitySystemLibrary | WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	/*
	 * Character start up ability
	 */
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Character Class Defaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Character Class Defaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Character Class Defaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Ability info")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	/*
	 * Critical and blocked hit 
	 */
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlocked);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCritical);
	
	/*
	 * Knock back
	 */
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static float GetKnockForceMagnitude(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static FVector GetKnockForce(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetKnockbackForceMagnitude(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InKnockbackForceMagnitude);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce);
	
	/*
	 * Radial damage
	 */
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius);	
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FVector InRadialDamageOrigin);
	
	/*
	 * Debuff
	 */
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDamage);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDuration);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InFrequency);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetDebuffDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);
	
	/*
	 * Gameplay mechanic
	 */
	
	/**
	 * Get all actors who is alive within a sphere radius
	 * 
	 * @param WorldContextObject 
	 * @param OutOverlappingActors 
	 * @param ActorsToIgnore 
	 * @param Radius 
	 * @param SphereOrigin 
	 */
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Machanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Machanics")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Machanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	/**
	 * Find random points in a defined radius then adjust those points' 
	 * Z to ground
	 * 
	 * @param WorldContextObject 
	 * @param OutPoints Output points
	 * @param ActorToIgnore Actor that need to be ignored while tracing to find ground 
	 * @param NumPoints Number of points to generate
	 * @param Radius Max radius for points to be generated within
	 * @param SphereOrigin Origin point / center of radius
	 * @param RadiusForPoint Radius for each point, it will be used for distribute points evenly if
	 * max radius is big enough to contain all points otherwise points may overlap or too
	 * close to each other
	 * @param TraceHalfSize Half of tracing length for find ground
	 * @param bRandomIfSinglePoint If true then NumPoints will be considered and point is at
	 * center of radius when NumPoints=1, false then 
	 * NumPoints will not be considered. Default to false
	 * @param bShuffle Shuffle final points order in Array
	 */
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Machanics")
	static void RandomPointsInRadiusOnGround(const UObject* WorldContextObject, TArray<FVector>& OutPoints, TArray<AActor*>& ActorToIgnore, int32 NumPoints, const float Radius, const FVector& SphereOrigin, const float RadiusForPoint=50.f, float TraceHalfSize=100.f, bool bRandomIfSinglePoint = false, bool bShuffle = false);

	/**
	 * Generate a 2D point in a radius
	 * 
	 * Using Polar Coordinates with Area Correction method
	 * 
	 * Reference: https://www.google.com/search?q=algorithm+for+random+locations+in+circle+radius+evenlly&ie=UTF-8
	 * 
	 * @param Radius Boundary of radius
	 * @return Vector2D
	 */
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Machanics")
	static FVector2D RandomPointInRadius(float Radius);

	/**
	 * Algorithm for finding the best point in a radius with number of points to be considered
	 * 
	 * To generate number of points with evenly distributed using this function:
	 * Store each returned point in an array then use array as AcceptedPointsHistory to find number
	 * of points that are evenly distributed in the max radius.
	 * 
	 * Reference: https://www.google.com/search?q=Polar+Coordinates+with+Area+Correction+random+points+without+overlapping+&sxsrf=APpeQnvk7Aru9flOaJOP3rD5g2DNCSGt2Q%3A1792216232185
	 * 
	 * Implemented with Mitchell's Best-Candidate Algorithm:
	 * For each new point, generate a fixed number of "candidate" points (e.g., 10 or 20). 
	 * Pick the candidate that is furthest from all existing points. This creates a visually 
	 * pleasing, tightly packed distribution.
	 * 
	 * @param MaxRadius Boundary of radius
	 * @param MinDistBetween2Points Used to find best point evenly distributed 
	 * @param AcceptedPointsHistory Array of points to be considered for finding the best point, if
	 * this parameter contain no points then a random point is returned
	 * @param MaxAttempts How many attempts / candidates to find the best point. 
	 * Warning: higher value cause more calculations to be done may result in freeze. Default to 10
	 * @return Vector2D point
	 */
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Machanics")
	static FVector2D MitchellBestCandidateAlgo(float MaxRadius, float MinDistBetween2Points, const TArray<FVector2D>& AcceptedPointsHistory, int32 MaxAttempts=10);

	/**
	 * Take FDamageEffectParams as parameters then use source Ability System component 
	 * create an FGameplayEffectSpecHandle and FGameplayEffectContextHandle, 
	 * set properties on them then apply gameplay effect
	 * (FGameplayEffectSpecHandle) to target Ability System component
	 * @param DamageEffectParams 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Damage Effect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams DamageEffectParams);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Machanics")
	static TArray<FRotator> EvenlySpaceRotators(const FVector& ForwardVector, const FVector& Axis, const float Spread, const int32 NumRotators);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Machanics")
	static TArray<FVector> EvenlyRotateVectors(const FVector& ForwardVector, const FVector& Axis, const float Spread, const int32 NumVectors);
	
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
	
private:
	/**
	 * Create a GameplayEffectContextHandle with SourceObject
	 * @param ASC Ability System Component
	 * @param SourceObject Optional.
	 * The object that this effect created from
	 * @return FGameplayEffectContextHandle
	 */
	static FGameplayEffectContextHandle MakeEffectContextHandle(UAbilitySystemComponent* ASC, const UObject* SourceObject = nullptr);
};
