// NP Game Developer


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);

	check(AttributeInformation);

	for (auto& Pair : AS->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInformation->FindAttributeInfoForTag(
			Pair.Key
			);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
	
	/*FAuraAttributeInfo Info = AttributeInformation->FindAttributeInfoForTag(
		FAuraGameplayTags::Get().Attributes_Primary_Strength
		);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);*/
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
}
