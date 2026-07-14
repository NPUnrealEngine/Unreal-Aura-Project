#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	
	FDamageEffectParams(){}
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;
	
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::Zero();
	
	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public: // Getter
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	float GetKnockbackChance() const { return KnockbackChance; }
	float GetKnockbackForceMagnitude() const { return KnockbackForceMagnitude; }
	FVector GetKnockbackForce() const { return KnockbackForce; }

public: // Setter
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetKnockbackChance(const float InKnockbackChacne) { KnockbackChance = InKnockbackChacne; }
	void SetKnockbackForceMagnitude(const float InKnockbackForceMagnitude) { KnockbackForceMagnitude = InKnockbackForceMagnitude; }
	void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce; }
	
public:
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	
public: // Override
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
	
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	
	UPROPERTY()
	float DebuffDamage = 0.f;
	
	UPROPERTY()
	float DebuffDuration = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
	
	UPROPERTY()
	float KnockbackChance = 0.f;
	
	UPROPERTY()
	float KnockbackForceMagnitude = 0.f;
	
	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
	
	TSharedPtr<FGameplayTag> DamageType;
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