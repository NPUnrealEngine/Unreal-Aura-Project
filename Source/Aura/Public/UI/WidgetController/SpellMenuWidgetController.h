// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, 
	bool, bSpendPointsEnabled, bool, bEquipButonEnabled, FString, Description, FString, NextLevelDescription);

/**
 * Wait for equip selection signature
 * 
 * @param  FGamePlayTag AbilityType -Offensive or -Passive
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, 
	const FGameplayTag&, AbilityType);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag AbilityStatus = FGameplayTag();
};

/* 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	/**
	 * Call when spell points changed
	 */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointsChanged;

	/**
	 * Call when spell globe on UI is selected and
	 * the delegate associate with boolean values which 
	 * determine spell points & equip buttons should be 
	 * enabled on UI or not
	 */
	UPROPERTY(BlueprintAssignable)
 	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	/**
	 * Call when entering equipping ability state
	 */
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipSelectionDelegate;
	
	/**
	 * Call when leave equipping ability state
	 */
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitForEquipSelectionDelegate;
	
public: // Override
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
public:
	/**
	 * Call this when an ability spell globe on UI is selected
	 * @param AbilityTag 
	 */
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	/**
	 * Call this when spend point button clicked on UI
	 */
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();
	
	/**
	 * Call this when a spell globe deselected on UI
	 */
	UFUNCTION(BlueprintCallable)
	void SpellGlobeDeselected();
	
	/**
	 * Call this when equip button clicked on UI
	 */
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	/**
	 * Call this when a globe in spell row clicked on UI 
	 * @param SlotTag input tag
	 * @param AbilityType type of ability -offensive, -passive
	 */
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag);

private:
	/**
	 * Tracing selected ability on UI
	 */
	FSelectedAbility SelectedAbility = {FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked};

	/**
	 * For storing spell points temporary
	 */
	int32 CurrentSpellPoints = 0;

	/**
	 * Whether we are waiting for equipping an ability state
	 */
	bool bWaitForEquipSelection = false;
	
	FGameplayTag SelectedSlot;
	
private:
	/**
	 * Determine whether spell points & equip button should be enabled or not
	 * @param AbilityStatus selected ability status tag
	 * @param SpellPoints spell points
	 * @param bShouldEnableSpellPointsButton output boolean to determine button should be enabled or not 
	 * @param bShouldEnableEquipButton output boolean to determine button should be enabled or not 
	 */
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);
};
