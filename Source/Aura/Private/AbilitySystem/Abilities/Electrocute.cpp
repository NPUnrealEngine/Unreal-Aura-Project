// NP Game Developer


#include "AbilitySystem/Abilities/Electrocute.h"

#include "GameplayEffectComponents/AdditionalEffectsGameplayEffectComponent.h"
#include "GameplayEffectComponents/ChanceToApplyGameplayEffectComponent.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScalableDamage =  Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 NumChainTarget = NumChainingTargets.AsInteger(Level);
	float ChanceToStun = 0.f;
	if (UGameplayEffect* DamageEffectCDO = DamageEffectClass.GetDefaultObject())
	{
		if (const UAdditionalEffectsGameplayEffectComponent* AdditionalEffectsComponent = DamageEffectCDO->FindComponent<UAdditionalEffectsGameplayEffectComponent>())
		{
			if (AdditionalEffectsComponent->OnApplicationGameplayEffects.Num() > 0)
			{
				FConditionalGameplayEffect CondEffect = AdditionalEffectsComponent->OnApplicationGameplayEffects[0];
				if (UGameplayEffect* StunEffectCDO = CondEffect.EffectClass.GetDefaultObject())
				{
					if (const UChanceToApplyGameplayEffectComponent* ChanceApplyEffectComponent = StunEffectCDO->FindComponent<UChanceToApplyGameplayEffectComponent>())
					{
						ChanceToStun = ChanceApplyEffectComponent->GetChanceToApplyToTarget().GetValueAtLevel(Level) * 100.f;
					}
				}
			}
			
		}
	}
	
	if (NumChainTarget <= 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Electrocute</>\n\n"
			
			// Detail
			"<Small>Level</> <Level>%d</>\n"
			"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
			"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
			
			// Description
			"<Default>Emit a beam of lightning, connecting with the target, causing:</> "
			"<Damage>%d</> "
			"<Default>lightning damage with a %.0f%% chance to stun</>"), 
			
			// Values
			Level, 
			ManaCost,
			Cooldown,
			ScalableDamage,
			ChanceToStun);
	}
	
	return FString::Printf(TEXT(
		// Title
		"<Title>Electrocute</>\n\n"
		
		// Detail
		"<Small>Level</> <Level>%d</>\n"
		"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
		"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
		
		// Description
		"<Default>Emit a beam of lightning,  connecting with the target, causing:</> "
		"<Damage>%d</> "
		"<Default>lightning damage with a %.0f%% chance to stun</>. "
		"<Default>Chaining %d additional nearby targets</>"), 
		
		// Values
		Level, 
		ManaCost,
		Cooldown,
		ScalableDamage,
		ChanceToStun,
		NumChainTarget);
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	return GetDescription(Level);
}
