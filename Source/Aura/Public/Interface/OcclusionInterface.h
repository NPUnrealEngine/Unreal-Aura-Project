// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OcclusionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UOcclusionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IOcclusionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnOcclusionBegin();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnOcclusionEnd();
};
