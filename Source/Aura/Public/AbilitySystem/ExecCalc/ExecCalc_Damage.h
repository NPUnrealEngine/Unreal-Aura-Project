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
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                    const FGameplayEffectSpec& Spec,
	                    FAggregatorEvaluateParameters EvaluateAggregatorParameters) const;

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
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
