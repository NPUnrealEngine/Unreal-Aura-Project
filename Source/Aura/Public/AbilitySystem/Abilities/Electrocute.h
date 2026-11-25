// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Electrocute | Stun")
	FScalableFloat StunDuration;

	/**
	 * Stun chance from 0 ~ 1
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Electrocute | Stun")
	FScalableFloat StunChance;
	
public:
	/**
	 * Get stun duration
	 * @param Level ability level
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	float GetStunDuration(const float Level);

	/**
	 * Get stun chance
	 * @param Level ability level
	 * @return value between 0 ~ 1
	 */
	UFUNCTION(BlueprintCallable)
	float GetStunChance(const float Level);

	/**
	 * Determine whether to apply stun or not
	 * 
	 * Generate a random number between 0 ~ 1 and see if number is under threshold
	 * @param CalculatedStunChance Stun chance to compare
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	bool CanApplyStun(const float CalculatedStunChance);
	
public: // Override
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
