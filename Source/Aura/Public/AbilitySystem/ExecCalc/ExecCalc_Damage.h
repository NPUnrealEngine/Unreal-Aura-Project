// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * Damage execution calculation
 * 
 * https://github.com/tranek/GASDocumentation?tab=readme-ov-file#4512-gameplay-effect-execution-calculation
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UExecCalc_Damage();
	
public:
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                    const FGameplayEffectSpec& Spec,
	                    FAggregatorEvaluateParameters EvaluateAggregatorParameters) const;

public: // Override
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

public: // Helper functions
	/**
	 * Reference to InternalTakeRadialDamage function in AActor class in UnrealEngine source code
	 * 
	 * Calculate radial damage with damage falloff
	 * @param ClosestDistanceFromCenter  Distance from hit point/target location to radial 
	 * center
	 * @param InnerRadius Inner radial radius 
	 * @param OuterRadius Outer radial radius
	 * @param BaseDamage Base damage
	 * @param MinimumDamage Minimum damage default to 0
	 * @param DamageFalloff Used to calculate exponential damage falloff. 
	 * A value of 1.0 is linear, while 2.0 creates a squared/exponential curve 
	 * @return Damage value
	 */
	static float CalculateRadialDamage(float ClosestDistanceFromCenter, float InnerRadius, float OuterRadius, float BaseDamage, float MinimumDamage = 0.f, float DamageFalloff = 1.f);

	/**
	 * Reference to GetDamageScale function in FRadialDamageEvent in UnrealEngin source code
	 * 
	 * Get radial damage scale
	 * @param DistanceFromCenter Distance from hit point/target location to radial center
	 * @param InnerRadius Inner radial radius 
	 * @param OuterRadius Outer radial radius
	 * @param DamageFalloff Used to calculate exponential damage falloff. 
	 * A value of 1.0 is linear, while 2.0 creates a squared/exponential curve
	 * @return scaled value from 0 ~ 1
	 */
	static float GetRadialDamageScale(float DistanceFromCenter, float InnerRadius, float OuterRadius, float DamageFalloff);
	
protected:
	/**
	 * Apply an instant GameplayEffect which change an attribute's value by magnitude
	 * @param ASC AbilitySystemComponent this effect will be applied to
	 * @param Magnitude Value to change to an attribute
	 * @param Attribute The attribute whose value will be changed
	 * @param EffectName Name of this GameplayEffect
	 */
	void ApplyInstantDynamicEffect(UAbilitySystemComponent* ASC, float Magnitude, FGameplayAttribute Attribute, FString EffectName) const;
};
