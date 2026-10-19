// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "OcclusionAwarePlayerController.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class AMagicCircle;
class UNiagaraSystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZoomInSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZoomOutSignature);

class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UAuraInputConfig;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public AOcclusionAwarePlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

public:
	UPROPERTY(BlueprintAssignable)
	FZoomInSignature ZoomInDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FZoomOutSignature ZoomOutDelegate;
	
public:
	/**
	 * Set particular collision for cursor trace under mouse
	 * @param Channel Channel to set
	 */
	FORCEINLINE void SetCursorTraceUnderMouseChannel(ECollisionChannel Channel)
	{
		CursorTraceChannelUnderMouse = Channel;
	}
	
	/**
	 * Set cursor trace channel under mouse back to ECC_Visibility
	 */
	FORCEINLINE void SetCursorTraceUnderMouseChannelToDefault() { CursorTraceChannelUnderMouse = ECC_Visibility; }
	
public: // Override
	virtual void PlayerTick(float DeltaTime) override;
	
public:
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float Damage, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	/**
	 * Add input blocking tags
	 * 
	 * @param TagsToAdd 
	 */
	UFUNCTION(BlueprintCallable)
	void AddInputBlockingTags(FGameplayTagContainer TagsToAdd);

	/**
	 * Remove input blocking tags
	 * 
	 * @param TagsToRemove 
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveInputBlockingTags(FGameplayTagContainer TagsToRemove);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	/**
	 * Current used channel for tracing object hit under cursor mouse
	 */
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<ECollisionChannel> CursorTraceChannelUnderMouse = ECollisionChannel::ECC_Visibility;
	
protected: // Override
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	/** Input mapping context */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> AuraInputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> ShiftAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> ZoomAction;

	TScriptInterface<class IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
	FHitResult CursorHit;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	/**
	 * A reference to AuraAbilitySystemComponent
	 *
	 * Do not access directly instead of  using GetASC() method
	 */
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	/**
	 * Final destination where character will move to
	 */
	FVector CachedDestination = FVector::ZeroVector;

	/**
	 * Total time spend of character moving key held down 
	 */
	float FollowTime = 0.0f;

	/**
	 * Threshold to determine whether character need to follow destination
	 * or move to destination automatically
	 */
	float ShortPressThreshold = 0.5f;

	/**
	 * Auto moving character to destination or follow destination manually
	 */
	bool bAutoRunning = false;

	/**
	 * Whether the cursor is hovering over enemy or not
	 */
	bool bTargeting = false;

	/**
	 * In auto running movement.
	 * 
	 * How far the distance between target point and character should be
	 * in order to be accepted as reaching target point 
	 */
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	/**
	 * A spline component
	 *
	 * Used to smooth out the path along multiple target points in the path
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	bool bShiftKeyDown = false;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	
	UPROPERTY(VisibleInstanceOnly)
	FGameplayTagCountContainer InputBlockingTags;
	
private:
	void Move(const struct FInputActionValue& InputActionValue);
	void Zoom(const FInputActionValue& InputActionValue);
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	/**
	 * Get aura ability system component
	 * 
	 * @return UAuraAbilitySystemComponent
	 */
	UAuraAbilitySystemComponent* GetASC();

	/**
	 * Monitoring cursor hit
	 * 
	 * Called every ticks
	 */
	void CursorTrace();
	
	/**
	 * Auto run for character
	 * 
	 * Called every ticks
	 */
	void AutoRun();
};
