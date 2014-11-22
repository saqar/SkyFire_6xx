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

#include "Common.h"
#include "DatabaseEnv.h"
#include "DB2Enums.h"
#include "DB2Structure.h"
#include "DB2Stores.h"
#include "Field.h"
#include "GarrisonMgr.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

GarrisonMgr::~GarrisonMgr()
{
    for (auto itr : m_GarrisonsStore)
        delete itr;

    for (auto itr : m_GarrisonInfoStore)
        delete itr;

    for (auto itr : m_FollowerStore)
        delete itr;

    for (auto itr : m_BuildingSet)
        delete itr;

    for (auto itr : m_PlotInfoStore)
        delete itr;

    for (auto itr : m_MissionStore)
        delete itr;

    for (auto itr : m_AbilitiesStore)
        delete itr;

    for (auto itr : m_buildingStore)
        delete itr;

    m_GarrisonsStore.clear();
    m_GarrisonInfoStore.clear();
    m_FollowerStore.clear();
    m_BuildingSet.clear();
    m_PlotInfoStore.clear();
    m_MissionStore.clear();
    m_AbilitiesStore.clear();
    m_buildingStore.clear();
}

void GarrisonMgr::LoadFromDB()
{
    LoadAbilitiesFromDb();
    LoadBuildingsFromDb();
    LoadMissionsFromDb();
    LoadWorkOrdersFromDb();
    LoadFollowersFromDb();
}

void GarrisonMgr::LoadAbilitiesFromDb()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT ID, `Type`, Name, Description, SpellID, Data1, Data2 FROM garrison_abilities");
    if (!result)
    {
        TC_LOG_INFO("sql.sql", ">> Loaded 0 Garrison Ability store entries, table `garrison_abilities` is empty!");
        return;
    }

    uint32 count = 0;
    int p;
    do
    {
        p = 0;
        Field* fields = result->Fetch();

        uint32 ID                   = fields[p++].GetUInt32();
        uint32 Type                 = fields[p++].GetUInt32();
        std::string Name            = fields[p++].GetString();
        std::string Description     = fields[p++].GetString();
        uint32 SpellId              = fields[p++].GetUInt32();
        uint32 Data1                = fields[p++].GetUInt32();
        uint32 Data2                = fields[p++].GetUInt32();

        if (HasAbilityId(ID))
        {
            TC_LOG_ERROR("sql.sql", "Ability id %u defined in `garrison_abilities` already exists, skipped!", ID);
            continue;
        }

        Abilities* ability = new Abilities(ID, Type, Name, Description, SpellId, Data1, Data2);

        m_AbilitiesStore.insert(ability);
        count++;

    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Garrison Abilities store entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void GarrisonMgr::LoadBuildingsFromDb()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT ID, `SpawnIDH`, SpawnIDA, Data0, Data1, BuildingRank, BuildingNameH, BuildingNameA,"
        "Description, Effect, BuildTime, field11, ResourcesCost, field13, field14, DisplayId, field16, field17, field18, field19, WorkOrders"
        "field21, field22, GoldCost FROM garrison_building");
    if (!result)
    {
        TC_LOG_INFO("sql.sql", ">> Loaded 0 Garrison Buildings store entries, table `garrison_buildings` is empty!");
        return;
    }

    uint32 count = 0;
    int p;
    do
    {
        p = 0;
        Field* fields = result->Fetch();

        uint32 ID               = fields[p++].GetUInt32();
        uint32 SpawnIDH         = fields[p++].GetUInt32();
        uint32 SpawnIDA         = fields[p++].GetUInt32();
        uint32 Data0            = fields[p++].GetUInt32();
        uint32 Data1            = fields[p++].GetUInt32();
        uint32 BuildingRank     = fields[p++].GetUInt32();
        std::string NameH       = fields[p++].GetString();
        std::string NameA       = fields[p++].GetString();
        std::string Description = fields[p++].GetString();
        std::string Effect      = fields[p++].GetString();
        uint32 BuildTime        = fields[p++].GetUInt32();
        uint32 field11          = fields[p++].GetUInt32();
        uint32 ResourcesCost    = fields[p++].GetUInt32();
        uint32 field13          = fields[p++].GetUInt32();
        uint32 field14          = fields[p++].GetUInt32();
        uint32 DisplayId        = fields[p++].GetUInt32();
        uint32 field16          = fields[p++].GetUInt32();
        uint32 field17          = fields[p++].GetUInt32();
        uint32 field18          = fields[p++].GetUInt32();
        uint32 field19          = fields[p++].GetUInt32();
        uint32 WorkOrders       = fields[p++].GetUInt32();
        uint32 field21          = fields[p++].GetUInt32();
        uint32 field22          = fields[p++].GetUInt32();
        uint32 GoldCost         = fields[p++].GetUInt32();

        if (HasBuildingId(ID))
        {
            TC_LOG_ERROR("sql.sql", "Ability id %u defined in `garrison_abilities` already exists, skipped!", ID);
            continue;
        }


        Buildings* building = new Buildings(ID, SpawnIDH, SpawnIDA, Data0, Data1, BuildingRank, NameH, NameA, Description, Effect, BuildTime,
            field11, ResourcesCost, field13, field14, DisplayId, field16, field17, field18, field19, WorkOrders, field21, field22, GoldCost);

        m_buildingStore.insert(building);
        count++;

    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Garrison Buildings store entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void GarrisonMgr::LoadMissionsFromDb()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT ID, Level, ItemLevel, Type, Followers, field5, Time, field7, field8,"
        " Category, Name, Description, Location, MissionID, field14, Resources, BonusReward, Experience, Percentage FROM garrison_mission");
    if (!result)
    {
        TC_LOG_INFO("sql.sql", ">> Loaded 0 Garrison Missions store entries, table `garrison_mission` is empty!");
        return;
    }

    uint32 count = 0;
    int p;
    do
    {
        p = 0;
        Field* fields = result->Fetch();

        uint32 ID                   = fields[p++].GetUInt32();
        uint32 Level                = fields[p++].GetUInt32();
        uint32 ItemLevel            = fields[p++].GetUInt32();
        uint32 Type                 = fields[p++].GetUInt32();
        uint32 Followers            = fields[p++].GetUInt32();
        uint32 field5               = fields[p++].GetUInt32();
        uint32 Time                 = fields[p++].GetUInt32();
        uint32 _followers           = fields[p++].GetUInt32();
        uint32 field7               = fields[p++].GetUInt32();
        uint32 field8               = fields[p++].GetUInt32();
        uint32 Categeory            = fields[p++].GetUInt32();
        std::string Name            = fields[p++].GetString();
        std::string Description     = fields[p++].GetString();
        std::string Location        = fields[p++].GetString();
        uint32 MissionId            = fields[p++].GetUInt32();
        uint32 field14              = fields[p++].GetUInt32();
        uint32 Resources            = fields[p++].GetUInt32();
        uint32 BonusReward          = fields[p++].GetUInt32();
        uint32 Experience           = fields[p++].GetUInt32();
        uint32 Percentage           = fields[p++].GetUInt32();

        count++;

    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Garrison Missions store entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool GarrisonMgr::HasAbilityId(uint32 id)
{
    for (auto citr : m_AbilitiesStore)
        if ((citr)->Id == id)
            return true;

    return false;
}

bool GarrisonMgr::HasBuildingId(uint32 id)
{
    for (auto citr : m_BuildingSet)
        if ((citr)->GarrBuildingID == id)
            return true;

    return false;
}

bool GarrisonMgr::HasMissionId(uint32 id)
{
    for (auto citr : m_MissionStore)
        if ((citr)->MissionRecID == id)
            return true;

    return false;
}

bool GarrisonMgr::HasFollowerId(uint32 id)
{
    for (auto citr : m_FollowerStore)
        if ((citr)->GarrFollowerID == id)
            return true;

    return false;
}

void ReadGarrisonMission(WorldPacket& data, Missions const* mission)
{
    data << uint64(mission->missionEntry);
    data << uint32(mission->MissionRecID);

    data << uint32(mission->OfferTime);
    data << uint32(mission->OfferDuration);
    data << uint32(mission->StartTime);
    data << uint32(mission->TravelDuration);
    data << uint32(mission->MissionDuration);

    data << uint32(mission->MissionState);
}

void GarrisonMgr::SendGarrisonGetInfo(WorldSession* session)
{
    TC_LOG_DEBUG("network", "World: Sent SMSG_GET_GARR_INFO_RESULT");

    WorldPacket data(SMSG_GET_GARR_INFO_RESULT, 500);				// SizeUnknown

    for (auto itr : m_GarrisonInfoStore)
    {
        const GarrisonInfo* info = itr;

        data << uint32(info->GarrisonBuildingInfoCount); // 21
        data << uint32(0);                               // 22
        data << uint32(0);                               // 8

        data << uint32(info->GarrisonBuildingInfoCount);
        data << uint32(info->GarrisonPlotInfoCount);
        data << uint32(info->GarrisonFollowerCount);
        data << uint32(info->GarrisonMission);
        data << uint32(info->ArchivedMissions);


        if (info->GarrisonBuildingInfoCount)
        {
            for (auto itr : m_BuildingSet)
            {
                const GarrisonBuildingInfo* building = itr;

                data << uint32(building->GarrPlotInstanceID);
                data << uint32(building->GarrBuildingID);
                data << uint32(building->Time);
                data << uint32(0);
                data << uint32(building->CurrentGarSpecID);
                data.WriteBit(building->Active);
            }
        }

        if (info->GarrisonPlotInfoCount)
        {
            for (auto itr : m_PlotInfoStore)
            {
                const PlotInfo* plot = itr;

                data << uint32(plot->GarrPlotInstanceID);
                data << float(plot->PosX);
                data << float(plot->PosY);
                data << float(plot->PosZ);
                data << uint32(plot->PlotType);
            }
        }

        if (info->GarrisonFollowerCount)
        {
            for (auto itr : m_FollowerStore)
            {
                const Followers* follower = itr;
                // We can't really know just what these are for sure
                // But we can definitely guess

                data << uint64(follower->npcEntry);
                data << uint32(follower->GarrFollowerID);
                data << uint32(follower->CreatureID);
                data << uint32(follower->Quality);
                data << uint32(follower->FollowerLevel);
                data << uint32(follower->CurrentMissionID);
                data << uint32(follower->CurrentBuildingID);
                data << uint32(follower->ItemLevelArmor);
                data << uint32(follower->ItemLevelWeapon);
                data << uint32(info->AbilityIDCount);
                data << uint32(follower->_Gender);

                for (uint32 z = 0; z < info->AbilityIDCount; z++)
                    data << uint32(follower->AbilityID);
            }
        }

        if (info->GarrisonMission)
        {
            for (auto itr : m_MissionStore)
            {
                const Missions* mission = itr;
                data << uint64(mission->missionEntry); // DbID
                data << uint32(mission->MissionRecID);
                data << uint32(mission->OfferTime);
                data << uint32(mission->OfferDuration);
                data << uint32(mission->StartTime);
                data << uint32(mission->TravelDuration);
                data << uint32(mission->MissionDuration);
                data << uint32(mission->MissionState);
            }
        }

        for (uint32 i = 0; i < info->ArchivedMissions; i++)
            data << uint32(info->ArchivedMissions);
    }

    session->SendPacket(&data);
}

void GarrisonMgr::SendGarrisonArchitect(WorldSession* session)
{
    TC_LOG_DEBUG("network", "World: Sending SMSG_GARR_OPEN_ARCHITECT");

    WorldPacket data(SMSG_GARR_OPEN_ARCHITECT, 16);
    ObjectGuid NpcGUID;

    data << NpcGUID;

    session->SendPacket(&data);
}

void GarrisonMgr::SendGarrisonPlotPlacedResult(WorldSession* session)
{
    TC_LOG_DEBUG("network", "World: Sending SMSG_GARR_PLOT_PLACED");
    WorldPacket data(SMSG_GARR_PLOT_PLACED);
    PlotInfo const* plot = 0;

    data << uint32(plot->GarrPlotInstanceID);
    data << float(plot->PosX);
    data << float(plot->PosY);
    data << float(plot->PosZ);
    data << uint32(plot->PlotType);

    session->SendPacket(&data);
}

void GarrisonMgr::SendGarrisonActivateBuilding(WorldSession* session)
{
    WorldPacket data(SMSG_GARR_BUILDING_ACTIVATED, 4);
    GarrisonBuildingInfo const* building = 0;

    data << uint32(building->GarrBuildingID);

    session->SendPacket(&data);
}

void GarrisonMgr::SendGarrisonCompleteMission(WorldSession* session)
{
    WorldPacket data(SMSG_GARR_COMPLETE_MISSION_RESULT, 200);                    // We guess size
    Missions const* mission = 0;

    // Should be ReadGarrisonMission();
    data << uint64(mission->missionEntry);
    data << uint32(mission->MissionRecID);

    data << uint32(mission->OfferTime);
    data << uint32(mission->OfferDuration);
    data << uint32(mission->StartTime);
    data << uint32(mission->TravelDuration);
    data << uint32(mission->MissionDuration);

    data << uint32(mission->MissionState);

    data << uint32(mission->MissionRecID);
    data << uint32(0);                          // Result

    session->SendPacket(&data);
}