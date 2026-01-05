// NP Game Developer


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

/**
 * Damage static that
 * define captured attributes 
 */
struct AuraDamageStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	AuraDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
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
	RelevantAttributesToCapture.Add(DamageStatic().BlockChanceDef);
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
	FAggregatorEvaluateParameters EvaluateAggregatorParameters;
	EvaluateAggregatorParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateAggregatorParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	// Get damage from SetByCallerMagnitude
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	
	// Block chance
	float TargetBlockChance = 0.f;
	
	/*
	 * Capture block chance
	 */
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatic().BlockChanceDef,
		EvaluateAggregatorParameters,
		TargetBlockChance
	);
	TargetBlockChance = FMath::Max(TargetBlockChance, 0.f);
	
	/*
	 * Do calculation
	 */
	// Whether damage is blocked
	bool bBlocked = FMath::RandRange(0, 100) < TargetBlockChance;
	
	// Cut damage in half
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	/*
	 * Add evaluated data to the output modifier
	 */
	FGameplayModifierEvaluatedData EvaluateData(
		UAuraAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::AddFinal,
		Damage
	);
	OutExecutionOutput.AddOutputModifier(EvaluateData);
}
