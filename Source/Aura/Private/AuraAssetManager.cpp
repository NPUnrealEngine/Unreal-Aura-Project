// NP Game Developer


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Initialize Gameplay tags 
	FAuraGameplayTags::InitializeNativeGameplayTags();

	/**
	 * This is not necessary after Unreal Engine 5.3
	 * This will be called from module automatically
	 */
	// UAbilitySystemGlobals::Get().InitGlobalData();
}
