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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumShards = 11;

	/**
	 * Shards that will be spawned in this radius
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage|Radius")
	FScalableFloat ShardSpawnRadius;
	
public: // Override
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
