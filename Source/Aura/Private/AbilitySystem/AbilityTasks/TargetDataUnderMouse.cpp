// NP Game Developer


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	/**
	 * On Client: Send mouse cursor data to server
	 * On Server: Wait for data to arrive
	 */

	const bool bIsLocallyControlled = IsLocallyControlled();
	
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();

		/**
		 * Register a callback to respond to target data arrived
		 */
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			SpecHandle,
			PredictionKey
		).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicateCallback);

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
}

void UTargetDataUnderMouse::SendMouseCursorData() const
{
	/**
	 * Sending mouse cursor target data to server side
	 * so server will know which target it is going to hit
	 */
	
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	/**
	 * Get hit result from mouse cursor
	 */
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	/**
	 * Package mouse cursor hit result into target data
	 * and add it into a target data handle
	 */
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	/**
	 * Send/Replicate data to server
	 */
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	/**
	 * If ability is still active then broadcast
	 * event with data
	 */
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicateCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag) const
{
	/**
	 * Since AbilitySystemComponent will cached target data associated with a prediction key
	 * then we need to tell AbilitySystemComponent to consume it in order to clear it from
	 * cached
	 */
	AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey()
	);

	/**
	 * If ability is still active then broadcast
	 * event with data
	 */
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
