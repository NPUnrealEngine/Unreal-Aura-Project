// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Macro to declare damage and resistance type
 * @param Type type of damage and resistance e.g Fire, Ice
 */
#define DECLARE_DAMAGE_TYPE_AND_RESISTANCE(Type) \
	FGameplayTag Damage_##Type; \
	FGameplayTag Attributes_Resistance_##Type; \

/**
 * Macro to define damage and resistance type
 * @param Type type of damage and resistance e.g Fire, Ice
 * 
 * Note: use DECLARE_DAMAGE_TYPE_AND_RESISTANCE to declare type first
 */
#define DEFINE_DAMAGE_TYPE_AND_RESISTANCE(Type) \
{ \
	FName DamageName = "Damage."#Type; \
	FString DamageDesc = #Type" damage type"; \
	FName ResistName = "Attributes.Resistance."#Type; \
	FString ResistDesc = "Resistance to "#Type; \
	GameplayTags.Damage_##Type = UGameplayTagsManager::Get().AddNativeGameplayTag( \
		FName(DamageName), \
		FString(DamageDesc) \
	); \
	GameplayTags.Attributes_Resistance_##Type = UGameplayTagsManager::Get().AddNativeGameplayTag( \
		FName(ResistName), \
		FString(ResistDesc) \
	); \
};

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 * Declare gameplay tags and add those tags to
 * UGameplayTagsManager
 */
struct FAuraGameplayTags
{
public:
	/*
	 * Primary attributes
	 */
	
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	/*
	 * Secondary attributes
	 */
	
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;

	/*
	 * Game ability startup/initial input tag
	 * 
	 * A Gameplay input tag associate with a Game ability
	 * when the ability is given
	 */
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	/**
	 * Parent Damage gameplay tag
	 * contain a list of damage type
	 * 
	 * E.g:
	 * 
	 * Damage.Fire, Damage.Ice
	 */
	FGameplayTag Damage;
	
	/*
	 * Damage types
	 */
	
	/* Easy way to declare damage and resistance with macros */
	DECLARE_DAMAGE_TYPE_AND_RESISTANCE(Fire);
	DECLARE_DAMAGE_TYPE_AND_RESISTANCE(Lightning);
	DECLARE_DAMAGE_TYPE_AND_RESISTANCE(Arcane);
	DECLARE_DAMAGE_TYPE_AND_RESISTANCE(Physical);
	
	/*FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;*/
	
	/*
	 * Damage type resistances
	 */
	
	/*FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;*/
	
	FGameplayTag Abilities_Attack;

	/**
	 * A mapper for DamageTypes to Resistances
	 */
	TMap<FGameplayTag,FGameplayTag> DamageTypesToResistances;
	
	/*
	 * Tag for getting hit
	 */
	
	FGameplayTag Effects_HitReact;
	
public:
	static const FAuraGameplayTags& Get() {return GameplayTags;}

	/**
	 * Initialize Gameplay tags and add those tags to
	 * UGameplayTagsManager
	 */
	static void InitializeNativeGameplayTags();
	
private:
	static FAuraGameplayTags GameplayTags;
};
