// NP Game Developer


#include "Input/AuraInputConfig.h"

#include "InputAction.h"

const UInputAction* UAuraInputConfig::FindAbilityInputAction(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const auto& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT(
			"Can't find AbilityInputAction for Tag [%s] on InputConfig [%s]"),
			*InputTag.ToString(),
			*GetNameSafe(this)
			);
	}
	return nullptr;
}
