// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UGameplayEffect;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

public:
	class UAttributeSet* GetAttributeSet() const {return AttributeSet;}
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, Category="Properties | Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/**
	 * Character's default primary attributes
	 *
	 * Note: The effect must be type of instant effect in order
	 * to change attributes permanently 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	/**
	 * Character's default secondary attributes
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	/**
	 * Apply gameplay effect to character itself
	 * 
	 * @param GameplayEffectClass class of gameplay effect
	 * @param Level level
	 */
	void ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	/**
	 * Initialize default attributes:
	 *   - Primary attributes
	 *   - Secondary attributes
	 */
	void InitializeDefaultAttributes() const;
};
