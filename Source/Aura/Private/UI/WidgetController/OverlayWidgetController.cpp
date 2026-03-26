// NP Game Developer


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

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
