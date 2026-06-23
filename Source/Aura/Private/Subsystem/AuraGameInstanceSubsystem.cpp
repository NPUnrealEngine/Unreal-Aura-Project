// NP Game Developer


#include "Subsystem/AuraGameInstanceSubsystem.h"

#include "Aura/AuraLogChannel.h"

void UAuraGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
			FString::Printf(TEXT("%s Initialized"), *GetName()));
	}
	UE_LOG(LogAura, Warning, TEXT("%s Initialized"), *GetName());
#endif
}

void UAuraGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
			FString::Printf(TEXT("%s Deinitialized"), *GetName()));
	}
	UE_LOG(LogAura, Warning, TEXT("%s Deinitialized"), *GetName());
#endif
}

bool UAuraGameInstanceSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer))
	{
		return true;
	}
	return false;
}
