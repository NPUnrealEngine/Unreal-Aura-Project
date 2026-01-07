// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 *  Custom AuraAbilitySystemGlobals required for our 
 *  custom FAuraGameplayEffectContext
 *  
 *  How to configure project to use custom AuraAbilitySystemGlobals
 *  Project Settings -> Gameplay Abilities Settings
 *  https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/GameplayAbilities/UAbilitySystemGlobals
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
