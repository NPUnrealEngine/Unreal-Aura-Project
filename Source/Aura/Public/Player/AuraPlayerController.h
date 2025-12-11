// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UAuraInputConfig;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
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
	
private:
	void Move(const struct FInputActionValue& InputActionValue);
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	/**
	 * Get aura ability system component
	 * 
	 * @return UAuraAbilitySystemComponent
	 */
	UAuraAbilitySystemComponent* GetASC();

	void AutoRun();
};
