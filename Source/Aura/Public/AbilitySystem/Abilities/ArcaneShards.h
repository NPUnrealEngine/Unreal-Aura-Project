// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	/**
	 * Number of shards by curve table
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage|Shards")
	FScalableFloat NumShards;

	/**
	 * Shards that will be spawned in this radius, which is by curve table
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage|Radius")
	FScalableFloat ShardSpawnRadius;
	
public: // Override
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
