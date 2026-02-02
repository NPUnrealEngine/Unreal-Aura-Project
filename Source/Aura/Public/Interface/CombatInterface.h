// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

/**
 * A struct that associate a gameplay tag with an animation montage
 */
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Get player level
	 * @return level
	 */
	virtual int32 GetPlayerLevel();

	/**
	 * Fire position of socket location from the weapon
	 * held by character
	 * 
	 * @return Location
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	/**
	 * Related to motion wrapping component
	 * Tell character to rotate and facing target while in motion wrapping state
	 *
	 * e.g: Casting spell while rotate toward target
	 * 
	 * @param TargetLocation target in which character need to face to
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);

	/**
	 * Get animation montage for hit react
	 * @return Animation montage
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	/**
	 * Make character die
	 */
	virtual void Die() = 0;

	/**
	 * Check whether the character is dead or not
	 * @return 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	/**
	 * Get the actor of this character
	 * @return 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	/**
	 * Get a collection of attack montages
	 * @return A list of struct which associate a gameplay tag with animation montage
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
};
