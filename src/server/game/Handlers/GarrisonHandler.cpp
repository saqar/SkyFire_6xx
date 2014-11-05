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

#include "GarrisonMgr.h"

#include "WorldSession.h"
#include "WorldPacket.h"

void WorldSession::HandleGarrCompleteMission(WorldPacket& recvData)
{
    ObjectGuid npcGuid;
    uint32 MissionRecID;

    recvData >> MissionRecID;

    //sGarrisonMgr(garr->MissionFunct);
}

void WorldSession::HandleGarrSetBuildingActive(WorldPacket& recvData)
{ 
    TC_LOG_ERROR("network", "SENDING CMSG_GARR_SET_BUILDING_ACTIVE");
    uint32 PlotInstanceID;
    recvData >> PlotInstanceID;

    WorldPacket data(SMSG_GARRISON_BUILDING_ACTIVATED);
    GarrisonBuildingInfo* _building = NULL;

    data << uint32(_building->ID);

    SendPacket(&data);
}

void WorldSession::HandleGarrGenerateRecruits(WorldPacket& recvData)
{
}

void WorldSession::HandleGarrPurchaseBuilding(WorldPacket& recvData)
{
    ObjectGuid npcGuid;
    uint32 BuildingID;
    uint32 PlotInstanceID;

    recvData >> npcGuid;
    recvData >> BuildingID;
    recvData >> PlotInstanceID;

    // Functions for
    // sGarrisonMgr(garr->PurchaseBuilding);
}

void WorldSession::HandleGarrUpgrade(WorldPacket& recvData)
{
}

void WorldSession::HandleGarrGetInfo(WorldPacket& recvData)
{
}

void WorldSession::HandleGarrStartMission(WorldPacket& recvData)
{
    ObjectGuid npcGuid;
    uint32 MissionRecID;
    uint64 FollowerDBIDs;

    recvData >> npcGuid;
    recvData >> MissionRecID;

    for (uint64 i = 0; i < MAX_FOLLOWER_SLOTS; i++)
        recvData >> uint64(FollowerDBIDs);

    // Garrison function to start mission
}