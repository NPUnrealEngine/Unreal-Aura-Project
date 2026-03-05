// NP Game Developer


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSummonLocations()
{
	/* Get owing character's forward vector and location */
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	/* Evenly spread minions; how many angle to rotate per minion */
	const float DeltaSpread = SpawnSpread / NumMinions;
	
	TArray<FVector> SpawnLocations;
	
	const FVector LeftSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	for (int32 i = 0; i < NumMinions; i++)
	{
		// Rotate to new direction
		const FVector Direction = LeftSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		
		// Calculate location and add it to spawned location
		FVector ChosenSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		
		// Calculate Z axis
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			ChosenSpawnLocation + FVector(0.f, 0.f, 400.f),
			ChosenSpawnLocation - FVector(0.f, 0.f, 400.f),
			ECollisionChannel::ECC_Visibility
		);
		if (HitResult.bBlockingHit)
		{
			ChosenSpawnLocation.Z = HitResult.ImpactPoint.Z;
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
		
		/* Draw debug */
		if (DrawDebug)
		{
			DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 15.f, 12, FColor::Blue, false, 5.f);
			UKismetSystemLibrary::DrawDebugArrow(
				GetAvatarActorFromActorInfo(),
				Location,
				Location + Direction * MaxSpawnDistance,
				4.f,
				FColor::Green,
				3.f
			);
			DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Red, false, 5.f);
			DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Red, false, 5.f);
		}
	}
	
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 SelectedIndex =  FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[SelectedIndex];
}
