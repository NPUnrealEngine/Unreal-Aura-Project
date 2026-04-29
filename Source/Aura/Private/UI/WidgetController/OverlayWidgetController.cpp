// NP Game Developer


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	// Broadcast health and mana information
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
	// Broadcast player level information
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	OnPlayerLevelChangedDelegate.Broadcast(AuraPlayerState->GetPlayerLevel());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	AuraPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}	
	);
	
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	/*
	 * Register GetGameplayAttributeValueChangeDelegate event
	 * and broadcast attributes change event
	 */
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()
	)
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()
	)
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()
	)
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()
	)
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraASC->bStartupAbilityGiven)
		{
			OnInitializeStartupAbilities(AuraASC);
		}
		else
		{
			AuraASC->AbilityGivenDelegate.AddUObject(this, &ThisClass::OnInitializeStartupAbilities);
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	//TODO: Get information about all given abilities, look up their ability info and broadcast it to widget
	if (!AuraAbilitySystemComponent->bStartupAbilityGiven) return;
	
	// Create a delegate for callback and bind to lambda
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
		{
			//TODO: Need a way to figure it out ability tag from ability spec
			
			// Find the ability info
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			
			// Set the ability info's input tag
			Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			
			// Notify the delegate about the ability info
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	
	// Loop through all abilities
	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
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
