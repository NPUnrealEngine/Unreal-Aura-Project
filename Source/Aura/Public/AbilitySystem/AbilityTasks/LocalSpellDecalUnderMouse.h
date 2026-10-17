// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "LocalSpellDecalUnderMouse.generated.h"

class UInputAction;
class ASpellDecal;
class AAuraPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancelPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskDestroySignature);

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

	/**
	 * Action for confirm the location
	 */
	UPROPERTY()
	UInputAction* ConfirmAction;

	/**
	 * Action for cancel
	 */
	UPROPERTY()
	UInputAction* CancelAction;

	/**
	 * On confirm action pressed
	 */
	UPROPERTY(BlueprintAssignable)
	FOnConfirmPressedSignature OnConfirmPressed;

	/**
	 * On cancel action pressed
	 */
	UPROPERTY(BlueprintAssignable)
	FOnCancelPressedSignature OnCancelPressed;

	/**
	 * Determine whether to end task automatically when
	 * confirm or cancel action pressed
	 */
	bool bEndTaskOnConfirmOrCancelPressed = false;
	
public: // Factory
	/**
	 * Create a task that handle spell decal which follow player cursor pointing
	 * 
	 * Optionally given confirm or cancel action to bind to input key
	 * 
	 * Use this task for client only
	 * 
	 * @param OwningAbility The ability own this task
	 * @param SpellDecalClass Spell decal class
	 * @param InSpellDecalRadius [Optional] Radius of spell decal
	 * @param InSpellDecalHeight [Optional] Height of spell decal
	 * @param InConfirmAction [Optional] Action for confirm the location. If not set then you need to
	 * use node such WaitInputPress to capture ability key pressed
	 * @param InCancelAction [Optional] Action for cancel E.G End the ability
	 * @param bInEndTaskOnConfirmOrCancelPressed Whether to end task automatically when
	 * confirm or cancel action pressed, or you want to end task manually. Default to false
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Task", 
		meta=(DisplayName="LocalSpellDecalUnderMouse", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static ULocalSpellDecalUnderMouse* CreateLocalSpellDecalUnderMouse(UGameplayAbility* OwningAbility, TSubclassOf<ASpellDecal>SpellDecalClass, 
		float InSpellDecalRadius=100.f, float InSpellDecalHeight=50.f, 
		UInputAction* InConfirmAction=nullptr, UInputAction* InCancelAction=nullptr, 
		bool bInEndTaskOnConfirmOrCancelPressed = false);

protected: // Override
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
protected:
	virtual void OnConfirm(const FInputActionValue& InputActionValue);
	virtual void OnCancel(const FInputActionValue& InputActionValue);
};
