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

#include "RatedInfo.h"
#include "RatedMgr.h"
#include "BattlegroundMgr.h"
#include "BattlegroundQueue.h"
#include "Chat.h"
#include "Group.h"
#include "Log.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "Player.h"

/*********************************************************/
/***            BATTLEGROUND QUEUE SYSTEM              ***/
/*********************************************************/

BattlegroundQueue::BattlegroundQueue()
{
    for (uint32 i = 0; i < BG_TEAMS_COUNT; ++i)
    {
        for (uint32 j = 0; j < MAX_BATTLEGROUND_BRACKETS; ++j)
        {
            m_SumOfWaitTimes[i][j] = 0;
            m_WaitTimeLastPlayer[i][j] = 0;
            for (uint32 k = 0; k < COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME; ++k)
                m_WaitTimes[i][j][k] = 0;
        }
    }
}

BattlegroundQueue::~BattlegroundQueue()
{
    m_events.KillAllEvents(false);

    for (int i = 0; i < MAX_BATTLEGROUND_BRACKETS; ++i)
    {
        for (uint32 j = 0; j < BG_QUEUE_GROUP_TYPES_COUNT; ++j)
        {
            for (GroupsQueueType::iterator itr = m_QueuedGroups[i][j].begin(); itr!= m_QueuedGroups[i][j].end(); ++itr)
                delete (*itr);
        }
    }
}

/*********************************************************/
/***      BATTLEGROUND QUEUE SELECTION POOLS           ***/
/*********************************************************/

void BattlegroundQueue::SelectionPool::Init()
{
    SelectedGroups.clear();
    PlayerCount = 0;
}

bool BattlegroundQueue::SelectionPool::KickGroup(uint32 size)
{
    bool found = false;
    GroupsQueueType::iterator groupToKick = SelectedGroups.begin();
    for (GroupsQueueType::iterator itr = groupToKick; itr != SelectedGroups.end(); ++itr)
    {
        if (abs((int32)((*itr)->players.size() - size)) <= 1)
        {
            groupToKick = itr;
            found = true;
        }
        else if (!found && (*itr)->players.size() >= (*groupToKick)->players.size())
            groupToKick = itr;
    }
    if (GetPlayerCount())
    {
        GroupQueueInfo* ginfo = (*groupToKick);
        SelectedGroups.erase(groupToKick);
        PlayerCount -= ginfo->players.size();
        if (ginfo->players.size() <= size + 1)
            return false;
    }
    return true;
}

bool BattlegroundQueue::SelectionPool::AddGroup(GroupQueueInfo* ginfo, uint32 desiredCount)
{
    if (!ginfo->isInvitedToBGInstanceGUID && desiredCount >= PlayerCount + ginfo->players.size())
    {
        SelectedGroups.push_back(ginfo);
        PlayerCount += ginfo->players.size();
        return true;
    }
    if (PlayerCount < desiredCount)
        return true;
    return false;
}

/*********************************************************/
/***               BATTLEGROUND QUEUES                 ***/
/*********************************************************/

GroupQueueInfo* BattlegroundQueue::AddGroup(Player* leader, Group* grp, BattlegroundTypeId bgTypeId, PvPDifficultyEntry const*  bracketEntry, RatedType ratedType, bool isRated, bool isPremade, uint32 arenaRating, uint32 matchmakerRating)
{
    BattlegroundBracketId bracketId = bracketEntry->GetBracketId();

    GroupQueueInfo* ginfo                   = new GroupQueueInfo;
    ginfo->bgTypeId                         = bgTypeId;
    ginfo->ratedType                        = ratedType;
    ginfo->isRated                          = isRated;
    ginfo->isInvitedToBGInstanceGUID        = 0;
    ginfo->joinTime                         = getMSTime();
    ginfo->removeInviteTime                 = 0;
    ginfo->leaderGUID                       = leader->GetGUID();
    ginfo->team                             = leader->GetTeam();
    ginfo->teamRating                       = arenaRating;
    ginfo->teamMatchmakerRating             = matchmakerRating;
    ginfo->opponentsTeamRating              = 0;
    ginfo->opponentsTeamMatchmakerRating    = 0;

    ginfo->players.clear();

    uint32 index = 0;
    if (!isRated && !isPremade)
        index += BG_TEAMS_COUNT;
    if (ginfo->team == HORDE)
        index++;
    TC_LOG_DEBUG("bg.battleground", "Adding Group to BattlegroundQueue bgTypeId : %u, bracket_id : %u, index : %u", bgTypeId, bracketId, index);

    uint32 lastOnlineTime = getMSTime();

    if (isRated && sWorld->getBoolConfig(CONFIG_ARENA_QUEUE_ANNOUNCER_ENABLE))
    {
        /* Need to set the new announce text
        ArenaTeam* Team = sArenaTeamMgr->GetArenaTeamById(arenateamid);
        if (Team)
            sWorld->SendWorldText(LANG_ARENA_QUEUE_ANNOUNCE_WORLD_JOIN, Team->GetName().c_str(), ginfo->RatedType, ginfo->RatedType, ginfo->ArenaTeamRating);
        */
    }

    if (grp)
    {
        for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* member = itr->GetSource();
            if (!member)
                continue;
            PlayerQueueInfo& pl_info = m_QueuedPlayers[member->GetGUID()];
            pl_info.LastOnlineTime   = lastOnlineTime;
            pl_info.GroupInfo        = ginfo;
            ginfo->players[member->GetGUID()]  = &pl_info;
        }
    }
    else
    {
        PlayerQueueInfo& pl_info = m_QueuedPlayers[leader->GetGUID()];
        pl_info.LastOnlineTime   = lastOnlineTime;
        pl_info.GroupInfo        = ginfo;
        ginfo->players[leader->GetGUID()]  = &pl_info;
    }

    {
        m_QueuedGroups[bracketId][index].push_back(ginfo);

        if (!isRated && !isPremade && sWorld->getBoolConfig(CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_ENABLE))
        {
            if (Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(ginfo->bgTypeId))
            {
                char const* bgName = bg->GetName();
                uint32 MinPlayers = bg->GetMinPlayersPerTeam();
                uint32 qHorde = 0;
                uint32 qAlliance = 0;
                uint32 q_min_level = bracketEntry->minLevel;
                uint32 q_max_level = bracketEntry->maxLevel;
                GroupsQueueType::const_iterator itr;
                for (itr = m_QueuedGroups[bracketId][BG_QUEUE_NORMAL_ALLIANCE].begin(); itr != m_QueuedGroups[bracketId][BG_QUEUE_NORMAL_ALLIANCE].end(); ++itr)
                    if (!(*itr)->isInvitedToBGInstanceGUID)
                        qAlliance += (*itr)->players.size();
                for (itr = m_QueuedGroups[bracketId][BG_QUEUE_NORMAL_HORDE].begin(); itr != m_QueuedGroups[bracketId][BG_QUEUE_NORMAL_HORDE].end(); ++itr)
                    if (!(*itr)->isInvitedToBGInstanceGUID)
                        qHorde += (*itr)->players.size();

                if (sWorld->getBoolConfig(CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_PLAYERONLY))
                    ChatHandler(leader->GetSession()).PSendSysMessage(LANG_BG_QUEUE_ANNOUNCE_SELF, bgName, q_min_level, q_max_level, qAlliance, (MinPlayers > qAlliance) ? MinPlayers - qAlliance : (uint32)0, qHorde, (MinPlayers > qHorde) ? MinPlayers - qHorde : (uint32)0);
                else
                    sWorld->SendWorldText(LANG_BG_QUEUE_ANNOUNCE_WORLD, bgName, q_min_level, q_max_level, qAlliance, (MinPlayers > qAlliance) ? MinPlayers - qAlliance : (uint32)0, qHorde, (MinPlayers > qHorde) ? MinPlayers - qHorde : (uint32)0);
            }
        }
    }

    return ginfo;
}

void BattlegroundQueue::PlayerInvitedToBGUpdateAverageWaitTime(GroupQueueInfo* ginfo, BattlegroundBracketId bracket_id)
{
    uint32 timeInQueue = getMSTimeDiff(ginfo->joinTime, getMSTime());
    uint8 team_index = TEAM_ALLIANCE;
    if (!ginfo->ratedType)
    {
        if (ginfo->team == HORDE)
            team_index = TEAM_HORDE;
    }
    else
    {
        if (ginfo->isRated)
            team_index = TEAM_HORDE;
    }

    uint32* lastPlayerAddedPointer = &(m_WaitTimeLastPlayer[team_index][bracket_id]);
    m_SumOfWaitTimes[team_index][bracket_id] -= m_WaitTimes[team_index][bracket_id][(*lastPlayerAddedPointer)];
    m_WaitTimes[team_index][bracket_id][(*lastPlayerAddedPointer)] = timeInQueue;
    m_SumOfWaitTimes[team_index][bracket_id] += timeInQueue;
    (*lastPlayerAddedPointer)++;
    (*lastPlayerAddedPointer) %= COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME;
}

uint32 BattlegroundQueue::GetAverageQueueWaitTime(GroupQueueInfo* ginfo, BattlegroundBracketId bracket_id) const
{
    uint8 team_index = TEAM_ALLIANCE;
    if (!ginfo->ratedType)
    {
        if (ginfo->team == HORDE)
            team_index = TEAM_HORDE;
    }
    else
    {
        if (ginfo->isRated)
            team_index = TEAM_HORDE;
    }
    if (m_WaitTimes[team_index][bracket_id][COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME - 1])
        return (m_SumOfWaitTimes[team_index][bracket_id] / COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME);
    else
        return 0;
}

void BattlegroundQueue::RemovePlayer(uint64 guid, bool decreaseInvitedCount)
{
    int32 bracket_id = -1;
    QueuedPlayersMap::iterator itr;

    itr = m_QueuedPlayers.find(guid);
    if (itr == m_QueuedPlayers.end())
    {
        std::string playerName = "Unknown";
        if (Player* player = ObjectAccessor::FindPlayer(guid))
            playerName = player->GetName();
        TC_LOG_ERROR("bg.battleground", "BattlegroundQueue: couldn't find player %s (GUID: %u)", playerName.c_str(), GUID_LOPART(guid));
        return;
    }

    GroupQueueInfo* group = itr->second.GroupInfo;
    GroupsQueueType::iterator group_itr;
    uint32 index = (group->team == HORDE) ? BG_QUEUE_PREMADE_HORDE : BG_QUEUE_PREMADE_ALLIANCE;

    for (int32 bracket_id_tmp = MAX_BATTLEGROUND_BRACKETS - 1; bracket_id_tmp >= 0 && bracket_id == -1; --bracket_id_tmp)
    {
        for (uint32 j = index; j < BG_QUEUE_GROUP_TYPES_COUNT; j += BG_TEAMS_COUNT)
        {
            GroupsQueueType::iterator k = m_QueuedGroups[bracket_id_tmp][j].begin();
            for (; k != m_QueuedGroups[bracket_id_tmp][j].end(); ++k)
            {
                if ((*k) == group)
                {
                    bracket_id = bracket_id_tmp;
                    group_itr = k;
                    index = j;
                    break;
                }
            }
        }
    }

    if (bracket_id == -1)
    {
        TC_LOG_ERROR("bg.battleground", "BattlegroundQueue: ERROR Cannot find groupinfo for player GUID: %u", GUID_LOPART(guid));
        return;
    }
    TC_LOG_DEBUG("bg.battleground", "BattlegroundQueue: Removing player GUID %u, from bracket_id %u", GUID_LOPART(guid), (uint32)bracket_id);

    std::map<uint64, PlayerQueueInfo*>::iterator pitr = group->players.find(guid);
    if (pitr != group->players.end())
        group->players.erase(pitr);

    if (decreaseInvitedCount && group->isInvitedToBGInstanceGUID)
        if (Battleground* bg = sBattlegroundMgr->GetBattleground(group->isInvitedToBGInstanceGUID, group->bgTypeId))
            bg->DecreaseInvitedCount(group->team);

    m_QueuedPlayers.erase(itr);

    // announce to world if arena team left queue for rated match, show only once
    /* need to update message
    if (group->RatedType && group->IsRated && group->Players.empty() && sWorld->getBoolConfig(CONFIG_ARENA_QUEUE_ANNOUNCER_ENABLE))
        if (ArenaTeam* Team = sArenaTeamMgr->GetArenaTeamById(group->ArenaTeamId))
            sWorld->SendWorldText(LANG_ARENA_QUEUE_ANNOUNCE_WORLD_EXIT, Team->GetName().c_str(), group->RatedType, group->RatedType, group->ArenaTeamRating);
    */

    if (group->isInvitedToBGInstanceGUID && group->isRated && decreaseInvitedCount)
    {
        TC_LOG_DEBUG("bg.battleground", "UPDATING memberLost's personal arena rating for %u by opponents rating: %u", GUID_LOPART(guid), group->opponentsTeamRating);
        
        int16 personalRatingChange, matchmakerRatingChange;
        RatedInfo* rInfo = nullptr;

        if (Player* player = ObjectAccessor::FindPlayer(guid))
        {
            rInfo = sRatedMgr->GetRatedInfo(player->GetGUID());
            rInfo->UpdateStats(group->ratedType, group->opponentsTeamMatchmakerRating, personalRatingChange, matchmakerRatingChange, false, false);
        }
        else
        {
            rInfo = sRatedMgr->GetRatedInfo(guid);
            rInfo->UpdateStats(group->ratedType, group->opponentsTeamMatchmakerRating, personalRatingChange, matchmakerRatingChange, false, true);
        }
    }

    if (group->players.empty())
    {
        m_QueuedGroups[bracket_id][index].erase(group_itr);
        delete group;
        return;
    }

    if (!group->isInvitedToBGInstanceGUID && group->isRated)
    {

        if (Player* plr2 = ObjectAccessor::FindPlayer(group->players.begin()->first))
        {
            Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(group->bgTypeId);
            BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(group->bgTypeId, group->ratedType);
            uint32 queueSlot = plr2->GetBattlegroundQueueIndex(bgQueueTypeId);

            plr2->RemoveBattlegroundQueueId(bgQueueTypeId);
            WorldPacket data;
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, plr2, queueSlot, STATUS_NONE, plr2->GetBattlegroundQueueJoinTime(group->bgTypeId), 0, RATED_TYPE_NOT_RATED);
            plr2->GetSession()->SendPacket(&data);
        }
        RemovePlayer(group->players.begin()->first, decreaseInvitedCount);
    }
}

bool BattlegroundQueue::IsPlayerInvited(uint64 pl_guid, const uint32 bgInstanceGuid, const uint32 removeTime)
{
    QueuedPlayersMap::const_iterator qItr = m_QueuedPlayers.find(pl_guid);
    return (qItr != m_QueuedPlayers.end()  && qItr->second.GroupInfo->isInvitedToBGInstanceGUID == bgInstanceGuid && qItr->second.GroupInfo->removeInviteTime == removeTime);
}

bool BattlegroundQueue::GetPlayerGroupInfoData(uint64 guid, GroupQueueInfo* ginfo)
{
    QueuedPlayersMap::const_iterator qItr = m_QueuedPlayers.find(guid);
    if (qItr == m_QueuedPlayers.end())
        return false;
    *ginfo = *(qItr->second.GroupInfo);
    return true;
}

uint32 BattlegroundQueue::GetPlayersInQueue(TeamId id)
{
    return m_SelectionPools[id].GetPlayerCount();
}

bool BattlegroundQueue::InviteGroupToBG(GroupQueueInfo* ginfo, Battleground* bg, uint32 side)
{
    if (side)
        ginfo->team = side;

    if (!ginfo->isInvitedToBGInstanceGUID)
    {
        ginfo->isInvitedToBGInstanceGUID = bg->GetInstanceID();
        BattlegroundTypeId bgTypeId = bg->GetTypeID();
        BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, bg->GetRatedType());
        BattlegroundBracketId bracket_id = bg->GetBracketId();

        ginfo->removeInviteTime = getMSTime() + INVITE_ACCEPT_WAIT_TIME;

        for (std::map<uint64, PlayerQueueInfo*>::iterator itr = ginfo->players.begin(); itr != ginfo->players.end(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(itr->first);
            if (!player)
                continue;

            PlayerInvitedToBGUpdateAverageWaitTime(ginfo, bracket_id);
            bg->IncreaseInvitedCount(ginfo->team);
            player->SetInviteForBattlegroundQueueType(bgQueueTypeId, ginfo->isInvitedToBGInstanceGUID);
            BGQueueInviteEvent* inviteEvent = new BGQueueInviteEvent(player->GetGUID(), ginfo->isInvitedToBGInstanceGUID, bgTypeId, ginfo->ratedType, ginfo->removeInviteTime);
            m_events.AddEvent(inviteEvent, m_events.CalculateTime(INVITATION_REMIND_TIME));
            BGQueueRemoveEvent* removeEvent = new BGQueueRemoveEvent(player->GetGUID(), ginfo->isInvitedToBGInstanceGUID, bgTypeId, bgQueueTypeId, ginfo->removeInviteTime);
            m_events.AddEvent(removeEvent, m_events.CalculateTime(INVITE_ACCEPT_WAIT_TIME));

            WorldPacket data;
            uint32 queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);
            TC_LOG_DEBUG("bg.battleground", "Battleground: invited player %s (%u) to BG instance %u queueindex %u bgtype %u", player->GetName().c_str(), player->GetGUIDLow(), bg->GetInstanceID(), queueSlot, bg->GetTypeID());
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, player, queueSlot, STATUS_WAIT_JOIN, INVITE_ACCEPT_WAIT_TIME, player->GetBattlegroundQueueJoinTime(bgTypeId), ginfo->ratedType);
            player->GetSession()->SendPacket(&data);
        }
        return true;
    }

    return false;
}

void BattlegroundQueue::FillPlayersToBG(Battleground* bg, BattlegroundBracketId bracket_id)
{
    int32 hordeFree = bg->GetFreeSlotsForTeam(HORDE);
    int32 aliFree   = bg->GetFreeSlotsForTeam(ALLIANCE);

    GroupsQueueType::const_iterator Ali_itr = m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE].begin();
    uint32 aliCount = m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE].size();
    uint32 aliIndex = 0;

    for (; aliIndex < aliCount && m_SelectionPools[TEAM_ALLIANCE].AddGroup((*Ali_itr), aliFree); aliIndex++)
        ++Ali_itr;

    GroupsQueueType::const_iterator Horde_itr = m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_HORDE].begin();
    uint32 hordeCount = m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_HORDE].size();
    uint32 hordeIndex = 0;
    for (; hordeIndex < hordeCount && m_SelectionPools[TEAM_HORDE].AddGroup((*Horde_itr), hordeFree); hordeIndex++)
        ++Horde_itr;

    if (sWorld->getIntConfig(CONFIG_BATTLEGROUND_INVITATION_TYPE) == 0)
        return;

    /*
    if we reached this code, then we have to solve NP - complete problem called Subset sum problem
    So one solution is to check all possible invitation subgroups, or we can use these conditions:
    1. Last time when BattlegroundQueue::Update was executed we invited all possible players - so there is only small possibility
        that we will invite now whole queue, because only 1 change has been made to queues from the last BattlegroundQueue::Update call
    2. Other thing we should consider is group order in queue
    */

    int32 diffAli   = aliFree   - int32(m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount());
    int32 diffHorde = hordeFree - int32(m_SelectionPools[TEAM_HORDE].GetPlayerCount());
    while (abs(diffAli - diffHorde) > 1 && (m_SelectionPools[TEAM_HORDE].GetPlayerCount() > 0 || m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount() > 0))
    {
        if (diffAli < diffHorde)
        {
            if (m_SelectionPools[TEAM_ALLIANCE].KickGroup(diffHorde - diffAli))
            {
                for (; aliIndex < aliCount && m_SelectionPools[TEAM_ALLIANCE].AddGroup((*Ali_itr), (aliFree >= diffHorde) ? aliFree - diffHorde : 0); aliIndex++)
                    ++Ali_itr;
            }
            if (!m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount())
            {
                if (aliFree <= diffHorde + 1)
                    break;
                m_SelectionPools[TEAM_HORDE].KickGroup(diffHorde - diffAli);
            }
        }
        else
        {
            if (m_SelectionPools[TEAM_HORDE].KickGroup(diffAli - diffHorde))
            {
                for (; hordeIndex < hordeCount && m_SelectionPools[TEAM_HORDE].AddGroup((*Horde_itr), (hordeFree >= diffAli) ? hordeFree - diffAli : 0); hordeIndex++)
                    ++Horde_itr;
            }
            if (!m_SelectionPools[TEAM_HORDE].GetPlayerCount())
            {
                if (hordeFree <= diffAli + 1)
                    break;
                m_SelectionPools[TEAM_ALLIANCE].KickGroup(diffAli - diffHorde);
            }
        }
        diffAli   = aliFree   - int32(m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount());
        diffHorde = hordeFree - int32(m_SelectionPools[TEAM_HORDE].GetPlayerCount());
    }
}

bool BattlegroundQueue::CheckPremadeMatch(BattlegroundBracketId bracket_id, uint32 MinPlayersPerTeam, uint32 MaxPlayersPerTeam)
{
    if (!m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE].empty() && !m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_HORDE].empty())
    {
        GroupsQueueType::const_iterator ali_group, horde_group;
        for (ali_group = m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE].begin(); ali_group != m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE].end(); ++ali_group)
            if (!(*ali_group)->isInvitedToBGInstanceGUID)
                break;
        for (horde_group = m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_HORDE].begin(); horde_group != m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_HORDE].end(); ++horde_group)
            if (!(*horde_group)->isInvitedToBGInstanceGUID)
                break;

        if (ali_group != m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE].end() && horde_group != m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_HORDE].end())
        {
            m_SelectionPools[TEAM_ALLIANCE].AddGroup((*ali_group), MaxPlayersPerTeam);
            m_SelectionPools[TEAM_HORDE].AddGroup((*horde_group), MaxPlayersPerTeam);
            uint32 maxPlayers = std::min(m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount(), m_SelectionPools[TEAM_HORDE].GetPlayerCount());
            GroupsQueueType::const_iterator itr;
            for (uint32 i = 0; i < BG_TEAMS_COUNT; i++)
            {
                for (itr = m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + i].begin(); itr != m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + i].end(); ++itr)
                {
                    if (!(*itr)->isInvitedToBGInstanceGUID && !m_SelectionPools[i].AddGroup((*itr), maxPlayers))
                        break;
                }
            }
            return true;
        }
    }
    // now check if we can move group from Premade queue to normal queue (timer has expired) or group size lowered!!
    // this could be 2 cycles but i'm checking only first team in queue - it can cause problem -
    // if first is invited to BG and seconds timer expired, but we can ignore it, because players have only 80 seconds to click to enter bg
    // and when they click or after 80 seconds the queue info is removed from queue
    uint32 time_before = getMSTime() - sWorld->getIntConfig(CONFIG_BATTLEGROUND_PREMADE_GROUP_WAIT_FOR_MATCH);
    for (uint32 i = 0; i < BG_TEAMS_COUNT; i++)
    {
        if (!m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE + i].empty())
        {
            GroupsQueueType::iterator itr = m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE + i].begin();
            if (!(*itr)->isInvitedToBGInstanceGUID && ((*itr)->joinTime < time_before || (*itr)->players.size() < MinPlayersPerTeam))
            {
                m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + i].push_front((*itr));
                m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE + i].erase(itr);
            }
        }
    }
    return false;
}

bool BattlegroundQueue::CheckNormalMatch(Battleground* /*bg_template*/, BattlegroundBracketId bracket_id, uint32 minPlayers, uint32 maxPlayers)
{
    GroupsQueueType::const_iterator itr_team[BG_TEAMS_COUNT];
    for (uint32 i = 0; i < BG_TEAMS_COUNT; i++)
    {
        itr_team[i] = m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + i].begin();
        for (; itr_team[i] != m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + i].end(); ++(itr_team[i]))
        {
            if (!(*(itr_team[i]))->isInvitedToBGInstanceGUID)
            {
                m_SelectionPools[i].AddGroup(*(itr_team[i]), maxPlayers);
                if (m_SelectionPools[i].GetPlayerCount() >= minPlayers)
                    break;
            }
        }
    }
    uint32 j = TEAM_ALLIANCE;
    if (m_SelectionPools[TEAM_HORDE].GetPlayerCount() < m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount())
        j = TEAM_HORDE;
    if (sWorld->getIntConfig(CONFIG_BATTLEGROUND_INVITATION_TYPE) != 0 && m_SelectionPools[TEAM_HORDE].GetPlayerCount() >= minPlayers && m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount() >= minPlayers)
    {
        ++(itr_team[j]);
        for (; itr_team[j] != m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + j].end(); ++(itr_team[j]))
        {
            if (!(*(itr_team[j]))->isInvitedToBGInstanceGUID)
                if (!m_SelectionPools[j].AddGroup(*(itr_team[j]), m_SelectionPools[(j + 1) % BG_TEAMS_COUNT].GetPlayerCount()))
                    break;
        }
        if (abs((int32)(m_SelectionPools[TEAM_HORDE].GetPlayerCount() - m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount())) > 2)
            return false;
    }
    if (sBattlegroundMgr->isTesting() && (m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount() || m_SelectionPools[TEAM_HORDE].GetPlayerCount()))
        return true;
    return m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount() >= minPlayers && m_SelectionPools[TEAM_HORDE].GetPlayerCount() >= minPlayers;
}

bool BattlegroundQueue::CheckSkirmishForSameFaction(BattlegroundBracketId bracket_id, uint32 minPlayersPerTeam)
{
    if (m_SelectionPools[TEAM_ALLIANCE].GetPlayerCount() < minPlayersPerTeam && m_SelectionPools[TEAM_HORDE].GetPlayerCount() < minPlayersPerTeam)
        return false;
    uint32 teamIndex = TEAM_ALLIANCE;
    uint32 otherTeam = TEAM_HORDE;
    uint32 otherTeamId = HORDE;
    if (m_SelectionPools[TEAM_HORDE].GetPlayerCount() == minPlayersPerTeam)
    {
        teamIndex = TEAM_HORDE;
        otherTeam = TEAM_ALLIANCE;
        otherTeamId = ALLIANCE;
    }
    m_SelectionPools[otherTeam].Init();
    GroupQueueInfo* ginfo = m_SelectionPools[teamIndex].SelectedGroups.back();
    GroupsQueueType::iterator itr_team = m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + teamIndex].begin();
    for (; itr_team != m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + teamIndex].end(); ++itr_team)
        if (ginfo == *itr_team)
            break;
    if (itr_team == m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + teamIndex].end())
        return false;
    GroupsQueueType::iterator itr_team2 = itr_team;
    ++itr_team2;
    for (; itr_team2 != m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + teamIndex].end(); ++itr_team2)
    {
        if (!(*itr_team2)->isInvitedToBGInstanceGUID && !m_SelectionPools[otherTeam].AddGroup(*itr_team2, minPlayersPerTeam))
            break;
    }
    if (m_SelectionPools[otherTeam].GetPlayerCount() != minPlayersPerTeam)
        return false;

    for (GroupsQueueType::iterator itr = m_SelectionPools[otherTeam].SelectedGroups.begin(); itr != m_SelectionPools[otherTeam].SelectedGroups.end(); ++itr)
    {
        (*itr)->team = otherTeamId;
        m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + otherTeam].push_front(*itr);
        GroupsQueueType::iterator itr2 = itr_team;
        ++itr2;
        for (; itr2 != m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + teamIndex].end(); ++itr2)
        {
            if (*itr2 == *itr)
            {
                m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE + teamIndex].erase(itr2);
                break;
            }
        }
    }
    return true;
}

void BattlegroundQueue::UpdateEvents(uint32 diff)
{
    m_events.Update(diff);
}

/*
this method is called when group is inserted, or player / group is removed from BG Queue - there is only one player's status changed, so we don't use while (true) cycles to invite whole queue
it must be called after fully adding the members of a group to ensure group joining
should be called from Battleground::RemovePlayer function in some cases
*/
void BattlegroundQueue::BattlegroundQueueUpdate(uint32 /*diff*/, BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id, RatedType ratedType, bool isRated, uint32 arenaRating)
{
    if (m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE].empty() && m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_HORDE].empty() &&
        m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE].empty() && m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_HORDE].empty())
        return;

    BGFreeSlotQueueContainer& bgQueues = sBattlegroundMgr->GetBGFreeSlotQueueStore(bgTypeId);
    for (auto itr : bgQueues)
    {
        Battleground* bg = itr; ++itr;
        if (!bg->IsRated() && bg->GetTypeID() == bgTypeId && bg->GetBracketId() == bracket_id && bg->GetStatus() > STATUS_WAIT_QUEUE && bg->GetStatus() < STATUS_WAIT_LEAVE)
        {
            m_SelectionPools[TEAM_ALLIANCE].Init();
            m_SelectionPools[TEAM_HORDE].Init();
            FillPlayersToBG(bg, bracket_id);

            for (GroupsQueueType::const_iterator citr = m_SelectionPools[TEAM_ALLIANCE].SelectedGroups.begin(); citr != m_SelectionPools[TEAM_ALLIANCE].SelectedGroups.end(); ++citr)
                InviteGroupToBG((*citr), bg, (*citr)->team);

            for (GroupsQueueType::const_iterator citr = m_SelectionPools[TEAM_HORDE].SelectedGroups.begin(); citr != m_SelectionPools[TEAM_HORDE].SelectedGroups.end(); ++citr)
                InviteGroupToBG((*citr), bg, (*citr)->team);

            if (!bg->HasFreeSlots())
                bg->RemoveFromBGFreeSlotQueue();
        }
    }

    Battleground* bg_template = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg_template)
    {
        TC_LOG_ERROR("bg.battleground", "Battleground: Update: bg template not found for %u", bgTypeId);
        return;
    }

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketById(bg_template->GetMapId(), bracket_id);
    if (!bracketEntry)
    {
        TC_LOG_ERROR("bg.battleground", "Battleground: Update: bg bracket entry not found for map %u bracket id %u", bg_template->GetMapId(), bracket_id);
        return;
    }

    uint32 MinPlayersPerTeam = bg_template->GetMinPlayersPerTeam();
    uint32 MaxPlayersPerTeam = bg_template->GetMaxPlayersPerTeam();

    if (bg_template->IsArena())
    {
        MaxPlayersPerTeam = ratedType;
        MinPlayersPerTeam = sBattlegroundMgr->isArenaTesting() ? 1 : ratedType;
    }
    else if (sBattlegroundMgr->isTesting())
        MinPlayersPerTeam = 1;

    m_SelectionPools[TEAM_ALLIANCE].Init();
    m_SelectionPools[TEAM_HORDE].Init();

    if (bg_template->IsBattleground())
    {
        if (CheckPremadeMatch(bracket_id, MinPlayersPerTeam, MaxPlayersPerTeam))
        {
            Battleground* bg2 = sBattlegroundMgr->CreateNewBattleground(bgTypeId, bracketEntry, RATED_TYPE_NOT_RATED, false);
            if (!bg2)
            {
                TC_LOG_ERROR("bg.battleground", "BattlegroundQueue::Update - Cannot create battleground: %u", bgTypeId);
                return;
            }
            for (uint32 i = 0; i < BG_TEAMS_COUNT; i++)
                for (GroupsQueueType::const_iterator citr = m_SelectionPools[TEAM_ALLIANCE + i].SelectedGroups.begin(); citr != m_SelectionPools[TEAM_ALLIANCE + i].SelectedGroups.end(); ++citr)
                    InviteGroupToBG((*citr), bg2, (*citr)->team);

            bg2->StartBattleground();
            m_SelectionPools[TEAM_ALLIANCE].Init();
            m_SelectionPools[TEAM_HORDE].Init();
        }
    }

    if (!isRated)
    {
        if (CheckNormalMatch(bg_template, bracket_id, MinPlayersPerTeam, MaxPlayersPerTeam) || (bg_template->IsArena() && CheckSkirmishForSameFaction(bracket_id, MinPlayersPerTeam)))
        {
            Battleground* bg2 = sBattlegroundMgr->CreateNewBattleground(bgTypeId, bracketEntry, ratedType, false);
            if (!bg2)
            {
                TC_LOG_ERROR("bg.battleground", "BattlegroundQueue::Update - Cannot create battleground: %u", bgTypeId);
                return;
            }

            for (uint32 i = 0; i < BG_TEAMS_COUNT; i++)
                for (GroupsQueueType::const_iterator citr = m_SelectionPools[TEAM_ALLIANCE + i].SelectedGroups.begin(); citr != m_SelectionPools[TEAM_ALLIANCE + i].SelectedGroups.end(); ++citr)
                    InviteGroupToBG((*citr), bg2, (*citr)->team);

            bg2->StartBattleground();
        }
    }
    else if (bg_template->IsArena())
    {
        if (!arenaRating)
        {
            GroupQueueInfo* front1 = NULL;
            GroupQueueInfo* front2 = NULL;
            if (!m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE].empty())
            {
                front1 = m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE].front();
                arenaRating = front1->teamMatchmakerRating;
            }
            if (!m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_HORDE].empty())
            {
                front2 = m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_HORDE].front();
                arenaRating = front2->teamMatchmakerRating;
            }
            if (front1 && front2)
            {
                if (front1->joinTime < front2->joinTime)
                    arenaRating = front1->teamMatchmakerRating;
            }
            else if (!front1 && !front2)
                return;
        }

        uint32 arenaMinRating = (arenaRating <= sBattlegroundMgr->GetMaxRatingDifference()) ? 0 : arenaRating - sBattlegroundMgr->GetMaxRatingDifference();
        uint32 arenaMaxRating = arenaRating + sBattlegroundMgr->GetMaxRatingDifference();
        uint32 discardTime = getMSTime() - sBattlegroundMgr->GetRatingDiscardTimer();
        GroupsQueueType::iterator itr_teams[BG_TEAMS_COUNT];
        uint8 found = 0;
        uint8 team = 0;

        for (uint8 i = BG_QUEUE_PREMADE_ALLIANCE; i < BG_QUEUE_NORMAL_ALLIANCE; i++)
        {
            GroupsQueueType::iterator itr2 = m_QueuedGroups[bracket_id][i].begin();
            for (; itr2 != m_QueuedGroups[bracket_id][i].end(); ++itr2)
            {
                if (!(*itr2)->isInvitedToBGInstanceGUID &&
                    (((*itr2)->teamMatchmakerRating >= arenaMinRating && (*itr2)->teamMatchmakerRating <= arenaMaxRating) || (*itr2)->joinTime < discardTime))
                {
                    itr_teams[found++] = itr2;
                    team = i;
                    break;
                }
            }
        }

        if (!found)
            return;

        if (found == 1)
        {
            for (GroupsQueueType::iterator itr3 = itr_teams[0]; itr3 != m_QueuedGroups[bracket_id][team].end(); ++itr3)
            {
                if (!(*itr3)->isInvitedToBGInstanceGUID && (((*itr3)->teamMatchmakerRating >= arenaMinRating && (*itr3)->teamMatchmakerRating <= arenaMaxRating) ||
                    (*itr3)->joinTime < discardTime) && (*itr_teams[0])->leaderGUID != (*itr3)->leaderGUID)
                {
                    itr_teams[found++] = itr3;
                    break;
                }
            }
        }

        if (found == 2)
        {
            GroupQueueInfo* aTeam = *itr_teams[TEAM_ALLIANCE];
            GroupQueueInfo* hTeam = *itr_teams[TEAM_HORDE];
            Battleground* arena = sBattlegroundMgr->CreateNewBattleground(bgTypeId, bracketEntry, ratedType, true);
            if (!arena)
            {
                TC_LOG_ERROR("bg.battleground", "BattlegroundQueue::Update couldn't create arena instance for rated arena match!");
                return;
            }

            aTeam->opponentsTeamRating = hTeam->teamRating;
            hTeam->opponentsTeamRating = aTeam->teamRating;
            aTeam->opponentsTeamMatchmakerRating = hTeam->teamMatchmakerRating;
            hTeam->opponentsTeamMatchmakerRating = aTeam->teamMatchmakerRating;
            TC_LOG_DEBUG("bg.battleground", "setting oposite teamrating to %u", aTeam->opponentsTeamRating);
            TC_LOG_DEBUG("bg.battleground", "setting oposite teamrating to %u", hTeam->opponentsTeamRating);

            if (aTeam->team != ALLIANCE)
            {
                m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE].push_front(aTeam);
                m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_HORDE].erase(itr_teams[TEAM_ALLIANCE]);
            }

            if (hTeam->team != HORDE)
            {
                m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_HORDE].push_front(hTeam);
                m_QueuedGroups[bracket_id][BG_QUEUE_PREMADE_ALLIANCE].erase(itr_teams[TEAM_HORDE]);
            }

            arena->SetTeamMatchmakerRating(ALLIANCE, aTeam->teamMatchmakerRating);
            arena->SetTeamMatchmakerRating(   HORDE, hTeam->teamMatchmakerRating);
            InviteGroupToBG(aTeam, arena, ALLIANCE);
            InviteGroupToBG(hTeam, arena, HORDE);

            TC_LOG_DEBUG("bg.battleground", "Starting rated arena match!");
            arena->StartBattleground();
        }
    }
}

/*********************************************************/
/***            BATTLEGROUND QUEUE EVENTS              ***/
/*********************************************************/

bool BGQueueInviteEvent::Execute(uint64 /*e_time*/, uint32 /*p_time*/)
{
    Player* player = ObjectAccessor::FindPlayer(m_PlayerGuid);
    if (!player)
        return true;

    Battleground* bg = sBattlegroundMgr->GetBattleground(m_BgInstanceGUID, m_BgTypeId);
    if (!bg)
        return true;

    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bg->GetTypeID(), bg->GetRatedType());
    uint32 queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);
    if (queueSlot < PLAYER_MAX_BATTLEGROUND_QUEUES)
    {
        BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        if (bgQueue.IsPlayerInvited(m_PlayerGuid, m_BgInstanceGUID, m_RemoveTime))
        {
            WorldPacket data;
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, player, queueSlot, STATUS_WAIT_JOIN, INVITE_ACCEPT_WAIT_TIME - INVITATION_REMIND_TIME, player->GetBattlegroundQueueJoinTime(m_BgTypeId), m_RatedType);
            player->GetSession()->SendPacket(&data);
        }
    }
    return true;
}

void BGQueueInviteEvent::Abort(uint64 /*e_time*/)
{
    //do nothing
}

/*
    this event has many possibilities when it is executed:
    1. player is in battleground (he clicked enter on invitation window)
    2. player left battleground queue and he isn't there any more
    3. player left battleground queue and he joined it again and IsInvitedToBGInstanceGUID = 0
    4. player left queue and he joined again and he has been invited to same battleground again -> we should not remove him from queue yet
    5. player is invited to bg and he didn't choose what to do and timer expired - only in this condition we should call queue::RemovePlayer
    we must remove player in the 5. case even if battleground object doesn't exist!
*/
bool BGQueueRemoveEvent::Execute(uint64 /*e_time*/, uint32 /*p_time*/)
{
    Player* player = ObjectAccessor::FindPlayer(m_PlayerGuid);
    if (!player)
        return true;

    Battleground* bg = sBattlegroundMgr->GetBattleground(m_BgInstanceGUID, m_BgTypeId);
    uint32 queueSlot = player->GetBattlegroundQueueIndex(m_BgQueueTypeId);
    if (queueSlot < PLAYER_MAX_BATTLEGROUND_QUEUES)
    {
        BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(m_BgQueueTypeId);
        if (bgQueue.IsPlayerInvited(m_PlayerGuid, m_BgInstanceGUID, m_RemoveTime))
        {
            TC_LOG_DEBUG("bg.battleground", "Battleground: removing player %u from bg queue for instance %u because of not pressing enter battle in time.", player->GetGUIDLow(), m_BgInstanceGUID);

            player->RemoveBattlegroundQueueId(m_BgQueueTypeId);
            bgQueue.RemovePlayer(m_PlayerGuid, true);
            if (bg && bg->IsBattleground() && bg->GetStatus() != STATUS_WAIT_LEAVE)
                sBattlegroundMgr->ScheduleQueueUpdate(0, RATED_TYPE_NOT_RATED, m_BgQueueTypeId, m_BgTypeId, bg->GetBracketId());

            WorldPacket data;
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, player, queueSlot, STATUS_NONE, player->GetBattlegroundQueueJoinTime(m_BgTypeId), 0, RATED_TYPE_NOT_RATED);
            player->GetSession()->SendPacket(&data);
        }
    }
    return true;
}

void BGQueueRemoveEvent::Abort(uint64 /*e_time*/)
{
    //do nothing
}
