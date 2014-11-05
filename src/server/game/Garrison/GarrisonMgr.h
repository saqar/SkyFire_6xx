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
#include "DB2Structure.h"
#include "DB2Stores.h"
#include "DB2Enums.h"

#define MAX_BUILDING_PLOTS 3
#define MAX_FOLLOWER_SLOTS 5
#define MAX_GARRISON_LEVEL 3
#define MAX_RESOURCES      10000

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
        GarrAbilityEntry* _category;
        GarrBuildingEntry* _buildingInfo;
    };
};

class GarrisonMgr
{
public:
    GarrisonMgr();
    ~GarrisonMgr();
};

#endif
