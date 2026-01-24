// NP Game Developer


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/**
	 * Primary attributes
	 */
	
	GameplayTags.Attributes_Primary_Strength =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increase physical damage")
	);

	GameplayTags.Attributes_Primary_Intelligence =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increase magical damage")
	);

	GameplayTags.Attributes_Primary_Resilience =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increase Armor and Armor Penetration")
	);

	GameplayTags.Attributes_Primary_Vigor =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increase Health")
	);

	/**
	 * Secondary attributes
	 */
	
	GameplayTags.Attributes_Secondary_Armor =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduce damage taken, increase Block Chance")
	);

	GameplayTags.Attributes_Secondary_ArmorPenetration =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignore percentage of enemy armor and increase critical hit chance")
	);

	GameplayTags.Attributes_Secondary_BlockChance =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half")
	);

	GameplayTags.Attributes_Secondary_CriticalHitChance =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus")
	);

	GameplayTags.Attributes_Secondary_CriticalHitDamage =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored")
	);

	GameplayTags.Attributes_Secondary_CriticalHitResistance =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduce critical hit chance of attacking enemies")
	);

	GameplayTags.Attributes_Secondary_HealthRegeneration =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of health to regenerate for every 1 second")
	);

	GameplayTags.Attributes_Secondary_ManaRegeneration =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of mana to regenerate for every 1 second")
	);

	GameplayTags.Attributes_Secondary_MaxHealth =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum health")
	);

	GameplayTags.Attributes_Secondary_MaxMana =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum mana")
	);

	/**
	 * Input Tags
	 */
	
	GameplayTags.InputTag_LMB =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input tag for left mouse button")
	);

	GameplayTags.InputTag_RMB =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input tag for right mouse button")
	);

	GameplayTags.InputTag_1 =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input tag 1 key")
	);

	GameplayTags.InputTag_2 =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input tag 2 key")
	);

	GameplayTags.InputTag_3 =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input tag 3 key")
	);

	GameplayTags.InputTag_4 =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input tag 4 key")
	);
	
	/*
	 * Damage and types
	 */
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
	);
	
	/* Easy way to define damage and resistance type with macros */
	DEFINE_DAMAGE_TYPE_AND_RESISTANCE(Fire);
	DEFINE_DAMAGE_TYPE_AND_RESISTANCE(Lightning);
	DEFINE_DAMAGE_TYPE_AND_RESISTANCE(Arcane);
	DEFINE_DAMAGE_TYPE_AND_RESISTANCE(Physical);
	
	/*GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire damage type")
	);
	
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning damage type")
	);
	
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane damage type")
	);
	
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical damage type")
	);*/
	
	/*
	 * Resistance
	 */
	
	/*GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire")
	);
	
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning")
	);
	
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("Resistance to Arcane")
	);
	
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical")
	);*/
	
	/*
	 * Mapping damage type to resistance 
	 */
	
	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Fire,
		GameplayTags.Attributes_Resistance_Fire
	);
	
	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Lightning,
		GameplayTags.Attributes_Resistance_Lightning
	);
	
	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Arcane,
		GameplayTags.Attributes_Resistance_Arcane
	);
	
	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Physical,
		GameplayTags.Attributes_Resistance_Physical
	);
	
	/*
	 * Effects
	 */
	
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when hit react")
	);
	
	/*
	 * Abilities
	 */
	
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack ability tag")
	);
}
