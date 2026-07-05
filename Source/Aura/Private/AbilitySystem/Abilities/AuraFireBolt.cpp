// NP Game Developer


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScalableDamage =  Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Fire Bolt</>\n\n"
			
			// Detail
			"<Small>Level</> <Level>%d</>\n"
			"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
			"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
			
			// Description
			"<Default>Launch a bolt of fire, exploding on impact and dealing:</> <Damage>%d</> "
			"<Default>fire damage with a chance to burn</>"), 
			
			// Values
			Level, 
			ManaCost,
			Cooldown,
			ScalableDamage);
	}
	
	return FString::Printf(TEXT(
		// Title
		"<Title>Fire Bolt</>\n\n"
		
		// Detail
		"<Small>Level</> <Level>%d</>\n"
		"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
		"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
		
		// Description
		"<Default>Launch %d bolts of fire, exploding on impact and dealing:</> <Damage>%d</> "
		"<Default>fire damage with a chance to burn</>"), 
		
		// Values
		Level, 
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles), 
		ScalableDamage);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
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
		"<Default>Launch %d bolts of fire, exploding on impact and dealing:</> <Damage>%d</> "
		"<Default>fire damage with a chance to burn</>"),
		
		// Values
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles), 
		ScalableDamage);
}