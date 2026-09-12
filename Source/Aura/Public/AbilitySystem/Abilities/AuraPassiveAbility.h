// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraPassiveAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	/**
	 * Class of gameplay effect that will be made
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
protected:
	/**
	 * Callback funtion for receive deactivate event
	 * @param AbilityTag 
	 */
	void ReceiveDeactivate(const FGameplayTag& AbilityTag);

	/**
	 * Helper function to make gameplay effect
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	FGameplayEffectSpecHandle MakeGameplayEffect();
};
