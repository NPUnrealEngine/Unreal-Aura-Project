// NP Game Developer


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actors/AuraProjectile.h"
#include "Interface/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bPitchOverride, float OverridePitch)
{
	// We only spawn projectile on server
	const bool IsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!IsServer) return;

	/*
	 * Prepare projectile properties
	 */
	
	// Get weapon socket location & rotation
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag
	);
	FRotator ProjectileRotation = (ProjectileTargetLocation - SocketLocation ).Rotation();
	
	if (bPitchOverride)
	{
		ProjectileRotation.Pitch = OverridePitch;
	}

	// Set projectile location & rotation
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(ProjectileRotation.Quaternion());

	/*
	 * Spawn projectile
	 */
	
	// Spawn projectile but not immediately since we are going to configure projectile properties
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn	
	);
	
	// Set damage effect params to projectile
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(nullptr);

	Projectile->FinishSpawning(SpawnTransform);
}
