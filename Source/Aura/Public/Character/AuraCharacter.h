// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	
public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

#pragma region InterfaceOverride
	/* CombatInterface*/
	
	virtual int32 GetPlayerLevel() override;

	/* End CombatInterface*/
#pragma endregion 

protected:
	virtual void InitAbilityActorInfo() override;
};
