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

#ifndef GARRISON_MGR_H
#define GARRISON_MGR_H

#include "SharedDefines.h"
#include "Common.h"

struct GarrisonBuildingInfo
{
    uint32 ID;                              // Building's entry ID
    uint32 SpawnIDH;                        // Building's spawn ID Horde
    uint32 SpawnIDA;                        // Building's spawn ID Alliance
    uint32 Data0;                           // 3 - Unknown, all are 0
    uint32 Data1;                           // 4 - Unknown, all ranks of one building type share this ID. Probably something important.
    uint32 BuildingRank;                    // Building's Rank. 1 - Small, 2 - Medium, 3 - Large.
    std::string BuildingNameH;              // Building's name Horde
    std::string BuildingNameA;              // Building's name Alliance
    std::string Description;                // Description of the building
    std::string Effect;                     // Building's bonus effect
    uint32 BuildTime;                       // Building's required build time
    uint32 Data2;                           // 11 - Unknown all are 824. Maybe experience relted.
    uint32 ResourcesCost;                   // Building's cost of Garrison Resources.
    uint32 Data3;                           // 13 - Unknown
    uint32 Data4;                           // 14 - Unknown 
    uint32 DisplayID;                       // Building's DisplayID
    uint32 Data6;                           // 16 - Unknown
    uint32 Data7;                           // 17 - Unknown, may relate to default buildings
    uint32 Data8;                           // 18 - Unknown
    uint32 Data9;                           // 19 - Unknown 
    uint32 WorkOrders;                      // Building's number of work orders
    uint32 Data10;                          // 21 - Unknown
    uint32 Data11;                          // 22 - Unknown, all 0
    uint32 GoldCost;                        // Building's gold cost
};

struct GarrisonAbilityInfo
{
    uint32 ID;                              // AbilityID
    uint32 Type;                            // Type 1 - Trait, 2 - Spell
    std::string Name;                       // Ability's name
    std::string Description;                // Ability's description
    uint32 SpellID;                         // Ability's spellID for ingame use
    uint32 Data0;                           // Unknown
    uint32 Category;                        // Ability's category
};

struct GarrisonFollowerInfo
{

};

struct GarrisonPlotBuilding
{
    uint32 ID;                              // ID
    uint32 PlotType;                        // PlotTypeID
    uint32 BuildingId;                      // BuildingID
};

struct GarrisonPlot
{
    uint32 ID;                              // PlotTypeID
    uint32 PlotSize;                        // PlotSize
    uint32 EntryID;                         // EntryID
    uint32 Data0;                           // Unknown, all 0.
    std::string PlotType;                   // PlotType
    uint32 Data1;                           // Unknown
    uint32 Data2;                           // Unknown
    uint32 Data3;                           // Unknown
    uint32 Data4;                           // Unknown
};

enum GarrisonBuildings
{
    GARR_BUILDING_SMALL                     = 1,
    GARR_BUILDING_MEDIUM                    = 2,
    GARR_BUILDING_LARGE                     = 3,
    GARR_BUILDING_SPECIAL                   = 4
};

enum AbilityCategory
{
    GARR_ABILITY_CATEGORY_NONE              = 0,
    GARR_ABILITY_CATEGORY_SLAYER            = 1,
    GARR_ABILITY_CATEGORY_RACIAL_PREFERENCE = 2,
    GARR_ABILITY_CATEGORY_PROFESSION        = 3,
    GARR_ABILITY_CATEGORY_OTHER             = 4,
    GARR_ABILITY_CATEGORY_INCREASED_REWARDS = 5,
    GARR_ABILITY_CATEGORY_MISSION_DURATION  = 6,
    GARR_ABILITY_CATEGORY_ENV_PREFERENCE    = 7
};


#define MAX_BUILDING_PLOTS 3
#define MAX_FOLLOWER_SLOTS 5

class Garrison
{
    public : class GarrisonBuilding
    {
    public:
        void Reset();
        static void DeleteFromDB(uint32 lowguid);
        void LoadFromDB(PreparedQueryResult garrisonResult);
        void SaveToDB(SQLTransaction& trans);

    private:
        AbilityCategory* _category;
        GarrisonBuildingInfo* _buildingInfo;
    };
};

class GarrisonMgr
{
public:
    GarrisonMgr();
    ~GarrisonMgr();
};

#endif
