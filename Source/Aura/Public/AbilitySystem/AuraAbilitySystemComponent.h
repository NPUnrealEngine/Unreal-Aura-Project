// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FAbilityGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

/**
 * Ability status changed delegate
 * @param First-FGameplayTag is Ability Tag
 * @param Second-FGameplayTag is Status Tag
 */
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /* Ability Tag */, const FGameplayTag& /* Status Tag*/, int32 AbilityLevel /* Ability level*/);

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

	/**
	 * Call when ability status changed
	 */
	FAbilityStatusChanged AbilityStatusChanged;
	
	bool bStartupAbilityGiven = false;
	
public: // Override
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

public:
	/**
	 * Add startup abilities
	 * 
	 * @param StartupAbilities List of abilities 
	 */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	/**
	 * Add startup passive abilities
	 * @param StartupAbilities 
	 */
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

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
	 * Get ability input tag from the ability spec
	 * @param AbilitySpec 
	 * @return FGameplayTag
	 */
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/**
	 * Get ability status tag from the ability spec
	 * @param AbilitySpec 
	 * @return 
	 */
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/**
	 * Get ability spec by ability tag
	 * @param AbilityTag Ability tag for searching
	 * @return 
	 */
	FGameplayAbilitySpec* GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag);

	/**
	 * Upgrade an attribute
	 * @param AttributeTag 
	 */
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	/**
	 * Update ability status
	 * @param Level player level
	 */
	void UpdateAbilityStatus(int32 Level);

	/**
	 * Spend a point in a spell on server side
	 * @param AbilityTag ability tag
	 */
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);
	
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
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
};
