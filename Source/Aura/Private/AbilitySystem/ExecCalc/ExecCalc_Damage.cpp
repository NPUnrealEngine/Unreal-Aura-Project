// NP Game Developer


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

/**
 * Damage static that
 * define captured attributes 
 */
struct AuraDamageStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	
	AuraDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
	}
};

/**
 * Get Damage static 
 * @return AuraDamageStatic singleton instance 
 */
static const AuraDamageStatic& DamageStatic()
{
	static AuraDamageStatic DStatic;
	return DStatic;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatic().ArmorDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	/*
	 * Get Ability System Component for source and target
	 */
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	/*
	 * Get Avatar actor for source and target
	 */
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	/*
	 * Create FAggregatorEvaluateParameters and
	 * set its Source and Target Tags
	 */
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	// Armor value we want to calculate on
	float Armor = 0.f;
	
	// Attempting to calculate captured attribute value
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatic().ArmorDef,
		EvaluateParameters,
		Armor
	);
	
	/*
	 * Do calculation
	 */
	Armor = FMath::Max(0.f, Armor);
	++Armor;
	
	/*
	 * Add evaluated data to the output modifier
	 */
	FGameplayModifierEvaluatedData EvaluateData(
		DamageStatic().ArmorProperty,
		EGameplayModOp::AddFinal,
		Armor
	);
	OutExecutionOutput.AddOutputModifier(EvaluateData);
}
