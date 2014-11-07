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
#include <set>

#include "DB2Structure.h"
#include "DB2Stores.h"
#include "DB2Enums.h"
#include "Garrisons.h"

typedef std::set<Garrisons*> GarrisonSet;

#define MAX_BUILDING_PLOTS 3
#define MAX_FOLLOWER_SLOTS 5
#define MAX_GARRISON_LEVEL 3
#define MAX_RESOURCES      10000

class GarrisonMgr
{
public:
    GarrisonMgr();
    ~GarrisonMgr();

    Player* GetOwner() const { return m_owner; }
    Garrisons* GetGarrison(uint32 garrisonId) const;
    void GarrisonCreate()

    void Reset();
    static void DeleteFromDB(uint32 lowguid);
    void LoadFromDB(PreparedQueryResult result) {};
    void SaveToDB(SQLTransaction& trans);

    static void DeleteFollowerFromDB(uint32 garrId);
    void SaveFollowerToDB(SQLTransaction& trans);
    void LoadFollowerFromDB(PreparedQueryResult result) {};

    CreatureTemplate* GetCreatureTemplate() const { return m_creature; }

    // Garrison Functions
    GarrMissionCategory* GetMissionCategory() const { return m_category; }
    GarrMissionEntry* GetMission() const { return m_garrMission; }
    GarrAbilityEntry* GetAbility() const { return m_garrAbility; }

    uint8 GarrisonGetFollowerSlots(uint32 garrisonId);
    uint8 GetFollowerSlot(uint32 followerId);

private:
    Player* m_owner;
    Unit* m_unit;

    CreatureTemplate* m_creature;
    GarrAbilityEntry* m_garrAbility;
    GarrMissionEntry* m_garrMission;
    GarrMissionCategory* m_category;

    GarrisonSet m_garrisonSet;
};

#endif
