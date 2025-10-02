// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

protected:
	UPROPERTY(EditAnywhere, Category="Properties | Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
protected:
	virtual void BeginPlay() override;

};
