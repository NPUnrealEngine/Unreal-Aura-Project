// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerControlInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPlayerControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerControlInterface
{
	GENERATED_BODY()

public:
	/**
	 * Enable character facing target
	 * 
	 * Enable character rotation by controller
	 * @param InRotationSpeed Speed of rotation for constant rotation interpolation
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableFacingTarget(float InRotationSpeed=200.0f);
	
	/**
	 * Disable character facing target
	 * 
	 * Disable character rotation by controller
	 */
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableFacingTarget();

	/**
	 * Update character facing target
	 *
	 * @param InFacingTarget Desired target the character facing toward
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetFacingTarget(FVector InFacingTarget);
};
