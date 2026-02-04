// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Character Class Defaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Character Class Defaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Character Class Defaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlocked);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Effect")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCritical);

	/**
	 * Get all actors who is alive within a sphere radius
	 * 
	 * @param WorldContextObject 
	 * @param OutOverlappingActors 
	 * @param ActorsToIgnore 
	 * @param Radius 
	 * @param SphereOrigin 
	 */
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | Gameplay Machanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary | Gameplay Machanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);
private:
	/**
	 * Create a GameplayEffectContextHandle with SourceObject
	 * @param ASC Ability System Component
	 * @param SourceObject Optional.
	 * The object that this effect created from
	 * @return FGameplayEffectContextHandle
	 */
	static FGameplayEffectContextHandle MakeEffectContextHandle(UAbilitySystemComponent* ASC, const UObject* SourceObject = nullptr);
};
