// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/RepNiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"


UCLASS()
class AURA_API UDebuffNiagaraComponent : public URepNiagaraComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;
	
protected: // Override
	virtual void BeginPlay() override;
	
protected:
	void OnDebuffTagChanged(const FGameplayTag InDebuffTag, int32 NewCount);
	
	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
