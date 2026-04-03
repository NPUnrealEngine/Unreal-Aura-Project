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
 * Asynchronous task for blueprint
 * 
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	/**
	 * The delegate for ability cooldown has started
	 */
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	/**
	 * The delegate for ability cooldown has end
	 */
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;
	
public:
	/**
	 * Asynchronous task
	 * 
	 * Listen for particular ability's cooldown in ability system component 
	 * then notify the corresponding delegate
	 * 
	 * @param AbilitySystemComponent 
	 * @param InCooldownTag Ability's cooldown tag 
	 * @return 
	 */
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
