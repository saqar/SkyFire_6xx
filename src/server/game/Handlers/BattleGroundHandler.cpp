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

#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "RatedInfo.h"
#include "RatedMgr.h"
#include "Chat.h"
#include "Language.h"
#include "Log.h"
#include "Player.h"
#include "Object.h"
#include "Opcodes.h"
#include "DisableMgr.h"
#include "Group.h"

void WorldSession::HandleBattlemasterHelloOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    recvData >> guid;

    Creature* unit = GetPlayer()->GetMap()->GetCreature(guid);
    if (!unit)
        return;

    if (!unit->IsBattleMaster())
        return;

    unit->StopMoving();

    BattlegroundTypeId bgTypeId = sBattlegroundMgr->GetBattleMasterBG(unit->GetEntry());

    if (!_player->GetBGAccessByLevel(bgTypeId))
    {
        SendNotification(LANG_YOUR_BG_LEVEL_REQ_ERROR);
        return;
    }

    SendBattleGroundList(guid, bgTypeId);
    
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEMASTER_HELLO Message from (GUID: %u TypeId:%u)", GUID_LOPART(guid), GuidHigh2TypeId(GUID_HIPART(guid)));
}

void WorldSession::SendBattleGroundList(ObjectGuid guid, BattlegroundTypeId bgTypeId)
{
    WorldPacket data;
    sBattlegroundMgr->BuildBattlegroundListPacket(&data, guid, _player, bgTypeId);
    SendPacket(&data);
}

void WorldSession::HandleWargameRequest(WorldPacket& recvData)
{
    recvData.read_skip<uint32>(); // unk (mostly 0)
    recvData.read_skip<uint32>(); // realm ID
    
    ObjectGuid Opponent;
    ObjectGuid bgGUID;

    recvData >> bgGUID;
    recvData >> Opponent;

    // Send informative message to challenger
    WorldPacket data(SMSG_WARGAME_REQUEST_SENT, 16);    

    recvData >> Opponent;

    _player->GetSession()->SendPacket(&data);    
}

void WorldSession::HandleBattlemasterJoinOpcode(WorldPacket& recvData)
{
    uint32 bgTypeId_;
    uint8 asGroup;
    bool isPremade = false;
    Group* grp = NULL;
    ObjectGuid guid;
    uint8 roleMask = 0;

    recvData >> guid;
    recvData >> roleMask;
    for (int i = 0; i < 2; i++) // blacklistedMapIds
        recvData.read_skip<uint32>();
    asGroup = recvData.ReadBit();

    bgTypeId_ = GUID_LOPART(guid);

    if (!sBattlemasterListStore.LookupEntry(bgTypeId_))
    {
        TC_LOG_ERROR("network", "Battleground: invalid bgtype (%u) received. possible cheater? player guid %u", bgTypeId_, _player->GetGUIDLow());
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeId_, NULL))
    {
        ChatHandler(this).PSendSysMessage(LANG_BG_DISABLED);
        return;
    }

    BattlegroundTypeId bgTypeId = BattlegroundTypeId(bgTypeId_);
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, RATED_TYPE_NOT_RATED);
    BattlegroundQueueTypeId bgQueueTypeIdRandom = BattlegroundMgr::BGQueueTypeId(BATTLEGROUND_RB, RATED_TYPE_NOT_RATED);

    if (_player->InBattleground())
        return;

    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg)
        return;

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
    if (!bracketEntry)
        return;

    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;

    if (!asGroup)
    {
        if (GetPlayer()->isUsingLfg())
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, ERR_LFG_CANT_USE_BATTLEGROUND);
            GetPlayer()->GetSession()->SendPacket(&data);
            return;
        }

        if (!_player->CanJoinToBattleground(bg))
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        if (_player->GetBattlegroundQueueIndex(bgQueueTypeIdRandom) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, ERR_IN_RANDOM_BG);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        if (_player->InBattlegroundQueue() && bgTypeId == BATTLEGROUND_RB)
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, ERR_IN_NON_RANDOM_BG);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        if (_player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            return;

        if (!_player->HasFreeBattlegroundQueueId())
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        GroupQueueInfo* ginfo = bgQueue.AddGroup(_player, NULL, bgTypeId, bracketEntry, RATED_TYPE_NOT_RATED, false, isPremade, 0, 0);

        uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        uint32 queueSlot = _player->AddBattlegroundQueueId(bgQueueTypeId);

        _player->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->joinTime);

        WorldPacket data;
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->joinTime, ginfo->ratedType);
        SendPacket(&data);

        TC_LOG_DEBUG("bg.battleground", "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s",
                       bgQueueTypeId, bgTypeId, _player->GetGUIDLow(), _player->GetName().c_str());
    }
    else
    {
        grp = _player->GetGroup();

        if (!grp)
            return;

        if (grp->GetLeaderGUID() != _player->GetGUID())
            return;

        err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 0, bg->GetMaxPlayersPerTeam(), false, 0);
        isPremade = (grp->GetMembersCount() >= bg->GetMinPlayersPerTeam());

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        GroupQueueInfo* ginfo = NULL;
        uint32 avgTime = 0;

        if (!err)
        {
            TC_LOG_DEBUG("bg.battleground", "Battleground: the following players are joining as group:");
            ginfo = bgQueue.AddGroup(_player, grp, bgTypeId, bracketEntry, RATED_TYPE_NOT_RATED, false, isPremade, 0, 0);
            avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        }

        for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* member = itr->GetSource();
            if (!member)
                continue;

            if (err)
            {
                WorldPacket data;
                sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, err);
                member->GetSession()->SendPacket(&data);
                continue;
            }

            uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

            member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->joinTime);

            WorldPacket data;
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->joinTime, ginfo->ratedType);
            member->GetSession()->SendPacket(&data);
            TC_LOG_DEBUG("bg.battleground", "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s",
                bgQueueTypeId, bgTypeId, member->GetGUIDLow(), member->GetName().c_str());
        }
        TC_LOG_DEBUG("bg.battleground", "Battleground: group end");
    }

    sBattlegroundMgr->ScheduleQueueUpdate(0, RATED_TYPE_NOT_RATED, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
}

void WorldSession::HandleBattlegroundPlayerPositionsOpcode(WorldPacket& /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEGROUND_PLAYER_POSITIONS Message");

    Battleground* bg = _player->GetBattleground();
    if (!bg)
        return;

    uint32 acount = 0;
    uint32 hcount = 0;
    Player* aplr = NULL;
    Player* hplr = NULL;

    if (ObjectGuid guid = bg->GetFlagPickerGUID(TEAM_ALLIANCE))
    {
        aplr = ObjectAccessor::FindPlayer(guid);
        if (aplr)
            ++acount;
    }

    if (ObjectGuid guid = bg->GetFlagPickerGUID(TEAM_HORDE))
    {
        hplr = ObjectAccessor::FindPlayer(guid);
        if (hplr)
            ++hcount;
    }

    ObjectGuid aguid = aplr ? aplr->GetGUID() : 0;
    ObjectGuid hguid = hplr ? hplr->GetGUID() : 0;

    WorldPacket data(SMSG_BATTLEGROUND_PLAYER_POSITIONS);

    data.WriteBits(acount, 22);
    for (uint8 i = 0; i < acount; i++)
        data << aguid;

    data.WriteBits(hcount, 22);
    for (uint8 i = 0; i < hcount; i++)
        data << hguid;

    for (uint8 i = 0; i < hcount; i++)
    {
        data << float(hplr->GetPositionY());
        data << float(hplr->GetPositionX());
    }

    for (uint8 i = 0; i < acount; i++)
    {
        data << float(aplr->GetPositionX());
        data << float(aplr->GetPositionY());
    }

    SendPacket(&data);
}

void WorldSession::HandlePVPLogDataOpcode(WorldPacket & /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_PVP_LOG_DATA Message");

    Battleground* bg = _player->GetBattleground();
    if (!bg)
        return;

    if (bg->IsArena())
        return;

    WorldPacket data;
    sBattlegroundMgr->BuildPvpLogDataPacket(&data, bg);
    SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_PVP_LOG_DATA Message");
}

void WorldSession::HandleBattlefieldListOpcode(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEFIELD_LIST Message");

    uint32 ListID;
    recvData >> ListID;

    BattlemasterListEntry const* bl = sBattlemasterListStore.LookupEntry(ListID);
    if (!bl)
    {
        TC_LOG_DEBUG("bg.battleground", "BattlegroundHandler: invalid ListID (%u) with player (Name: %s, GUID: %u) received.", ListID, _player->GetName().c_str(), _player->GetGUIDLow());
        return;
    }

    WorldPacket data;
    sBattlegroundMgr->BuildBattlegroundListPacket(&data, 0, _player, BattlegroundTypeId(ListID));
    SendPacket(&data);
}

void WorldSession::HandleBattleFieldPortOpcode(WorldPacket &recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEFIELD_PORT Message");

    uint32 time;
    uint32 queueSlot;
    uint32 id;
    uint8 action;
    ObjectGuid guid;

    recvData >> guid;
    recvData >> id;
    recvData >> queueSlot;
    recvData >> time;
    action = recvData.ReadBit();

    if (!_player->InBattlegroundQueue())
    {
        TC_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u. Player not in queue!",
            GetPlayerInfo().c_str(), queueSlot, id, time, action);
        return;
    }

    BattlegroundQueueTypeId bgQueueTypeId = _player->GetBattlegroundQueueTypeId(queueSlot);
    if (bgQueueTypeId == BATTLEGROUND_QUEUE_NONE)
    {
        TC_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u. Invalid queueSlot!",
            GetPlayerInfo().c_str(), queueSlot, id, time, action);
        return;
    }

    BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
    GroupQueueInfo ginfo;
    if (!bgQueue.GetPlayerGroupInfoData(_player->GetGUID(), &ginfo))
    {
        TC_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u. Player not in queue (No player Group Info)!",
            GetPlayerInfo().c_str(), queueSlot, id, time, action);
        return;
    }

    if (!ginfo.isInvitedToBGInstanceGUID && action == 1)
    {
        TC_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u. Player is not invited to any bg!",
            GetPlayerInfo().c_str(), queueSlot, id, time, action);
        return;
    }

    BattlegroundTypeId bgTypeId = BattlegroundMgr::BGTemplateId(bgQueueTypeId);
    Battleground* bg = sBattlegroundMgr->GetBattleground(ginfo.isInvitedToBGInstanceGUID, bgTypeId == BATTLEGROUND_AA ? BATTLEGROUND_TYPE_NONE : bgTypeId);
    if (!bg)
    {
        if (action)
        {
            TC_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u. Cant find BG with id %u!",
                GetPlayerInfo().c_str(), queueSlot, id, time, action, ginfo.isInvitedToBGInstanceGUID);
            return;
        }

        bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
        if (!bg)
        {
            TC_LOG_ERROR("network", "BattlegroundHandler: bg_template not found for type id %u.", bgTypeId);
            return;
        }
    }

    TC_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u.",
        GetPlayerInfo().c_str(), queueSlot, id, time, action);

    bgTypeId = bg->GetTypeID();
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
    if (!bracketEntry)
        return;

    if (action == 1 && ginfo.ratedType == 0)
    {
        if (!_player->CanJoinToBattleground(bg))
        {
            WorldPacket data2;
            sBattlegroundMgr->BuildStatusFailedPacket(&data2, bg, _player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            _player->GetSession()->SendPacket(&data2);
            action = 0;
            TC_LOG_DEBUG("bg.battleground", "Player %s (%u) has a deserter debuff, do not port him to battleground!", _player->GetName().c_str(), _player->GetGUIDLow());
        }

        if (_player->getLevel() > bg->GetMaxLevel())
        {
            TC_LOG_DEBUG("network", "Player %s (%u) has level (%u) higher than maxlevel (%u) of battleground (%u)! Do not port him to battleground!",
                _player->GetName().c_str(), _player->GetGUIDLow(), _player->getLevel(), bg->GetMaxLevel(), bg->GetTypeID());
            action = 0;
        }
    }

    WorldPacket data;
    if (action)
    {
        if (!_player->IsInvitedForBattlegroundQueueType(bgQueueTypeId))
            return;

        if (!_player->InBattleground())
            _player->SetBattlegroundEntryPoint();

        if (!_player->IsAlive())
        {
            _player->ResurrectPlayer(1.0f);
            _player->SpawnCorpseBones();
        }

        if (_player->IsInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }

        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, queueSlot, STATUS_IN_PROGRESS, _player->GetBattlegroundQueueJoinTime(bgTypeId), bg->GetElapsedTime(), bg->GetRatedType());
        _player->GetSession()->SendPacket(&data);

        bgQueue.RemovePlayer(_player->GetGUID(), false);
        if (Battleground* currentBg = _player->GetBattleground())
            currentBg->RemovePlayerAtLeave(_player->GetGUID(), false, true);

        _player->SetBattlegroundId(bg->GetInstanceID(), bgTypeId);
        _player->SetBGTeam(ginfo.team);

        sBattlegroundMgr->SendToBattleground(_player, ginfo.isInvitedToBGInstanceGUID, bgTypeId);

        TC_LOG_DEBUG("bg.battleground", "Battleground: player %s (%u) joined battle for bg %u, bgtype %u, queue type %u.", _player->GetName().c_str(), _player->GetGUIDLow(), bg->GetInstanceID(), bg->GetTypeID(), bgQueueTypeId);
    }
    else
    {
        if (ginfo.isRated && ginfo.isInvitedToBGInstanceGUID)
        {
            int16 matchmakerRatingChange, personalRatingChange;
            RatedInfo* rInfo = sRatedMgr->GetRatedInfo(_player->GetGUID());
            rInfo->UpdateStats(ginfo.ratedType, ginfo.opponentsTeamMatchmakerRating, personalRatingChange, matchmakerRatingChange, false, true);
            TC_LOG_DEBUG("bg.battleground", "UPDATING memberLost's personal arena rating for %u by opponents rating: %u, because he has left queue!", GUID_LOPART(_player->GetGUID()), ginfo.opponentsTeamRating);
        }
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, queueSlot, STATUS_NONE, _player->GetBattlegroundQueueJoinTime(bgTypeId), 0, RATED_TYPE_NOT_RATED);
        SendPacket(&data);

        _player->RemoveBattlegroundQueueId(bgQueueTypeId);
        bgQueue.RemovePlayer(_player->GetGUID(), true);

        if (!ginfo.ratedType)
            sBattlegroundMgr->ScheduleQueueUpdate(ginfo.teamMatchmakerRating, ginfo.ratedType, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

        TC_LOG_DEBUG("bg.battleground", "Battleground: player %s (%u) left queue for bgtype %u, queue type %u.", _player->GetName().c_str(), _player->GetGUIDLow(), bg->GetTypeID(), bgQueueTypeId);
    }
}

void WorldSession::HandleBattlefieldLeaveOpcode(WorldPacket& /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEFIELD_LEAVE Message");

    if (_player->IsInCombat())
        if (Battleground* bg = _player->GetBattleground())
            if (bg->GetStatus() != STATUS_WAIT_LEAVE)
                return;

    _player->LeaveBattleground();
}

void WorldSession::HandleBattlefieldStatusOpcode(WorldPacket & /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEFIELD_STATUS Message");

    WorldPacket data;
    Battleground* bg = NULL;

    for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    {
        BattlegroundQueueTypeId bgQueueTypeId = _player->GetBattlegroundQueueTypeId(i);
        if (!bgQueueTypeId)
            continue;
        BattlegroundTypeId bgTypeId = BattlegroundMgr::BGTemplateId(bgQueueTypeId);
        RatedType ratedType = BattlegroundMgr::GetRatedTypeByQueue(bgQueueTypeId);
        if (bgTypeId == _player->GetBattlegroundTypeId())
        {
            bg = _player->GetBattleground();
            if (bg && bg->GetRatedType() == ratedType)
            {
                sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, GetPlayer(), i, STATUS_IN_PROGRESS, _player->GetBattlegroundQueueJoinTime(bgTypeId), bg->GetElapsedTime(), ratedType);
                SendPacket(&data);
                continue;
            }
        }

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        GroupQueueInfo ginfo;
        if (!bgQueue.GetPlayerGroupInfoData(_player->GetGUID(), &ginfo))
            continue;
        if (ginfo.isInvitedToBGInstanceGUID)
        {
            bg = sBattlegroundMgr->GetBattleground(ginfo.isInvitedToBGInstanceGUID, bgTypeId);
            if (!bg)
                continue;

            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, GetPlayer(), i, STATUS_WAIT_JOIN, getMSTimeDiff(getMSTime(), ginfo.removeInviteTime), _player->GetBattlegroundQueueJoinTime(bgTypeId), ratedType);
            SendPacket(&data);
        }
        else
        {
            bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
            if (!bg)
                continue;

            PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
            if (!bracketEntry)
                continue;

            uint32 avgTime = bgQueue.GetAverageQueueWaitTime(&ginfo, bracketEntry->GetBracketId());
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, GetPlayer(), i, STATUS_WAIT_QUEUE, avgTime, _player->GetBattlegroundQueueJoinTime(bgTypeId), ratedType);
            SendPacket(&data);
        }
    }
}

void WorldSession::HandleBattlemasterJoinArena(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_BATTLEMASTER_JOIN_ARENA");

    uint8 arenaSlot;    
    recvData >> arenaSlot;

    if (!RatedInfo::IsArenaSlot(arenaSlot))
        return;

    if (_player->InBattleground())
        return;

    uint32 TeamArenaRating = 0;
    uint32 TeamMatchmakerRating = 0;

    RatedType arenatype = RatedInfo::GetRatedTypeBySlot(arenaSlot);

    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
    if (!bg)
    {
        TC_LOG_ERROR("network", "Battleground: template bg (all arenas) not found");
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
    {
        ChatHandler(this).PSendSysMessage(LANG_ARENA_DISABLED);
        return;
    }

    BattlegroundTypeId bgTypeId = bg->GetTypeID();
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
    if (!bracketEntry)
        return;

    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;

    Group* grp = _player->GetGroup();
    if (!grp)
        return;

    if (grp->GetLeaderGUID() != _player->GetGUID())
        return;

    RatedType ratedType = RatedInfo::GetRatedTypeBySlot(arenaSlot);
    GroupRatedStats stats = grp->GetRatedStats(ratedType);
        
    TeamArenaRating = stats.averageRating;
    TeamMatchmakerRating = stats.averageMMR;

    if (TeamArenaRating <= 0)
        TeamArenaRating = 1;

    BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

    uint32 avgTime = 0;
    GroupQueueInfo* ginfo = NULL;

    err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, arenatype, arenatype, true, arenaSlot);
    if (!err)
    {
        TC_LOG_DEBUG("bg.battleground", "Battleground: Group (GUID: %u), Leader (%s, GUID: %u) just queued arena (Average MMR: %u, Average Rating: %u, Type %u)",
            grp->GetGUID(), _player->GetName().c_str(), _player->GetGUID(), TeamMatchmakerRating, TeamArenaRating, arenatype);

        ginfo = bgQueue.AddGroup(_player, grp, bgTypeId, bracketEntry, arenatype, true, false, TeamArenaRating, TeamMatchmakerRating);
        avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
    }

    for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->GetSource();
        if (!member)
            continue;

        if (err)
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, err);
            member->GetSession()->SendPacket(&data);
            continue;
        }

        uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

        member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->joinTime);

        WorldPacket data;
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->joinTime, arenatype);
        member->GetSession()->SendPacket(&data);

        TC_LOG_DEBUG("bg.battleground", "Battleground: player joined queue for arena as group bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, member->GetGUIDLow(), member->GetName().c_str());
    }

    sBattlegroundMgr->ScheduleQueueUpdate(TeamMatchmakerRating, arenatype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
}

void WorldSession::HandleReportPvPAFK(WorldPacket& recvData)
{
    ObjectGuid playerGuid;

    recvData >> playerGuid;

    Player* reportedPlayer = ObjectAccessor::FindPlayer(playerGuid);
    if (!reportedPlayer)
    {
        TC_LOG_DEBUG("bg.battleground", "WorldSession::HandleReportPvPAFK: player not found");
        return;
    }

    TC_LOG_DEBUG("bg.battleground", "WorldSession::HandleReportPvPAFK: %s reported %s", _player->GetName().c_str(), reportedPlayer->GetName().c_str());

    reportedPlayer->ReportedAfkBy(_player);
}

void WorldSession::HandleRequestRatedInfo(WorldPacket & recvData)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_REQUEST_RATED_INFO");

    RatedInfo* rInfo = sRatedMgr->GetRatedInfo(_player->GetGUID());

    WorldPacket data(SMSG_BATTLEFIELD_RATED_INFO, 4 * 8 * MAX_RATED_SLOT);

    for (uint8 i = 0; i < MAX_RATED_SLOT; ++i)
    {
        RatedType ratedType = RatedInfo::GetRatedTypeBySlot(i);

        const StatsBySlot* stats = rInfo->GetStatsBySlot(ratedType);
        ASSERT(stats && "Stats must be already initialized");
        
        data << uint32(stats->PersonalRating);
        data << uint32(stats->Rank);
        data << uint32(stats->SeasonGames);
        data << uint32(stats->SeasonWins);
        data << uint32(stats->WeekGames);
        data << uint32(stats->WeekWins);
        data << uint32(stats->WeekBest);
        data << uint32(stats->SeasonBest);
    }

    SendPacket(&data);
}

void WorldSession::HandleRequestPvpOptions(WorldPacket& /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_REQUEST_PVP_OPTIONS_ENABLED");

    WorldPacket data(SMSG_PVP_OPTIONS_ENABLED, 1);

    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);   

    SendPacket(&data);
}

void WorldSession::HandleRequestPvpReward(WorldPacket& /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_REQUEST_PVP_REWARDS");

    _player->SendPvpRewards();
}