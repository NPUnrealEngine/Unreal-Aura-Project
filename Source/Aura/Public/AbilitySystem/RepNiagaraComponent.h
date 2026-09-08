// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "RepNiagaraComponent.generated.h"


/**
 * A replicatable version of UNiagaraComponent
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API URepNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URepNiagaraComponent();

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	/**
	 * Whether this niagara effect is activated or not
	 * Replicatable property 
	 */
	UPROPERTY(ReplicatedUsing=OnRep_IsActivated)
	bool bIsActivated = false;
	
protected:
	/**
	 * Call to activate Niagara effect with network replication
	 */
	virtual void ActivateRep();
	
	/**
	 * Call to deactivate Niagara effect with network replication
	 */
	virtual void DeactivateRep();

	/**
	 * Called when client side received replicated bIsActivated value changed
	 * from server
	 * 
	 * Do rollback here to sync with server
	 * @param bActivate 
	 */
	virtual void Client_ActivationStateChanged(bool bActivate);
	
private:
	/**
	 * Client only callback function for client when bIsActivated changed on server side
	 * replicate to client
	 */
	UFUNCTION()
	virtual void OnRep_IsActivated();
};
