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

#include "LFGMgr.h"
#include "ObjectMgr.h"
#include "Group.h"
#include "Player.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void BuildPlayerLockDungeonBlock(WorldPacket& data, lfg::LfgLockMap const& lock)
{
    data << uint32(lock.size());                           // Size of lock dungeons
    for (auto lockMap : lock)
    {
        data << uint32(lockMap.first);                     // Dungeon entry (id + type)
        data << uint32(lockMap.second);                    // Lock status
        data << uint32(0);                                 // Required itemLevel
        data << uint32(0);                                 // Current itemLevel
    }
}

void BuildPartyLockDungeonBlock(WorldPacket& data, lfg::LfgLockPartyMap const& lockMap)
{
    data << uint8(lockMap.size());

    for (auto lockMapP : lockMap)
    {
        data << ObjectGuid(lockMapP.first);                         // Player guid
        BuildPlayerLockDungeonBlock(data, lockMapP.second);
    }
}

void BuildQuestReward(WorldPacket& data, Quest const* quest, Player* player)
{
    //LFGPlayerRewards
    uint32 rewCount = 0;

    data << uint32(quest->GetRewMoney());				// RewardMoney
    data << uint32(0);									// QueuedSlot
    data << uint32(quest->XPValue(player));				// AddedXP
    data << uint32(0);									// ActualSlot
    data << uint32(rewCount);							// Not actually sure what this is.

    if (rewCount)
    {
        for (uint32 i = 0; i < 4; ++i)
        {
            data << uint32(0);											// BonusCurrency
            data << uint32(quest->RewardItemIdCount[i]);
            data << uint32(quest->RewardCurrencyCount[i]);
            data.WriteBit(1);                                           // Is currency
        }
    }
}

void WorldSession::HandleLfgJoinOpcode(WorldPacket& recvData)
{
    if (!sLFGMgr->isOptionEnabled(lfg::LFG_OPTION_ENABLE_DUNGEON_FINDER | lfg::LFG_OPTION_ENABLE_RAID_BROWSER) ||
        (GetPlayer()->GetGroup() && GetPlayer()->GetGroup()->GetLeaderGUID() != GetPlayer()->GetGUID() &&
        (GetPlayer()->GetGroup()->GetMembersCount() == MAXGROUPSIZE || !GetPlayer()->GetGroup()->isLFGGroup())))
    {
        recvData.rfinish();
        return;
    }

    uint32 roles;
    uint32 numDungeons;
    bool QueueAsGroup;
    uint8 PartyIndex;

    QueueAsGroup = recvData.ReadBit();
    uint32 commentLen = recvData.ReadBits(8);
    recvData >> PartyIndex;
    recvData >> roles;

    for (int32 i = 0; i < 3; ++i)
        recvData.read_skip<uint32>(); // Needs

    recvData >> numDungeons;
    std::string comment = recvData.ReadString(commentLen);

    if (!numDungeons)
    {
        TC_LOG_DEBUG("lfg", "CMSG_LFG_JOIN %s no dungeons selected", GetPlayerInfo().c_str());
        recvData.rfinish();
        return;
    }

    lfg::LfgDungeonSet newDungeons;
    for (uint32 i = 0; i < numDungeons; ++i)
    {
        uint32 dungeon;
        recvData >> dungeon;
        newDungeons.insert((dungeon & 0x00FFFFFF));        // remove the type from the dungeon entry
    }

    TC_LOG_DEBUG("lfg", "CMSG_LFG_JOIN %s roles: %u, Dungeons: %u, Comment: %s",
        GetPlayerInfo().c_str(), roles, uint8(newDungeons.size()), comment.c_str());

    sLFGMgr->JoinLfg(GetPlayer(), uint8(roles), newDungeons, comment);
}
void WorldSession::HandleLfgLeaveOpcode(WorldPacket& recvData)
{
    ObjectGuid leaveGuid;
    Group* group = GetPlayer()->GetGroup();
    ObjectGuid RequesterGuid = GetPlayer()->GetGUID128();
    uint64 gguid = group ? group->GetGUID() : RequesterGuid;
    uint32 ClientInstanceId, QueueId, joinTime;

    sLFGMgr->ReadRideTicket(recvData, RequesterGuid, ClientInstanceId, QueueId, joinTime);

    recvData >> leaveGuid;

    TC_LOG_DEBUG("lfg", "CMSG_LFG_LEAVE %s in group: %u sent guid " UI64FMTD ".", GetPlayerInfo().c_str(), group ? 1 : 0, uint64(leaveGuid));

    // Check cheating - only leader can leave the queue
    if (!group || group->GetLeaderGUID() == RequesterGuid)
        sLFGMgr->LeaveLfg(gguid);
}

void WorldSession::HandleLfgProposalResultOpcode(WorldPacket& recvData)
{
    uint32 proposalID;  // Proposal ID
    uint32 Id, Type, joinTime;
    uint64 InstanceID;
    bool accept;

    ObjectGuid RequesterGuid;
    ObjectGuid guid2;

    sLFGMgr->ReadRideTicket(recvData, RequesterGuid, Id, Type, joinTime);

    // UserClientDFProposalResponse
    recvData >> InstanceID;
    recvData >> proposalID;
    accept = recvData.ReadBit();

    TC_LOG_DEBUG("lfg", "CMSG_LFG_PROPOSAL_RESULT %s proposal: %u accept: %u", GetPlayerInfo().c_str(), proposalID, accept ? 1 : 0);

    sLFGMgr->UpdateProposal(proposalID, GetPlayer()->GetGUID128(), accept);
}

void WorldSession::HandleLfgSetRolesOpcode(WorldPacket& recvData)
{
    uint8 roles;
    uint32 roldesDesired;

    recvData >> roldesDesired;
    recvData >> roles;

    ObjectGuid guid = GetPlayer()->GetGUID128();
    Group* group = GetPlayer()->GetGroup();

    if (!group)
    {
        TC_LOG_DEBUG("lfg", "CMSG_LFG_SET_ROLES %s Not in group", GetPlayerInfo().c_str());
        return;
    }

    ObjectGuid gguid = group->GetGUID();

    TC_LOG_DEBUG("lfg", "CMSG_LFG_SET_ROLES: Group %u, Player %s, Roles: %u", GUID_LOPART(gguid), GetPlayerInfo().c_str(), roles);
    sLFGMgr->UpdateRoleCheck(gguid, guid, roles);
}

void WorldSession::HandleLfgSetCommentOpcode(WorldPacket&  recvData)
{
    std::string comment;

    recvData >> comment;

    TC_LOG_DEBUG("lfg", "CMSG_LFG_SET_COMMENT %s comment: %s", GetPlayerInfo().c_str(), comment.c_str());

    sLFGMgr->SetComment(GetPlayer()->GetGUID(), comment);
}

void WorldSession::HandleLfgSetBootVoteOpcode(WorldPacket& recvData)
{
    bool agree;
    recvData >> agree;

    uint64 guid = GetPlayer()->GetGUID();
    TC_LOG_DEBUG("lfg", "CMSG_LFG_SET_BOOT_VOTE %s agree: %u", GetPlayerInfo().c_str(), agree ? 1 : 0);

    sLFGMgr->UpdateBoot(guid, agree);
}

void WorldSession::HandleLfgTeleportOpcode(WorldPacket& recvData)
{
    bool out;

    out = recvData.ReadBit();

    TC_LOG_DEBUG("lfg", "CMSG_LFG_TELEPORT %s out: %u", GetPlayerInfo().c_str(), out ? 1 : 0);

    sLFGMgr->TeleportPlayer(GetPlayer(), out, true);
}

void WorldSession::HandleLfgGetLockInfoOpcode(WorldPacket& recvData)
{
    bool forPlayer = recvData.ReadBit();
    TC_LOG_DEBUG("lfg", "CMSG_LFG_LOCK_INFO_REQUEST %s for %s", GetPlayerInfo().c_str(), (forPlayer ? "player" : "party"));

    if (forPlayer)
        SendLfgPlayerLockInfo();
    else
        SendLfgPartyLockInfo();
}

void WorldSession::SendLfgPlayerLockInfo()
{
    TC_LOG_DEBUG("lfg", "SMSG_LFG_PLAYER_INFO %s", GetPlayerInfo().c_str());

    uint64 guid = GetPlayer()->GetGUID();

    // Get Random dungeons that can be done at a certain level and expansion
    uint8 level = GetPlayer()->getLevel();
    lfg::LfgDungeonSet const& randomDungeons = sLFGMgr->GetRandomAndSeasonalDungeons(level, GetPlayer()->GetSession()->Expansion());

    // Get player locked Dungeons
    lfg::LfgLockMap const& lock = sLFGMgr->GetLockedDungeons(guid);
    uint32 rsize = uint32(randomDungeons.size());
    uint32 lsize = uint32(lock.size());
    size_t pSize = 1 + rsize * (4 + 1 + 4 + 4 + 4 + 4 + 1 + 4 + 4 + 4) + 4 + lsize * (1 + 4 + 4 + 4 + 4 + 1 + 4 + 4 + 4);

    WorldPacket data(SMSG_LFG_PLAYER_INFO, pSize);

    // LFG Blacklist
    bool hasPlayerGuid = true;
    ObjectGuid playerGuid;

    data.WriteBit(0);                            // HasPlayerGuid
    data << uint32(0);                           // LFGBlackListCount

    if (hasPlayerGuid)
        data << playerGuid;

    for (int i = 0; i < 24; i++)
    {
        data << uint32(0);                       // Slot
        data << uint32(0);                       // Reason
        data << uint32(0);                       // Sub1
        data << uint32(0);                       // Sub2
    }

    data << uint8(randomDungeons.size());                  // Random Dungeon count
    for (auto randomDungeonsMap : randomDungeons)
    {
        data << uint32(randomDungeonsMap);                               // Dungeon Entry (id + type)
        lfg::LfgReward const* reward = sLFGMgr->GetRandomDungeonReward(randomDungeonsMap, level);
        lfg::LfgPlayerDungeonInfo const* info = nullptr;
        Quest const* quest = NULL;
        bool done = false;
        if (reward)
        {
            quest = sObjectMgr->GetQuestTemplate(reward->firstQuest);
            if (quest)
            {
                done = !GetPlayer()->CanRewardQuest(quest, false);
                if (done)
                    quest = sObjectMgr->GetQuestTemplate(reward->otherQuest);
            }
        }

        data << uint32(info->Slot);                                              // Slot
        data << uint32(info->CompletionQuantity);                                // CompletionQuantity
        data << uint32(info->CompletionLimit);                                   // CompletionLimit
        data << uint32(info->CompletionCurrencyID);                              // CompletionCurrencyID
        data << uint32(info->SpecificQuantity);                                  // SpecificQuantity
        data << uint32(info->SpecificLimit);                                     // SpecificLimit
        data << uint32(info->OverallQuantity);                                   // OverallQuantity
        data << uint32(info->OverallLimit);                                      // OverallLimit
        data << uint32(info->PurseWeeklyQuantity);                               // PurseWeeklyQuantity
        data << uint32(info->PurseWeeklyLimit);                                  // PurseWeeklyLimit
        data << uint32(info->PurseQuantity);                                     // PurseQuantity
        data << uint32(info->PurseLimit);                                        // PurseLimit
        data << uint32(info->Quantity);                                          // Quantity
        data << uint32(info->CompletedMask);                                     // CompletedMask

        data << uint32(info->ShortageRewardCount);                               // ShortageRewardCount

        // Rewards

        data << uint32(info->Mask);                                              // Mask
        data << uint32(info->RewardMoney);                                       // RewardMoney
        data << uint32(info->RewardExperience);                                  // RewardXP
        data << uint32(info->itemCount);                                         // ItemCount
        data << uint32(info->currencyCount);                                     // currencyCount
        data << uint32(info->bonusCurrencyCount);                                // bonusCurrencyCount

        // Items
        for (uint32 j = 0; j < info->itemCount; j++)
        {
            data << uint32(info->ItemID);                                        // ItemID
            data << uint32(info->Quantity);                                      // Quantity
        }

        // Currency
        for (uint32 j = 0; j < info->currencyCount; j++)
        {
            data << uint32(info->CurrencyID);                                    // CurrencyID
            data << uint32(info->Quantity);                                      // Quantity
        }

        // BonusCurrency
        for (uint32 j = 0; j < info->bonusCurrencyCount; j++)
        {
            data << uint32(info->CurrencyID);                                    // CurrencyID
            data << uint32(info->Quantity);                                  // Quantity
        }

        bool bit3 = true;
        if (bit3)
            data << uint32(0);

        // ShortageReward
        for (uint32 k = 0; k < info->ShortageRewardCount; k++)
        {
            // Rewards

            data << uint32(info->Mask);                                          // Mask
            data << uint32(info->RewardMoney);                                   // RewardMoney
            data << uint32(info->RewardExperience);                              // RewardXP
            data << uint32(info->itemCount);                                     // ItemCount
            data << uint32(info->currencyCount);                                 // currencyCount
            data << uint32(info->bonusCurrencyCount);                            // bonusCurrencyCount

            // Items
            for (uint32 k = 0; k < info->itemCount; k++)
            {
                data << uint32(info->ItemID);                                    // ItemID
                data << uint32(info->Quantity);                                  // Quantity
            }

            // Currency
            for (uint32 k = 0; k < info->currencyCount; k++)
            {
                data << uint32(info->CurrencyID);                                // CurrencyID
                data << uint32(info->Quantity);                                  // Quantity
            }

            // BonusCurrency
            for (uint32 k = 0; k < info->bonusCurrencyCount; k++)
            {
                data << uint32(info->CurrencyID);                                // CurrencyID
                data << uint32(info->Quantity);                                  // Quantity
            }

            bool bit3 = true;
            if (bit3)
                data << uint32(0);
        }

        data.WriteBit(info->FirstReward);                                        // FirstReward
        data.WriteBit(info->ShortageEligible);                                   // ShortageEligible
    }

    SendPacket(&data);
}

void WorldSession::SendLfgPartyLockInfo()
{
    uint64 guid = GetPlayer()->GetGUID();
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    // Get the locked dungeons of the other party members
    lfg::LfgLockPartyMap lockMap;
    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* plrg = itr->GetSource();
        if (!plrg)
            continue;

        uint64 pguid = plrg->GetGUID();
        if (pguid == guid)
            continue;

        lockMap[pguid] = sLFGMgr->GetLockedDungeons(pguid);
    }

    uint32 size = 0;
    for (auto lockMapP : lockMap)
        size += 8 + 4 + uint32(lockMapP.second.size()) * (4 + 4 + 4 + 4);

    TC_LOG_DEBUG("lfg", "SMSG_LFG_PARTY_INFO %s", GetPlayerInfo().c_str());

    WorldPacket data(SMSG_LFG_PARTY_INFO, 1 + size);

    BuildPartyLockDungeonBlock(data, lockMap);

    SendPacket(&data);
}

void WorldSession::HandleLfgGetStatus(WorldPacket& /*recvData*/)
{
    TC_LOG_DEBUG("lfg", "CMSG_LFG_GET_STATUS %s", GetPlayerInfo().c_str());

    if (!GetPlayer()->isUsingLfg())
        return;

    ObjectGuid guid = GetPlayer()->GetGUID();
    lfg::LfgUpdateData updateData = sLFGMgr->GetLfgStatus(guid);

    if (GetPlayer()->GetGroup())
    {
        SendLfgUpdateStatus(updateData, true);
        updateData.dungeons.clear();
        SendLfgUpdateStatus(updateData, false);
    } else
    {
        SendLfgUpdateStatus(updateData, false);
        updateData.dungeons.clear();
        SendLfgUpdateStatus(updateData, true);
    }
}

void WorldSession::SendLfgUpdateStatus(lfg::LfgUpdateData const& updateData, bool party)
{
    bool join = false;
    bool queued = false;
    uint8 size = uint8(updateData.dungeons.size());
    ObjectGuid RequesterGuid = _player->GetGUID();
    time_t joinTime = sLFGMgr->GetQueueJoinTime(_player->GetGUID());
    uint32 queueId = sLFGMgr->GetQueueId(_player->GetGUID());

    switch (updateData.updateType)
    {
        case lfg::LFG_UPDATETYPE_JOIN_QUEUE_INITIAL:            // Joined queue outside the dungeon
            join = true;
            break;
        case lfg::LFG_UPDATETYPE_JOIN_QUEUE:
        case lfg::LFG_UPDATETYPE_ADDED_TO_QUEUE:                // Rolecheck Success
            join = true;
            queued = true;
            break;
        case lfg::LFG_UPDATETYPE_PROPOSAL_BEGIN:
            join = true;
            break;
        case lfg::LFG_UPDATETYPE_UPDATE_STATUS:
            join = updateData.state != lfg::LFG_STATE_ROLECHECK && updateData.state != lfg::LFG_STATE_NONE;
            queued = updateData.state == lfg::LFG_STATE_QUEUED;
            break;
        default:
            break;
    }

    TC_LOG_DEBUG("lfg", "SMSG_LFG_UPDATE_STATUS %s updatetype: %u, party %s", GetPlayerInfo().c_str(), updateData.updateType, party ? "true" : "false");

    WorldPacket data(SMSG_LFG_UPDATE_STATUS, 1 + 8 + 3 + 2 + 1 + updateData.comment.length() + 4 + 4 + 1 + 1 + 1 + 4 + size);

    sLFGMgr->BuildRideTicket(data, RequesterGuid, 0, queueId, joinTime);

    data << uint8(0);                                      // Reason
    data << uint8(0);                                      // Subtype
    data << uint8(updateData.updateType);                  // Lfg Update type
    for (uint8 i = 0; i < 3; ++i)
        data << uint8(0);                                  // Needs - Always 0
    data << uint32(3);
    for (auto DungeonsUpdateData : updateData.dungeons)
        data << uint32(DungeonsUpdateData);
    data.WriteBit(party);
    data.WriteBit(size > 0);                               // Extra info
    data.WriteBit(join);                                   // LFG Join
    data.WriteBit(queued);                                 // Join the queue
    data.WriteBit(0);                                      // NotifyUI
    data.WriteBits(updateData.comment.length(), 8);
    data.WriteString(updateData.comment);

    SendPacket(&data);
}

void WorldSession::SendLfgRoleChosen(uint64 guid, uint8 roles)
{
    TC_LOG_DEBUG("lfg", "SMSG_LFG_ROLE_CHOSEN %s guid: %u roles: %u",
        GetPlayerInfo().c_str(), GUID_LOPART(guid), roles);

    ObjectGuid playerGuid;
    bool accepted;

    WorldPacket data(SMSG_LFG_ROLE_CHOSEN, 8 + 1 + 4);

    data << playerGuid;                                    // Guid
    data << uint32(roles);                                 // Roles
    accepted = data.WriteBit(0);                           // Accepted

    SendPacket(&data);
}

void WorldSession::SendLfgRoleCheckUpdate(lfg::LfgRoleCheck const& roleCheck)
{
    lfg::LfgDungeonSet dungeons;
    if (roleCheck.rDungeonId)
        dungeons.insert(roleCheck.rDungeonId);
    else
        dungeons = roleCheck.dungeons;

    TC_LOG_DEBUG("lfg", "SMSG_LFG_ROLE_CHECK_UPDATE %s", GetPlayerInfo().c_str());

    bool isBeginning;

    WorldPacket data(SMSG_LFG_ROLE_CHECK_UPDATE, 4 + 1 + 1 + dungeons.size() * 4 + 1 + roleCheck.roles.size() * (8 + 1 + 4 + 1));

    data << uint8(roleCheck.state == lfg::LFG_ROLECHECK_INITIALITING);
    data << uint8(roleCheck.roles.size());                 // Players in group
    data << uint32(roleCheck.state);                       // Check result
    data << uint64(0);                                     // BgQueueID
    data << uint32(0);                                     // unk

    if (!dungeons.empty())
        for (auto dungeonsMap : dungeons)
            data << uint32(sLFGMgr->GetLFGDungeonEntry(dungeonsMap)); // Dungeon

    if (!roleCheck.roles.empty())
    {
        // Leader info MUST be sent 1st :S
        ObjectGuid guid = roleCheck.leader;
        uint8 roles = roleCheck.roles.find(guid)->second;
        Player* player = ObjectAccessor::FindPlayer(guid);

        data << ObjectGuid(guid);                        // Guid
        data << uint32(roles);                           // Roles
        data << uint8(player ? player->getLevel() : 0);  // Level
        data.WriteBit(0);                                // RoleCheckComplete

        for (auto roleCheckMap : roleCheck.roles)
        {
            if (roleCheckMap.first == roleCheck.leader)
                continue;

            guid = roleCheckMap.first;
            roles = roleCheckMap.second;
            player = ObjectAccessor::FindPlayer(guid);

            data << ObjectGuid(guid);                      // Guid
            data << uint32(roles);                         // Roles
            data << uint8(player ? player->getLevel() : 0);// Level
            data.WriteBit(0);                              // RoleCheckComplete
        }
    }

    isBeginning = data.WriteBit(0);
    data.WriteBit(0);

    SendPacket(&data);
}

void WorldSession::SendLfgJoinResult(lfg::LfgJoinResultData const& joinData)
{
    uint32 size = 0;
    ObjectGuid RequesterGuid = GetPlayer()->GetGUID();
    uint32 queueId = sLFGMgr->GetQueueId(_player->GetGUID());
    for (auto it : joinData.lockmap)
        size += 8 + 4 + uint32(it.second.size()) * (4 + 4 + 4 + 4);

    TC_LOG_DEBUG("lfg", "SMSG_LFG_JOIN_RESULT %s checkResult: %u checkValue: %u", GetPlayerInfo().c_str(), joinData.result, joinData.state);

    WorldPacket data(SMSG_LFG_JOIN_RESULT, 4 + 4 + size);
    uint32 blacklistCount = 0;
    uint32 slotsCount = 0;

    sLFGMgr->BuildRideTicket(data, RequesterGuid, 0, 0, 0);

    data << uint8(joinData.result);						// Result
    data << uint8(joinData.state);						// ResultDetail

    for (uint32 i = 0; i < blacklistCount; i++)
    {
        data << RequesterGuid;

        for (uint32 j = 0; j < slotsCount; i++)
        {
            data << uint32(joinData.Slot);				// Slot
            data << uint32(joinData.Reason);			// Reason
            data << uint32(joinData.SubReasonOne);		// SubReason1
            data << uint32(joinData.SubReasonTwo);		// SubReason2
        }
    }

    SendPacket(&data);
}

void WorldSession::SendLfgQueueStatus(lfg::LfgQueueStatusData const& queueData)
{
    TC_LOG_DEBUG("lfg", "SMSG_LFG_QUEUE_STATUS %s dungeon: %u, waitTime: %d, "
        "avgWaitTime: %d, waitTimeTanks: %d, waitTimeHealer: %d, waitTimeDps: %d, "
        "queuedTime: %u, tanks: %u, healers: %u, dps: %u",
        GetPlayerInfo().c_str(), queueData.dungeonId, queueData.waitTime, queueData.waitTimeAvg,
        queueData.waitTimeTank, queueData.waitTimeHealer, queueData.waitTimeDps,
        queueData.queuedTime, queueData.tanks, queueData.healers, queueData.dps);

    ObjectGuid RequesterGuid = _player->GetGUID128();
    WorldPacket data(SMSG_LFG_QUEUE_STATUS, 500);                   // We guess size

    sLFGMgr->BuildRideTicket(data, RequesterGuid, 0, 0, 0);

    data << uint32(queueData.subType);
    data << uint32(queueData.reason);

    for (auto i = 0; i < QUEUE_NEEDS; i++)
        data << uint32(queueData.needs);

    data << uint32(queueData.slotsCount);
    data << uint32(queueData.requestedRoles);
    data << uint32(queueData.SuspendedPlayersCount);

    for (uint32 i = 0; i < queueData.slotsCount; i++)
        data << uint32(queueData.slots);

    for (uint32 i = 0; i < queueData.SuspendedPlayersCount; i++)
        data << ObjectGuid(queueData.suspendedPlayers);

    data.WriteBit(queueData.IsParty);
    data.WriteBit(queueData.Notify);
    data.WriteBit(queueData.Joined);
    data.WriteBit(queueData.LfgJoined);
    data.WriteBit(queueData.Queued);

    data.WriteBits(0, 8);
    data.WriteString(queueData.Comment);

    SendPacket(&data);
}

void WorldSession::SendLfgPlayerReward(lfg::LfgPlayerRewardData const& rewardData)
{
    if (!rewardData.rdungeonEntry || !rewardData.sdungeonEntry || !rewardData.quest)
        return;

    TC_LOG_DEBUG("lfg", "SMSG_LFG_PLAYER_REWARD %s rdungeonEntry: %u, sdungeonEntry: %u, done: %u",
        GetPlayerInfo().c_str(), rewardData.rdungeonEntry, rewardData.sdungeonEntry, rewardData.done);

    WorldPacket data(SMSG_LFG_PLAYER_REWARD, 4 + 4 + 1 + 4 + 4 + 4 + 4 + 4 + 1 * (4 + 4 + 4));

    BuildQuestReward(data, rewardData.quest, GetPlayer());

    SendPacket(&data);
}

void WorldSession::SendLfgBootProposalUpdate(lfg::LfgPlayerBoot const& boot)
{
    ObjectGuid guid = GetPlayer()->GetGUID();
    lfg::LfgAnswer playerVote = boot.votes.find(guid)->second;
    uint8 votesNum = 0;
    uint8 agreeNum = 0;
    uint32 secsleft = uint8((boot.cancelTime - time(NULL)) / 1000);
    for (auto it : boot.votes)
    {
        if (it.second != lfg::LFG_ANSWER_PENDING)
        {
            ++votesNum;
            if (it.second == lfg::LFG_ANSWER_AGREE)
                ++agreeNum;
        }
    }
    TC_LOG_DEBUG("lfg", "SMSG_LFG_BOOT_PROPOSAL_UPDATE %s inProgress: %u - "
        "didVote: %u - agree: %u - victim: %u votes: %u - agrees: %u - left: %u - "
        "needed: %u - reason %s",
        GetPlayerInfo().c_str(), uint8(boot.inProgress), uint8(playerVote != lfg::LFG_ANSWER_PENDING),
        uint8(playerVote == lfg::LFG_ANSWER_AGREE), GUID_LOPART(boot.victim), votesNum, agreeNum,
        secsleft, lfg::LFG_GROUP_KICK_VOTES_NEEDED, boot.reason.c_str());

    WorldPacket data(SMSG_LFG_BOOT_PROPOSAL_UPDATE, 1 + 1 + 1 + 1 + 8 + 4 + 4 + 4 + 4 + boot.reason.length());

    data.WriteBit(boot.inProgress);                                 // VoteInProgress
    data.WriteBit(boot.VotePassed);                                 // VotePassed
    data.WriteBit(boot.MyVoteComplete);                             // MyVoteComplete
    data.WriteBit(boot.MyVote);                                     // MyVote
    data.WriteBits(0, 8);                                           // ReasonLen
    data << ObjectGuid(boot.victim);                                // VictimGUID
    data << uint32(boot.TotalVotes);                                // TotalVotes
    data << uint32(boot.BootVotes);                                 // BootVotes
    data << uint32(boot.TimeLeft);                                  // TimeLeft
    data << uint32(lfg::LFG_GROUP_KICK_VOTES_NEEDED);               // NeededVotes
    data << boot.reason.c_str();                                    // Reason

    SendPacket(&data);
}

void WorldSession::SendLfgUpdateProposal(lfg::LfgProposal const& proposal)
{
    ObjectGuid RequesterGuid = GetPlayer()->GetGUID();
    ObjectGuid gguid = proposal.players.find(RequesterGuid)->second.group;
    bool silent = !proposal.isNew && gguid == proposal.group;
    uint32 dungeonEntry = proposal.dungeonId;
    uint32 queueId = sLFGMgr->GetQueueId(_player->GetGUID());
    time_t joinTime = sLFGMgr->GetQueueJoinTime(_player->GetGUID());

    TC_LOG_DEBUG("lfg", "SMSG_LFG_PROPOSAL_UPDATE %s state: %u", GetPlayerInfo().c_str(), proposal.state);

    // show random dungeon if player selected random dungeon and it's not lfg group
    if (!silent)
    {
        lfg::LfgDungeonSet const& playerDungeons = sLFGMgr->GetSelectedDungeons(RequesterGuid);
        if (playerDungeons.find(proposal.dungeonId) == playerDungeons.end())
            dungeonEntry = (*playerDungeons.begin());
    }

    dungeonEntry = sLFGMgr->GetLFGDungeonEntry(dungeonEntry);

    WorldPacket data(SMSG_LFG_PROPOSAL_UPDATE, 4 + 1 + 4 + 4 + 1 + 1 + proposal.players.size() * (4 + 1 + 1 + 1 + 1 + 1));

    sLFGMgr->BuildRideTicket(data, RequesterGuid, 0, queueId, joinTime);

    //ClientLFGProposalUpdate
    data << uint64(proposal.InstanceID);                    // InstanceID
    data << uint32(proposal.CompletedMask);                 // CompletedMask
    data << uint32(proposal.Slot);                          // Slot
    data << uint8(proposal.state);                          // State
    data << uint32(proposal.ProposalID);                    // ProposalID
    data << uint32(0);                                      // Unknown

    data.WriteBit(silent);

    for (auto proposalPlayersMap : proposal.players)
    {
        lfg::LfgProposalPlayer const& player = proposalPlayersMap.second;

        data << uint32(player.Role);                        // Role
        data.WriteBit(player.Me);                           // Me
        data.WriteBit(player.SameParty);                    // SameParty
        data.WriteBit(player.MyParty);                      // MyParty
        data.WriteBit(player.Responded);                    // Responded
        data.WriteBit(player.Accepted);                     // Accepted
    }

    SendPacket(&data);
}

void WorldSession::SendLfgLfrList(bool update)
{
    TC_LOG_DEBUG("lfg", "SMSG_LFG_LFR_LIST %s update: %u", GetPlayerInfo().c_str(), update ? 1 : 0);

    WorldPacket data(SMSG_LFG_UPDATE_SEARCH, 1);

    data << uint8(update);                                 // In Lfg Queue?

    SendPacket(&data);
}

void WorldSession::SendLfgDisabled()
{
    TC_LOG_DEBUG("lfg", "SMSG_LFG_DISABLED %s", GetPlayerInfo().c_str());

    WorldPacket data(SMSG_LFG_DISABLED, 0);

    SendPacket(&data);
}

void WorldSession::SendLfgOfferContinue(uint32 dungeonEntry)
{
    TC_LOG_DEBUG("lfg", "SMSG_LFG_OFFER_CONTINUE %s dungeon entry: %u", GetPlayerInfo().c_str(), dungeonEntry);

    WorldPacket data(SMSG_LFG_OFFER_CONTINUE, 4);

    data << dungeonEntry;

    SendPacket(&data);
}

void WorldSession::SendLfgTeleportError(uint8 err)
{
    TC_LOG_DEBUG("lfg", "SMSG_LFG_TELEPORT_DENIED %s reason: %u", GetPlayerInfo().c_str(), err);
    WorldPacket data(SMSG_LFG_TELEPORT_DENIED, 4);

    data << uint32(err);

    SendPacket(&data);
}

void WorldSession::HandleDungeonFinderOpen(WorldPacket& data)
{
    TC_LOG_DEBUG("lfg", "SMSG_OPEN_LFG_DUNGEON_FINDER");

    data << uint32(0);

    SendPacket(&data);
}

void WorldSession::HandleLfgGetSystemInfo(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "World: Receiving CMSG_LFG_GET_SYSTEM_INFO");

    uint8 partyIndex;
    bool player;

    player = recvData.ReadBit();
    recvData >> partyIndex;
}
