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
	SelectedAbility.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.AbilityStatus = FAuraGameplayTags::Get().Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(
		false,
		false,
		FString(),
		FString()
	);
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
