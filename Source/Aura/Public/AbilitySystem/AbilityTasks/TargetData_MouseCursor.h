// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetData_MouseCursor.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UTargetData_MouseCursor : public UAbilityTask
{
	GENERATED_BODY()
	
protected:
	/**
	 * Client: data will be available after SendMouseCursorData is called
	 * Server: data will be available after OnTargetHitDataArrived is called
	 */
	UPROPERTY()
	FGameplayAbilityTargetDataHandle TargetHitDataHandle;
	
protected:
	/**
	 * Call this on client to send data to server
	 * 
	 * Data is FGameplayAbilityTargetData_SingleTargetHit
	 * 
	 * Send mouse cursor data to server
	 */
	FGameplayAbilityTargetDataHandle& SendMouseCursorData();

	/**
	 * Override this on client to get hit result under mouse
	 * 
	 * The default one use ECC_Visibility to do tracing
	 * @param HitResult 
	 */
	virtual void GetMouseCursorHitResult(FHitResult& HitResult);

	/**
	 * Call this on server to wait for client to send data
	 */
	void WaitForTargetHitData();

	/**
	 * This will be called on server when client's data arrived on server
	 * 
	 * Override this to received callback, you must call Super::OnTargetHitDataArrived
	 * 
	 * @param DataHandle 
	 * @param ActivationTag 
	 */
	virtual void OnTargetHitDataArrived(const FGameplayAbilityTargetDataHandle& DataHandle,
	                                    FGameplayTag ActivationTag);
};
