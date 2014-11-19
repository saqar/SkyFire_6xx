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

#ifndef TRINITY_DB2STRUCTURE_H
#define TRINITY_DB2STRUCTURE_H

#include "Common.h"
#include "ItemPrototype.h"
#include "NPCHandler.h"

// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

// Structures used to access raw DB2 data and required packing to portability
struct BattlePetAbilityEntry
{
    uint32 AbilityId;                                       // 0 - battle pet ability id
    int32 FamilyId;                                         // 1 - battle pet family id or -1 for aura
    uint32 IconId;                                          // 2 - icon id
    uint32 Cooldown;                                        // 3 - cooldown in turns
    //uint32 Unk1;                                          // 4 - something to do with linked abilities
    //uint32 Unk2;                                          // 5
    LocalizedString* Name;                                  // 6 - name text
    LocalizedString* Description;                           // 7 - description text
};

struct BattlePetAbilityStateEntry
{
    uint32 Id;                                              // 0
    uint32 AbilityId;                                       // 1 - battle pet ability id (BattlePetAbility.db2)
    uint32 StateId;                                         // 2 - battle pet state id (BattlePetState.db2)
    uint32 Value;                                           // 3 - value for state
};

struct BattlePetBreedStateEntry
{
    uint32 Id;                                              // 0
    uint32 BreedId;                                         // 1 - battle pet breed id
    uint32 StateId;                                         // 2 - battle pet state id (BattlePetState.db2)
    uint32 Value;                                           // 3 - value for state
};

typedef std::set<uint32> BattlePetBreedSet;

struct BattlePetSpeciesEntry
{
    uint32 SpeciesId;                                       // 0 - battle pet species id
    uint32 NpcId;                                           // 1 - npc id
    uint32 IconId;                                          // 2 - icon id
    uint32 SpellId;                                         // 3 - summon spell id (Spell.dbc)
    uint32 FamilyId;                                        // 4 - battle pet family id
    //int32 Unk1;                                           // 5
    uint32 Flags;                                           // 6 - flags (see BattlePetSpeciesFlags enum)
    LocalizedString* Description;                           // 7 - description text, contains method to obtain and cost
    LocalizedString* Flavor;                                // 8 - flavor text
};

struct BattlePetSpeciesStateEntry
{
    uint32 Id;                                              // 0
    uint32 SpeciesId;                                       // 1 - battle pet species id (BattlePetSpecies.db2)
    uint32 StateId;                                         // 2 - battle pet state id (BattlePetState.db2)
    int32 Modifier;                                         // 3 - modifier value for state
};

struct BattlePetSpeciesXAbilityEntry
{
    uint32 Id;                                              // 0
    uint32 SpeciesId;                                       // 1 - battle pet species id (BattlePetSpecies.db2)
    uint32 AbilityId;                                       // 2 - battle pet ability id (BattlePetAbility.db2)
    uint32 RequiredLevel;                                   // 3 - required level to use this ability
    uint32 SlotId;                                          // 4 - ability slot id (0-2)
};

struct BattlePetStateEntry
{
    uint32 StateId;                                         // 0 - battle pet state id
    //uint32 Unk1;                                          // 1 - only states 21 and 34 (linked states?)
    LocalizedString* Name;                                  // 2 - name text
    uint32 Flags;                                           // 3 - flags
};

typedef std::map<uint32, uint16> BattlePetItemXSpeciesStore;

struct ItemToBattlePetEntry
{
    uint32 ItemId;                                          // 0 - item id
    uint32 SpeciesId;                                       // 1 - battle pet species id (BattlePetSpecies.db2)
};

struct BroadcastTextEntry
{
    uint32   Id;                                            // 0
    uint32   Language;                                      // 1
    LocalizedString* MaleText;                              // 2
    LocalizedString* FemaleText;                            // 3
    uint32   Emote[MAX_GOSSIP_TEXT_EMOTES];                 // 4-6
    uint32   EmoteDelay[MAX_GOSSIP_TEXT_EMOTES];            // 7-9
    uint32   SoundId;                                       // 10
    uint32   EndEmoteId;                                    // 11
    uint32   Type;                                          // 12
};

struct GarrAbilityEntry
{
    uint32 ID;                                              // AbilityID
    uint32 Type;                                            // Type 1 - Trait, 2 - Spell
    std::string Name;                                       // Ability's name
    std::string Description;                                // Ability's description
    uint32 SpellID;                                         // Ability's spellID for ingame use
    uint32 Data0;                                           // Unknown
    uint32 Category;                                        // Ability's category
};

struct GarrAbilityCategoryEntry
{
    uint32 ID;                                              // Category ID
    std::string Name;                                       // Category Name
};

struct GarrBuildingEntry
{
    uint32 ID;                                              // Building's entry ID
    uint32 SpawnIDH;                                        // Building's spawn ID Horde
    uint32 SpawnIDA;                                        // Building's spawn ID Alliance
    uint32 Data0;                                           // 3 - Unknown, all are 0
    uint32 Data1;                                           // 4 - Unknown, all ranks of one building type share this ID. Probably something important.
    uint32 BuildingRank;                                    // Building's Rank. 1 - Small, 2 - Medium, 3 - Large.
    std::string BuildingNameH;                              // Building's name Horde
    std::string BuildingNameA;                              // Building's name Alliance
    std::string Description;                                // Description of the building
    std::string Effect;                                     // Building's bonus effect
    uint32 BuildTime;                                       // Building's required build time
    uint32 Data2;                                           // 11 - Unknown all are 824. Maybe experience relted.
    uint32 ResourcesCost;                                   // Building's cost of Garrison Resources.
    uint32 Data3;                                           // 13 - Unknown
    uint32 Data4;                                           // 14 - Unknown 
    uint32 DisplayID;                                       // Building's DisplayID
    uint32 Data6;                                           // 16 - Unknown
    uint32 Data7;                                           // 17 - Unknown, may relate to default buildings
    uint32 Data8;                                           // 18 - Unknown
    uint32 Data9;                                           // 19 - Unknown 
    uint32 WorkOrders;                                      // Building's number of work orders
    uint32 Data10;                                          // 21 - Unknown
    uint32 Data11;                                          // 22 - Unknown, all 0
    uint32 GoldCost;                                        // Building's gold cost
};

struct GarrClassSpecEntry
{
    uint32 ID;                                              // EntryID
    std::string Name;                                       // Name
    uint32 Data0;                                           // Unknown
    uint32 Data1;                                           // Unknown
    uint32 ClassID;                                         // ClassID
    uint32 SpecID;                                          // SpecID
};

struct GarrEncounterEntry
{
    uint32 ID;                                              // Encounter ID
    uint32 CreatureID;                                      // Creature ID
    std::string Name;                                       // Encounter Name
    float Data0;                                            // Unknown
    float Data1;                                            // Unknown
    uint32 Data2;                                           // Unknown
};

struct GarrFollItemSetEntry
{
    uint32 ID;                                              // ID
};

struct GarrFollowerEntry
{
    uint32 ID;                                              // ID
    uint32 DisplayIDH;                                      // Horde Display ID
    uint32 DisplayIDA;                                      // Alliance Display ID
    uint32 ClassH;                                          // Class Horde
    uint32 ClassA;                                          // Class Aliance
    uint32 Quality;                                         // Follower Quality
    uint32 SpecH;                                           // Spec Horde
    uint32 SpecA;                                           // Spec Alliance
    uint32 Data0;                                           // Unknown H
    uint32 Data1;                                           // Unknown A
    uint32 Level;                                           // Follower Level
    uint32 ItemLevelH;                                      // Follower Item Level Horde
    uint32 ItemLevelA;                                      // Follower Item Level Alliance
    uint32 Data2;                                           // Unknown
    uint32 Data3;                                           // Unknown
    std::string GossipH;                                    // Gossip for Horde, includes Quests, achievements, and vendors. Not really sure.
    std::string GossipA;                                    // Gossip for Alliance, includes Quests, achievements, and verndors. Again, not sure.
    uint32 Data4;                                           // Unknown most -1 H
    uint32 Data5;                                           // Unknown most -1 A
    uint32 UnkIDH;                                          // Unknown H ID, has a slot in most entries.
    uint32 UnkIDA;                                          // Unknown A ID, has a slot in most entries.
};

struct GarrFollowerLevelXPEntry
{
    uint32 ID;                                              // EntryID
    uint32 Level;                                           // Level
    uint32 XPRequired;                                      // Required experience for level
};

struct GarrFollowerQualityEntry
{
    uint32 ID;                                              // ID
    uint32 QualityID;                                       // QualityID
    uint32 Quality;                                         // Quality
};

struct GarrMissionEntry
{
    uint32 ID;                                              // EntryID
    uint32 Level;                                           // Required Level
    uint32 ItemLevel;                                       // Required Item Level
    uint32 Type;                                            // Mission Type
    uint32 Followers;                                       // Max followers
    uint32 Data0;                                           // Unknown
    uint32 Time;                                            // Time for completion
    uint32 Data1;                                           // Unknown
    uint32 Data2;                                           // Unknown 
    uint32 Category;                                        // Mission Category
    std::string Name;                                       // Mission Name
    std::string Description;                                // Mission Description
    std::string Location;                                   // Mission Location
    uint32 Data3;                                           // Unknown
    uint32 Resources;                                       // Garrison Resources awarded
    uint32 BonusReward;                                     // Has bonus reward?
    uint32 Experience;                                      // Experience awarded
    uint32 BonusChance;                                     // Percentage of chance for bonus reward
};

struct GarrMissionTypeEntry
{
    uint32 ID;                                              // EntryID
    std::string Name;                                       // Mission type name
    uint32 MissionData;                                     // Uknown data, really should know this.
};

struct GarrMechanicEntry
{
    uint32 ID;                                              // ID
    uint32 Data0;                                           // Unknown
    float Data1;                                            // Probably mechanic coefficient
};

struct GarrPlotEntry
{
    uint32 ID;                                              // PlotTypeID
    uint32 PlotSize;                                        // PlotSize
    uint32 EntryID;                                         // EntryID
    uint32 Data0;                                           // Unknown, all 0.
    std::string PlotType;                                   // PlotType
    uint32 Data1;                                           // Unknown
    uint32 Data2;                                           // Unknown
    uint32 Data3;                                           // Unknown
    uint32 Data4;                                           // Unknown
};

struct GarrPlotBuildingEntry
{
    uint32 ID;                                              // ID
    uint32 PlotType;                                        // PlotTypeID
    uint32 BuildingId;                                      // BuildingID
};

struct GarrPlotUICategoryEntry
{
    uint32 ID;                                              // PlotID
    std::string Name;                                       // Plot Size Name
    uint32 Data;                                            // Unknown
};

struct GarrSpecializationEntry
{
    uint32 ID;                                              // ID
    uint32 SpecializationID;                                // Specialization ID
    uint32 Data0;                                           // Unknown
    uint32 Data1;                                           // Unknown 
    uint32 Data2;                                           // Unknown
    float Data3;                                            // Unknown
    uint32 Data4;                                           // Unknown
    std::string Name;                                       // Specialization Name
    std::string Description;                                // Specialization Description
};

struct ItemEntry
{
    uint32   ID;                                             // 0
    uint32   Class;                                          // 1
    uint32   SubClass;                                       // 2
    int32    SoundOverrideSubclass;                          // 3
    int32    Material;                                       // 4
    uint32   InventoryType;                                  // 5
    uint32   Sheath;                                         // 6
    uint32   AppearenceId;                                   // 7
};

struct ItemAppearanceEntry
{
    uint32 ID;
    uint32 displayId;
    // uint32 appearenceID;
};

struct ItemModifiedAppearanceEntry
{
    uint32 itemID;
    uint32 modifiedAppearanceID;
};

struct ItemCurrencyCostEntry
{
    //uint32  Id;
    uint32  ItemId;
};

struct ItemSparseEntry
{
    uint32     ID;                                           // 0
    uint32     Quality;                                      // 1
    uint32     Flags;                                        // 2
    uint32     Flags2;                                       // 3
    uint32     Flags3;                                       // 4
    float      Unk430_1;                                     // 5
    float      Unk430_2;                                     // 6
    uint32     BuyCount;                                     // 7
    uint32     BuyPrice;                                     // 8
    uint32     SellPrice;                                    // 9
    uint32     InventoryType;                                // 10
    int32      AllowableClass;                               // 11
    int32      AllowableRace;                                // 12
    uint32     ItemLevel;                                    // 13
    int32      RequiredLevel;                                // 14
    uint32     RequiredSkill;                                // 15
    uint32     RequiredSkillRank;                            // 16
    uint32     RequiredSpell;                                // 17
    uint32     RequiredHonorRank;                            // 18
    uint32     RequiredCityRank;                             // 19
    uint32     RequiredReputationFaction;                    // 20
    uint32     RequiredReputationRank;                       // 21
    uint32     MaxCount;                                     // 22
    uint32     Stackable;                                    // 23
    uint32     ContainerSlots;                               // 24
    int32      ItemStatType[MAX_ITEM_PROTO_STATS];           // 25 - 34
    uint32     ItemStatValue[MAX_ITEM_PROTO_STATS];          // 35 - 44
    int32      ScalingValue[MAX_ITEM_PROTO_STATS];           // 45 - 54
    float      SocketCostRate[MAX_ITEM_PROTO_STATS];         // 55 - 64
    uint32     ScalingStatDistribution;                      // 65
    uint32     DamageType;                                   // 66
    uint32     Delay;                                        // 67
    float      RangedModRange;                               // 68
    /* Moved to another DBC file
    int32      SpellId[MAX_ITEM_PROTO_SPELLS];               // 69 - 73
    int32      SpellTrigger[MAX_ITEM_PROTO_SPELLS];          // 74 - 78
    int32      SpellCharges[MAX_ITEM_PROTO_SPELLS];          // 79 - 83
    int32      SpellCooldown[MAX_ITEM_PROTO_SPELLS];         // 84 - 88
    int32      SpellCategory[MAX_ITEM_PROTO_SPELLS];         // 89 - 89
    int32      SpellCategoryCooldown[MAX_ITEM_PROTO_SPELLS]; // 94 - 98
    */
    uint32     Bonding;                                      // 99
    LocalizedString* Name;                                   // 100
    LocalizedString* Name2;                                  // 101
    LocalizedString* Name3;                                  // 102
    LocalizedString* Name4;                                  // 103
    LocalizedString* Description;                            // 104
    uint32     PageText;                                     // 105
    uint32     LanguageID;                                   // 106
    uint32     PageMaterial;                                 // 107
    uint32     StartQuest;                                   // 108
    uint32     LockID;                                       // 109
    int32      Material;                                     // 110
    uint32     Sheath;                                       // 111
    uint32     RandomProperty;                               // 112
    uint32     RandomSuffix;                                 // 113
    uint32     ItemSet;                                      // 114
    uint32     Area;                                         // 115
    uint32     Map;                                          // 116
    uint32     BagFamily;                                    // 117
    uint32     TotemCategory;                                // 118
    uint32     Color[MAX_ITEM_PROTO_SOCKETS];                // 119 - 121
    //uint32     Content[MAX_ITEM_PROTO_SOCKETS];            // 122 - 124
    int32      SocketBonus;                                  // 125
    uint32     GemProperties;                                // 126
    float      ArmorDamageModifier;                          // 127
    uint32     Duration;                                     // 128
    uint32     ItemLimitCategory;                            // 129
    uint32     HolidayId;                                    // 130
    float      StatScalingFactor;                            // 131
    int32      CurrencySubstitutionId;                       // 132
    int32      CurrencySubstitutionCount;                    // 133
};

#define MAX_ITEM_EXT_COST_ITEMS         5
#define MAX_ITEM_EXT_COST_CURRENCIES    5

struct ItemExtendedCostEntry
{
    uint32      ID;                                         // 0 extended-cost entry id
    //uint32    reqhonorpoints;                             // 1 required honor points
    //uint32    reqarenapoints;                             // 2 required arena points
    uint32      RequiredArenaSlot;                          // 3 arena slot restrictions (min slot value)
    uint32      RequiredItem[MAX_ITEM_EXT_COST_ITEMS];      // 4-8 required item id
    uint32      RequiredItemCount[MAX_ITEM_EXT_COST_ITEMS]; // 9-13 required count of 1st item
    uint32      RequiredPersonalArenaRating;                // 14 required personal arena rating
    //uint32    ItemPurchaseGroup;                          // 15
    uint32      RequiredCurrency[MAX_ITEM_EXT_COST_CURRENCIES];// 16-20 required curency id
    uint32      RequiredCurrencyCount[MAX_ITEM_EXT_COST_CURRENCIES];// 21-25 required curency count
    uint32      RequiredFactionId;
    uint32      RequiredFactionStanding;
    uint32      RequirementFlags;
    uint32      RequiredGuildLevel;
};

#define KEYCHAIN_SIZE   32

struct KeyChainEntry
{
    uint32      Id;
    uint8       Key[KEYCHAIN_SIZE];
};

// QuestPackageItem.db2
struct QuestPackageItemEntry
{
    uint32      ID;                                         // 0
    uint32      QuestPackageID;                             // 1
    uint32      ItemID;                                     // 2
    uint32      Unk1;                                       // 3
    uint32      Unk2;                                       // 4
};

// SceneScript.db2
struct SceneScriptEntry
{
    uint32 m_ID;                                             // 0         m_ID
    LocalizedString* m_name;                                 // 1         m_name
    LocalizedString* m_script;                               // 2         m_script
    uint32 m_prevScriptPartID;                               // 3         m_prevScriptPartID - Prev Script Part Id From Chain
    uint32 m_nextScriptPartID;                               // 4         m_nextScriptPartID - Next Script Part Id From Chain
};

// PvpItem.db2
struct PvpItemEntry
{
    uint32 itemId;
    uint32 ilvl;
};
#define MAX_SPELL_REAGENTS 8
#define MAX_SPELL_REAGENTS2 10

// SpellReagents.db2
struct SpellReagentsEntry
{
    //uint32    Id;                                          // 0         m_ID
    int32     Reagent[MAX_SPELL_REAGENTS];                   // 1  - 10   m_reagent
    uint32    ReagentCount[MAX_SPELL_REAGENTS2];             // 11 - 20   m_reagentCount
};

struct AreaPOIEntry
{
    uint32 id;              // 0
    uint32 icon[11];        // 1-11
    float x;                // 12
    float y;                // 13
    uint32 mapId;           // 14
    // uint32 val1;         // 15
    uint32 zoneId;          // 16
    // char* name;          // 17 - name
    // char* name2;         // 18 - name2
    uint32 worldState;      // 19
    // uint32 val2;         // 20
    // uint32 unk;          // 21
    // uint32 unk2;         // 21 - Pandaria
};

struct HolidaysEntry
{
    uint32 Id;                                              // 0        m_ID
    uint32 Duration[MAX_HOLIDAY_DURATIONS];                 // 1-10     m_duration
    uint32 Date[MAX_HOLIDAY_DATES];                         // 11-36    m_date (dates in unix time starting at January, 1, 2000)
    uint32 Region;                                          // 37       m_region (wow region)
    uint32 Looping;                                         // 38       m_looping
    //uint32 holidayNameId;                                 // 49       m_holidayNameID (HolidayNames.db2)
    //uint32 holidayDescriptionId;                          // 50       m_holidayDescriptionID (HolidayDescriptions.db2)
    char* TextureFilename;                                  // 51       m_textureFilename
    uint32 Priority;                                        // 52       m_priority
    uint32 CalendarFilterType;                              // 53       m_calendarFilterType (-1 = Fishing Contest, 0 = Unk, 1 = Darkmoon Festival, 2 = Yearly holiday)
    //uint32 flags;                                         // 54       m_flags (0 = Darkmoon Faire, Fishing Contest and Wotlk Launch, rest is 1)
};

struct OverrideSpellDataEntry
{
    uint32      id;                                         // 0
    uint32      spellId[MAX_OVERRIDE_SPELL];                // 1-10
    //uint32      unk0;                                     // 11
    //char*     SpellBarName;                               // 12
};


#define MAX_SPELL_TOTEMS            2

// SpellTotems.db2
struct SpellTotemsEntry
{
    uint32    Id;                                             // 0        m_ID
    uint32    TotemCategory[MAX_SPELL_TOTEMS];                // 1        m_requiredTotemCategoryID
    uint32    Totem[MAX_SPELL_TOTEMS];                        // 2        m_totem
};

// SpellPower.db2
struct SpellPowerEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32  spellId;                                      // 1  - Pandaria
    //uint32  unk0;                                         // 2  - Pandaria always after spellId
    uint32    powerType;                                    // 3       m_powerType
    uint32    manaCost;                                     // 4       m_manaCost
    uint32    manaCostPerlevel;                             // 5       m_manaCostPerLevel
    uint32    manaPerSecond;                                // 6       m_manaPerSecond
    uint32    manaPerSecondPerLevel;                        // 7       m_manaPerSecondPerLevel
    //uint32  PowerDisplayId;                               // 8       m_powerDisplayID - id from PowerDisplay.db2, new in 3.1
    float     ManaCostPercentageFloat;                      // 9       4.3.0
    // float  unk1                                          // 10 - Pandaria
    // float  unk2                                          // 11 - Pandaria
    // float  unk3                                          // 12 - Pandaria
};

// SpellClassOptions.db2
struct SpellClassOptionsEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32    modalNextSpell;                             // 1       m_modalNextSpell not used
    flag128   SpellFamilyFlags;                             // 2-5
    uint32    SpellFamilyName;                              // 6       m_spellClassSet
};

// SpellAuraRestrictions.db2/
struct SpellAuraRestrictionsEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32 spellId;                                       // 1  - Pandaria
    //uint32 unk0;                                          // 2  - Panadraia always  after spellId
    uint32    CasterAuraState;                              // 3       m_casterAuraState
    uint32    TargetAuraState;                              // 4       m_targetAuraState
    uint32    CasterAuraStateNot;                           // 5       m_excludeCasterAuraState
    uint32    TargetAuraStateNot;                           // 6       m_excludeTargetAuraState
    uint32    casterAuraSpell;                              // 7       m_casterAuraSpell
    uint32    targetAuraSpell;                              // 8       m_targetAuraSpell
    uint32    excludeCasterAuraSpell;                       // 9       m_excludeCasterAuraSpell
    uint32    excludeTargetAuraSpell;                       // 10      m_excludeTargetAuraSpell
};

// SpellCastingRequirements.db2
struct SpellCastingRequirementsEntry
{
    //uint32    Id;                                           // 0        m_ID
    uint32    FacingCasterFlags;                            // 1       m_facingCasterFlags
    //uint32    MinFactionId;                               // 2      m_minFactionID not used
    //uint32    MinReputation;                              // 3      m_minReputation not used
    int32     AreaGroupId;                                  // 4      m_requiredAreaGroupId
    //uint32    RequiredAuraVision;                         // 5      m_requiredAuraVision not used
    uint32    RequiresSpellFocus;                           // 6       m_requiresSpellFocus
};

// SpellMisc.db2
struct SpellMiscEntry
{
    uint32    Id;                                           // 0        m_ID
    uint32    Attributes;                                   // 1        m_attribute
    uint32    AttributesEx;                                 // 2        m_attributesEx
    uint32    AttributesEx2;                                // 3        m_attributesExB
    uint32    AttributesEx3;                                // 4        m_attributesExC
    uint32    AttributesEx4;                                // 5        m_attributesExD
    uint32    AttributesEx5;                                // 6        m_attributesExE
    uint32    AttributesEx6;                                // 7        m_attributesExF
    uint32    AttributesEx7;                                // 8        m_attributesExG
    uint32    AttributesEx8;                                // 9        m_attributesExH
    uint32    AttributesEx9;                                // 10       m_attributesExI
    uint32    AttributesEx10;                               // 11       m_attributesExJ
    uint32    AttributesEx11;                               // 12       m_attributesExK
    uint32    AttributesEx12;                               // 13       m_attributesExL
    uint32    AttributesEx13;                               // 14       m_attributesExM
    uint32    CastingTimeIndex;                             // 15       m_castingTimeIndex
    uint32    DurationIndex;                                // 16       m_durationIndex
    uint32    rangeIndex;                                   // 17       m_rangeIndex
    float     speed;                                        // 18       m_speed
    uint32    SpellVisual[2];                               // 19-20    m_spellVisualID
    uint32    SpellIconID;                                  // 21       m_spellIconID
    uint32    activeIconID;                                 // 22       m_activeIconID
    uint32    SchoolMask;                                   // 23       m_schoolMask
};

struct SpellRuneCostEntry
{
    uint32  ID;                                             // 0
    uint32  RuneCost[3];                                    // 1-3 (0=blood, 1=frost, 2=unholy)
    //uint32 unk0                                           // 4 - 4th rune ??
    uint32  runePowerGain;                                  // 5

    bool NoRuneCost() const { return RuneCost[0] == 0 && RuneCost[1] == 0 && RuneCost[2] == 0; }
    bool NoRunicPowerGain() const { return runePowerGain == 0; }
};

struct TaxiNodesEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    map_id;                                       // 1        m_ContinentID
    float     x;                                            // 2        m_x
    float     y;                                            // 3        m_y
    float     z;                                            // 4        m_z
    char* name;                                             // 5        m_Name_lang
    uint32    MountCreatureID[2];                           // 6-7      m_MountCreatureID[2]
    //uint32 unk1                                           // 8 - Pandaria
    uint32    Flags;                                        // 9        m_Flags
    //uint32 unk1                                           // 10 - Cata
    //uint32 unk1                                           // 11 - Cata
};

struct TaxiPathEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    from;                                         // 1        m_FromTaxiNode
    uint32    to;                                           // 2        m_ToTaxiNode
    uint32    price;                                        // 3        m_Cost
};

struct TaxiPathNodeEntry
{
                                                            // 0        m_ID
    uint32    path;                                         // 1        m_PathID
    uint32    index;                                        // 2        m_NodeIndex
    uint32    mapid;                                        // 3        m_ContinentID
    float     x;                                            // 4        m_LocX
    float     y;                                            // 5        m_LocY
    float     z;                                            // 6        m_LocZ
    uint32    actionFlag;                                   // 7        m_flags
    uint32    delay;                                        // 8        m_delay
    uint32    arrivalEventID;                               // 9        m_arrivalEventID
    uint32    departureEventID;                             // 10       m_departureEventID
};

struct TaxiPathBySourceAndDestination
{
    TaxiPathBySourceAndDestination() : ID(0), price(0) { }
    TaxiPathBySourceAndDestination(uint32 _id, uint32 _price) : ID(_id), price(_price) { }

    uint32    ID;
    uint32    price;
};
typedef std::map<uint32, TaxiPathBySourceAndDestination> TaxiPathSetForSource;
typedef std::map<uint32, TaxiPathSetForSource> TaxiPathSetBySource;

struct TaxiPathNodePtr
{
    TaxiPathNodePtr() : i_ptr(NULL) { }
    TaxiPathNodePtr(TaxiPathNodeEntry const* ptr) : i_ptr(ptr) { }
    TaxiPathNodeEntry const* i_ptr;
    operator TaxiPathNodeEntry const& () const { return *i_ptr; }
};

typedef Path<TaxiPathNodePtr, TaxiPathNodeEntry const> TaxiPathNodeList;
typedef std::vector<TaxiPathNodeList> TaxiPathNodesByPath;

#define TaxiMaskSize 198
typedef uint8 TaxiMask[TaxiMaskSize];

// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

#endif