// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/**
	 * Input tag associated with this gameplay ability
	 * in AbilitySystemComponent
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

public: // Override
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
public:
	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);
	
protected:
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f);

	/**
	 * Called when this ability's level changed
	 * @param AbilityLevel 
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnAbilityLevelChanged(int32 AbilityLevel);
	
private:
	/**
	 * Callback function for ability level changed delegate from AuraAbilitySystemComponent
	 * @param AbilitySpec 
	 * @param AbilityLevel 
	 */
	void CallbackAbilityLevelChanged(const FGameplayAbilitySpec& AbilitySpec, int32 AbilityLevel);
};
