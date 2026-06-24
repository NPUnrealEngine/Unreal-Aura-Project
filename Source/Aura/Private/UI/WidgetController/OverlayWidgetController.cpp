// NP Game Developer


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	
	// Broadcast health and mana information
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}	
	);

	/*
	 * Register GetGameplayAttributeValueChangeDelegate event
	 * and broadcast attributes change event
	 */
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetHealthAttribute()
	)
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxHealthAttribute()
	)
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetManaAttribute()
	)
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxManaAttribute()
	)
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		GetAuraASC()->AbilityEquippedDelegate.AddUObject(this, &ThisClass::OnAbilityEquipped);
		
		if (AuraASC->bStartupAbilityGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			AuraASC->AbilityGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
		}
		
		/*
		* Register event for gameplay effect applied to ASC with tags
		*/
		AuraASC->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
			{
				// Get all tags under Message tag
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				for (const FGameplayTag& Tag : AssetTags)
				{
					/*
					 * Broadcast tag with information to widget if the tag is type of Message tag
					 */
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
	
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo, please fill out LevelUpInfo property in AuraPlayerState blueprint"));
	
	// Find current level
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	
	// Find max level
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();
	
	if (Level <= MaxLevel && Level > 0)
	{
		// Get current level xp requirement
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		
		// Get previous level xp requirement
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		
		// The difference between current level and previous level xp requirement
		const int32 DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		
		// How much xp we have at current level
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
		
		// Calculate xp percentage
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);
		
		OnXPPercentChanged.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag) const
{
	
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlotTag;
	// Broadcast empty info if previous slot is a valid slot, On if equipping on already equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);
	
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = SlotTag;
	AbilityInfoDelegate.Broadcast(Info);
}
