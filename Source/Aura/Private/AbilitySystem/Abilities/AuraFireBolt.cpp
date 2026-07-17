// NP Game Developer


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScalableDamage =  Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Fire Bolt</>\n\n"
			
			// Detail
			"<Small>Level</> <Level>%d</>\n"
			"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
			"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
			
			// Description
			"<Default>Launch a bolt of fire, exploding on impact and dealing:</> <Damage>%d</> "
			"<Default>fire damage with a chance to burn</>"), 
			
			// Values
			Level, 
			ManaCost,
			Cooldown,
			ScalableDamage);
	}
	
	return FString::Printf(TEXT(
		// Title
		"<Title>Fire Bolt</>\n\n"
		
		// Detail
		"<Small>Level</> <Level>%d</>\n"
		"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
		"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
		
		// Description
		"<Default>Launch %d bolts of fire, exploding on impact and dealing:</> <Damage>%d</> "
		"<Default>fire damage with a chance to burn</>"), 
		
		// Values
		Level, 
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles), 
		ScalableDamage);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScalableDamage =  Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>Next Level</>\n\n"
		
		// Detail
		"<Small>Level</> <Level>%d</>\n"
		"<Small>Mana Cost</> <ManaCost>%.1f</>\n"
		"<Small>Cooldown</> <ManaCost>%.1f</>\n\n"
		
		// Description
		"<Default>Launch %d bolts of fire, exploding on impact and dealing:</> <Damage>%d</> "
		"<Default>fire damage with a chance to burn</>"),
		
		// Values
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles), 
		ScalableDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bPitchOverride, float OverridePitch, AActor* HomingTarget)
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
	if (bPitchOverride) ProjectileRotation.Pitch = OverridePitch;
	
	const FVector ForwardVector = ProjectileRotation.Vector();
	const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = ForwardVector.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);
	
	// NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	
	
	if (NumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			
			UKismetSystemLibrary::DrawDebugArrow(
				GetAvatarActorFromActorInfo(),
				SocketLocation,
				SocketLocation + Direction * 75.f,
				5.f,
				FLinearColor::Red,
				120.f,
				2.f
			);
		}
	}
	else
	{
		// Single projectile		
	}
	
	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		SocketLocation,
		SocketLocation + ProjectileRotation.Vector() * 100.f,
		5.f,
		FLinearColor::Gray,
		120.f,
		2.f
	);
	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		SocketLocation,
		SocketLocation + LeftOfSpread * 100.f,
		5.f,
		FLinearColor::Gray,
		120.f,
		2.f
	);
	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		SocketLocation,
		SocketLocation + RightOfSpread * 100.f,
		5.f,
		FLinearColor::Gray,
		120.f,
		2.f
	);
}
