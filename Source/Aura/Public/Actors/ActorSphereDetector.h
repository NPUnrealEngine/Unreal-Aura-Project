// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ActorSphereDetector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDetectedSignature, AActor*, Actor);

class USphereComponent;

/**
 * This actor will follow an actor and cast a sphere trigger with radius
 * around the following actor then detect other actors around the
 * following actor
 */
UCLASS()
class AURA_API AActorSphereDetector : public AActor
{
	GENERATED_BODY()

public:
	AActorSphereDetector();
	
public:
	/**
	 * Get detector radius
	 * @param bScaled Whether to return radius by scaled 
	 * if true then radius = radius / SphereCollisionComponent->GetShapeScale()
	 * @return 
	 */
	float GetRadius(bool bScaled) const;
	
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/**
	 * Radius for detection
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Detection", meta=(ExposeOnSpawn="true"))
	float Radius = 5.f;

	/**
	 * Whether to use scaled radius
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Detection")
	bool bUseScaledRadius = true;
	
	/**
	 * Delegate called when an actor enter detection area
	 */
	UPROPERTY(BlueprintAssignable)
	FOnActorDetectedSignature OnActorEntered;
	
	/**
	 * Delegate called when an actor leave detection area
	 */
	UPROPERTY(BlueprintAssignable)
	FOnActorDetectedSignature OnActorLeave;
	
public: // Override
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
public:
	/**
	 * Start detection
	 * @param InActorsToIgnore 
	 */
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InActorsToIgnore"))
	void EnableDetection(const TArray<AActor*>& InActorsToIgnore);

	/**
	 * End detection
	 */
	UFUNCTION(BlueprintCallable)
	void DisableDetection();

	/**
	 * Add an actor to ignored list
	 * @param Actor 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	AActor* AddActorToIgnore(AActor* Actor);

	/**
	 * Remove an actor from ignored list
	 * @param Actor 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	AActor* RemoveActorFromIgnore(AActor* Actor);
	
protected:
	/**
	 * Sphere collision component to detect overlapping actors
	 */
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereCollisionComponent;

	/**
	 * The actor you want this detector to follow
	 */
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TObjectPtr<AActor> ActorToFollow;
	
	/**
	 * Actors to ignore
	 */
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TArray<TObjectPtr<AActor>> ActorsToIgnore;

	/**
	 * Any detected actors need to conform to this interface to be
	 * considered as detected actor
	 */
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TSubclassOf<UInterface> ConformToInterface;

	/**
	 * Any detected actors' class must subclass of or as same as this class
	 * in order to be considered as detected actor
	 */
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	UClass* FilterClass = nullptr;
	
protected:
	/**
	 * Callback function for collision component 
	 * @param OverlappedComponent 
	 * @param OtherActor 
	 * @param OtherComp 
	 * @param OtherBodyIndex 
	 * @param bFromSweep 
	 * @param SweepResult 
	 */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * Callback function for collision component 
	 * @param OverlappedComponent 
	 * @param OtherActor 
	 * @param OtherComp 
	 * @param OtherBodyIndex 
	 */
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	/**
	 * Indicate whether we should broadcast or not
	 */
	bool bBroadcast = false;
};
