// NP Game Developer


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "../../../../../../../Epic Games/Games/UE_5.7/Engine/Plugins/Editor/GameplayTagsEditor/Source/GameplayTagsEditor/Private/GameplayTagEditorUtilities.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	
	// Broadcast ability information
	BroadcastAbilityInfo();
	
	// Broadcast initial spell points
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
	// Bind callback to ability status changed
	GetAuraASC()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
		{
			if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.AbilityStatus = StatusTag;	
			}
			
			bool bEnableSpellPointsButton = false;
			bool bEnableEquipButton = false;
			ShouldEnableButtons(
				StatusTag, 
				CurrentSpellPoints, 
				bEnableSpellPointsButton, 
				bEnableEquipButton
			);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionsByAbilityTag(
				AbilityTag, 
				Description, 
				NextLevelDescription
			);
			SpellGlobeSelectedDelegate.Broadcast(
				bEnableSpellPointsButton, 
				bEnableEquipButton, 
				Description, 
				NextLevelDescription
			);
			
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	
	GetAuraASC()->AbilityEquippedDelegate.AddUObject(this, &ThisClass::OnAbilityEquipped);
	
	// Bind callback to spell points changed
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			SpellPointsChanged.Broadcast(Points);
			CurrentSpellPoints = Points;
			
			bool bEnableSpellPointsButton = false;
			bool bEnableEquipButton = false;
			ShouldEnableButtons(
				SelectedAbility.AbilityStatus, 
				CurrentSpellPoints, 
				bEnableSpellPointsButton, 
				bEnableEquipButton
			);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionsByAbilityTag(
				SelectedAbility.AbilityTag, 
				Description, 
				NextLevelDescription
			);
			SpellGlobeSelectedDelegate.Broadcast(
				bEnableSpellPointsButton, 
				bEnableEquipButton, 
				Description, 
				NextLevelDescription
			);
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	// If we are in wait for equipping ability state then we need to leave the state
	if (bWaitForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitForEquipSelectionDelegate.Broadcast(SelectedAbilityType);
		bWaitForEquipSelection = false;
	}
	
	int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	FGameplayTag AbilityStatus;
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetAbilitySpecFromAbilityTag(AbilityTag);
	
	if (!AbilityTag.IsValid() || AbilitySpec == nullptr)
	{
		AbilityStatus = Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusTagFromSpec(*AbilitySpec);
	}
	
	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.AbilityStatus = AbilityStatus;
	
	bool bEnableSpellPointsButton = false;
	bool bEnableEquipButton = false;
	
	ShouldEnableButtons(
		AbilityStatus, 
		SpellPoints, 
		bEnableSpellPointsButton, 
		bEnableEquipButton
	);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(
		AbilityTag, 
		Description, 
		NextLevelDescription
	);
	SpellGlobeSelectedDelegate.Broadcast(
		bEnableSpellPointsButton, 
		bEnableEquipButton, 
		Description, 
		NextLevelDescription
	);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC() && SelectedAbility.AbilityTag.IsValid())
	{
		// Call server side Aura Ability System Component to spend a point in spell
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::SpellGlobeDeselected()
{
	// If we are in wait for equipping ability state then we need to leave the state
	if (bWaitForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitForEquipSelectionDelegate.Broadcast(SelectedAbilityType);
		bWaitForEquipSelection = false;
	}
	
	
	SelectedAbility.AbilityTag = Abilities_None;
	SelectedAbility.AbilityStatus = Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(
		false,
		false,
		FString(),
		FString()
	);
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
	WaitForEquipSelectionDelegate.Broadcast(AbilityType);
	bWaitForEquipSelection = true;
	
	/*
	 * Tracing selected ability's input tag
	 */
	const FGameplayTag SelectedStatusTag = GetAuraASC()->GetStatusTagFromAbilityTag(SelectedAbility.AbilityTag);
	if (SelectedStatusTag.MatchesTagExact(Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetInputTagFromAbilityTag(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	// Do nothing if not in wait for equip selected ability state
	if (!bWaitForEquipSelection) return;
	
	// Do nothing if equipping ability type is not as same as spell row globe ability type
	// e.g -equpping ability type: Offensive and -spell row globe ability type: Passive 
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetAuraASC()->ServerEquipAbility(SelectedAbility.AbilityTag, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag)
{
	bWaitForEquipSelection = false;
	
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.AbilityTag = Abilities_None;
	LastSlotInfo.StatusTag = Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlotTag;
	// Broadcast empty info if previous slot is a valid slot, On if equipping on already equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);
	
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = SlotTag;
	AbilityInfoDelegate.Broadcast(Info);
	
	StopWaitForEquipSelectionDelegate.Broadcast(Info.AbilityType);
	SpellGlobeReassignDelegate.Broadcast(AbilityTag);
	SpellGlobeDeselected();
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
	if (AbilityStatus.MatchesTagExact(Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(Abilities_Status_Eligible))
	{
		bShouldEnableEquipButton = false;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(Abilities_Status_Locked))
	{
		bShouldEnableEquipButton = false;
		bShouldEnableSpellPointsButton = false;
	}
}
