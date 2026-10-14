// NP Game Developer


#include "Actors/Decal/SpellDecal.h"
#include "Components/DecalComponent.h"


FSpellDecalSize::FSpellDecalSize()
	:DecalSize(FVector(100.f, 100.f, 100.f))
{
}

FSpellDecalSize::FSpellDecalSize(const FVector2D& Size, float Height)
{
	DecalSize.X = Height;
	DecalSize.Y = Size.Y;
	DecalSize.Z = Size.X;
}

ASpellDecal::ASpellDecal()
{
	PrimaryActorTick.bCanEverTick = true;
	DecalComponent = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	DecalComponent->SetupAttachment(GetRootComponent());
	
	/*
	 * Because spell decal can only be placed on ground so rotate 90 degree on Y(Pitch)
	 */
	DecalComponent->SetRelativeRotationExact(FRotator(90.f, 0.f, 0.f));
}

void ASpellDecal::SetDecalSize_Implementation(FSpellDecalSize& SpellDecalSize)
{
	DecalComponent->DecalSize = SpellDecalSize.DecalSize;
}

