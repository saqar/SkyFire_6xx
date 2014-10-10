/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _UPDATEFIELDS_H
#define _UPDATEFIELDS_H

// Auto generated for version 5.4.7 18019

enum EObjectFields
{
    OBJECT_FIELD_GUID                                        = 0x0, // Size: 2, Flags: UF_FLAG_PUBLIC
    OBJECT_FIELD_DATA                                        = 0x2, // Size: 2, Flags: UF_FLAG_PUBLIC
    OBJECT_FIELD_TYPE                                        = 0x4, // Size: 1, Flags: UF_FLAG_PUBLIC
    OBJECT_FIELD_ENTRY_ID                                    = 0x5, // Size: 1, Flags: UF_FLAG_VIEWER_DEPENDENT
    OBJECT_FIELD_DYNAMIC_FLAGS                               = 0x6, // Size: 1, Flags: UF_FLAG_VIEWER_DEPENDENT, UF_FLAG_URGENT
    OBJECT_FIELD_SCALE                                       = 0x7, // Size: 1, Flags: UF_FLAG_PUBLIC
    OBJECT_END                                               = 0x8,
};

enum EItemFields
{
    ITEM_FIELD_OWNER                                         = OBJECT_END + 0x00, // Size: 2, Flags: UF_FLAG_PUBLIC
    ITEM_FIELD_CONTAINED_IN                                  = OBJECT_END + 0x02, // Size: 2, Flags: UF_FLAG_PUBLIC
    ITEM_FIELD_CREATOR                                       = OBJECT_END + 0x04, // Size: 2, Flags: UF_FLAG_PUBLIC
    ITEM_FIELD_GIFT_CREATOR                                  = OBJECT_END + 0x06, // Size: 2, Flags: UF_FLAG_PUBLIC
    ITEM_FIELD_STACK_COUNT                                   = OBJECT_END + 0x08, // Size: 1, Flags: UF_FLAG_OWNER
    ITEM_FIELD_EXPIRATION                                    = OBJECT_END + 0x09, // Size: 1, Flags: UF_FLAG_OWNER
    ITEM_FIELD_SPELL_CHARGES                                 = OBJECT_END + 0x0A, // Size: 5, Flags: UF_FLAG_OWNER
    ITEM_FIELD_DYNAMIC_FLAGS                                 = OBJECT_END + 0x0F, // Size: 1, Flags: UF_FLAG_PUBLIC
    ITEM_FIELD_ENCHANTMENT                                   = OBJECT_END + 0x10, // Size: 39, Flags: UF_FLAG_PUBLIC
    ITEM_FIELD_PROPERTY_SEED                                 = OBJECT_END + 0x37, // Size: 1, Flags: UF_FLAG_PUBLIC
    ITEM_FIELD_RANDOM_PROPERTIES_ID                          = OBJECT_END + 0x38, // Size: 1, Flags: UF_FLAG_PUBLIC
    ITEM_FIELD_DURABILITY                                    = OBJECT_END + 0x39, // Size: 1, Flags: UF_FLAG_OWNER
    ITEM_FIELD_MAX_DURABILITY                                = OBJECT_END + 0x3A, // Size: 1, Flags: UF_FLAG_OWNER
    ITEM_FIELD_CREATE_PLAYED_TIME                            = OBJECT_END + 0x3B, // Size: 1, Flags: UF_FLAG_PUBLIC
    ITEM_FIELD_MODIFIERS_MASK                                = OBJECT_END + 0x3C, // Size: 1, Flags: UF_FLAG_OWNER
    ITEM_END                                                 = OBJECT_END + 0x3D,
};

enum ItemDynamicFields
{
    ITEM_DYNAMIC_MODIFIERS = 0x0,
    ITEM_DYNAMIC_END = 0x1
};

enum EContainerFields
{
    CONTAINER_FIELD_SLOTS                                    = ITEM_END + 0x00, // Size: 72, Flags: UF_FLAG_PUBLIC
    CONTAINER_FIELD_NUM_SLOTS                                = ITEM_END + 0x48, // Size: 1, Flags: UF_FLAG_PUBLIC
    CONTAINER_END                                            = ITEM_END + 0x49
};

enum EUnitFields
{
    UNIT_FIELD_CHARM                                         = OBJECT_END + 0x00, // Size: 2, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_SUMMON                                        = OBJECT_END + 0x02, // Size: 2, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_CRITTER                                       = OBJECT_END + 0x04, // Size: 2, Flags: UF_FLAG_PRIVATE
    UNIT_FIELD_CHARMED_BY                                    = OBJECT_END + 0x06, // Size: 2, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_SUMMONED_BY                                   = OBJECT_END + 0x08, // Size: 2, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_CREATED_BY                                    = OBJECT_END + 0x0A, // Size: 2, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_DEMON_CREATOR                                 = OBJECT_END + 0x0C, // Size: 2, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_TARGET                                        = OBJECT_END + 0x0E, // Size: 2, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_BATTLE_PET_COMPANION_GUID                     = OBJECT_END + 0x10, // Size: 2, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_CHANNEL_OBJECT                                = OBJECT_END + 0x12, // Size: 2, Flags: UF_FLAG_PUBLIC, UF_FLAG_URGENT
    UNIT_FIELD_CHANNEL_SPELL                                 = OBJECT_END + 0x14, // Size: 1, Flags: UF_FLAG_PUBLIC, UF_FLAG_URGENT
    UNIT_FIELD_SUMMONED_BY_HOME_REALM                        = OBJECT_END + 0x15, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_SEX                                           = OBJECT_END + 0x16, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_DISPLAY_POWER                                 = OBJECT_END + 0x17, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_OVERRIDE_DISPLAY_POWER_ID                     = OBJECT_END + 0x18, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_HEALTH                                        = OBJECT_END + 0x19, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_POWER                                         = OBJECT_END + 0x1A, // Size: 5, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_MAX_HEALTH                                    = OBJECT_END + 0x1F, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_MAX_POWER                                     = OBJECT_END + 0x20, // Size: 5, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER                     = OBJECT_END + 0x25, // Size: 5, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER, UF_FLAG_UNIT_ALL
    UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER         = OBJECT_END + 0x2A, // Size: 5, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER, UF_FLAG_UNIT_ALL
    UNIT_FIELD_LEVEL                                         = OBJECT_END + 0x2F, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_EFFECTIVE_LEVEL                               = OBJECT_END + 0x30, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_FACTION_TEMPLATE                              = OBJECT_END + 0x31, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_VIRTUAL_ITEM_ID                               = OBJECT_END + 0x32, // Size: 3, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_FLAGS                                         = OBJECT_END + 0x35, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_FLAGS2                                        = OBJECT_END + 0x36, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_AURA_STATE                                    = OBJECT_END + 0x37, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_ATTACK_ROUND_BASE_TIME                        = OBJECT_END + 0x38, // Size: 2, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_RANGED_ATTACK_ROUND_BASE_TIME                 = OBJECT_END + 0x3A, // Size: 1, Flags: UF_FLAG_PRIVATE
    UNIT_FIELD_BOUNDING_RADIUS                               = OBJECT_END + 0x3B, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_COMBAT_REACH                                  = OBJECT_END + 0x3C, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_DISPLAY_ID                                    = OBJECT_END + 0x3D, // Size: 1, Flags: UF_FLAG_VIEWER_DEPENDENT, UF_FLAG_URGENT
    UNIT_FIELD_NATIVE_DISPLAY_ID                             = OBJECT_END + 0x3E, // Size: 1, Flags: UF_FLAG_PUBLIC, UF_FLAG_URGENT
    UNIT_FIELD_MOUNT_DISPLAY_ID                              = OBJECT_END + 0x3F, // Size: 1, Flags: UF_FLAG_PUBLIC, UF_FLAG_URGENT
    UNIT_FIELD_MIN_DAMAGE                                    = OBJECT_END + 0x40, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER, UF_FLAG_EMPATH
    UNIT_FIELD_MAX_DAMAGE                                    = OBJECT_END + 0x41, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER, UF_FLAG_EMPATH
    UNIT_FIELD_MIN_OFF_HAND_DAMAGE                           = OBJECT_END + 0x42, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER, UF_FLAG_EMPATH
    UNIT_FIELD_MAX_OFF_HAND_DAMAGE                           = OBJECT_END + 0x43, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER, UF_FLAG_EMPATH
    UNIT_FIELD_ANIM_TIER                                     = OBJECT_END + 0x44, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_PET_NUMBER                                    = OBJECT_END + 0x45, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_PET_NAME_TIMESTAMP                            = OBJECT_END + 0x46, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_PET_EXPERIENCE                                = OBJECT_END + 0x47, // Size: 1, Flags: UF_FLAG_OWNER
    UNIT_FIELD_PET_NEXT_LEVEL_EXPERIENCE                     = OBJECT_END + 0x48, // Size: 1, Flags: UF_FLAG_OWNER
    UNIT_FIELD_MOD_CASTING_SPEED                             = OBJECT_END + 0x49, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_MOD_SPELL_HASTE                               = OBJECT_END + 0x4A, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_MOD_HASTE                                     = OBJECT_END + 0x4B, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_MOD_RANGED_HASTE                              = OBJECT_END + 0x4C, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_MOD_HASTE_REGEN                               = OBJECT_END + 0x4D, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_CREATED_BY_SPELL                              = OBJECT_END + 0x4E, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_NPC_FLAGS                                     = OBJECT_END + 0x4F, // Size: 2, Flags: UF_FLAG_PUBLIC, UF_FLAG_VIEWER_DEPENDENT
    UNIT_FIELD_NPC_EMOTESTATE                                = OBJECT_END + 0x51, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_STATS                                         = OBJECT_END + 0x52, // Size: 5, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_STAT_POS_BUFF                                 = OBJECT_END + 0x57, // Size: 5, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_STAT_NEG_BUFF                                 = OBJECT_END + 0x5C, // Size: 5, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_RESISTANCES                                   = OBJECT_END + 0x61, // Size: 7, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER, UF_FLAG_EMPATH
    UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE                 = OBJECT_END + 0x68, // Size: 7, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE                 = OBJECT_END + 0x6F, // Size: 7, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_BASE_MANA                                     = OBJECT_END + 0x76, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_BASE_HEALTH                                   = OBJECT_END + 0x77, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_SHAPESHIFT_FORM                               = OBJECT_END + 0x78, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_ATTACK_POWER                                  = OBJECT_END + 0x79, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_ATTACK_POWER_MOD_POS                          = OBJECT_END + 0x7A, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_ATTACK_POWER_MOD_NEG                          = OBJECT_END + 0x7B, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_ATTACK_POWER_MULTIPLIER                       = OBJECT_END + 0x7C, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER                           = OBJECT_END + 0x7D, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS                   = OBJECT_END + 0x7E, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MOD_NEG                   = OBJECT_END + 0x7F, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER                = OBJECT_END + 0x80, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_MIN_RANGED_DAMAGE                             = OBJECT_END + 0x81, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_MAX_RANGED_DAMAGE                             = OBJECT_END + 0x82, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_POWER_COST_MODIFIER                           = OBJECT_END + 0x83, // Size: 7, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_POWER_COST_MULTIPLIER                         = OBJECT_END + 0x8A, // Size: 7, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_MAX_HEALTH_MODIFIER                           = OBJECT_END + 0x91, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_OWNER
    UNIT_FIELD_HOVER_HEIGHT                                  = OBJECT_END + 0x92, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_MIN_ITEM_LEVEL                                = OBJECT_END + 0x93, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_MAX_ITEM_LEVEL                                = OBJECT_END + 0x94, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_WILD_BATTLE_PET_LEVEL                         = OBJECT_END + 0x95, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP           = OBJECT_END + 0x96, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_FIELD_INTERACT_SPELL_ID                             = OBJECT_END + 0x97, // Size: 1, Flags: UF_FLAG_PUBLIC
    UNIT_END                                                 = OBJECT_END + 0x98
};

enum PlayerUpdateFields
{
    PLAYER_FIELD_DUEL_ARBITER                                = UNIT_END + 0x000, // Size: 4, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_WOW_ACCOUNT                                 = UNIT_END + 0x004, // Size: 4, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_LOOT_TARGET_GUID                            = UNIT_END + 0x008, // Size: 4, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_PLAYER_FLAGS                                = UNIT_END + 0x00C, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_PLAYER_FLAGS_EX                             = UNIT_END + 0x00D, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_GUILD_RANK_ID                               = UNIT_END + 0x00E, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_GUILD_DELETE_DATE                           = UNIT_END + 0x00F, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_GUILD_LEVEL                                 = UNIT_END + 0x010, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_HAIR_COLOR_ID                               = UNIT_END + 0x011, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_REST_STATE                                  = UNIT_END + 0x012, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_ARENA_FACTION                               = UNIT_END + 0x013, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_DUEL_TEAM                                   = UNIT_END + 0x014, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_GUILD_TIME_STAMP                            = UNIT_END + 0x015, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_QUEST_LOG                                   = UNIT_END + 0x016, // Size: 750, Flags: UF_FLAG_PARTY_MEMBER
    PLAYER_FIELD_VISIBLE_ITEMS                               = UNIT_END + 0x304, // Size: 57, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_PLAYER_TITLE                                = UNIT_END + 0x33D, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_FAKE_INEBRIATION                            = UNIT_END + 0x33E, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_VIRTUAL_PLAYER_REALM                        = UNIT_END + 0x33F, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_CURRENT_SPEC_ID                             = UNIT_END + 0x340, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_TAXI_MOUNT_ANIM_KIT_ID                      = UNIT_END + 0x341, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_AVG_ITEM_LEVEL_TOTAL                        = UNIT_END + 0x342, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_AVG_ITEM_LEVEL_EQUIPPED                     = UNIT_END + 0x343, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_CURRENT_BATTLE_PET_BREED_QUALITY            = UNIT_END + 0x344, // Size: 1, Flags: UF_FLAG_PUBLIC
    PLAYER_FIELD_INV_SLOTS                                   = UNIT_END + 0x345, // Size: 736, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_PACK_SLOTS                                  = UNIT_END + 0x3A1, // Size: 62, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_BANK_SLOTS                                  = UNIT_END + 0x3DF, // Size: 112, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_BANKBAG_SLOTS                               = UNIT_END + 0x44F, // Size: 24  Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_VENDORBUYBACK_SLOTS                         = UNIT_END + 0x467, // Size: 48, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_FARSIGHT_OBJECT                             = UNIT_END + 0x625, // Size: 4, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_KNOWN_TITLES                                = UNIT_END + 0x629, // Size: 10, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_COINAGE                                     = UNIT_END + 0x633, // Size: 2, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_XP                                          = UNIT_END + 0x635, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_NEXT_LEVEL_XP                               = UNIT_END + 0x636, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SKILL                                       = UNIT_END + 0x637, // Size: 448, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SKILL_LINEIDS                               = UNIT_END + 0x637, // Size: 64, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SKILL_STEPS                                 = UNIT_END + 0x677, // Size: 64, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SKILL_RANKS                                 = UNIT_END + 0x6B7, // Size: 64, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SKILL_STARTING_RANKS                        = UNIT_END + 0x6F7, // Size: 64, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SKILL_MAX_RANKS                             = UNIT_END + 0x737, // Size: 64, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SKILL_MODIFIERS                             = UNIT_END + 0x747, // Size: 64, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SKILL_TALENTS                               = UNIT_END + 0x7B7, // Size: 64, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_CHARACTER_POINTS                            = UNIT_END + 0x7F7, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MAX_TALENT_TIERS                            = UNIT_END + 0x7F8, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_TRACK_CREATURE_MASK                         = UNIT_END + 0x7F9, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_TRACK_RESOURCE_MASK                         = UNIT_END + 0x7FA, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MAINHAND_EXPERTISE                          = UNIT_END + 0x7FB, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_OFFHAND_EXPERTISE                           = UNIT_END + 0x7FC, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_RANGED_EXPERTISE                            = UNIT_END + 0x7FD, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_COMBAT_RATING_EXPERTISE                     = UNIT_END + 0x7FE, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_BLOCK_PERCENTAGE                            = UNIT_END + 0x7FF, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_DODGE_PERCENTAGE                            = UNIT_END + 0x800, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_PARRY_PERCENTAGE                            = UNIT_END + 0x801, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_CRIT_PERCENTAGE                             = UNIT_END + 0x802, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_RANGED_CRIT_PERCENTAGE                      = UNIT_END + 0x803, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_OFFHAND_CRIT_PERCENTAGE                     = UNIT_END + 0x804, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SPELL_CRIT_PERCENTAGE                       = UNIT_END + 0x805, // Size: 7, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SHIELD_BLOCK                                = UNIT_END + 0x80C, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SHIELD_BLOCK_CRIT_PERCENTAGE                = UNIT_END + 0x80D, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MASTERY                                     = UNIT_END + 0x80E, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_AMPLIFY                                     = UNIT_END + 0x80F, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MULTISTRIKE                                 = UNIT_END + 0x810, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MULTISTRIKE_EFFECT                          = UNIT_END + 0x811, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_READINESS                                   = UNIT_END + 0x812, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SPEED                                       = UNIT_END + 0x813, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_LIFESTEAL                                   = UNIT_END + 0x814, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_AVOIDANCE                                   = UNIT_END + 0x815, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_STURDINESS                                  = UNIT_END + 0x816, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_CLEAVE                                      = UNIT_END + 0x817, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_VERSATILITY                                 = UNIT_END + 0x818, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_VERSATILITY_BONUS                           = UNIT_END + 0x819, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_PVP_POWER_DAMAGE                            = UNIT_END + 0x81A, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_PVP_POWER_HEALING                           = UNIT_END + 0x81B, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_EXPLORED_ZONES                              = UNIT_END + 0x81C, // Size: 200, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_REST_STATE_BONUS_POOL                       = UNIT_END + 0x8E4, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_DAMAGE_DONE_POS                         = UNIT_END + 0x8E5, // Size: 7, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_DAMAGE_DONE_NEG                         = UNIT_END + 0x8EC, // Size: 7, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_DAMAGE_DONE_PERCENT                     = UNIT_END + 0x8F3, // Size: 7, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_HEALING_DONE_POS                        = UNIT_END + 0x8FA, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_HEALING_PERCENT                         = UNIT_END + 0x8FB, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_HEALING_DONE_PERCENT                    = UNIT_END + 0x8FC, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_PERIODIC_HEALING_DONE_PERCENT           = UNIT_END + 0x8FD, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_WEAPON_DMG_MULTIPLIERS                      = UNIT_END + 0x8FE, // Size: 3, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_WEAPON_ATK_SPEED_MULTIPLIERS                = UNIT_END + 0x901, // Size: 3, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_SPELL_POWER_PERCENT                     = UNIT_END + 0x904, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_RESILIENCE_PERCENT                      = UNIT_END + 0x905, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_OVERRIDE_SPELL_POWER_BY_APPERCENT           = UNIT_END + 0x906, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_OVERRIDE_APBY_SPELL_POWER_PERCENT           = UNIT_END + 0x907, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_TARGET_RESISTANCE                       = UNIT_END + 0x908, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE              = UNIT_END + 0x909, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_LOCAL_FLAGS                                 = UNIT_END + 0x90A, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_LIFETIME_MAX_RANK                           = UNIT_END + 0x90B, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SELF_RES_SPELL                              = UNIT_END + 0x90C, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_PVP_MEDALS                                  = UNIT_END + 0x90D, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_BUYBACK_PRICE                               = UNIT_END + 0x90E, // Size: 12, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_BUYBACK_TIMESTAMP                           = UNIT_END + 0x91A, // Size: 12, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_YESTERDAY_HONORABLE_KILLS                   = UNIT_END + 0x926, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_LIFETIME_HONORABLE_KILLS                    = UNIT_END + 0x927, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_WATCHED_FACTION_INDEX                       = UNIT_END + 0x928, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_COMBAT_RATINGS                              = UNIT_END + 0x929, // Size: 32, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_PVP_INFO                                    = UNIT_END + 0x949, // Size: 36, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MAX_LEVEL                                   = UNIT_END + 0x96D, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_RUNE_REGEN                                  = UNIT_END + 0x96E, // Size: 4, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_NO_REAGENT_COST_MASK                        = UNIT_END + 0x972, // Size: 4, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_GLYPH_SLOTS                                 = UNIT_END + 0x976, // Size: 6, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_GLYPHS                                      = UNIT_END + 0x97C, // Size: 6, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_GLYPH_SLOTS_ENABLED                         = UNIT_END + 0x982, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_PET_SPELL_POWER                             = UNIT_END + 0x983, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_RESEARCHING                                 = UNIT_END + 0x984, // Size: 10, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_PROFESSION_SKILL_LINE                       = UNIT_END + 0x98E, // Size: 2, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_UI_HIT_MODIFIER                             = UNIT_END + 0x990, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_UI_SPELL_HIT_MODIFIER                       = UNIT_END + 0x991, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_HOME_REALM_TIME_OFFSET                      = UNIT_END + 0x992, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_MOD_PET_HASTE                               = UNIT_END + 0x993, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID                    = UNIT_END + 0x994, // Size: 4, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_OVERRIDE_SPELLS_ID                          = UNIT_END + 0x998, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_UNK_WOD
    PLAYER_FIELD_LFG_BONUS_FACTION_ID                        = UNIT_END + 0x999, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_LOOT_SPEC_ID                                = UNIT_END + 0x99A, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_OVERRIDE_ZONE_PVPTYPE                       = UNIT_END + 0x99B, // Size: 1, Flags: UF_FLAG_PRIVATE, UF_FLAG_UNK_WOD
    PLAYER_FIELD_ITEM_LEVEL_DELTA                            = UNIT_END + 0x99C, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_BAG_SLOT_FLAGS                              = UNIT_END + 0x99D, // Size: 4, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_BANK_BAG_SLOT_FLAGS                         = UNIT_END + 0x9A1, // Size: 7, Flags: UF_FLAG_PRIVATE
    PLAYER_FIELD_INSERT_ITEMS_LEFT_TO_RIGHT                  = UNIT_END + 0x9A8, // Size: 1, Flags: UF_FLAG_PRIVATE
    PLAYER_END                                               = UNIT_END + 0x9A9
};
enum EGameObjectFields
{
    GAMEOBJECT_FIELD_CREATED_BY                              = OBJECT_END + 0x0, // Size: 2, Flags: UF_FLAG_PUBLIC
    GAMEOBJECT_FIELD_DISPLAY_ID                              = OBJECT_END + 0x2, // Size: 1, Flags: UF_FLAG_PUBLIC
    GAMEOBJECT_FIELD_FLAGS                                   = OBJECT_END + 0x3, // Size: 1, Flags: UF_FLAG_PUBLIC, UF_FLAG_URGENT
    GAMEOBJECT_FIELD_PARENT_ROTATION                         = OBJECT_END + 0x4, // Size: 4, Flags: UF_FLAG_PUBLIC
    GAMEOBJECT_FIELD_FACTION_TEMPLATE                        = OBJECT_END + 0x8, // Size: 1, Flags: UF_FLAG_PUBLIC
    GAMEOBJECT_FIELD_LEVEL                                   = OBJECT_END + 0x9, // Size: 1, Flags: UF_FLAG_PUBLIC
    GAMEOBJECT_FIELD_PERCENT_HEALTH                          = OBJECT_END + 0xA, // Size: 1, Flags: UF_FLAG_PUBLIC, UF_FLAG_URGENT
    GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID                   = OBJECT_END + 0xB, // Size: 1, Flags: UF_FLAG_PUBLIC, UF_FLAG_URGENT
    GAMEOBJECT_END                                           = OBJECT_END + 0xC
};

enum EDynamicObjectFields
{
    DYNAMICOBJECT_FIELD_CASTER                               = OBJECT_END + 0x0, // Size: 2, Flags: UF_FLAG_PUBLIC
    DYNAMICOBJECT_FIELD_TYPE_AND_VISUAL_ID                   = OBJECT_END + 0x2, // Size: 1, Flags: UF_FLAG_VIEWER_DEPENDENT
    DYNAMICOBJECT_FIELD_SPELL_ID                             = OBJECT_END + 0x3, // Size: 1, Flags: UF_FLAG_PUBLIC
    DYNAMICOBJECT_FIELD_RADIUS                               = OBJECT_END + 0x4, // Size: 1, Flags: UF_FLAG_PUBLIC
    DYNAMICOBJECT_FIELD_CAST_TIME                            = OBJECT_END + 0x5, // Size: 1, Flags: UF_FLAG_PUBLIC
    DYNAMICOBJECT_END                                        = OBJECT_END + 0x6
};

enum ECorpseFields
{
    CORPSE_FIELD_OWNER                                       = OBJECT_END + 0x00, // Size: 2, Flags: UF_FLAG_PUBLIC
    CORPSE_FIELD_PARTY_GUID                                  = OBJECT_END + 0x02, // Size: 2, Flags: UF_FLAG_PUBLIC
    CORPSE_FIELD_DISPLAY_ID                                  = OBJECT_END + 0x04, // Size: 1, Flags: UF_FLAG_PUBLIC
    CORPSE_FIELD_ITEMS                                       = OBJECT_END + 0x05, // Size: 19, Flags: UF_FLAG_PUBLIC
    CORPSE_FIELD_SKIN_ID                                     = OBJECT_END + 0x18, // Size: 1, Flags: UF_FLAG_PUBLIC
    CORPSE_FIELD_FACIAL_HAIR_STYLE_ID                        = OBJECT_END + 0x19, // Size: 1, Flags: UF_FLAG_PUBLIC
    CORPSE_FIELD_FLAGS                                       = OBJECT_END + 0x1A, // Size: 1, Flags: UF_FLAG_PUBLIC
    CORPSE_FIELD_DYNAMIC_FLAGS                               = OBJECT_END + 0x1B, // Size: 1, Flags: UF_FLAG_VIEWER_DEPENDENT
    CORPSE_END                                               = OBJECT_END + 0x1C
};

enum EAreaTriggerFields
{
    AREATRIGGER_FIELD_CASTER                                 = OBJECT_END + 0x0, // Size: 2, Flags: UF_FLAG_PUBLIC
    AREATRIGGER_FIELD_DURATION                               = OBJECT_END + 0x2, // Size: 1, Flags: UF_FLAG_PUBLIC
    AREATRIGGER_FIELD_SPELL_ID                               = OBJECT_END + 0x3, // Size: 1, Flags: UF_FLAG_PUBLIC
    AREATRIGGER_FIELD_SPELL_VISUAL_ID                        = OBJECT_END + 0x4, // Size: 1, Flags: UF_FLAG_VIEWER_DEPENDENT
    AREATRIGGER_FIELD_EXPLICIT_SCALE                         = OBJECT_END + 0x5, // Size: 1, Flags: UF_FLAG_PUBLIC, UF_FLAG_URGENT
    AREATRIGGER_END                                          = OBJECT_END + 0x6
};

enum ESceneObjectFields
{
    SCENEOBJECT_FIELD_SCRIPT_PACKAGE_ID                      = OBJECT_END + 0x0, // Size: 1, Flags: UF_FLAG_PUBLIC
    SCENEOBJECT_FIELD_RND_SEED_VAL                           = OBJECT_END + 0x1, // Size: 1, Flags: UF_FLAG_PUBLIC
    SCENEOBJECT_FIELD_CREATED_BY                             = OBJECT_END + 0x2, // Size: 2, Flags: UF_FLAG_PUBLIC
    SCENEOBJECT_FIELD_SCENE_TYPE                             = OBJECT_END + 0x4, // Size: 1, Flags: UF_FLAG_PUBLIC
    SCENEOBJECT_FIELD_END                                    = OBJECT_END + 0x5
};

#endif // _UPDATEFIELDS_H
