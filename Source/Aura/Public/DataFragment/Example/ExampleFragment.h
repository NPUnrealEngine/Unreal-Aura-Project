// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "DataFragment/DataFragment.h"
#include "ExampleFragment.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AURA_API UExampleFragment : public UDataFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText ExampleName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ExampleAge = 0;
};
