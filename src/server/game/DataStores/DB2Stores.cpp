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

#include "BattlePet.h"
#include "DB2Stores.h"
#include "DB2fmt.h"
#include "DB2Utility.h"
#include "Common.h"
#include "Log.h"
#include "World.h"
#include "DBCStores.h"

DB2Storage<AreaPOIEntry> sAreaPOIStore(AreaPOIEntryfmt);
DB2Storage<BattlePetAbilityEntry> sBattlePetAbilityStore(BattlePetAbilityfmt);
DB2Storage<BattlePetAbilityStateEntry> sBattlePetAbilityStateStore(BattlePetAbilityStatefmt);
DB2Storage<BattlePetBreedStateEntry> sBattlePetBreedStateStore(BattlePetBreedStatefmt);
DB2Storage<BattlePetSpeciesEntry> sBattlePetSpeciesStore(BattlePetSpeciesfmt);
DB2Storage<BattlePetSpeciesStateEntry> sBattlePetSpeciesStateStore(BattlePetSpeciesStatefmt);
DB2Storage<BattlePetSpeciesXAbilityEntry> sBattlePetSpeciesXAbilityStore(BattlePetSpeciesXAbilityfmt);
DB2Storage<BattlePetStateEntry> sBattlePetStateStore(BattlePetStatefmt);
DB2Storage<ItemToBattlePetEntry> sItemToBattlePetStore(ItemToBattlePetfmt);

BattlePetBreedSet sBattlePetBreedSet;

DB2Storage<BroadcastTextEntry> sBroadcastTextStore(BroadcastTextfmt, &DB2Utilities::HasBroadcastTextEntry, &DB2Utilities::WriteBroadcastTextDbReply);
DB2Storage<GarrAbilityEntry> sGarrAbilityStore(GarrAbilityEntryfmt);
DB2Storage<GarrAbilityCategoryEntry> sGarrAbilityCategoryStore(GarrAbilityCategoryEntryfmt);
DB2Storage<GarrBuildingEntry> sGarrBuildingStore(GarrBuildingEntryfmt);
DB2Storage<GarrClassSpecEntry> sGarrClassSpecStore(GarrClassSpecEntryfmt);
DB2Storage<GarrEncounterEntry> sGarrEncounterStore(GarrEncounterEntryfmt);
DB2Storage<GarrFollItemSetEntry> sGarrFollItemSetStore(GarrFollItemSetEntryfmt);
DB2Storage<GarrFollowerEntry> sGarrFollowerStore(GarrFollowerEntryfmt);
DB2Storage<GarrFollowerLevelXPEntry> sGarrFollowerLevelXPStore(GarrFollowerLevelXPEntryfmt);
DB2Storage<GarrFollowerQualityEntry> sGarrFollowerQualityStore(GarrFollowerQualityEntryfmt);
DB2Storage<GarrMissionEntry> sGarrMissionStore(GarrMissionEntryfmt);
DB2Storage<GarrMissionTypeEntry> sGarrMissionTypeStore(GarrMissionTypeEntryfmt);
DB2Storage<GarrMechanicEntry> sGarrMechanicStore(GarrMechanicEntryfmt);
DB2Storage<GarrPlotEntry> sGarrPlotStore(GarrPlotEntryfmt);
DB2Storage<GarrPlotBuildingEntry> sGarrPlotBuildingStore(GarrPlotBuildingEntryfmt);
DB2Storage<GarrPlotUICategoryEntry> sGarrPlotUICategoryStore(GarrPlotUICategoryEntryfmt);
DB2Storage<GarrSpecializationEntry> sGarrSpecializationStore(GarrSpecializationEntryfmt);
DB2Storage<HolidaysEntry> sHolidaysStore(Holidaysfmt);
DB2Storage<ItemEntry> sItemStore(Itemfmt, &DB2Utilities::HasItemEntry, &DB2Utilities::WriteItemDbReply);
DB2Storage<ItemAppearanceEntry> sItemAppearanceStore(ItemAppearancefmt);
DB2Storage<ItemModifiedAppearanceEntry> sItemModifiedAppearanceStore(ItemModifiedAppearancefmt);
DB2Storage<ItemCurrencyCostEntry> sItemCurrencyCostStore(ItemCurrencyCostfmt);
DB2Storage<ItemExtendedCostEntry> sItemExtendedCostStore(ItemExtendedCostEntryfmt);
DB2Storage<ItemSparseEntry> sItemSparseStore(ItemSparsefmt, &DB2Utilities::HasItemSparseEntry, &DB2Utilities::WriteItemSparseDbReply);
DB2Storage<KeyChainEntry> sKeyChainStore(KeyChainfmt);
DB2Storage<OverrideSpellDataEntry> sOverrideSpellDataStore(OverrideSpellDatafmt);
DB2Storage<QuestPackageItemEntry> sQuestPackageItemStore(QuestPackageItemfmt);
DB2Storage<SpellMiscEntry> sSpellMiscStore(SpellMiscfmt);
DB2Storage<SpellTotemsEntry> sSpellTotemsStore(SpellTotemsEntryfmt);
DB2Storage<SpellPowerEntry> sSpellPowerStore(SpellPowerEntryfmt);
DB2Storage<SpellClassOptionsEntry> sSpellClassOptionsStore(SpellClassOptionsEntryfmt);
DB2Storage<SpellAuraRestrictionsEntry> sSpellAuraRestrictionsStore(SpellAuraRestrictionsEntryfmt);
DB2Storage<SceneScriptEntry> sSceneScriptStore(SceneScriptfmt);
DB2Storage<PvpItemEntry> sPvpItemStore(PvpItemfmt);
DB2Storage<SpellReagentsEntry> sSpellReagentsStore(SpellReagentsfmt);
DB2Storage<SpellCastingRequirementsEntry> sSpellCastingRequirementsStore(SpellCastingRequirementsEntryfmt);
DB2Storage<TaxiNodesEntry> sTaxiNodesStore(TaxiNodesEntryfmt);
DB2Storage<SpellRuneCostEntry> sSpellRuneCostStore(SpellRuneCostfmt);
static DB2Storage<TaxiPathNodeEntry> sTaxiPathNodeStore(TaxiPathNodeEntryfmt);
DB2Storage<TaxiPathEntry> sTaxiPathStore(TaxiPathEntryfmt);

// DBC used only for initialization sTaxiPathSetBySource at startup.
TaxiPathSetBySource sTaxiPathSetBySource;

// DBC used only for initialization sTaxiPathNodeStore at startup.
TaxiPathNodesByPath sTaxiPathNodesByPath;

typedef std::list<std::string> DB2StoreProblemList;

typedef std::map<uint32 /*hash*/, DB2StorageBase*> DB2StorageMap;
DB2StorageMap DB2Stores;

uint32 DB2FilesCount = 0;

static bool LoadDB2_assert_print(uint32 fsize, uint32 rsize, std::string const& filename)
{
    TC_LOG_ERROR("misc", "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

template<class T>
inline void LoadDB2(uint32& availableDb2Locales, DB2StoreProblemList& errlist, DB2Storage<T>& storage, std::string const& db2_path, std::string const& filename)
{
    // compatibility format and C++ structure sizes
    ASSERT(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDB2_assert_print(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DB2FilesCount;

    std::string db2_filename = db2_path + filename;
    if (storage.Load(db2_filename.c_str(), uint32(sWorld->GetDefaultDbcLocale())))
    {
        for (uint32 i = 0; i < TOTAL_LOCALES; ++i)
        {
            if (!(availableDb2Locales & (1 << i)))
                continue;

            if (uint32(sWorld->GetDefaultDbcLocale()) == i)
                continue;

            std::string localizedName(db2_path);
            localizedName.append(localeNames[i]);
            localizedName.push_back('/');
            localizedName.append(filename);

            if (!storage.LoadStringsFrom(localizedName.c_str(), i))
                availableDb2Locales &= ~(1<<i);             // mark as not available for speedup next checks
        }
    }
    else
    {
        // sort problematic db2 to (1) non compatible and (2) nonexistent
        if (FILE* f = fopen(db2_filename.c_str(), "rb"))
        {
            char buf[100];
            snprintf(buf, 100,"(exist, but have %d fields instead " SIZEFMTD ") Wrong client version DBC file?", storage.GetFieldCount(), strlen(storage.GetFormat()));
            errlist.push_back(db2_filename + buf);
            fclose(f);
        }
        else
            errlist.push_back(db2_filename);
    }

    DB2Stores[storage.GetHash()] = &storage;
}

void LoadDB2Stores(std::string const& dataPath)
{
    std::string db2Path = dataPath + "dbc/";

    DB2StoreProblemList bad_db2_files;
    uint32 availableDb2Locales = 0xFF;

    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetAbilityStore, db2Path, "BattlePetAbility.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetAbilityStateStore, db2Path, "BattlePetAbilityState.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetBreedStateStore, db2Path, "BattlePetBreedState.db2");

    for (uint32 i = 0; i < sBattlePetBreedStateStore.GetNumRows(); i++)
        if (BattlePetBreedStateEntry const* breedStateEntry = sBattlePetBreedStateStore.LookupEntry(i))
            if (sBattlePetBreedSet.find(breedStateEntry->BreedId) == sBattlePetBreedSet.end())
                sBattlePetBreedSet.insert(breedStateEntry->BreedId);

    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetSpeciesStore, db2Path, "BattlePetSpecies.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetSpeciesStateStore, db2Path, "BattlePetSpeciesState.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetSpeciesXAbilityStore, db2Path, "BattlePetSpeciesXAbility.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetStateStore, db2Path, "BattlePetState.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemToBattlePetStore, db2Path, "ItemToBattlePetSpecies.db2");

    LoadDB2(availableDb2Locales, bad_db2_files, sBroadcastTextStore, db2Path, "BroadcastText.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemStore, db2Path, "Item.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemAppearanceStore, db2Path, "ItemAppearance.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemModifiedAppearanceStore, db2Path, "ItemModifiedAppearance.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemCurrencyCostStore, db2Path, "ItemCurrencyCost.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemSparseStore, db2Path, "Item-sparse.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemExtendedCostStore, db2Path, "ItemExtendedCost.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sKeyChainStore, db2Path, "KeyChain.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sQuestPackageItemStore, db2Path, "QuestPackageItem.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sSceneScriptStore, db2Path, "SceneScript.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sPvpItemStore, db2Path, "PvpItem.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sSpellReagentsStore, db2Path, "SpellReagents.db2");

   // LoadDB2(availableDb2Locales, bad_db2_files, sAreaPOIStore,                db2Path, "AreaPOI.db2");//15595
    LoadDB2(availableDb2Locales, bad_db2_files, sHolidaysStore,               db2Path, "Holidays.db2");//15595
    LoadDB2(availableDb2Locales, bad_db2_files, sOverrideSpellDataStore,      db2Path, "OverrideSpellData.db2");//15595
    LoadDB2(availableDb2Locales, bad_db2_files, sSpellTotemsStore,            db2Path,"SpellTotems.db2");//15595
    LoadDB2(availableDb2Locales, bad_db2_files, sSpellPowerStore,             db2Path,"SpellPower.db2");//15595
    LoadDB2(availableDb2Locales, bad_db2_files, sSpellClassOptionsStore,      db2Path,"SpellClassOptions.db2");//15595
    LoadDB2(availableDb2Locales, bad_db2_files, sSpellAuraRestrictionsStore,  db2Path,"SpellAuraRestrictions.db2");//15595
    LoadDB2(availableDb2Locales, bad_db2_files, sSpellCastingRequirementsStore, db2Path,"SpellCastingRequirements.db2");//15595
    LoadDB2(availableDb2Locales, bad_db2_files, sSpellMiscStore,              db2Path, "SpellMisc.db2");//17538
    LoadDB2(availableDb2Locales, bad_db2_files, sSpellRuneCostStore,          db2Path, "SpellRuneCost.db2");//15595

    LoadDB2(availableDb2Locales, bad_db2_files, sTaxiNodesStore,              db2Path, "TaxiNodes.db2");//15595
    LoadDB2(availableDb2Locales, bad_db2_files, sTaxiPathStore,               db2Path, "TaxiPath.db2");//15595
    for (uint32 i = 1; i < sTaxiPathStore.GetNumRows(); ++i)
        if (TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(i))
            sTaxiPathSetBySource[entry->from][entry->to] = TaxiPathBySourceAndDestination(entry->ID, entry->price);
    uint32 pathCount = sTaxiPathStore.GetNumRows();

    //## TaxiPathNode.db2 ## Loaded only for initialization different structures
    LoadDB2(availableDb2Locales, bad_db2_files, sTaxiPathNodeStore,           db2Path, "TaxiPathNode.db2");//15595
    // Calculate path nodes count
    std::vector<uint32> pathLength;
    pathLength.resize(pathCount);                           // 0 and some other indexes not used
    for (uint32 i = 1; i < sTaxiPathNodeStore.GetNumRows(); ++i)
        if (TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(i))
        {
            if (pathLength[entry->path] < entry->index + 1)
                pathLength[entry->path] = entry->index + 1;
        }
    // Set path length
    sTaxiPathNodesByPath.resize(pathCount);                 // 0 and some other indexes not used
    for (uint32 i = 1; i < sTaxiPathNodesByPath.size(); ++i)
        sTaxiPathNodesByPath[i].resize(pathLength[i]);
    // fill data
    for (uint32 i = 1; i < sTaxiPathNodeStore.GetNumRows(); ++i)
        if (TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(i))
            sTaxiPathNodesByPath[entry->path].set(entry->index, entry);

    // Initialize global taxinodes mask
    // include existed nodes that have at least single not spell base (scripted) path
    {
        std::set<uint32> spellPaths;
        for (uint32 i = 1; i < sSpellEffectStore.GetNumRows(); ++i)
            if (SpellEffectEntry const* sInfo = sSpellEffectStore.LookupEntry (i))
                if (sInfo->Effect == SPELL_EFFECT_SEND_TAXI)
                    spellPaths.insert(sInfo->EffectMiscValue);

        memset(sTaxiNodesMask, 0, sizeof(sTaxiNodesMask));
        memset(sOldContinentsNodesMask, 0, sizeof(sOldContinentsNodesMask));
        memset(sHordeTaxiNodesMask, 0, sizeof(sHordeTaxiNodesMask));
        memset(sAllianceTaxiNodesMask, 0, sizeof(sAllianceTaxiNodesMask));
        memset(sDeathKnightTaxiNodesMask, 0, sizeof(sDeathKnightTaxiNodesMask));
        for (uint32 i = 1; i < sTaxiNodesStore.GetNumRows(); ++i)
        {
            TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(i);
            if (!node)
                continue;

            TaxiPathSetBySource::const_iterator src_i = sTaxiPathSetBySource.find(i);
            if (src_i != sTaxiPathSetBySource.end() && !src_i->second.empty())
            {
                bool ok = false;
                for (TaxiPathSetForSource::const_iterator dest_i = src_i->second.begin(); dest_i != src_i->second.end(); ++dest_i)
                {
                    // not spell path
                    if (spellPaths.find(dest_i->second.ID) == spellPaths.end())
                    {
                        ok = true;
                        break;
                    }
                }

                if (!ok)
                    continue;
            }

            // valid taxi network node
            uint8  field   = (uint8)((i - 1) / 8);
            uint32 submask = 1 << ((i-1) % 8);

            sTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[0] && node->MountCreatureID[0] != 32981)
                sHordeTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[1] && node->MountCreatureID[1] != 32981)
                sAllianceTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[0] == 32981 || node->MountCreatureID[1] == 32981)
                sDeathKnightTaxiNodesMask[field] |= submask;

            // old continent node (+ nodes virtually at old continents, check explicitly to avoid loading map files for zone info)
            if (node->map_id < 2 || i == 82 || i == 83 || i == 93 || i == 94)
                sOldContinentsNodesMask[field] |= submask;

            // fix DK node at Ebon Hold and Shadow Vault flight master
            if (i == 315 || i == 333)
                ((TaxiNodesEntry*)node)->MountCreatureID[1] = 32981;
        }
    }
    // error checks
    if (bad_db2_files.size() >= DB2FilesCount)
    {
        TC_LOG_ERROR("misc", "\nIncorrect DataDir value in worldserver.conf or ALL required *.db2 files (%d) not found by path: %sdb2", DB2FilesCount, dataPath.c_str());
        exit(1);
    }
    else if (!bad_db2_files.empty())
    {
        std::string str;
        for (std::list<std::string>::iterator i = bad_db2_files.begin(); i != bad_db2_files.end(); ++i)
            str += *i + "\n";

        TC_LOG_ERROR("misc", "\nSome required *.db2 files (%u from %d) not found or not compatible:\n%s", (uint32)bad_db2_files.size(), DB2FilesCount, str.c_str());
        exit(1);
    }

    // Check loaded DB2 files proper version
    if (!sBattlePetAbilityStore.LookupEntry(1443)           // last battle pet ability added in 6.0.3 (19116)
        || !sBattlePetSpeciesStore.LookupEntry(1615)        // last battle pet species added in 6.0.3 (19116)
        || !sBattlePetStateStore.LookupEntry(191)           // last battle pet state added in 6.0.3 (19116)
        || !sBroadcastTextStore.LookupEntry(92123)          // last broadcast text added in 6.0.3 (19116)
        || !sItemToBattlePetStore.LookupEntry(120309)       // last battle pet item added in 6.0.3 (19116)
        || !sItemStore.LookupEntry(120945)                  // last item added in 6.0.3 (19116)
        || !sItemExtendedCostStore.LookupEntry(5511)        // last item extended cost added in 6.0.3 (19116)
        || !sQuestPackageItemStore.LookupEntry(3560)        // last quest package item in 6.0.3 (19116)
        || !sSceneScriptStore.LookupEntry(13097))           // last scene script added in 6.0.3 (19116)
    {
        TC_LOG_ERROR("misc", "You have _outdated_ DB2 files, Please extract correct db2 files from client 6.0.3 19116");
        exit(1);
    }

    TC_LOG_INFO("misc", ">> Initialized %d DB2 data stores.", DB2FilesCount);
}

DB2StorageBase const* GetDB2Storage(uint32 type)
{
    DB2StorageMap::const_iterator itr = DB2Stores.find(type);
    if (itr != DB2Stores.end())
        return itr->second;

    return NULL;
}

#define BATTLE_PET_MAIN_STAT_VALUE  8.0f
#define BATTLE_PET_MAIN_STAT_DIV    200.0f
#define BATTLE_PET_MAIN_STAT_OFFSET 1600.0f

float BattlePetSpeciesMainStat(uint16 stateId, uint16 speciesId)
{
    if (stateId != BATTLE_PET_STATE_STAT_POWER
        && stateId != BATTLE_PET_STATE_STAT_STAMINA
        && stateId != BATTLE_PET_STATE_STAT_SPEED)
        return 0;

    for (uint32 i = 0; i < sBattlePetSpeciesStateStore.GetNumRows(); i++)
    {
        BattlePetSpeciesStateEntry const* stateEntry = sBattlePetSpeciesStateStore.LookupEntry(i);
        if (!stateEntry)
            continue;

        if (stateEntry->StateId == stateId && stateEntry->SpeciesId == speciesId)
            return BATTLE_PET_MAIN_STAT_VALUE + ((float)stateEntry->Modifier / BATTLE_PET_MAIN_STAT_DIV);
    }

    return BATTLE_PET_MAIN_STAT_VALUE;
}

float BattlePetBreedMainStatModifier(uint16 stateId, uint8 breedId)
{
    if (stateId != BATTLE_PET_STATE_STAT_POWER
        && stateId != BATTLE_PET_STATE_STAT_STAMINA
        && stateId != BATTLE_PET_STATE_STAT_SPEED)
        return 0.0f;

    for (uint32 i = 0; i < sBattlePetBreedStateStore.GetNumRows(); i++)
    {
        BattlePetBreedStateEntry const* stateEntry = sBattlePetBreedStateStore.LookupEntry(i);
        if (!stateEntry)
            continue;

        if (stateEntry->StateId == stateId && stateEntry->BreedId == breedId)
            return ((float)stateEntry->Value - BATTLE_PET_MAIN_STAT_OFFSET) / BATTLE_PET_MAIN_STAT_DIV;
    }

    return 0.0f;
}

uint32 BattlePetGetSummonSpell(uint16 speciesId)
{
    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesId);
    if (!speciesEntry)
        return 0u;

    return speciesEntry->SpellId;
}

bool BattlePetSpeciesHasFlag(uint16 speciesId, uint16 flag)
{
    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesId);
    if (!speciesEntry)
        return false;

    return (speciesEntry->Flags & flag) != 0;
}
