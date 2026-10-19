// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TargetData_MouseCursor.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetData_SpellDecalUnderMouse.generated.h"

class UInputAction;
class ASpellDecal;
class AAuraPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmPressedSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancelPressedSignature);

/**
 * 
 */
UCLASS()
class AURA_API UTargetData_SpellDecalUnderMouse : public UTargetData_MouseCursor
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
	
public: // Factory
	/**
	 * Create a task that handle spell decal which follow player cursor pointing.
	 * Allow player to bind input action to confirm and cancel.
	 * 
	 * Client: Show spell decal and update its location and send cursor hit location data to
	 * server.
	 * 
	 * Server: Do not show spell decal but only wait for cursor hit location data arrived from
	 * client.
	 * 
	 * @param OwningAbility The ability own this task
	 * @param SpellDecalClass Spell decal class
	 * @param InConfirmAction Action for confirm the decal location
	 * @param InCancelAction Action for cancel
	 * @param InSpellDecalRadius [Optional] Radius of spell decal
	 * @param InSpellDecalHeight [Optional] Height of spell decal
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Task", 
		meta=(DisplayName="SpellDecalUnderMouse", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static UTargetData_SpellDecalUnderMouse* CreateSpellDecalUnderMouse(UGameplayAbility* OwningAbility, 
		TSubclassOf<ASpellDecal> SpellDecalClass, UInputAction* InConfirmAction=nullptr, 
		UInputAction* InCancelAction=nullptr, float InSpellDecalRadius=100.f, 
		float InSpellDecalHeight=50.f);
	
protected:
	/**
	 * Whether cancel action pressed or not
	 */
	bool bIsCancelled = false;
	
protected:
	void BindInputActions();
	void RemoveBindingInputActions();
	
protected: // Override
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	virtual void GetMouseCursorHitResult(FHitResult& HitResult) override;
	virtual void OnTargetHitDataArrived(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) override;
	

protected:
	/**
	 * Confirm action input callback.
	 * 
	 * Bind this callback on client only
	 * @param InputActionValue 
	 */
	virtual void OnConfirm(const FInputActionValue& InputActionValue);

	/**
	 * Cancel action input callback.
	 * 
	 * Bind this callback on client only
	 * @param InputActionValue 
	 */
	virtual void OnCancel(const FInputActionValue& InputActionValue);
	
private:
	/**
	 * Cached for binding input action handler id
	 */
	TArray<uint32> InputActionBindingHandles;
};
