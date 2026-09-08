// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/RepNiagaraComponent.h"
#include "PassiveNiagaraComponent.generated.h"


UCLASS()
class AURA_API UPassiveNiagaraComponent : public URepNiagaraComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;
	
protected: // Override
	virtual void BeginPlay() override;
	
protected:
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
};
