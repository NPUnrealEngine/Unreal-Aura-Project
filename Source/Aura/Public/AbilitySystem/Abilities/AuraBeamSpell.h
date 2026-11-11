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
	 * @param TargetInterfaceClass Found target must implement this interface to be considered as
	 * target
	 * @param TraceExtend Extend trace by amount, if 0 then tracing only from start to end location.
	 * Default to 10
	 * @param DrawDebug Show debug or not. It draws a circle to indicate max range then draw tracing
	 * only if hitting target is within max range
	 * @return FFoundBeamTarget
	 */
	UFUNCTION(BlueprintCallable)
	FFoundBeamTarget FindPrimaryTarget(const FVector StartLocation, const FVector EndLocation, const float SphereRadius, TSubclassOf<UInterface> TargetInterfaceClass, float TraceExtend = 10.f, bool DrawDebug = false);
	
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
	UPROPERTY(EditDefaultsOnly, Category="Beam|Range")
	FScalableFloat PrimaryBeamRange;
	
	/**
	 * Beam range used to find secondary/additional targets
	 */
	UPROPERTY(EditDefaultsOnly, Category="Beam|Range")
	FScalableFloat SecondaryBeamRange;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
