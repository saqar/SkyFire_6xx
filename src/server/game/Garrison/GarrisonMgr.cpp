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

	m_GarrisonsStore.clear();
	m_GarrisonInfoStore.clear();
	m_FollowerStore.clear();
	m_BuildingSet.clear();
	m_PlotInfoStore.clear();
}

/* I WANT TO REWORK THE GARRISON SYSTEM ENTIRELY, NOW THAT I'VE PLAYED IT FOR REAL, I KNOW HOW IT SHOULD BE DONE*/

void GarrisonMgr::SendGarrisonGetInfo(WorldSession* session)
{
	TC_LOG_DEBUG("network", "World: Sent SMSG_GET_GARR_INFO_RESULT");

	WorldPacket data(SMSG_GET_GARR_INFO_RESULT, 500);				// SizeUnknown

	for (GarrisonInfoSet::const_iterator itr = m_GarrisonInfoStore.begin(); itr != m_GarrisonInfoStore.end(); itr++)
	{
		const GarrisonInfo* info = *itr;

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

		for (uint32 i = 0; i < info->GarrisonFollowerCount; i++)
		{
			// sub_61D8C6
			data << uint64(0); // DbID
			data << uint32(0); // 2
			data << uint32(0); // 3
			data << uint32(0); // 4
			data << uint32(0); // 5
			data << uint32(0); // 6
			data << uint32(0); // 7
			data << uint32(0); // 8
			data << uint32(0); // 9
			data << uint32(info->AbilityIDCount);
			data << uint32(0); // 14
			for (uint32 z = 0; z < info->AbilityIDCount; z++)
				data << uint32(0); // AbilityID
		}

		for (uint32 i = 0; i < info->GarrisonMission; i++)
		{
			// sub_61BED1
			data << uint64(0); // DbID
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
		}

		for (uint32 i = 0; i < info->ArchivedMissions; i++)
			data << uint32(info->ArchivedMissions);
	}

	session->SendPacket(&data);
}