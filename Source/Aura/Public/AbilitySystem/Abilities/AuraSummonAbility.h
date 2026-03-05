// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	/**
	 * Number of minions to summon
	 */
	UPROPERTY(EditAnywhere, Category="Summon")
	int32 NumMinions = 5;

	/**
	 * Classes of minion to summon
	 */
	UPROPERTY(EditDefaultsOnly, Category="Summon")
	TArray<TSubclassOf<APawn>> MinionClasses;

	/**
	 * Minimum spawn distance
	 */
	UPROPERTY(EditDefaultsOnly, Category="Summon")
	float MinSpawnDistance = 50.f;

	/**
	 * Maximum spawn distance
	 */
	UPROPERTY(EditDefaultsOnly, Category="Summon")
	float MaxSpawnDistance = 250.f;

	/**
	 * Spawn spread in angle
	 */
	UPROPERTY(EditDefaultsOnly, Category="Summon")
	float SpawnSpread = 90.f;
	
public:
	/**
	 * Get a list of summon locations
	 * 
	 * @return list of locations 
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSummonLocations();
};
