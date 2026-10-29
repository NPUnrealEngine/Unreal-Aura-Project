// NP Game Developer


#include "AbilitySystem/AbilityTasks/TargetData_UnderMouseTick.h"

UTargetData_UnderMouseTick* UTargetData_UnderMouseTick::CreateUnderMouseTick(UGameplayAbility* OwningAbility,
																	float InMiniDistToUpdate)
{
	UTargetData_UnderMouseTick* MyTask = NewAbilityTask<UTargetData_UnderMouseTick>(OwningAbility);
	
	MyTask->bTickingTask = true;
	MyTask->MiniDistToUpdate = InMiniDistToUpdate;
	
	/*
	 * Maks sure hit result initialized with no hit and
	 * infinite for impact point
	 */
	MyTask->LastHitResult.bBlockingHit = false;
	MyTask->LastHitResult.ImpactPoint = FVector(
		TNumericLimits<float>::Max(),
		TNumericLimits<float>::Max(),
		TNumericLimits<float>::Max()
	);
	
	return MyTask;
}

void UTargetData_UnderMouseTick::Activate()
{
	Super::Activate();
	
	if (IsLocallyControlled())
	{
		/*
		 * Client we must tick to tracing hit under cursor
		 */
		bTickingTask = true;
		PlayerController = Cast<APlayerController>(Cast<APawn>(GetAvatarActor())->GetController());
	}
	else
	{
		/*
		 * Server we don't tick but wait for data from client
		 */
		bTickingTask = false;
		WaitForTargetHitData();
	}
}

void UTargetData_UnderMouseTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	// If we are control locally and PlayerController is valid
	if (IsLocallyControlled() && PlayerController.IsValid())
	{
		// Find hit result under cursor
		FHitResult NewHitResult;
		PlayerController->GetHitResultUnderCursor(
			ECC_Visibility, 
			false, 
			NewHitResult
		);
		
		// Check this hit and last hit is reach minimum distance then we send data to server
		float Distance = (NewHitResult.ImpactPoint - LastHitResult.ImpactPoint).Length();
		if (Distance >= MiniDistToUpdate)
		{
			bool bSendData = false;
			/*
			 * UNetConnection only exists in client 
			 */
			if (UNetConnection* NetConnection = PlayerController->GetNetConnection())
			{
				// We are in client then check if network bandwidth is no saturated
				if (NetConnection->IsNetReady())
				{
					bSendData = true;
				}
				else
				{
					bSendData = false;
				}
			}
			else // we are on listen server 
			{
				bSendData = true;
			}
			
			if (bSendData)
			{
				// Send data to server
				FGameplayAbilityTargetDataHandle& DataHandle = SendMouseCursorData();
				
				// Update last hit result
				LastHitResult = NewHitResult;
				
				if (ValidData.IsBound() && ShouldBroadcastAbilityTaskDelegates())
				{
					ValidData.Broadcast(DataHandle);
				}
			}
		}
	}
}

void UTargetData_UnderMouseTick::OnTargetHitDataArrived(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	Super::OnTargetHitDataArrived(DataHandle, ActivationTag);
	
	if (ValidData.IsBound() && ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
