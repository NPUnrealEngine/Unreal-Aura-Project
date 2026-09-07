// NP Game Developer


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interface/CombatInterface.h"
#include "Net/UnrealNetwork.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
	
	// Set replication as default
	SetIsReplicatedByDefault(true);
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		GetOwner()))
	{
		OwnerASC->RegisterGameplayTagEvent(
			DebuffTag, 
			EGameplayTagEventType::NewOrRemoved
		).AddUObject(this, &UDebuffNiagaraComponent::OnDebuffTagChanged);
	}
	
	// is not necessary to do so as we are using replication from server to client
	/*else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, 
			[this](UAbilitySystemComponent* InASC)
			{
				InASC->RegisterGameplayTagEvent(
					DebuffTag, 
					EGameplayTagEventType::NewOrRemoved
				).AddUObject(this, &UDebuffNiagaraComponent::OnDebuffTagChanged);
			}
		);
	}*/
	
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(
			this, 
			&UDebuffNiagaraComponent::UDebuffNiagaraComponent::OnOwnerDeath
		);
	}
}

void UDebuffNiagaraComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UDebuffNiagaraComponent, bIsActivated);
}

void UDebuffNiagaraComponent::OnDebuffTagChanged(const FGameplayTag InDebuffTag, int32 NewCount)
{
	const bool bIsValidOwner = IsValid(GetOwner());
	const bool bIsAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	if (NewCount > 0 && bIsValidOwner && bIsAlive)
	{
		ActivateDebuffEffect();
	}else
	{
		DeactivateDebuffEffect();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	DeactivateDebuffEffect();
}

void UDebuffNiagaraComponent::OnRep_IsActivated()
{
	if (bIsActivated)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::ActivateDebuffEffect()
{
	// Only run on server side
	if (GetOwner()->HasAuthority())
	{
		bIsActivated = true;
		Activate();
	}
}

void UDebuffNiagaraComponent::DeactivateDebuffEffect()
{
	// Only run on server side
	if (GetOwner()->HasAuthority())
	{
		bIsActivated = false;
		Deactivate();
	}
}
