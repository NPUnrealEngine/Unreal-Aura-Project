// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

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
	FVector GetCombatSocketLocation();

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
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
	
	virtual void Die() = 0;
};
