// NP Game Developer


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
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
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	
	AuraDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatic().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitDamageDef);
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
	
	// Initial damage value
	float Damage = 0.f;
	
	/*
	 * Sum all value from each damage type
	 * 
	 * Iterate over all child gameplay tags(represent damage type) under Damage gameplay tag
	 * and use GetSetByCallerMagnitude to get damage value for the damage type then add it
	 * to damage
	 */
	FGameplayTagContainer GameplayTagContainer = UGameplayTagsManager::Get().RequestGameplayTagChildren(
		FAuraGameplayTags::Get().Damage
	);
	for (auto DamageTypeTag : GameplayTagContainer)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);
		Damage += DamageTypeValue;
	}

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
	 * Capture critical hit chance
	 */
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatic().CriticalHitChanceDef,
		EvaluateAggregatorParameters,
		SourceCriticalHitChance
	);
	SourceCriticalHitChance = FMath::Max(SourceCriticalHitChance, 0.f);
	
	/*
	 * Capture critical hit resistance
	 */
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatic().CriticalHitResistanceDef,
		EvaluateAggregatorParameters,
		TargetCriticalHitResistance
	);
	TargetCriticalHitResistance = FMath::Max(TargetCriticalHitResistance, 0.f);
	
	/*
	 * Capture critical hit damage
	 */
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatic().CriticalHitDamageDef,
		EvaluateAggregatorParameters,
		SourceCriticalHitDamage
	);
	SourceCriticalHitDamage = FMath::Max(SourceCriticalHitDamage, 0.f);
	
	/*
	 * Do calculation
	 */
	// Whether damage is blocked
	bool bBlocked = FMath::RandRange(0, 100) < TargetBlockChance;
	
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(ContextHandle, bBlocked);
	
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
	
	// Get curve table for CriticalHitResistanceCoefficient
	FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(
		FName("CriticalHitResistance"), 
		FString()
	);
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(
		TargetCombatInterface->GetPlayerLevel()	
	);
	
	// Calculate critical hit chance
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	
	// RNG to check if critical hit
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	UAuraAbilitySystemLibrary::SetIsCriticalHit(ContextHandle, bCriticalHit);
	
	// Calculate damage
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;
	
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
