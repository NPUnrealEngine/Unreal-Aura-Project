// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

public:
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;
	
public:
	class UAttributeSet* GetAttributeSet() const {return AttributeSet;}

public:
	/* IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* IAbilitySystemInterface */
	
	/* ICombatInterface */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	/* ICombatInterface */

	/**
	 * Handle the character death
	 * 
	 * Run on server and all clients
	 */
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

protected:
	
	bool bDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Properties | Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category="Properties | Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category="Properties | Combat")
	FName RightHandSocketName;
	
	UPROPERTY(EditAnywhere, Category="Properties | Combat")
	FName LeftHandSocketName;

	/**
	 * Ability System Component of the character
	 */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/**
	 * AttributeSet of the character 
	 */
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

	/**
	 * Character's default vital attributes
	 * 
	 * Normally Health, MaxHealth, Mana, MaxMana 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	/**
	 * Material instance for dissolving character
	 */
	UPROPERTY(editAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	/**
	 * Material instance for dissolving character's weapon
	 */
	UPROPERTY(editAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;

	/**
	 * Dissolve character
	 * 
	 * Changing character mesh's material
	 */
	void Dissolve();

	/**
	 * Start dissolve effect on character 
	 * 
	 * Blueprint implemented only
	 * 
	 * @param DynamicMaterialInstance Material instance
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	/**
	 * Start dissolve effect on character's weapon 
	 * 
	 * Blueprint implemented only
	 * 
	 * @param DynamicMaterialInstance Material instance
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

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
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();

private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
