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
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Object.h"

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Opcodes.h"
#include "Player.h"


void WorldSession::SendBfInvitePlayerToWar(uint64 QueueID, uint32 AreaID, uint32 Time)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_ENTRY_INVITE, 4 + 4 + 16);
    
    data << QueueID;
    data << AreaID;
    data << Time;

    SendPacket(&data);
}

void WorldSession::SendBfInvitePlayerToQueue(ObjectGuid guid)
{
    ObjectGuid guidBytes = guid;

    WorldPacket data(SMSG_BATTLEFIELD_MGR_QUEUE_INVITE, 5);

    data.WriteBit(1);
    data.WriteBit(0);
    data.WriteBit(1);
    data << guidBytes;
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(0);
    data.WriteBit(1);
    data << uint8(1);               // Warmup

    SendPacket(&data);
}

void WorldSession::SendBfQueueInviteResponse(ObjectGuid guid, uint32 ZoneId, bool CanQueue, bool Full)
{
    const bool hasSecondGuid = false;
    const bool warmup = true;
    ObjectGuid playerGuid;

    WorldPacket data(SMSG_BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE, 16);
    data << float(0);
    data << uint32(ZoneId);
    data << uint8(CanQueue);
    data << playerGuid;
    data << uint8(warmup);
    data.WriteBit(Full);

    SendPacket(&data);
}

void WorldSession::SendBfEntered(ObjectGuid guid)
{
    uint8 isAFK = _player->isAFK() ? 1 : 0;

    WorldPacket data(SMSG_BATTLEFIELD_MGR_ENTERED, 11);

    data.WriteBit(0);
    data.WriteBit(isAFK);
    data.WriteBit(0);
    data << float(0);
    data.FlushBits();

    SendPacket(&data);
}

void WorldSession::SendBfLeaveMessage(ObjectGuid guid, BFLeaveReason reason)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_EJECTED, 11);
    data << float(0);
    data << uint8(2);               // BattleStatus
    data << uint8(reason);          // Reason
    data.WriteBit(0);               // Relocated

    SendPacket(&data);
}

void WorldSession::HandleBfQueueInviteResponse(WorldPacket& recvData)
{
    uint8 accepted;
    ObjectGuid guid;

    recvData >> guid;
    accepted = recvData.ReadBit();

    TC_LOG_ERROR("misc", "HandleQueueInviteResponse: GUID:"UI64FMTD" Accepted:%u", (uint64)guid, accepted);

    Battlefield* bf = sBattlefieldMgr->GetBattlefieldByGUID(guid);
    if (!bf)
        return;

    if (accepted)
        bf->PlayerAcceptInviteToQueue(_player);
}

void WorldSession::HandleBfEntryInviteResponse(WorldPacket& recvData)
{
    uint8 accepted;
    ObjectGuid guid;

    accepted = recvData.ReadBit();
    recvData >> guid;

    TC_LOG_ERROR("misc", "HandleBattlefieldInviteResponse: GUID:"UI64FMTD" Accepted:%u", (uint64)guid, accepted);

    Battlefield* bf = sBattlefieldMgr->GetBattlefieldByGUID(guid);
    if (!bf)
        return;

    if (accepted)
        bf->PlayerAcceptInviteToWar(_player);
    else
        if (_player->GetZoneId() == bf->GetZoneId())
            bf->KickPlayerFromBattlefield(_player->GetGUID());
}

void WorldSession::HandleBfExitRequest(WorldPacket& recvData)
{
    ObjectGuid guid;

    recvData >> guid;

    TC_LOG_ERROR("misc", "HandleBfExitRequest: GUID:"UI64FMTD" ", (uint64)guid);

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldByGUID(guid))
        bf->AskToLeaveQueue(_player);
}
