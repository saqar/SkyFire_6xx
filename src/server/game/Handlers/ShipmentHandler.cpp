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

#include "ShipmentMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleOpenShipmentNpc(WorldPacket& recvData)
{
    ObjectGuid npcGuid;

    recvData >> npcGuid;

    WorldPacket data(SMSG_OPEN_SHIPMENT_NPC_RESULT);
    ObjectGuid guid;
    
    data << guid;                           // NpcGUID
    data << uint32(0);                      // CharShipmentContainerID

    SendPacket(&data);
}

void WorldSession::HandleGetShipmentInfo(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GET_SHIPMENT_INFO");
    ObjectGuid NpcGUID;

    recvData >> NpcGUID;
}

void WorldSession::HandleGetShipmentOfType(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_GET_SHIPMENT_OF_TYPE");
    uint32 container = 0;

    recvData >> container;

    WorldPacket data(SMSG_GET_SHIPMENTS_OF_TYPE_RESPONSE);
    
    data << uint32(0);

    data << uint64(0);                      // ShipmentID
    data << uint32(0);                      // ShipmentRecID
    data << uint32(0);                      // CreationTime
    data << uint32(0);                      // ShipmentDuration

    SendPacket(&data);
}

void WorldSession::HandleCreateShipment(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_CREATE_SHIPMENT");
    ObjectGuid NpcGUID;

    recvData >> NpcGUID;

    WorldPacket data(SMSG_CREATE_SHIPMENT_RESPONSE);
    ObjectGuid ShipmentID;

    data << ShipmentID;
    data << uint32(0);                      // Result
    data << uint32(0);                      // ShipmentRecID

    SendPacket(&data);
}

void WorldSession::HandleCompleteShipment(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_COMPLETE_SHIPMENT");
    ObjectGuid ShipmentID, NpcGUID;

    recvData >> ShipmentID >> NpcGUID;
}

void WorldSession::HandleCompleteAllReadyShipment(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Received CMSG_COMPLETE_ALL_READY_SHIPMENTS");
}