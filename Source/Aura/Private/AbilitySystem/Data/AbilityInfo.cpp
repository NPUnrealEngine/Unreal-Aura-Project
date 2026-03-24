// NP Game Developer


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannel.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& AbilityInfo : AbilityInformation)
	{
		if (AbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AbilityInfo;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(
			LogAura, 
			Error, 
			TEXT("Can't find AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this)
		);	
	}
	
	return FAuraAbilityInfo();
}
