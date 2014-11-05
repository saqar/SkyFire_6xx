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

    WorldPacket data(SMSG_GARR_BUILDING_ACTIVATED);

    data << uint32(0);                              // BuildingID

    SendPacket(&data);
}

void WorldSession::HandleGarrGenerateRecruits(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint32 unk1, unk2;

    recvData >> guid;
    recvData >> unk1;
    recvData >> unk2;
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
    ObjectGuid guid;

    recvData >> guid;
}

void WorldSession::HandleGarrGetInfo(WorldPacket& recvData)
{
    WorldPacket data(SMSG_GET_GARR_INFO_RESULT);
    ObjectGuid guid;
    bool active;

    data << uint32(0);                              // ArchivedMissions
    data << uint32(0);                              // GarrSiteID
    data << uint32(0);                              // FactionIndex
    data << uint32(0);                              // GarrSiteLevelID
    data << uint32(0);                              // Unk
    data << uint32(0);                              // Unk
    data << uint32(0);                              // Unk
    data << uint32(0);                              // Unk
    data << uint32(0);                              // Unk

    //GarrisonMission
    data << uint64(0);                              // DBID
    data << uint32(0);                              // MissionRecID
    data << uint32(0);                              // OfferTime
    data << uint32(0);                              // OfferDuration
    data << uint32(0);                              // StartTime
    data << uint32(0);                              // TravelDuration
    data << uint32(0);                              // MissionDuration
    data << uint32(0);                              // MissionState

    // GarrisonBuildingInfo
    data << uint32(0);                              // GarrPlotInstanceID
    //data << float(0);                             // PositionX
    //data << float(0);                             // PositionY
    //data << float(0);                             // PositionZ
    data << uint32(0);
    data << uint32(0); 
    data << uint32(0);
    data << uint32(0);                              // GarrBuildingID
    active = data.WriteBit(0);                      // Active

    //GarrisonFollower
    data << uint64(0);                              // DBID
    data << uint32(0);                              // GarrFollowerID
    data << uint32(0);                              // CreatureID
    data << uint32(0);                              // Gender
    data << uint32(0);                              // Spec
    data << uint32(0);                              // Race
    data << uint32(0);                              // Quality
    data << uint32(0);                              // FollowerLevel
    data << uint32(0);                              // ItemLevelWeapon
    data << uint32(0);                              // ItemLevelArmor
    data << uint32(0);                              // Xp

    GarrAbilityEntry* ability;
    for (uint32 i = 0; i < ability->ID; i++)
        uint32(ability->SpellID);                   // AbilityID

    // GarrisonPlotInfo
    data << uint32(0);                              // GarrPlotInstanceID
    data << float(0);                               // PositionX
    data << float(0);                               // PositionY
    data << float(0);                               // PositionZ
    data << uint32(0);                              // PlotType


    data << uint32(0);                              // Unk

    SendPacket(&data);

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