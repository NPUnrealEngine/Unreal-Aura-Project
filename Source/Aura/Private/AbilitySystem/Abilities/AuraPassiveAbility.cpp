// NP Game Developer


#include "AbilitySystem/Abilities/AuraPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC))
	{
		AuraASC->DeactivatePassiveAbilityDelegate.AddUObject(this, &UAuraPassiveAbility::ReceiveDeactivate);
	}
}

void UAuraPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (GetAssetTags().HasTagExact(AbilityTag))
	{
		EndAbility(
			CurrentSpecHandle, 
			CurrentActorInfo, 
			CurrentActivationInfo, 
			true, 
			true
		);
	}
}

FGameplayEffectSpecHandle UAuraPassiveAbility::MakeGameplayEffect()
{
	checkf(GameplayEffectClass != nullptr, TEXT("GameplayEffectClass is null"));
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
		GameplayEffectClass, 
		GetAbilityLevel(), 
		ContextHandle
	);
	return SpecHandle;
}
