// NP Game Developer


#include "AbilitySystem/AbilityTasks/TargetData_MouseCursor.h"

#include "AbilitySystemComponent.h"


FGameplayAbilityTargetDataHandle& UTargetData_MouseCursor::SendMouseCursorData()
{
	/**
	 * Sending mouse cursor target data to server side
	 * so server will know which target it is going to hit
	 */
	
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	/**
	 * Get hit result from mouse cursor
	 */
	FHitResult CursorHit;
	GetMouseCursorHitResult(CursorHit);

	/**
	 * Package mouse cursor hit result into target data
	 * and add it into a target data handle
	 */
	FGameplayAbilityTargetData_SingleTargetHit* TargetHitData = new FGameplayAbilityTargetData_SingleTargetHit();
	TargetHitData->HitResult = CursorHit;
	TargetHitDataHandle.Clear();
	TargetHitDataHandle.Add(TargetHitData);

	/**
	 * Send/Replicate data to server
	 */
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		TargetHitDataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	return TargetHitDataHandle;
}

void UTargetData_MouseCursor::GetMouseCursorHitResult(FHitResult& HitResult)
{
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
}

void UTargetData_MouseCursor::WaitForTargetHitData()
{
	const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
	const FPredictionKey PredictionKey = GetActivationPredictionKey();
	
	/*
	 * Register a callback to respond to target data arrived
	 */
	AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
		SpecHandle,
		PredictionKey
	).AddUObject(this, &UTargetData_MouseCursor::OnTargetHitDataArrived);
	
	// Boolean value to see whether target data is arrived or not
	bool bCallDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(
		SpecHandle,
		PredictionKey
	);
	
	// If target data is not arrived, tell ability to wait for target data
	if (!bCallDelegate)
	{
		SetWaitingOnRemotePlayerData();
	}
}

void UTargetData_MouseCursor::OnTargetHitDataArrived(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	TargetHitDataHandle = DataHandle;
	
	/**
	 * Since AbilitySystemComponent will cached target data associated with a prediction key
	 * then we need to tell AbilitySystemComponent to consume it in order to clear it from
	 * cached
	 */
	AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey()
	);
}
