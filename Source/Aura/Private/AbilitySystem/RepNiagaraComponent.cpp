// NP Game Developer


#include "AbilitySystem/RepNiagaraComponent.h"

#include "Net/UnrealNetwork.h"

URepNiagaraComponent::URepNiagaraComponent()
{
	bAutoActivate = false;
	// Set replication as default
	SetIsReplicatedByDefault(true);
}

void URepNiagaraComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(URepNiagaraComponent, bIsActivated);
}

void URepNiagaraComponent::OnRep_IsActivated()
{
	if (bIsActivated)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
	Client_ActivationStateChanged(bIsActivated);
}

void URepNiagaraComponent::ActivateRep()
{
	// Only run on server side
	if (GetOwner()->HasAuthority())
	{
		bIsActivated = true;
		Activate();
	}
}

void URepNiagaraComponent::DeactivateRep()
{
	// Only run on server side
	if (GetOwner()->HasAuthority())
	{
		bIsActivated = false;
		Deactivate();
	}
}

void URepNiagaraComponent::Client_ActivationStateChanged(bool bActivate)
{
}


