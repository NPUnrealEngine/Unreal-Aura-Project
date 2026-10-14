// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "LocalSpellDecalUnderMouse.generated.h"

class ASpellDecal;
class AAuraPlayerController;
/**
 * 
 */
UCLASS()
class AURA_API ULocalSpellDecalUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	/**
	 * Spell decal actor
	 */
	UPROPERTY()
	TObjectPtr<ASpellDecal> DecalActor;
	
	/**
	 * Reference to client's/local AuraPlayerController 
	 */
	TWeakObjectPtr<AAuraPlayerController> AuraPlayerController;

	/**
	 * Spell decal radius
	 */
	float SpellDecalRadius;

	/**
	 * Spell decal height
	 */
	float SpellDecalHeight;
	
public: // Factory
	/**
	 * Create a task that handle spell decal which follow player cursor pointing
	 * 
	 * Use this task for client only
	 * 
	 * @param OwningAbility The ability own this task
	 * @param SpellDecalClass Spell decal class
	 * @param InSpellDecalRadius [Optional] Radius of spell decal
	 * @param InSpellDecalHeight [Optional] Height of spell decal
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Task", 
		meta=(DisplayName="LocalSpellDecalUnderMouse", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static ULocalSpellDecalUnderMouse* CreateLocalSpellDecalUnderMouse(UGameplayAbility* OwningAbility, TSubclassOf<ASpellDecal>SpellDecalClass, float InSpellDecalRadius=100.f, float InSpellDecalHeight=50.f);

protected: // Override
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
};
