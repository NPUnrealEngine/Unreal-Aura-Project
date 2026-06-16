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
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
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
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPointsButton, bEnableEquipButton);
			
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	
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
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPointsButton, bEnableEquipButton);
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	FGameplayTag AbilityStatus;
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetAbilitySpecFromAbilityTag(AbilityTag);
	
	if (!AbilityTag.IsValid() || AbilitySpec == nullptr)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
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
	
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPointsButton, bEnableEquipButton);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableEquipButton = false;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		bShouldEnableEquipButton = false;
		bShouldEnableSpellPointsButton = false;
	}
}
