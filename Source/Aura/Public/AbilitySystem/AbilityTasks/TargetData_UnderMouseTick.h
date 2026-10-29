// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "TargetData_MouseCursor.h"
#include "TargetData_UnderMouseTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValidDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class AURA_API UTargetData_UnderMouseTick : public UTargetData_MouseCursor
{
	GENERATED_BODY()
	
public:
	FHitResult LastHitResult;
	
	UPROPERTY(BlueprintAssignable)
	FOnValidDataSignature ValidData;
	
	float MiniDistToUpdate = 5.f;
	
public:
	/**
	 * Create a task that handle hit under mouse continuously
	 * 
	 * Client: Tracking hit under cursor every tick and send data to server only 2 conditions are met,
	 * 1. check if reach minimum distance between this hit and last hit 
	 * 2. network bandwidth is not saturated/reach limit 
	 * 
	 * Server: Wait for client's data arrived
	 * 
	 * @param OwningAbility 
	 * @param InMiniDistToUpdate The minimum distance between this hit and last hit
	 * for data to be sent to server
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Task",
		meta=(DisplayName="UnderMouseTick", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static UTargetData_UnderMouseTick* CreateUnderMouseTick(UGameplayAbility* OwningAbility, float InMiniDistToUpdate=5.f);
	
protected:
	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController;
	
protected:// Override
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnTargetHitDataArrived(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) override;
};
