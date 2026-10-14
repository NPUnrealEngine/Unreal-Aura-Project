// NP Game Developer


#include "AbilitySystem/Abilities/ArcaneShards.h"

#include <string>

FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 ScalableDamage =  Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float RadiusInMeter = ShardSpawnRadius.GetValueAtLevel(Level) / 100.f;
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Arcane Shards</>\n\n"
			
			// Detail
			"<Small>Level</> <Level>%d</>\n"
			"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
			"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
			
			// Description
			"<Default>Summon shards of arcane energy, causing radial arcane damage of </> "
			"<Damage>%d</> "
			"<Default>at shard origin.</>"), 
			
			// Values
			Level, 
			ManaCost,
			Cooldown,
			ScalableDamage
			);
	}
	
	return FString::Printf(TEXT(
		// Title
		"<Title>Arcane Shards</>\n\n"
		
		// Detail
		"<Small>Level</> <Level>%d</>\n"
		"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
		"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
		
		// Description
		"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </> "
		"<Damage>%d</> "
		"<Default>within</> "
		"<Distance>%.1f</> "
		"<Default>meter</>"), 
		
		// Values
		Level, 
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShards), 
		ScalableDamage,
		RadiusInMeter);
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 ScalableDamage =  Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float RadiusInMeter = ShardSpawnRadius.GetValueAtLevel(Level) / 100.f;
	
	return FString::Printf(TEXT(
		// Title
		"<Title>Next Level</>\n\n"
		
		// Detail
		"<Small>Level</> <Level>%d</>\n"
		"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
		"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
		
		// Description
		"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </> "
		"<Damage>%d</> "
		"<Default>within</> "
		"<Distance>%.1f</> "
		"<Default>meter</>"), 
		
		// Values
		Level, 
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShards), 
		ScalableDamage,
		RadiusInMeter);
}
