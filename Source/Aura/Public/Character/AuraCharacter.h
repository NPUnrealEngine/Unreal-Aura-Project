// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interface/PlayerControlInterface.h"
#include "Interface/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UGameplayCameraComponent;
class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface, public IPlayerControlInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	
public: // IPlayerInterface
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	
public: // IPlayerControlInterface
	virtual void EnableFacingTarget_Implementation(float InRotationSpeed) override;
	virtual void DisableFacingTarget_Implementation() override;
	virtual void SetFacingTarget_Implementation(FVector InFacingTarget) override;
	
public: // Override
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void Tick(float DeltaTime) override;
	
public: // CombatInterface
	virtual int32 GetPlayerLevel_Implementation() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelUp")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	/**
	 * Should we update facing target rotation
	 */
	bool bUpdateFacingTarget = false;

	/**
	 * Constant speed for facing target rotation
	 */
	float RotateToTargetSpeed;

	/**
	 * Desired target to facing toward
	 */
	FVector TargetToFace = FVector::ZeroVector;
	
protected: // Override
	virtual void InitAbilityActorInfo() override;
	
protected:
	/**
	 * Call to update character facing toward its target
	 * 
	 * @param DeltaTime 
	 */
	virtual void FacingTarget(float DeltaTime);
	
private:
	UFUNCTION(NetMulticast, Reliable)
	void LevelUpParticlesMulticast() const;
};
