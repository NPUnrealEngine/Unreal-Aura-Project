// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * AuraAssetManager
 *
 * Go to Project Settings -> Engine -> General Settings -> Default Classes
 * and change Asset Manager Class to this class in order to make it work
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAuraAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
