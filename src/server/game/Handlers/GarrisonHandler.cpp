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
    TC_LOG_DEBUG("network", "World: Received CMSG_GARR_COMPLETE_MISSION");
    ObjectGuid npcGuid;
    uint32 MissionRecID;

    recvData >> MissionRecID;

    //sGarrisonMgr(garr->MissionFunct);
}

void WorldSession::HandleGarrSetBuildingActive(WorldPacket& recvData)
{
    TC_LOG_ERROR("network", "World: Received CMSG_GARR_SET_BUILDING_ACTIVE");
    uint32 PlotInstanceID;
    recvData >> PlotInstanceID;

    WorldPacket data(SMSG_GARR_BUILDING_ACTIVATED);

    data << uint32(0);                              // BuildingID

    SendPacket(&data);
}

void WorldSession::HandleGarrGenerateRecruits(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GARR_GENERATE_RECRUITS");
    ObjectGuid guid;
    uint32 unk1, unk2;

    recvData >> guid;
    recvData >> unk1;
    recvData >> unk2;
}

void WorldSession::HandleGarrPurchaseBuilding(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GARR_PURCHASE_BUILDING");
    ObjectGuid npcGuid;
    uint32 BuildingID;
    uint32 PlotInstanceID;

    recvData >> npcGuid;
    recvData >> BuildingID;
    recvData >> PlotInstanceID;

    // Functions for
    // sGarrisonMgr(garr->PurchaseBuilding);

    WorldPacket data(SMSG_GARR_PLOT_PLACED);
    data << uint32(0);
    data << float(0);
    data << float(0);
    data << float(0);
    data << uint32(0);

    SendPacket(&data);
}

void WorldSession::HandleGarrUpgrade(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_UPGRADE_GARR");
    ObjectGuid guid;

    recvData >> guid;
}

void WorldSession::HandleGarrGetInfo(WorldPacket& recvData)
{ }

void WorldSession::HandleGarrStartMission(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GARR_START_MISSION");
    ObjectGuid npcGuid;
    uint32 MissionRecID;
    uint64 FollowerDBIDs;

    recvData >> npcGuid;
    recvData >> MissionRecID;

    for (uint64 i = 0; i < 5; i++)
        recvData >> uint64(FollowerDBIDs);

    // Garrison function to start mission

    WorldPacket data(SMSG_GARR_OPEN_ARCHITECT);
    ObjectGuid NpcGUID;

    data << NpcGUID;

    SendPacket(&data);
}

void WorldSession::HandleGarrRequestBlueprintData(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GARR_REQUEST_BLUEPRINT_AND_SPECIALIZATION_DATA");
}

void WorldSession::HandleGarrMissionBonusRoll(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GARR_MISSION_BONUS_ROLL");
    ObjectGuid guid;
    uint32 unk;

    recvData >> guid;
    recvData >> unk;
}

void WorldSession::HandleGarrSwapBuildings(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GARR_SWAP_BUILDING");
    ObjectGuid guid;
    uint32 unk1, unk2;

    recvData >> guid;
    recvData >> unk1;
    recvData >> unk2;
}

void WorldSession::HandleGarrAssignFollower(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GARR_ASSIGN_FOLLOWER_TO_BUILDING");
    ObjectGuid NpcGUID;
    uint32 PlotInstanceID;
    uint64 NpcDBID;

    recvData >> NpcGUID;
    recvData >> PlotInstanceID;
    recvData >> NpcDBID;
}

void WorldSession::HandleGarrRecruitFollower(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GARR_RECRUIT_FOLLOWER");
    ObjectGuid npcGuid;
    uint32 unk;

    recvData >> npcGuid;
    recvData >> unk;
}

void WorldSession::HandleGarrRemoveFollower(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GARR_REMOVE_FOLLOWER_FROM_BUILDING");
    ObjectGuid npcGuid;
    uint64 FollowerDBID = 0;

    recvData >> npcGuid;
    recvData >> FollowerDBID;
}
