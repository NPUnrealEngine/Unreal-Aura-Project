// NP Game Developer


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interface/CombatInterface.h"

/**
 * Damage static that
 * define captured attributes 
 */
struct AuraDamageStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	AuraDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatic().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatic().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().ArmorPenetrationDef);
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
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	/*
	 * Get ICombatInterface for source and target
	 */
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	
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

	/*
	 * Capture block chance
	 */
	// Block chance
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatic().BlockChanceDef,
		EvaluateAggregatorParameters,
		TargetBlockChance
	);
	TargetBlockChance = FMath::Max(TargetBlockChance, 0.f);
	
	/*
	 * Capture armor penetration
	 */
	// Armor penetration
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatic().ArmorPenetrationDef,
		EvaluateAggregatorParameters,
		SourceArmorPenetration
	);
	SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.f);
	
	/*
	 * Capture armor
	 */
	// Armor
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatic().ArmorDef,
		EvaluateAggregatorParameters,
		TargetArmor
	);
	TargetArmor = FMath::Max(TargetArmor, 0.f);
	
	/*
	 * Do calculation
	 */
	// Whether damage is blocked
	bool bBlocked = FMath::RandRange(0, 100) < TargetBlockChance;
	
	// Cut damage in half if blocked damage
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	// Get curve table for ArmorPenetrationCoefficient
	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(
		FName("ArmorPenetration"), 
		FString()
	);
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(
		SourceCombatInterface->GetPlayerLevel()
	);
	
	// Calculate damage for armor penetration
	const float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	
	// Get curve table for EffectiveArmorCoefficient
	FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(
		FName("EffectiveArmor"), 
		FString()
	);
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(
		TargetCombatInterface->GetPlayerLevel()	
	);
	
	// Calculate damage
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
	
	/*
	 * Add modifier evaluated data to the output modifier
	 */
	FGameplayModifierEvaluatedData EvaluateData(
		UAuraAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::AddBase,
		Damage
	);
	OutExecutionOutput.AddOutputModifier(EvaluateData);
}
