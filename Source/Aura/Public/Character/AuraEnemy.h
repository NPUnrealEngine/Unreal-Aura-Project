// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interface/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();
	
public:
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

#pragma region InterfaceOverride
	/* CombatInterface */
	
	virtual int32 GetPlayerLevel() override;

	/* End CombatInterface*/
#pragma endregion 

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Default")
	int32 Level = 1;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
