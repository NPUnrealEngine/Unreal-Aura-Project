// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS()
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;
	
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;
	
public:
	UFUNCTION(BlueprintCallable, DisplayName="WaitForCooldownChange", meta=(BlueprintInternalUseOnly = true))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	/**
	 * End this asynchronous task
	 */
	UFUNCTION(BlueprintCallable)
	void EndTask();
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	FGameplayTag CooldownTag;
	
protected:
	/**
	 * Callback function for cooldown effect removed
	 * @param InCooldownTag 
	 * @param NewCount 
	 */
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	/**
	 * Callback function for cooldown effect applied
	 * @param TargetASC 
	 * @param SpecApplied 
	 * @param ActiveEffectHandle 
	 */
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied,  FActiveGameplayEffectHandle ActiveEffectHandle);
};
