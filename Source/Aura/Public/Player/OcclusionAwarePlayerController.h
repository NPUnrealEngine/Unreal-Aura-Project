// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OcclusionAwarePlayerController.generated.h"


class UCapsuleComponent;
/**
 * A struct contain information about
 * occluded actor
 */
USTRUCT(BlueprintType)
struct FOccludedActorInfo
{
	GENERATED_BODY()

	/**
	 * Actor that is occluded
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> Actor;

	/**
	 * Whether this actor is occluded/hidden or not
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsOccluded;
};

/**
 * Detect obstruction between player controller's current camera and controlled pawn,
 * it manages all occluded actors who have implemented IOcclusionInterface. And tell
 * those actors to do occlusion or none-occlusion 
 * 
 * It required any actors who want to receive notification for occlusion/none-occlusion 
 * to implemented IOcclusionInterface and those actor must implement its own occlusion/
 * none-occlusion procedure for example hide / show actor
 * 
 * Any subclass of this controller have to call SyncOccludedActors() in interval 
 * in order to make it works.
 */
UCLASS()
class AURA_API AOcclusionAwarePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AOcclusionAwarePlayerController();
	
public:
	/**
	 * Call this function to update occluded actors
	 */
	UFUNCTION(BlueprintCallable)
	void SyncOccludedActors();
	
protected:
	/**
	 * Percentage for pawn's capsule radius and half height
	 * 
	 * Note: Pawn's capsule will be used for tracing
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Occlusion", meta=(ClampMin=0.1))
	float CapsulePercentageForTrace = 1.f;

	/**
	 * True to test against complex collision, false to test against simplified collision
	 * 
	 * Note: may reduce performance when enabled
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Occlusion")
	bool bTraceComplex = false;

	/**
	 * Enable to visualize tracing
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Occlusion | Debug")
	bool bDebugTracing = false;

	/**
	 * Enable to show tracing per frame otherwise
	 * it will show tracing for a duration
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Occlusion | Debug")
	bool bUseDebugTracingOneFrame = false;
	
	/**
	 * Duration for a tracing to be shown when debugging
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Occlusion | Debug")
	float DebugTracingDuration = 5.f;

	/**
	 * A list of object types for tracing to determine whether an actor with
	 * an object type should be occluded 
	 * 
	 * Note: WorldStatic is added as default
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Occlusion")
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;

	/**
	 * Capsule component that attached to pawn. The pawn which is controlled by
	 * this player controller
	 */
	UPROPERTY(BlueprintReadOnly, Category="Occlusion")
	TObjectPtr<UCapsuleComponent> PawnCapsuleComponent;

	/**
	 * A map that mapping occluded actor to their occluded information
	 * 
	 * This is for tracking occluded actors
	 */
	UPROPERTY(BlueprintReadOnly, Category="Occlusion")
	TMap<TObjectPtr<const AActor>, FOccludedActorInfo> OccludedActors;
	
protected:
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(class APawn* P) override;
	bool HideActor(AActor* Actor, FOccludedActorInfo& OutInfo);
	void ShowActor(FOccludedActorInfo& OccludedActorInfo);

private:
	int32 DebugPrintScreenKey = 0;
	
private:
	void UpdateCapsuleComponent(const APawn* InPawn);
	EDrawDebugTrace::Type GetDrawDebugTraceType() const;
};
