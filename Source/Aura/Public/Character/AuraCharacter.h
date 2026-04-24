// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	
public: // IPlayerInterface
	virtual void AddToXP_Implementation(int32 InXP) override;
	
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
