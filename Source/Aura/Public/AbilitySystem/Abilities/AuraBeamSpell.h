// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

USTRUCT(BlueprintType)
struct FFoundBeamTarget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsBlockHit;
	
	UPROPERTY(BlueprintReadOnly)
	FVector HitLocation;
	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> Target;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);
	
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	/**
	 * Fire a sphere trace toward the location and see if a target hit.
	 * And then update mouse hit actor and mouse hit location
	 * @param BeamTargetLocation 
	 */
	UFUNCTION(BlueprintCallable)
	void TraceTarget(const FVector& BeamTargetLocation);
	
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);

	/**
	 * Find primary target by using sphere trace(sweep sphere along a line)
	 * 
	 * @param StartLocation Start location for tracing
	 * @param EndLocation End location for tracing
	 * @param SphereRadius Size of sphere
	 * @param ConformToInterface Found target must implement this interface to be considered as
	 * target
	 * @param TraceExtend Extend trace by amount, if 0 then tracing only from start to end location.
	 * Default to 10
	 * @param DrawDebug Show debug or not. It draws a circle to indicate max range then draw tracing
	 * only if hitting target is within max range
	 * @param DrawDebugDuration How long the drawing debug should be shown
	 * @return FFoundBeamTarget
	 */
	UFUNCTION(BlueprintCallable)
	FFoundBeamTarget FindPrimaryTarget(const FVector StartLocation, const FVector EndLocation, const float SphereRadius, TSubclassOf<UInterface> ConformToInterface, float TraceExtend = 10.f, bool DrawDebug = false, float DrawDebugDuration = 5.f);

	/**
	 * Remove in future
	 * 
	 * Find secondary/additional targets from a location
	 * 
	 * Cast a sphere with a radius to find actors that overlapped then
	 * filter out actors that didn't implement specific interface.
	 * 
	 * @param CenterLocation Center location of tracing
	 * @param ActorToIgnored Actor to be ignored
	 * @param ConformToInterface Found target must implement this interface to be considered as
	 * target
	 * @param ObjectTypes Array of type of object to query/trace. Default to ECC_Pawn
	 * @param OutFoundTargets Output result of found targets
	 * @param ActorClassToFilter If set, will only return results of this class or subclasses of it
	 * @param DrawDebug If need to draw debug or not
	 * @param DrawDebugDuration How long the drawing debug should be shown
	 */
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="ActorToIgnored, ObjectTypes", DeprecatedFunction, DeprecationMessage="Will be removed and replaced by ActorSphereDetector"))
	void FindSecondaryTargets(const FVector CenterLocation, UPARAM(ref) TArray<AActor*>& ActorToIgnored, TSubclassOf<UInterface> ConformToInterface, UPARAM(ref) TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, TArray<AActor*>& OutFoundTargets, UClass* ActorClassToFilter = nullptr, bool DrawDebug = false, float DrawDebugDuration = 5.f);
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	FVector MouseHitLocation;
	
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<AActor> MouseHitActor;
	
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category="Beam")
	int32 MaxNumShockTargets = 5;

	/**
	 * Beam range used to find primary target
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Beam|Range")
	FScalableFloat PrimaryBeamRange;
	
	/**
	 * Beam range used to find secondary/additional targets
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Beam|Range")
	FScalableFloat SecondaryBeamRange;

	/**
	 * Number of secondary/additional chaining targets for beam
	 */
	UPROPERTY(EditDefaultsOnly, Category="Beam|Chain")
	FScalableFloat NumChainingTargets;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
