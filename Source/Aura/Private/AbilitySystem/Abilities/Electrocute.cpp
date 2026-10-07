// NP Game Developer


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScalableDamage =  Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Electrocute</>\n\n"
			
			// Detail
			"<Small>Level</> <Level>%d</>\n"
			"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
			"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
			
			// Description
			"<Default>Emit a beam of lightning, connecting with the target, repeatedly causing:</> "
			"<Damage>%d</> "
			"<Default>lightning damage with a chance to stun</>"), 
			
			// Values
			Level, 
			ManaCost,
			Cooldown,
			ScalableDamage);
	}
	
	return FString::Printf(TEXT(
		// Title
		"<Title>Electrocute</>\n\n"
		
		// Detail
		"<Small>Level</> <Level>%d</>\n"
		"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
		"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
		
		// Description
		"<Default>Emit a beam of lightning, "
		"propagating to %d additional targets nearby causing: </>"
		"<Damage>%d</> "
		"<Default>lightning damage with a chance to stun</>"), 
		
		// Values
		Level, 
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShockTargets-1), 
		ScalableDamage);
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 ScalableDamage =  Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>Next Level</>\n\n"
		
		// Detail
		"<Small>Level</> <Level>%d</>\n"
		"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
		"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
		
		// Description
		"<Default>Emit a beam of lightning, "
		"propagating to %d additional targets nearby causing: </>"
		"<Damage>%d</> "
		"<Default>lightning damage with a chance to stun</>"),
		
		// Values
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShockTargets-1), 
		ScalableDamage);
}
