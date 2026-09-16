// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()

public:
	AMagicCircle();

public:
	virtual void Tick(float DeltaTime) override;
	
public:
	FORCEINLINE UDecalComponent* GetDecalComponent() const {return MagicCircleDecal;}
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;
protected:
	virtual void BeginPlay() override;


};
