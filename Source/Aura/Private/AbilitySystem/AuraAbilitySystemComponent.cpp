// NP Game Developer


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannel.h"
#include "Interface/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/AuraGameInstanceSubsystem.h"

void UAuraAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	/* Listen to gameplay effect applied event */
	if (!EffectAppliedDelegateHandle.IsValid())
	{
		EffectAppliedDelegateHandle = OnGameplayEffectAppliedDelegateToSelf.AddUObject(
			this,
			&UAuraAbilitySystemComponent::ClientEffectApplied
			);
	}
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	// On Server
	
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			// Associate input tag with the ability
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);
			
			// Associate equipped tag with the ability
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	
	bStartupAbilityGiven = true;
	AbilityGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagPress(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				TArray<UGameplayAbility*> AbilityInstances = AbilitySpec.GetAbilityInstances();
				for (UGameplayAbility* Instance : AbilityInstances)
				{
					InvokeReplicatedEvent(
						EAbilityGenericReplicatedEvent::InputPressed, 
						AbilitySpec.Handle,
						Instance->GetCurrentActivationInfo().GetActivationPredictionKey()
					);
				}
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			
			TArray<UGameplayAbility*> AbilityInstances = AbilitySpec.GetAbilityInstances();
			for (UGameplayAbility* Instance : AbilityInstances)
			{
				InvokeReplicatedEvent(
					EAbilityGenericReplicatedEvent::InputReleased, 
					AbilitySpec.Handle,
					Instance->GetCurrentActivationInfo().GetActivationPredictionKey()
				);
			}
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	// Lock abilities to prevent being modified at run-time
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	// Loop through all activatable abilities
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// Notify the delegate about the ability spec
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Fail to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability->GetAssetTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag("Abilities.Status")))
		{
			return StatusTag;
		}	
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability->GetAssetTags())
		{
			if (Tag.MatchesTagExact(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		return GetStatusTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatus(int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (FAuraAbilityInfo& Info : AbilityInfo->GetAbilityInformation())
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		if (GetAbilitySpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(
				Info.AbilityTag, 
				Abilities_Status_Eligible,
				1
			);
		}
	}
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
	FString& OutNextLevelDescription)
{
	/*
	 * When ability is not locked
	 */
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	
	/*
	 * When ability is locked
	 */
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(Abilities_None))
	{
		OutDescription = FString();
	}
	else
	{
		if (AbilityInfo)
		{
			OutDescription = UAuraGameplayAbility::GetLockedDescription(
				AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement
			);	
		}else
		{
			OutDescription = FString();
		}
		
		OutNextLevelDescription = FString();
	}
	
	return false;
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, StatusTag, SlotTag, PreviousSlotTag);
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* AbilitySpec)
{
	/*
	 * Clear ability spec's input tag
	 */
	const FGameplayTag& SlotTag = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(SlotTag);
}

void UAuraAbilitySystemComponent::ClearAbilityOfSlot(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&AbilitySpec, SlotTag))
		{
			ClearSlot(&AbilitySpec);
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& SlotTag)
{
	for (const FGameplayTag& Tag : AbilitySpec->GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTagExact(SlotTag)) return true;
	}
	return false;
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                    const FGameplayTag& SlotTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag& PreviousSlotTag = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& StatusTag = GetStatusTagFromSpec(*AbilitySpec);
		const bool bStatusValid = StatusTag == Abilities_Status_Equipped || 
			StatusTag == Abilities_Status_Unlocked;
		
		if (bStatusValid)
		{
			ClearAbilityOfSlot(SlotTag);
			
			// Clear this ability's input tag just in case 
			ClearSlot(AbilitySpec);
			
			// Assign new input tag to this ability
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(SlotTag);
			
			// Change ability's unlocked status to equipped if necessary
			if (StatusTag.MatchesTagExact(Abilities_Status_Unlocked))
			{
				AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Abilities_Status_Unlocked);
				AbilitySpec->GetDynamicSpecSourceTags().AddTag(Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, Abilities_Status_Equipped, SlotTag, PreviousSlotTag);
	}
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag);
	if (AbilitySpec)
	{
		// Deduct 1 point from total spell points
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}
		
		FGameplayTag AbilityStatusTag = GetStatusTagFromSpec(*AbilitySpec);
		const FAuraGameplayTags GamePlayTags = FAuraGameplayTags::Get();
		
		/*
		 * Depend on ability status tag we do following case:
		 *	1. If eligible: change ability status tag to unlocked
		 *	2. If equipped or unlocked: add 1 level to ability
		 */
		if (AbilityStatusTag.MatchesTagExact(Abilities_Status_Eligible))
		{
			AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Abilities_Status_Eligible);
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(Abilities_Status_Unlocked);
			AbilityStatusTag = Abilities_Status_Unlocked;
		}
		else if (AbilityStatusTag.MatchesTagExact(Abilities_Status_Equipped) ||
			AbilityStatusTag.MatchesTagExact(Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		
		// Tell client an ability status had been updated
		ClientUpdateAbilityStatus(
			AbilityTag,
			AbilityStatusTag,
			AbilitySpec->Level
		);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetAvatarActor(), 
		AttributeTag, 
		Payload
	);
	
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	// On client
	if (!bStartupAbilityGiven)
	{
		bStartupAbilityGiven = true;
		AbilityGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
