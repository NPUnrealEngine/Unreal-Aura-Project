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
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	FVector MouseHitLocation;
	
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<AActor> MouseHitActor;
	
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

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
	
protected: // Override
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
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
	 * Get number of chainable targets
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	int32 GetNumChainingTargets();
};
