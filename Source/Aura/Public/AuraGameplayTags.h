// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

/* 
 * Legacy code no longer used but for education purpose 
 */

/**
 * Macro to declare damage and resistance type
 * @param Type type of damage and resistance e.g Fire, Ice
 */
/*#define DECLARE_DAMAGE_TYPE_AND_RESISTANCE(Type) \
	FGameplayTag Damage_##Type; \
	FGameplayTag Attributes_Resistance_##Type; \*/

/**
 * Macro to define damage and resistance type
 * @param Type type of damage and resistance e.g Fire, Ice
 * 
 * Note: use DECLARE_DAMAGE_TYPE_AND_RESISTANCE to declare type first
 */
/*#define DEFINE_DAMAGE_TYPE_AND_RESISTANCE(Type) \
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
};*/

/*
 * Declare necessary Gameplay Tags
 */

/*
 * Primary attributes
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor);

/*
 * Secondary attributes
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration);

/*
 * Meta attributes
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Meta_IncomingXP);

/*
 * Game ability startup/initial input tag
 * 
 * A Gameplay input tag associate with a Game ability
 * when the ability is given
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Passive_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Passive_2);

/*
 * Damage type
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Lightning);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Arcane);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);

/*
 * Resistance type
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Fire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Lightning);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Arcane);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Physical);

/*
 * Ability
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_None);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Attack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Summon);
	
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_HitReact);
	
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Locked);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Eligible);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Unlocked);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Equipped);
	
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_Offensive);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_Passive);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_None);
	
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Fire_FireBolt);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Lightning_Electrocute);

/*
 * Debuff
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Arcane);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Physical);
	
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Chance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Frequency);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Duration);

/*
 * Ability Cooldown
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Fire_FireBolt);

/*
 * Animation Montage Attack
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_2);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_3);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_4);

/*
 * Combat Sockets
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Weapon);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_RightHand);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_LeftHand);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Tail);

/*
 * Effects
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effects_HitReact);

/*
 * Player
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputPressed);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputHeld);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputReleased);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_CursorTrace);

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
	/**
	 * A mapper for DamageTypes to Resistances
	 */
	TMap<FGameplayTag,FGameplayTag> DamageTypesToResistances;
	
	/*
	 * A mapper for DamageTypes to Debuffs
	 */
	TMap<FGameplayTag,FGameplayTag> DamageTypesToDebuffs;
	
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
