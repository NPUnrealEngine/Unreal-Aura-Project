// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityGiven, UAuraAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/**
	 * Delegate for gameplay effect applied with its tags
	 */
	FEffectAssetTags EffectAssetTags;

	/**
	 * Call when startup abilities has given
	 */
	FAbilityGiven AbilityGivenDelegate;
	
	bool bStartupAbilityGiven = false;
	
public:
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	/**
	 * Add startup abilities
	 * 
	 * @param StartupAbilities List of abilities 
	 */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	/**
	 * Tell AbilitySystemComponent a key associated gameplay tag
	 * had held and need to activate ability
	 * @param InputTag Gameplay tag
	 */
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	/**
	 * Tell AbilitySystemComponent a key associated gameplay tag
	 * had released
	 * @param InputTag Gameplay tag
	 */
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/**
	 * Loop through all activatable abilities and notify
	 * the given delegate
	 * @param Delegate For callback
	 */
	void ForEachAbility(const FForEachAbility& Delegate);

	/**
	 * Get ability tag from the ability spec
	 * @param AbilitySpec 
	 * @return FGameplayTag
	 */
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/**
	 * Get ability input tag from the ability space
	 * @param AbilitySpec 
	 * @return FGameplayTag
	 */
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
protected:
	FDelegateHandle EffectAppliedDelegateHandle;
	
protected:
	virtual void OnRep_ActivateAbilities() override;
	
	/**
	 * Remote Procedure Call to apply gameplay effect
	 *
	 * This is a client only method which will be called
	 * on the owning client(Included listen server not dedicate server)
	 * when gameplay effect applied on ability system component
	 * 
	 * @param AbilitySystemComponent 
	 * @param EffectSpec 
	 * @param ActiveEffectHandle 
	 */
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
