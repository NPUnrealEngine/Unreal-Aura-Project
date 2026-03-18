// NP Game Developer


#include "Player/OcclusionAwarePlayerController.h"

#include "Components/CapsuleComponent.h"
#include "Interface/OcclusionInterface.h"
#include "Kismet/KismetSystemLibrary.h"

AOcclusionAwarePlayerController::AOcclusionAwarePlayerController()
{
	DebugPrintScreenKey = GetTypeHash(FGuid::NewGuid());
	CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
}

void AOcclusionAwarePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

bool AOcclusionAwarePlayerController::HideActor(AActor* Actor, FOccludedActorInfo& OutInfo)
{
	if (!Actor->Implements<UOcclusionInterface>()) return false;
	
	// Find information about the actor
	FOccludedActorInfo* FoundOccludedActorInfo = OccludedActors.Find(Actor);
	
	// This actor was occluded no more action
	if (FoundOccludedActorInfo && FoundOccludedActorInfo->bIsOccluded)
	{
		if (bDebugTracing)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, ignore it"), *Actor->GetName())
		}
		
		OutInfo = *FoundOccludedActorInfo;
		return true;
	}
	
	// Actor is not occluded, hide this actor
	if (FoundOccludedActorInfo && !FoundOccludedActorInfo->bIsOccluded)
	{
		if (bDebugTracing)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor %s was not occluded, hide it now"), *Actor->GetName())
		}
		
		// Set occluded to true
		FoundOccludedActorInfo->bIsOccluded = true;
		
		// Tell actor to beginning occlusion
		IOcclusionInterface::Execute_OnOcclusionBegin(FoundOccludedActorInfo->Actor);
		
		OutInfo = *FoundOccludedActorInfo;
		return true;
	}
	
	/* Actor does not exist, we create information for the actor and
	 * add it to occluded actors list then hide the actor
	 */
	FOccludedActorInfo NewOccludedActorInfo(Actor, true);
	OccludedActors.Add(Actor, NewOccludedActorInfo);
		
	// Tell actor to beginning occlusion
	IOcclusionInterface::Execute_OnOcclusionBegin(NewOccludedActorInfo.Actor);
	
	OutInfo = NewOccludedActorInfo;
	return true;
}

void AOcclusionAwarePlayerController::ShowActor(FOccludedActorInfo& OccludedActorInfo) const
{
	OccludedActorInfo.bIsOccluded = false;
	
	// Tell actor to beginning occlusion
	IOcclusionInterface::Execute_OnOcclusionEnd(OccludedActorInfo.Actor);
}

EDrawDebugTrace::Type AOcclusionAwarePlayerController::GetDrawDebugTraceType() const
{
	if (bUseDebugTracingOneFrame)
	{
		return EDrawDebugTrace::ForOneFrame;
	}
	
	return EDrawDebugTrace::ForDuration;
}

void AOcclusionAwarePlayerController::SyncOccludedActors()
{
	if (GetPawn() == nullptr) return;
	
	/* Get tracing start and end location */
	FVector TraceStart = PlayerCameraManager->GetCameraLocation() + Offset;
	FVector TraceEnd = GetPawn()->GetActorLocation() + Offset;
	FVector DirectionToTrace = (TraceEnd - TraceStart).GetSafeNormal();
	float DistanceToTrace = (TraceEnd - TraceStart).Length() * DistancePercentageForTrace;
	TraceEnd = TraceStart + DirectionToTrace * DistanceToTrace;
	
	/* Do tracing */
	TArray<FHitResult> HitResults;
	const TArray<TObjectPtr<AActor>> ActorsToIgnore;
	
	EDrawDebugTrace::Type DebugTraceType = EDrawDebugTrace::None;
	if (bDebugTracing)
	{
		DebugTraceType= GetDrawDebugTraceType();
	}
	
	const bool bHits = UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetWorld(),
		TraceStart,
		TraceEnd,
		BoxTraceHalfSize,
		PlayerCameraManager->GetCameraRotation(),
		CollisionObjectTypes,
		bTraceComplex,
		ActorsToIgnore,
		DebugTraceType,
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		DebugTracingDuration
	);
	
	/* Update occluded actors */
	if (bHits)
	{
		// Contain all actors is occluded in this call
		TSet<TObjectPtr<const AActor>> IntermediateActorsHitAndHide;
		
		/* Occluded / hidden all hitting actor */
		for (FHitResult& Hit : HitResults)
		{
			FOccludedActorInfo Info;
			if (HideActor(Hit.GetActor(), Info))
			{
				IntermediateActorsHitAndHide.Add(Info.Actor);
			}
		}
		
		/* Show all actors that isn't hit */
		for (auto It = OccludedActors.CreateIterator(); It; ++It)
		{
			// Remove occluded actor if the actor is no longer available
			if (!IsValid(It.Value().Actor))
			{
				It.RemoveCurrent();
				continue;
			}
			
			// If actor is not hit and was occluded / hidden
			if (!IntermediateActorsHitAndHide.Contains(It.Value().Actor) && 
				It.Value().bIsOccluded)
			{
				ShowActor(It.Value());
				
				// Remove occluded actor from list
				It.RemoveCurrent();
			}
		}
	}
	else
	{
		
		/* Show all occluded actors */
		for (auto It = OccludedActors.CreateIterator(); It; ++It)
		{
			ShowActor(It.Value());
			
			// Remove occluded actor from list
			It.RemoveCurrent();
		}
	}
	
	if (bDebugTracing)
	{
		GEngine->AddOnScreenDebugMessage(
			DebugPrintScreenKey,
			1.f,
			FColor::Cyan,
			FString::Printf(TEXT("OccludedActors: %d"), OccludedActors.Num())
		);
	}
	
}
