// NP Game Developer


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interface/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
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
	else if (CombatInterface)
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
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(
			this, 
			&UDebuffNiagaraComponent::UDebuffNiagaraComponent::OnOwnerDeath
		);
	}
}

void UDebuffNiagaraComponent::OnDebuffTagChanged(const FGameplayTag InDebuffTag, int32 NewCount)
{
	const bool bIsValidOwner = IsValid(GetOwner());
	const bool bIsAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	if (NewCount > 0 && bIsValidOwner && bIsAlive)
	{
		Activate();
	}else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
