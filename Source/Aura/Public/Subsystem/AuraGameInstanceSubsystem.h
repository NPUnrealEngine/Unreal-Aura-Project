// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AuraGameInstanceSubsystem.generated.h"

class UAbilityInfo;

/**
 * A customized UGameInstanceSubsystem
 * 
 * This Subsystem is designed only for blueprint usage, in other word
 * this subsystem will not be created by engine automatically
 * 
 * To use UAuraGameInstanceSubsystem
 * 
 * 1. Create a child blueprint derived from UAuraGameInstanceSubsystem
 * 2. Config AbilityInfo
 * 3. Create a new row and setup under Primary AssetTypes to Scan in Project settings -> AssetManager
 * 
 * Note: Under Primary AssetTypes to Scan in Project settings -> AssetManager
 *	- Add searching directory first (Doing this last might crash editor)
 *	- Check Has Blueprint Class
 *	- For Asset Base Class, choose your blueprint e.g BP_AuraGameInstanceSubsystem
 *	- For Primary Asset Type, give a name e.g AuraGameInstanceSubsystem
 */
UCLASS(Abstract, Blueprintable)
class AURA_API UAuraGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
