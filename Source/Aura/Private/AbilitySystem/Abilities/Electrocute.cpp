// NP Game Developer


#include "AbilitySystem/Abilities/Electrocute.h"

float UElectrocute::GetStunDuration(const float Level)
{
	return StunDuration.GetValueAtLevel(Level);
}

float UElectrocute::GetStunChance(const float Level)
{
	return StunChance.GetValueAtLevel(Level);
}

bool UElectrocute::CanApplyStun(const float CalculatedStunChance)
{
	if ((CalculatedStunChance < 1.f - SMALL_NUMBER) && (FMath::FRand() > CalculatedStunChance))
	{
		return false;
	}
	return true;
}

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScalableDamage =  Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 NumChainTarget = NumChainingTargets.AsInteger(Level);
	float ChanceToStun = GetStunChance(Level) * 100.f;
	float DurationForStun = GetStunDuration(Level);
	
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
			"<Default>lightning damage with a %.0f%% chance to stun for %.0f seconds</>"), 
			
			// Values
			Level, 
			ManaCost,
			Cooldown,
			ScalableDamage,
			ChanceToStun,
			DurationForStun);
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
		"<Default>lightning damage with a %.0f%% chance to stun for %.0f seconds</>. "
		"<Default>Chaining %d additional nearby targets</>"), 
		
		// Values
		Level, 
		ManaCost,
		Cooldown,
		ScalableDamage,
		ChanceToStun,
		DurationForStun,
		NumChainTarget);
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	return GetDescription(Level);
}
