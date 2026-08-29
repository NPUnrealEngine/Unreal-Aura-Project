// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraProjectile();

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FDamageEffectParams DamageEffectParams;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
	
public:
	UFUNCTION()
	void LaunchProjectile(AActor* InTarget, const FVector& InTargetLocation, const float AccelerationMin, const float AccelerationMax);

protected:
	FVector TargetLocation;
	FTimerHandle IntervalDistCheckTimer;
	
	UPROPERTY(EditAnywhere, Category="Aura | Projectile")
	float TargetDistanceAcceptanceRadius = 15.f;
	
	UPROPERTY(EditAnywhere, Category="Aura | Projectile")
	float IntervalToCheckTargetDistance = 0.1f;
	
protected: // Override
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	/**
	 * Call this when projectile hit something
	 */
	void OnHit();
	
	/**
	 * Callback function for timer to check if reaching target destination
	 */
	void CheckIfReachTargetLocation();

	/**
	 * Call when target location reach within acceptance radius
	 * 
	 * C++ default implementation is only destroy this actor
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnReachTargetLocation();

	/**
	 * Call from server side to notify clients reached target destination
	 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ReachTargetLocation();
	
private:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	/**
	 * Whether the projectile is hit or not
	 */
	bool bHit = false;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
};
