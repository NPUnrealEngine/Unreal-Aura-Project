// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);
/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(blueprintCallable, Category="Ability|Tasks",
		meta=(DisplayName="TargetDataUnderMouse", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	virtual void Activate() override;

	/**
	 * Send mouse cursor data to server
	 */
	void SendMouseCursorData() const;

	/**
	 * A callback for the event happen when target data arrive at server from client 
	 * @param DataHandle A handle contain target data 
	 * @param ActivationTag Ability Gameplay tag
	 */
	void OnTargetDataReplicateCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) const;
};
