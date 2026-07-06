#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	
	FDamageEffectParams(){}
	
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;
	
	UPROPERTY()
	float BaseDamage = 0.f;
	
	UPROPERTY()
	float AbilityLevel = 1.f;
	
	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY()
	float DebuffChance = 0.f;
	
	UPROPERTY()
	float DebuffDamage = 0.f;
	
	UPROPERTY()
	float DebuffDuration = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	virtual FGameplayEffectContext* Duplicate() const override;
	
protected:
	/**
	 * Whether it is blocked hit
	 */
	UPROPERTY()
	bool bIsBlockedHit = false;

	/**
	 * Whether it is critical hit
	 */
	UPROPERTY()
	bool bIsCriticalHit = false;
};


/**
* Specific to USTRUCT
* 
* https://stackoverflow.com/questions/53228877/when-i-should-define-operations-type-traits-for-a-ustruct
* 
* Tell Unreal Engine about FAuraGameplayEffectContext's traits/capabilities
* so Unreal Engine can invoke proper methods without compilation error.
* 
* Here we tell Unreal Engine FAuraGameplayEffectContext is capable of following
* - NetSerializer : Overriding NetSerializer method
* - Copy : Overriding Duplicate method
*/
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};