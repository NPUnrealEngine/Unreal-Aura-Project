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
	static void GiveStartupAbilitie(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
	
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
