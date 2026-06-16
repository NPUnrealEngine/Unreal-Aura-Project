// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, bool, bSpendPointsEnabled, bool, bEquipButonEnabled);
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
