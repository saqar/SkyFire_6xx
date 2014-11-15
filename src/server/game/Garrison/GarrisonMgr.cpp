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

/* I WANT TO REWORK THE GARRISON SYSTEM ENTIRELY, NOW THAT I'VE PLAYED IT FOR REAL, I KNOW HOW IT SHOULD BE DONE*/

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
    do
    {
        Field* fields = result->Fetch();

        uint32 ID					= fields[0].GetUInt32();
        uint32 Type					= fields[1].GetUInt32();
        std::string Name			= fields[2].GetString();
        std::string Description		= fields[3].GetString();
        uint32 SpellId				= fields[4].GetUInt32();
        uint32 Data1				= fields[5].GetUInt32();
		uint32 Data2				= fields[6].GetUInt32();

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
    do
    {
        Field* fields = result->Fetch();

        uint32 ID								= fields[0].GetUInt32();
        uint32 SpawnIDH							= fields[1].GetUInt32();
        uint32 SpawnIDA							= fields[2].GetUInt32();
        uint32 Data0							= fields[3].GetUInt32();
        uint32 Data1							= fields[5].GetUInt32();
		uint32 BuildingRank						= fields[6].GetUInt32();
		std::string NameH						= fields[7].GetString();
		std::string NameA						= fields[8].GetString();
		std::string Description					= fields[9].GetString();
		std::string Effect						= fields[10].GetString();
		uint32 BuildTime						= fields[11].GetUInt32();
		uint32 field11							= fields[12].GetUInt32();
		uint32 ResourcesCost					= fields[13].GetUInt32();
		uint32 field13							= fields[14].GetUInt32();
		uint32 field14							= fields[15].GetUInt32();
		uint32 DisplayId						= fields[16].GetUInt32();
		uint32 field16							= fields[17].GetUInt32();
		uint32 field17							= fields[18].GetUInt32();
		uint32 field18							= fields[19].GetUInt32();
		uint32 field19							= fields[20].GetUInt32();
		uint32 WorkOrders						= fields[21].GetUInt32();
		uint32 field21							= fields[22].GetUInt32();
		uint32 field22							= fields[23].GetUInt32();
		uint32 GoldCost							= fields[24].GetUInt32();

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

void GarrisonMgr::SendGarrisonGetInfo(WorldSession* session)
{
	TC_LOG_DEBUG("network", "World: Sent SMSG_GET_GARR_INFO_RESULT");

	WorldPacket data(SMSG_GET_GARR_INFO_RESULT, 500);				// SizeUnknown

	for (auto itr : m_GarrisonInfoStore)
	{
		const GarrisonInfo* info = itr;

		data << uint32(info->GarrisonBuildingInfoCount); // 21
		data << uint32(0); // 22
		data << uint32(0); // 8

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

				data << uint64(follower->DBID);
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

				for (auto z = 0; z < info->AbilityIDCount; z++)
					data << uint32(follower->AbilityID);
			}
		}

		if (info->GarrisonMission)
		{
			for (auto itr : m_MissionStore)
			{
				const Missions* mission = itr;
				data << uint64(mission->DBID); // DbID
				data << uint32(mission->MissionRecID);
				data << uint32(mission->OfferTime);
				data << uint32(mission->OfferDuration);
				data << uint32(mission->StartTime);
				data << uint32(mission->TravelDuration);
				data << uint32(mission->MissionDuration);
				data << uint32(mission->MissionState);
			}
		}

		for (auto i = 0; i < info->ArchivedMissions; i++)
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
	PlotInfo* plot;

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
	GarrisonBuildingInfo* building;

	data << uint32(building->GarrBuildingID);

	session->SendPacket(&data);
}