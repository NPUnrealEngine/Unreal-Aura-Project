// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/**
	 * Call when gameplay effect applied with its tags
	 */
	FEffectAssetTags EffectAssetTags;
	
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
	
protected:
	FDelegateHandle EffectAppliedDelegateHandle;
	
protected:
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
