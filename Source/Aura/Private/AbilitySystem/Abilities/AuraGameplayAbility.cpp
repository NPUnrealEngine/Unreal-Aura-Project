// NP Game Developer


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAuraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// Listen for ability level change event
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC))
	{
		AuraASC->OnAbilityLevelChangedDelegated.AddUObject(this, &UAuraGameplayAbility::CallbackAbilityLevelChanged);
	}
}

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default ability name - loumeipson loumeipson loumeipson loumeipson loumeipson loumeipson loumeipson loumeipson loumeipson", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next level: </><Level>%d</> \n<Default>Cause more damage</>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell locked until level</> <Level>%d</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Modifier : CostEffect->Modifiers)
		{
			if (Modifier.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel)
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}

void UAuraGameplayAbility::CallbackAbilityLevelChanged(const FGameplayAbilitySpec& AbilitySpec, int32 AbilityLevel)
{
	// Only broadcast ability level change event if ability is matched
	FGameplayTagContainer InAbilityAssetTags(AbilitySpec.Ability->GetAssetTags());
	bool bIsMatched = GetAssetTags().MatchesQuery(FGameplayTagQuery::MakeQuery_MatchAllTags(InAbilityAssetTags));
	if (bIsMatched)
	{
		OnAbilityLevelChanged(AbilityLevel);
	}
}

void UAuraGameplayAbility::OnAbilityLevelChanged_Implementation(int32 AbilityLevel)
{
}
