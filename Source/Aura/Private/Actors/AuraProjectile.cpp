// NP Game Developer


#include "Actors/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement((true));
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(
		LoopingSound,
		GetRootComponent(),
		NAME_None,
		FVector(ForceInit),
		FRotator::ZeroRotator,
		EAttachLocation::Type::KeepRelativeOffset,
		true
	);
}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(
		this,
		ImpactSound,
		GetActorLocation(),
		FRotator::ZeroRotator
	);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		ImpactEffect,
		GetActorLocation()
	);
	bHit = true;
}

void AAuraProjectile::Destroyed()
{
	/**
	 * Play sound and effect only when projectile not hit and not the owner
	 * This case happen when server projectile been destroyed before client's
	 * projectile. Which means client not yet play sound and effect. 
	 */
	if (!bHit && !HasAuthority())
	{
		OnHit();
	}
	
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Ignore actor if actor is the instigator
	if (GetInstigator() == OtherActor) return;
	if (!UAuraAbilitySystemLibrary::IsNotFriend(GetInstigator(), OtherActor)) return;
	if (!bHit) OnHit();
	
	/**
	 * Destroy projectile only if we are the owner
	 */
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// Calculate death impulse
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse += DeathImpulse;
			
			const bool bKnockback = FMath::RandRange(1.f, 100.f) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{ 
				// Add some pitch rotation
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				
				// calculate knockback force
				const FVector KnockForce = Rotation.Vector() * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce += KnockForce;
			}
			
			// Set target ability system component
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}
