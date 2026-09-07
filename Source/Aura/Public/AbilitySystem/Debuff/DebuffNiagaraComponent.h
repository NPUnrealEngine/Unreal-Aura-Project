// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"


UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UDebuffNiagaraComponent();
	
public:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UPROPERTY(ReplicatedUsing=OnRep_IsActivated)
	bool bIsActivated = false;
	
protected: // Override
	virtual void BeginPlay() override;
	
protected:
	void OnDebuffTagChanged(const FGameplayTag InDebuffTag, int32 NewCount);
	
	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);

	/**
	 * Callback function for client when bIsActivated changed on server side
	 */
	UFUNCTION()
	void OnRep_IsActivated();
	
private:
	/**
	 * Handle Niagara activation with network consideration
	 */
	void ActivateDebuffEffect();

	/**
	 * Handle Niagara deactivation with network consideration
	 */
	void DeactivateDebuffEffect();
};
