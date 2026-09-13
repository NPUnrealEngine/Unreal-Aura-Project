// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataFragment.generated.h"

/**
 * This is abstract class
 * 
 * Subclass of this class in order to represent a data fragment
 * in UDataFragmentContainer
 */
UCLASS(Blueprintable, Blueprintable, EditInlineNew, DefaultToInstanced, Abstract)
class AURA_API UDataFragment : public UObject
{
	GENERATED_BODY()
};
