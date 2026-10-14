// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellDecal.generated.h"

/**
 * Struct represent spell decal size
 * 
 * Given size in 2d XY will be YZ
 * 
 * Given Height will be X
 */
USTRUCT(BlueprintType)
struct FSpellDecalSize
{
	GENERATED_BODY()
public:
	FSpellDecalSize();
	FSpellDecalSize(const FVector2D& Size, float Height = 50.f);
	
public:
	/**
	 * True decal size
	 */
	FVector DecalSize;
};

/**
 * Decal only design for spell
 */
UCLASS(Abstract)
class AURA_API ASpellDecal : public AActor
{
	GENERATED_BODY()

public:
	ASpellDecal();

public:
	FORCEINLINE UDecalComponent* GetDecalComponent() const {return DecalComponent;}
	
public:
	/**
	 * Set decal size
	 * @param SpellDecalSize Decal size
	 */
	UFUNCTION(blueprintNativeEvent)
	void SetDecalSize(FSpellDecalSize& SpellDecalSize);
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UDecalComponent> DecalComponent;
};