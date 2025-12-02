// NP Game Developer


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	check(AttributeInfo);

	for (auto& Attribute : AttributeInfo->AttributeInformaiton)
	{
		BroadcastAttributeInfo(Attribute.AttributeTag, Attribute.GameplayAttribute);
	}

	/**
	 * Use hard code tags to gameplay attribute to
	 * brocast attribute info to widget
	 *
	 * Look TagsToAttributes in AuraAttributeSet.h
	 */
	/*UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}*/
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	check(AttributeInfo);

	for (auto& Attribute : AttributeInfo->AttributeInformaiton)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute.GameplayAttribute)
		.AddLambda(
			[this, Attribute](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Attribute.AttributeTag, Attribute.GameplayAttribute);
			}
		);
	}

	/**
	 * Use hard code tags to gameplay attribute to
	 * brocast attribute info to widget
	 *
	 * Look TagsToAttributes in AuraAttributeSet.h
	 */
	/*UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value())
		.AddLambda(
				[this, Pair](const FOnAttributeChangeData& Data)
				{
					BroadcastAttributeInfo(Pair.Key, Pair.Value());
				}
			);
	}*/
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(GameplayTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
