// NP Game Developer


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

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
