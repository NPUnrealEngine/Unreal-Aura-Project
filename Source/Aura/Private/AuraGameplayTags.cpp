// NP Game Developer


#include "AuraGameplayTags.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

/* 
 * Define necessary Gameplay Tags 
 */

/**
 * Primary attributes
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Strength, "Attributes.Primary.Strength",
	"Increase physical damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence",
	"Increase magical damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Resilience, "Attributes.Primary.Resilience",
	"Increase Armor and Armor Penetration");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Vigor, "Attributes.Primary.Vigor",
	"Increase Health");

/*
 * Secondary attributes
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_Armor, "Attributes.Secondary.Armor",
	"Reduce damage taken, increase Block Chance");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration",
	"Ignore percentage of enemy armor and increase critical hit chance");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance",
	"Chance to cut incoming damage in half");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance",
	"Chance to double damage plus critical hit bonus");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage",
	"Bonus damage added when a critical hit is scored");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance",
	"Reduce critical hit chance of attacking enemies");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration",
	"Amount of health to regenerate for every 1 second");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration", 
	"Amount of mana to regenerate for every 1 second");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", 
	"Maximum health");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana",
	"Maximum mana");

/*
 * Meta Attributes
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Meta_IncomingXP, "Attributes.Meta.IncomingXP",
	"Incoming XP");

/*
 * Input Tags
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LMB, "InputTag.LMB", 
	"Input tag for left mouse button");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_RMB, "InputTag.RMB", 
	"Input tag for right mouse button");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_1, "InputTag.1", 
	"Input tag 1 key");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_2, "InputTag.2", 
	"Input tag 2 key");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_3, "InputTag.3", 
	"Input tag 3 key");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_4, "InputTag.4", 
	"Input tag 4 key");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Passive_1, "InputTag.Passive.1", 
	"Input tag passive 1");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Passive_2, "InputTag.Passive.2", 
	"Input tag passive 2");

/*
 * Damage type
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Fire, "Damage.Fire", 
	"Damage Fire type");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Lightning, "Damage.Lightning", 
	"Damage Lightning type");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Arcane, "Damage.Arcane", 
	"Damage Arcane type");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Physical, "Damage.Physical", 
	"Damage Physical type");

/*
 * Resistance type
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Fire, "Attributes.Resistance.Fire", 
	"Resistance to Fire");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Lightning, "Attributes.Resistance.Lightning", 
	"Resistance to Lightning");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Arcane, "Attributes.Resistance.Arcane", 
	"Resistance to Arcane");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Physical, "Attributes.Resistance.Physical", 
	"Resistance to Physical");

/*
 * Abilities
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_None, "Abilities.None", 
	"None ability tag");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Attack, "Abilities.Attack", 
	"Attack ability tag");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Summon, "Abilities.Summon", 
	"Summon ability tag");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_HitReact, "Abilities.HitReact", 
	"Hit react ability tag");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Eligible, "Abilities.Status.Eligible", 
	"Eligible status");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Equipped, "Abilities.Status.Equipped", 
	"Equipped status");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Locked, "Abilities.Status.Locked", 
	"Locked status");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Unlocked, "Abilities.Status.Unlocked", 
	"Unlocked status");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Type_None, "Abilities.Type.None", 
	"Ability type none");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Type_Offensive, "Abilities.Type.Offensive", 
	"Ability type offensive");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Type_Passive, "Abilities.Type.Passive", 
	"Ability type passive");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Fire_FireBolt, "Abilities.Fire.FireBolt", 
	"FireBolt ability tag");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Lightning_Electrocute, "Abilities.Lightning.Electrocute", 
	"Electrocute ability tag");

/*
 * Debuff
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Arcane, "Debuff.Arcane", 
	"Debuff for Arcane damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Burn, "Debuff.Burn", 
	"Debuff for Fire damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Physical, "Debuff.Physical", 
	"Debuff for Physical damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Stun, "Debuff.Stun", 
	"Debuff for Lightning damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Chance, "Debuff.Chance", 
	"Debuff chance");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Damage, "Debuff.Damage", 
	"Debuff damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Duration, "Debuff.Duration", 
	"Debuff duration");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Frequency, "Debuff.Frequency", 
	"Debuff frequency");

/**
 * Ability Cooldown
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_Fire_FireBolt, "Cooldown.Fire.FireBolt", 
	"FireBolt cooldown tag");

/*
 * Animation Montage Attack
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_1, "Montage.Attack.1", 
	"Montage attack 1");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_2, "Montage.Attack.2", 
	"Montage attack 2");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_3, "Montage.Attack.3", 
	"Montage attack 3");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_4, "Montage.Attack.4", 
	"Montage attack 4");

/*
 * Combat socket
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_Weapon, "CombatSocket.Weapon", 
	"Weapon");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_RightHand, "CombatSocket.RightHand", 
	"Right hand");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_LeftHand, "CombatSocket.LeftHand", 
	"Left hand");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_Tail, "CombatSocket.Tail", 
	"Tail");

/*
 * Effects
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effects_HitReact, "Effects.HitReact", 
	"Tag granted when hit react");

/*
 * Player
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_InputPressed, "Player.Block.InputPressed", 
	"Block mouse input pressed");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_InputHeld, "Player.Block.InputHeld",
	"Block mouse input held");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_InputReleased, "Player.Block.InputReleased", 
	"Block mouse input released");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_CursorTrace, "Player.Block.CursorTrace",
	"Block trace under mouse cursor");

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Mapping damage type to resistance 
	 */
	GameplayTags.DamageTypesToResistances.Add(Damage_Fire, Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(Damage_Lightning, Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(Damage_Arcane, Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(Damage_Physical, Attributes_Resistance_Physical);
	
	/*
	 * Mapping DamageType to Debuffs
	 */
	GameplayTags.DamageTypesToDebuffs.Add(Damage_Fire, Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Add(Damage_Lightning, Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(Damage_Arcane, Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(Damage_Physical, Debuff_Physical);
}
