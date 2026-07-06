// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interface/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	/**
	 * Apply damage effect to target actor directly
	 * 
	 * @param TargetActor 
	 */
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(editDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(editDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;
	
	UPROPERTY(editDefaultsOnly, Category = "Debuff")
	float DebuffChance = 20.f;
	
	UPROPERTY(editDefaultsOnly, Category = "Debuff")
	float DebuffDamage = 5.f;
	
	UPROPERTY(editDefaultsOnly, Category = "Debuff")
	float DebuffFrequency = 1.f;
	
	UPROPERTY(editDefaultsOnly, Category = "Debuff")
	float DebuffDuration = 5.f;
	
protected:
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage> TaggedMontages) const;
};
