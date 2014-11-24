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
#include "ObjectMgr.h"
#include "World.h"
#include "WorldPacket.h"
#include "RatedMgr.h"
#include "RatedInfo.h"
#include "BattlegroundMgr.h"
#include "BattlegroundAV.h"
#include "BattlegroundAB.h"
#include "BattlegroundEY.h"
#include "BattlegroundWS.h"
#include "BattlegroundNA.h"
#include "BattlegroundBE.h"
#include "BattlegroundRL.h"
#include "BattlegroundSA.h"
#include "BattlegroundDS.h"
#include "BattlegroundRV.h"
#include "BattlegroundIC.h"
#include "BattlegroundTP.h"
#include "BattlegroundBFG.h"
#include "BattlegroundSM.h"
#include "BattlegroundDG.h"
#include "BattlegroundTOK.h"
#include "Chat.h"
#include "Map.h"
#include "MapInstanced.h"
#include "MapManager.h"
#include "Player.h"
#include "GameEventMgr.h"
#include "SharedDefines.h"
#include "Formulas.h"
#include "DisableMgr.h"
#include "Opcodes.h"

/*********************************************************/
/***            BATTLEGROUND MANAGER                   ***/
/*********************************************************/

BattlegroundMgr::BattlegroundMgr() :
m_NextRatedArenaUpdate(sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER)), m_ArenaTesting(false), m_Testing(false)
{ }

BattlegroundMgr::~BattlegroundMgr()
{
    DeleteAllBattlegrounds();
}

void BattlegroundMgr::DeleteAllBattlegrounds()
{
    for (BattlegroundDataContainer::iterator itr1 = bgDataStore.begin(); itr1 != bgDataStore.end(); ++itr1)
    {
        BattlegroundData& data = itr1->second;

        while (!data.m_Battlegrounds.empty())
            delete data.m_Battlegrounds.begin()->second;
        data.m_Battlegrounds.clear();

        while (!data.BGFreeSlotQueue.empty())
            delete data.BGFreeSlotQueue.front();
    }

    bgDataStore.clear();
}

void BattlegroundMgr::Update(uint32 diff)
{
    for (BattlegroundDataContainer::iterator itr1 = bgDataStore.begin(); itr1 != bgDataStore.end(); ++itr1)
    {
        BattlegroundContainer& bgs = itr1->second.m_Battlegrounds;
        BattlegroundContainer::iterator itrDelete = bgs.begin();
        for (BattlegroundContainer::iterator itr = ++itrDelete; itr != bgs.end();)
        {
            itrDelete = itr++;
            Battleground* bg = itrDelete->second;

            bg->Update(diff);
            if (bg->ToBeDeleted())
            {
                itrDelete->second = NULL;
                bgs.erase(itrDelete);
                BattlegroundClientIdsContainer& clients = itr1->second.m_ClientBattlegroundIds[bg->GetBracketId()];
                if (!clients.empty())
                    clients.erase(bg->GetClientInstanceID());

                delete bg;
            }
        }
    }

    for (int qtype = BATTLEGROUND_QUEUE_NONE; qtype < MAX_BATTLEGROUND_QUEUE_TYPES; ++qtype)
        m_BattlegroundQueues[qtype].UpdateEvents(diff);

    if (!m_QueueUpdateScheduler.empty())
    {
        std::vector<uint64> scheduled;
        std::swap(scheduled, m_QueueUpdateScheduler);

        for (uint8 i = 0; i < scheduled.size(); i++)
        {
            uint32 arenaMMRating = scheduled[i] >> 48;
            RatedType ratedType = RatedType(scheduled[i] >> 40 & 0xFF);
            BattlegroundQueueTypeId bgQueueTypeId = BattlegroundQueueTypeId(scheduled[i] >> 24 & 0xFFFF);
            BattlegroundTypeId bgTypeId = BattlegroundTypeId((scheduled[i] >> 8) & 0xFFFF);
            BattlegroundBracketId bracket_id = BattlegroundBracketId(scheduled[i] & 0xFF);
            m_BattlegroundQueues[bgQueueTypeId].BattlegroundQueueUpdate(diff, bgTypeId, bracket_id, ratedType, arenaMMRating > 0, arenaMMRating);
        }
    }

    if (sWorld->getIntConfig(CONFIG_ARENA_MAX_RATING_DIFFERENCE) && sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER))
    {
        if (m_NextRatedArenaUpdate < diff)
        {
            TC_LOG_TRACE("bg.arena", "BattlegroundMgr: UPDATING ARENA QUEUES");
            for (int qtype = BATTLEGROUND_QUEUE_2v2; qtype <= BATTLEGROUND_QUEUE_10v10; ++qtype)
                for (int bracket = BG_BRACKET_ID_FIRST; bracket < MAX_BATTLEGROUND_BRACKETS; ++bracket)
                    m_BattlegroundQueues[qtype].BattlegroundQueueUpdate(diff, BATTLEGROUND_AA, BattlegroundBracketId(bracket), BattlegroundMgr::GetRatedTypeByQueue(BattlegroundQueueTypeId(qtype)), true, 0);

            m_NextRatedArenaUpdate = sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER);
        }
        else
            m_NextRatedArenaUpdate -= diff;
    }
}
// CliRideTicket
void BattlegroundMgr::BuildRideTicket(WorldPacket* data, ObjectGuid PlayerGuid, uint32 ClientInstanceID, uint8 QueueSlot, uint32 JoinTime)
{
    *data << PlayerGuid;
    *data << uint32(ClientInstanceID);
    *data << uint32(QueueSlot);
    *data << uint32(JoinTime);
}

void BattlegroundMgr::BuildBattlegroundStatusPacket(WorldPacket* data, Battleground* bg, Player* player, uint8 QueueSlot, uint8 StatusID, uint32 Time1, uint32 JoinTime, RatedType ratedType)
{
    ObjectGuid playerGuid = player->GetGUID();
    ObjectGuid bgGuid;
    uint32 clientInstanceId = bg->GetClientInstanceID();

    if (bg)
        bgGuid = bg->GetGUID();
    else
        StatusID = STATUS_NONE;

    switch (StatusID)
    {
        case STATUS_NONE:
        {
            data->Initialize(SMSG_BATTLEFIELD_STATUS_NONE, 28);
            sBattlegroundMgr->BuildRideTicket(data, playerGuid, clientInstanceId, QueueSlot, JoinTime);
            break;
        }

        case STATUS_WAIT_QUEUE:
        {
            data->Initialize(SMSG_BATTLEFIELD_STATUS_QUEUED, 67);

            sBattlegroundMgr->BuildRideTicket(data, playerGuid, clientInstanceId, QueueSlot, JoinTime);

            *data << bgGuid;
            *data << uint8(bg->GetMinLevel());
            *data << uint8(bg->GetMaxLevel());
            *data << uint8(0);                          //packet.ReadByte("unk byte");
            *data << uint32(bg->GetClientInstanceID());
            data->WriteBit(0);                          //packet.ReadBit("unk bit");
            data->WriteBit(0);                          //packet.ReadBit("unk bit");

            *data << uint32(GetMSTimeDiffToNow(JoinTime)); // AverageWaitTime
            *data << uint32(JoinTime);                     // Estimated Wait Time
            data->WriteBit(0);                          //var b2 = !packet.ReadBit("unk bit");
            data->WriteBit(1);                          // Eligible In Queue 
            data->WriteBit(0);                          //var b4 = !packet.ReadBit("unk bit");

            //*data << uint8(0);                          // TeamSize
            //*data << uint32(Time1);                     // Join Time
            //data->WriteBit(bg->IsRated());              // Is Rated
            //data->WriteBit(0);                          // AsGroup
            //data->WriteBit(0);                          // IsSuspended
            break;
        }

        case STATUS_WAIT_JOIN:
        {
            data->Initialize(SMSG_BATTLEFIELD_STATUS_NEEDCONFIRMATION, 67);

            sBattlegroundMgr->BuildRideTicket(data, playerGuid, clientInstanceId, QueueSlot, JoinTime);

            *data << bgGuid;
            *data << uint8(bg->GetMinLevel());
            *data << uint8(bg->GetMaxLevel());
            *data << uint8(0);                          //packet.ReadByte("unk byte");
            *data << uint32(bg->GetClientInstanceID());
            data->WriteBit(0);                          //packet.ReadBit("unk bit");
            data->WriteBit(0);                          //packet.ReadBit("unk bit");

            *data << uint32(GetMSTimeDiffToNow(JoinTime)); // AverageWaitTime
            *data << uint32(JoinTime);                     // Estimated Wait Time
            data->WriteBit(0);                          //var b2 = !packet.ReadBit("unk bit");
            data->WriteBit(1);                          // Eligible In Queue 
            data->WriteBit(0);                          //var b4 = !packet.ReadBit("unk bit");

            *data << uint32(bg->GetMapId());            // Map Id
            *data << uint32(Time1);                     // Time until closed
            *data << uint8(0);                          // Role

            //data->WriteBit(bg->IsRated());              // Is Rated
            break;
        }

        case STATUS_IN_PROGRESS:
        {
            data->Initialize(SMSG_BATTLEFIELD_STATUS_ACTIVE);

            sBattlegroundMgr->BuildRideTicket(data, playerGuid, clientInstanceId, QueueSlot, JoinTime);

            *data << bgGuid;
            *data << uint8(bg->GetMinLevel());
            *data << uint8(bg->GetMaxLevel());
            *data << uint8(0);                          // packet.ReadByte("unk byte");
            *data << uint32(bg->GetClientInstanceID());
            data->WriteBit(0);                          //packet.ReadBit("unk bit");
            data->WriteBit(0);                          //packet.ReadBit("unk bit");

            *data << uint32(GetMSTimeDiffToNow(JoinTime)); // AverageWaitTime
            *data << uint32(JoinTime);                     // Estimated Wait Time
            data->WriteBit(0);                          // var b2 = !packet.ReadBit("unk bit");
            data->WriteBit(1);                          // Eligible In Queue 
            data->WriteBit(0);                          //var b4 = !packet.ReadBit("unk bit");

            *data << uint32(bg->GetMapId());            // Map Id
            *data << uint32(Time1);                     // Join Time
            *data << uint32(bg->GetRemainingTime());    // Remaining Time
            data->WriteBit(player->GetBGTeam() == HORDE ? 0 : 1);
            data->WriteBit(0);                          // Left Early

            //data->WriteBit(bg->IsRated());              // Is Rated
            break;
        }

        case STATUS_WAIT_LEAVE:
            break;
    }
}

void BattlegroundMgr::BuildPvpLogDataPacket(WorldPacket* data, Battleground* bg)
{
    ByteBuffer buff;
    bool hasWinner;
    uint8 teamCount = 2;
    
    if (bg->GetStatus() == STATUS_WAIT_LEAVE)
        hasWinner = true;
    else 
        hasWinner = false;

    data->Initialize(SMSG_PVP_LOG_DATA, (1 + 1 + 4 + 40 * bg->GetPlayerScoresSize()));

    data->WriteBit(bg->IsRated());
    data->WriteBit(hasWinner);
    *data << uint32(bg->GetPlayersSize());
    
    *data << uint8(bg->GetPlayersCountByTeam(HORDE));
    *data << uint8(bg->GetPlayersCountByTeam(ALLIANCE));

    if (bg->IsRated())
    {
        //Prematch
        //Postmatch
        //PrematchMMR
    }

    if (hasWinner)
        *data << uint8(bg->GetWinner());

    *data << uint32(bg->GetPlayerScoresSize());

    for (Battleground::BattlegroundScoreMap::const_iterator itr = bg->GetPlayerScoresBegin(); itr != bg->GetPlayerScoresEnd(); ++itr)
    {
        if (!bg->IsPlayerInBattleground(itr->first))
        {
            TC_LOG_ERROR("network", "Player " UI64FMTD " has scoreboard entry for battleground %u but is not in battleground!", itr->first, bg->GetTypeID(true));
            continue;
        }

        Player* player = ObjectAccessor::FindPlayer(itr->first);
        ObjectGuid playerGUID = itr->first;
        BattlegroundScore* score = itr->second;
        bool HasClientPVPLogDataHonor = bg->IsArena();
        uint32 statCount = 0;

        *data << ObjectGuid(player->GetGUID128());
        *data << uint32(score->KillingBlows);
        *data << uint32(score->DamageDone);
        *data << uint32(score->HealingDone);
        size_t statCountPos = data->wpos();
        *data << uint32(0); // Stats count PH
        *data << int32(player->GetActiveSpecialization());
        *data << uint32(0); // faction?

        switch (bg->GetTypeID(true))                                                        // Custom values
        {
            case BATTLEGROUND_RB:
                switch (bg->GetMapId())
                {
                    case 489:
                        statCount = 2;
                        buff << uint32(((BattlegroundWGScore*)score)->FlagCaptures);        // flag captures
                        buff << uint32(((BattlegroundWGScore*)score)->FlagReturns);         // flag returns
                        break;
                    case 566:
                        statCount = 1;
                        buff << uint32(((BattlegroundEYScore*)score)->FlagCaptures);        // flag captures
                        break;
                    case 529:
                        statCount = 2;
                        buff << uint32(((BattlegroundABScore*)score)->BasesAssaulted);      // bases assaulted
                        buff << uint32(((BattlegroundABScore*)score)->BasesDefended);       // bases defended
                        break;
                    case 30:
                        statCount = 5;
                        buff << uint32(((BattlegroundAVScore*)score)->GraveyardsAssaulted); // GraveyardsAssaulted
                        buff << uint32(((BattlegroundAVScore*)score)->GraveyardsDefended);  // GraveyardsDefended
                        buff << uint32(((BattlegroundAVScore*)score)->TowersAssaulted);     // TowersAssaulted
                        buff << uint32(((BattlegroundAVScore*)score)->TowersDefended);      // TowersDefended
                        buff << uint32(((BattlegroundAVScore*)score)->MinesCaptured);       // MinesCaptured
                        break;
                    case 607:
                        statCount = 2;
                        buff << uint32(((BattlegroundSAScore*)score)->demolishers_destroyed);
                        buff << uint32(((BattlegroundSAScore*)score)->gates_destroyed);
                        break;
                    case 628:                                   // IC
                        statCount = 2;
                        buff << uint32(((BattlegroundICScore*)score)->BasesAssaulted);       // bases assaulted
                        buff << uint32(((BattlegroundICScore*)score)->BasesDefended);        // bases defended
                        break;
                    case 726:
                        statCount = 2;
                        buff << uint32(((BattlegroundTPScore*)score)->FlagCaptures);         // flag captures
                        buff << uint32(((BattlegroundTPScore*)score)->FlagReturns);          // flag returns
                        break;
                    case 761:
                        statCount = 2;
                        buff << uint32(((BattlegroundBFGScore*)score)->BasesAssaulted);      // bases assaulted
                        buff << uint32(((BattlegroundBFGScore*)score)->BasesDefended);       // bases defended
                        break;
                    default:
                        statCount = 0;
                        break;
                }
                break;
            case BATTLEGROUND_AV:
                statCount = 5;
                buff << uint32(((BattlegroundAVScore*)score)->GraveyardsAssaulted); // GraveyardsAssaulted
                buff << uint32(((BattlegroundAVScore*)score)->GraveyardsDefended);  // GraveyardsDefended
                buff << uint32(((BattlegroundAVScore*)score)->TowersAssaulted);     // TowersAssaulted
                buff << uint32(((BattlegroundAVScore*)score)->TowersDefended);      // TowersDefended
                buff << uint32(((BattlegroundAVScore*)score)->MinesCaptured);       // MinesCaptured
                break;
            case BATTLEGROUND_WS:
                statCount = 2;
                buff << uint32(((BattlegroundWGScore*)score)->FlagCaptures);        // flag captures
                buff << uint32(((BattlegroundWGScore*)score)->FlagReturns);         // flag returns
                break;
            case BATTLEGROUND_AB:
                statCount = 2;
                buff << uint32(((BattlegroundABScore*)score)->BasesAssaulted);      // bases assaulted
                buff << uint32(((BattlegroundABScore*)score)->BasesDefended);       // bases defended
                break;
            case BATTLEGROUND_EY:
                statCount = 1;
                buff << uint32(((BattlegroundEYScore*)score)->FlagCaptures);        // flag captures
                break;
            case BATTLEGROUND_SA:
                statCount = 2;
                buff << uint32(((BattlegroundSAScore*)score)->demolishers_destroyed);
                buff << uint32(((BattlegroundSAScore*)score)->gates_destroyed);
                break;
            case BATTLEGROUND_IC:
                statCount = 2;
                buff << uint32(((BattlegroundICScore*)score)->BasesAssaulted);       // bases assaulted
                buff << uint32(((BattlegroundICScore*)score)->BasesDefended);        // bases defended
                break;
            case BATTLEGROUND_TP:
                statCount = 2;
                buff << uint32(((BattlegroundTPScore*)score)->FlagCaptures);         // flag captures
                buff << uint32(((BattlegroundTPScore*)score)->FlagReturns);          // flag returns
                break;
            case BATTLEGROUND_BFG:
                statCount = 2;
                buff << uint32(((BattlegroundBFGScore*)score)->BasesAssaulted);      // bases assaulted
                buff << uint32(((BattlegroundBFGScore*)score)->BasesDefended);       // bases defended
                break;
            case BATTLEGROUND_NA:
            case BATTLEGROUND_BE:
            case BATTLEGROUND_AA:
            case BATTLEGROUND_RL:
            case BATTLEGROUND_DS:
            case BATTLEGROUND_RV:
                statCount = 0;
                break;
            default:
                statCount = 0;
                break;
        }

        data->append(buff);
        data->put(statCountPos, statCount);

        data->WriteBit(0); // var bit20 = !packet.ReadBit("unk bit 20", i);
        data->WriteBit(player->IsInWorld());
        data->WriteBit(HasClientPVPLogDataHonor);
        data->WriteBit(0); // var bit52 = packet.ReadBit("unk bit 52", i);
        data->WriteBit(0); // var bit60 = packet.ReadBit("unk bit 60", i);
        data->WriteBit(0); // var bit68 = packet.ReadBit("unk bit 68", i);
        data->WriteBit(0); // var bit76 = packet.ReadBit("unk bit 76", i);

        if (!HasClientPVPLogDataHonor)
        {
            *data << uint32(score->HonorableKills);
            *data << uint32(score->Deaths);
            *data << uint32(score->BonusHonor / 100); // ContributionPoints ?
        }
        
        //if (!bit60)
        //    packet.ReadUInt32("unk uint32 56", i);
        //if (!bit68)
        //    packet.ReadUInt32("unk uint32 64", i);
        //if (!bit76)
        //    packet.ReadUInt32("unk uint32 72", i);


        //data->WriteBit(!bg->IsArena());

        //data->WriteBit(bg->IsRated());
        //if (bg->IsRated())
        //    *data << int32(score->PersonalRatingChange);

    }
}

void BattlegroundMgr::BuildStatusFailedPacket(WorldPacket* data, Battleground* bg, Player* player, uint8 QueueSlot, GroupJoinBattlegroundResult result)
{
    ObjectGuid playerGuid = player->GetGUID128();
    ObjectGuid battlegroundGuid = bg->GetGUID();
    uint64 ClientID = 0;                            // Not a guid
    uint32 clientInstanceId = bg->GetClientInstanceID();  
    uint32 JoinTime = player->GetBattlegroundQueueJoinTime(bg->GetTypeID());

    data->Initialize(SMSG_BATTLEFIELD_STATUS_FAILED, 28 + 16 + 4 + 16);

    sBattlegroundMgr->BuildRideTicket(data, playerGuid, clientInstanceId, QueueSlot, JoinTime);

    *data << battlegroundGuid;
    *data << uint32(result);
    *data << ClientID;
}

void BattlegroundMgr::BuildUpdateWorldStatePacket(WorldPacket* data, uint32 field, uint32 value, bool Hidden)
{
    data->Initialize(SMSG_UPDATE_WORLD_STATE, 4 + 4);

    *data << uint32(field);
    *data << uint32(value);
    data->WriteBit(Hidden);
}

void BattlegroundMgr::BuildPlaySoundPacket(WorldPacket* data, uint32 soundId)
{
    data->Initialize(SMSG_PLAY_SOUND, 4 + 2);

    *data << uint32(soundId);
    *data << ObjectGuid(0);
}

void BattlegroundMgr::BuildPlayerLeftBattlegroundPacket(WorldPacket* data, uint64 guid)
{
    ObjectGuid Guid = guid;

    data->Initialize(SMSG_BATTLEGROUND_PLAYER_LEFT, 16);

    *data << Guid;
}

void BattlegroundMgr::BuildPlayerJoinedBattlegroundPacket(WorldPacket* data, uint64 guid)
{
    ObjectGuid Guid = guid;

    data->Initialize(SMSG_BATTLEGROUND_PLAYER_JOINED, 16);

    *data << Guid;
}

Battleground* BattlegroundMgr::GetBattlegroundThroughClientInstance(uint32 instanceId, BattlegroundTypeId bgTypeId)
{
    Battleground* bg = GetBattlegroundTemplate(bgTypeId);
    if (!bg)
        return NULL;

    if (bg->IsArena())
        return GetBattleground(instanceId, bgTypeId);

    BattlegroundDataContainer::const_iterator it = bgDataStore.find(bgTypeId);
    if (it == bgDataStore.end())
        return NULL;

    for (auto itr : it->second.m_Battlegrounds)
    {
        if (itr.second->GetClientInstanceID() == instanceId)
            return itr.second;
    }

    return NULL;
}

Battleground* BattlegroundMgr::GetBattleground(uint32 instanceId, BattlegroundTypeId bgTypeId)
{
    if (!instanceId)
        return NULL;

    BattlegroundDataContainer::const_iterator begin, end;

    if (bgTypeId == BATTLEGROUND_TYPE_NONE)
    {
        begin = bgDataStore.begin();
        end = bgDataStore.end();
    }
    else
    {
        end = bgDataStore.find(bgTypeId);
        if (end == bgDataStore.end())
            return NULL;
        begin = end++;
    }

    for (BattlegroundDataContainer::const_iterator it = begin; it != end; ++it)
    {
        BattlegroundContainer const& bgs = it->second.m_Battlegrounds;
        BattlegroundContainer::const_iterator itr = bgs.find(instanceId);
        if (itr != bgs.end())
            return itr->second;
    }

    return NULL;
}

Battleground* BattlegroundMgr::GetBattlegroundTemplate(BattlegroundTypeId bgTypeId)
{
    BattlegroundDataContainer::const_iterator itr = bgDataStore.find(bgTypeId);
    if (itr == bgDataStore.end())
        return NULL;

    BattlegroundContainer const& bgs = itr->second.m_Battlegrounds;
    return bgs.empty() ? NULL : bgs.begin()->second;
}

uint32 BattlegroundMgr::CreateClientVisibleInstanceId(BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id)
{
    if (IsArenaType(bgTypeId))
        return 0;

    BattlegroundClientIdsContainer& clientIds = bgDataStore[bgTypeId].m_ClientBattlegroundIds[bracket_id];
    uint32 lastId = 0;
    for (auto itr : clientIds)
    {
        if ((++lastId) != itr) 
            break;
        lastId = itr;
    }

    clientIds.insert(++lastId);
    return lastId;
}

Battleground* BattlegroundMgr::CreateNewBattleground(BattlegroundTypeId originalBgTypeId, PvPDifficultyEntry const* bracketEntry, RatedType ratedType, bool isRated)
{
    BattlegroundTypeId bgTypeId = originalBgTypeId;
    bool isRandom = false;

    switch (originalBgTypeId)
    {
        case BATTLEGROUND_RB:
            isRandom = true;
        case BATTLEGROUND_AA:
        case BATTLEGROUND_RATED_10_VS_10:
            bgTypeId = GetRandomBG(originalBgTypeId);
            break;
        default:
            break;
    }

    Battleground* bg_template = GetBattlegroundTemplate(bgTypeId);
    if (!bg_template)
    {
        TC_LOG_ERROR("bg.battleground", "Battleground: CreateNewBattleground - bg template not found for %u", bgTypeId);
        return NULL;
    }

    Battleground* bg = NULL;
    switch (bgTypeId)
    {
        case BATTLEGROUND_AV:
            bg = new BattlegroundAV(*(BattlegroundAV*)bg_template);
            break;
        case BATTLEGROUND_WS:
            bg = new BattlegroundWS(*(BattlegroundWS*)bg_template);
            break;
        case BATTLEGROUND_AB:
            bg = new BattlegroundAB(*(BattlegroundAB*)bg_template);
            break;
        case BATTLEGROUND_NA:
            bg = new BattlegroundNA(*(BattlegroundNA*)bg_template);
            break;
        case BATTLEGROUND_BE:
            bg = new BattlegroundBE(*(BattlegroundBE*)bg_template);
            break;
        case BATTLEGROUND_EY:
            bg = new BattlegroundEY(*(BattlegroundEY*)bg_template);
            break;
        case BATTLEGROUND_RL:
            bg = new BattlegroundRL(*(BattlegroundRL*)bg_template);
            break;
        case BATTLEGROUND_SA:
            bg = new BattlegroundSA(*(BattlegroundSA*)bg_template);
            break;
        case BATTLEGROUND_DS:
            bg = new BattlegroundDS(*(BattlegroundDS*)bg_template);
            break;
        case BATTLEGROUND_RV:
            bg = new BattlegroundRV(*(BattlegroundRV*)bg_template);
            break;
        case BATTLEGROUND_IC:
            bg = new BattlegroundIC(*(BattlegroundIC*)bg_template);
            break;
        case BATTLEGROUND_TP:
            bg = new BattlegroundTP(*(BattlegroundTP*)bg_template);
            break;
        case BATTLEGROUND_BFG:
            bg = new BattlegroundBFG(*(BattlegroundBFG*)bg_template);
            break;
        case BATTLEGROUND_TOK:
            bg = new BattlegroundTOK(*(BattlegroundTOK*)bg_template);
            break;
        case BATTLEGROUND_DG:
            bg = new BattlegroundDG(*(BattlegroundDG*)bg_template);
            break;
        case BATTLEGROUND_SM:
            bg = new BattlegroundSM(*(BattlegroundSM*)bg_template);
            break;
        case BATTLEGROUND_RB:
        case BATTLEGROUND_AA:
        case BATTLEGROUND_RATED_10_VS_10:
            bg = new Battleground(*bg_template);
            break;
        default:
            return NULL;
    }

    bg->SetBracket(bracketEntry);
    bg->SetInstanceID(sMapMgr->GenerateInstanceId());
    bg->SetClientInstanceID(CreateClientVisibleInstanceId(isRandom ? BATTLEGROUND_RB : bgTypeId, bracketEntry->GetBracketId()));
    bg->Reset();
    bg->SetStatus(STATUS_WAIT_JOIN);
    bg->SetRatedType(ratedType);
    bg->SetTypeID(originalBgTypeId);
    bg->SetRandomTypeID(bgTypeId);
    bg->SetRated(isRated);
    bg->SetRandom(isRandom);
    bg->SetGuid(MAKE_NEW_GUID(bgTypeId, 0, HIGHGUID_BATTLEGROUND));

    if (bg->IsRated())
    {
        uint32 maxPlayersPerTeam = 0;
        switch (ratedType)
        {
            case RATED_TYPE_2v2:
                maxPlayersPerTeam = 2;
                break;
            case RATED_TYPE_3v3:
                maxPlayersPerTeam = 3;
                break;
            case RATED_TYPE_5v5:
                maxPlayersPerTeam = 5;
                break;
            case RATED_TYPE_10v10:
                maxPlayersPerTeam = 10;
                break;
        }

        bg->SetMaxPlayersPerTeam(maxPlayersPerTeam);
        bg->SetMaxPlayers(maxPlayersPerTeam * 2);
    }

    return bg;
}

bool BattlegroundMgr::CreateBattleground(CreateBattlegroundData& data)
{
    Battleground* bg = NULL;
    switch (data.bgTypeId)
    {
        case BATTLEGROUND_AV:
            bg = new BattlegroundAV;
            break;
        case BATTLEGROUND_WS:
            bg = new BattlegroundWS;
            break;
        case BATTLEGROUND_AB:
            bg = new BattlegroundAB;
            break;
        case BATTLEGROUND_NA:
            bg = new BattlegroundNA;
            break;
        case BATTLEGROUND_BE:
            bg = new BattlegroundBE;
            break;
        case BATTLEGROUND_EY:
            bg = new BattlegroundEY;
            break;
        case BATTLEGROUND_RL:
            bg = new BattlegroundRL;
            break;
        case BATTLEGROUND_SA:
            bg = new BattlegroundSA;
            break;
        case BATTLEGROUND_DS:
            bg = new BattlegroundDS;
            break;
        case BATTLEGROUND_RV:
            bg = new BattlegroundRV;
            break;
        case BATTLEGROUND_IC:
            bg = new BattlegroundIC;
            break;
        case BATTLEGROUND_AA:
            bg = new Battleground;
            break;
        case BATTLEGROUND_RB:
            bg = new Battleground;
            bg->SetRandom(true);
            break;
        case BATTLEGROUND_TP:
            bg = new BattlegroundTP;
            break;
        case BATTLEGROUND_BFG:
            bg = new BattlegroundBFG;
            break;
        case BATTLEGROUND_TOK:
            bg = new BattlegroundTOK;
            break;
        case BATTLEGROUND_DG:
            bg = new BattlegroundDG;
            break;
        case BATTLEGROUND_SM:
            bg = new BattlegroundSM;
            break;
        default:
            return false;
    }

    bg->SetMapId(data.MapID);
    bg->SetTypeID(data.bgTypeId);
    bg->SetInstanceID(0);
    bg->SetArenaorBGType(data.IsArena);
    bg->SetMinPlayersPerTeam(data.MinPlayersPerTeam);
    bg->SetMaxPlayersPerTeam(data.MaxPlayersPerTeam);
    bg->SetMinPlayers(data.MinPlayersPerTeam * 2);
    bg->SetMaxPlayers(data.MaxPlayersPerTeam * 2);
    bg->SetName(data.BattlegroundName);
    bg->SetTeamStartLoc(ALLIANCE, data.Team1StartLocX, data.Team1StartLocY, data.Team1StartLocZ, data.Team1StartLocO);
    bg->SetTeamStartLoc(HORDE, data.Team2StartLocX, data.Team2StartLocY, data.Team2StartLocZ, data.Team2StartLocO);
    bg->SetStartMaxDist(data.StartMaxDist);
    bg->SetLevelRange(data.LevelMin, data.LevelMax);
    bg->SetScriptId(data.scriptId);
    bg->SetGuid(MAKE_NEW_GUID(data.bgTypeId, 0, HIGHGUID_BATTLEGROUND));

    AddBattleground(bg);

    return true;
}

void BattlegroundMgr::CreateInitialBattlegrounds()
{
    uint32 oldMSTime = getMSTime();
    //                                               0   1                  2                  3       4       5                 6               7              8            9             10         11        
    QueryResult result = WorldDatabase.Query("SELECT id, MinPlayersPerTeam, MaxPlayersPerTeam, MinLvl, MaxLvl, AllianceStartLoc, AllianceStartO, HordeStartLoc, HordeStartO, StartMaxDist, Weight, ScriptName FROM battleground_template");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 battlegrounds. DB table `battleground_template` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 bgTypeId = fields[0].GetUInt32();
        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeId, NULL))
            continue;

        BattlemasterListEntry const* bl = sBattlemasterListStore.LookupEntry(bgTypeId);
        if (!bl)
        {
            TC_LOG_ERROR("bg.battleground", "Battleground ID %u not found in BattleMasterList.dbc. Battleground not created.", bgTypeId);
            continue;
        }

        CreateBattlegroundData data;
        data.bgTypeId = BattlegroundTypeId(bgTypeId);
        data.IsArena = (bl->type == TYPE_ARENA);
        data.MinPlayersPerTeam = fields[1].GetUInt16();
        data.MaxPlayersPerTeam = fields[2].GetUInt16();
        data.LevelMin = fields[3].GetUInt8();
        data.LevelMax = fields[4].GetUInt8();
        float dist = fields[9].GetFloat();
        data.StartMaxDist = dist * dist;

        data.scriptId = sObjectMgr->GetScriptId(fields[11].GetCString());
        data.BattlegroundName = bl->name;
        data.MapID = bl->mapid[0];

        if (data.MaxPlayersPerTeam == 0 || data.MinPlayersPerTeam > data.MaxPlayersPerTeam)
        {
            TC_LOG_ERROR("sql.sql", "Table `battleground_template` for id %u has bad values for MinPlayersPerTeam (%u) and MaxPlayersPerTeam(%u)", data.bgTypeId, data.MinPlayersPerTeam, data.MaxPlayersPerTeam);
            continue;
        }

        if (data.LevelMin == 0 || data.LevelMax == 0 || data.LevelMin > data.LevelMax)
        {
            TC_LOG_ERROR("sql.sql", "Table `battleground_template` for id %u has bad values for LevelMin (%u) and LevelMax(%u)", data.bgTypeId, data.LevelMin, data.LevelMax);
            continue;
        }

        if (data.bgTypeId == BATTLEGROUND_AA || data.bgTypeId == BATTLEGROUND_RB)
        {
            data.Team1StartLocX = 0;
            data.Team1StartLocY = 0;
            data.Team1StartLocZ = 0;
            data.Team1StartLocO = fields[6].GetFloat();
            data.Team2StartLocX = 0;
            data.Team2StartLocY = 0;
            data.Team2StartLocZ = 0;
            data.Team2StartLocO = fields[8].GetFloat();
        }
        else
        {
            uint32 startId = fields[5].GetUInt32();
            if (WorldSafeLocsEntry const* start = sWorldSafeLocsStore.LookupEntry(startId))
            {
                data.Team1StartLocX = start->x;
                data.Team1StartLocY = start->y;
                data.Team1StartLocZ = start->z;
                data.Team1StartLocO = fields[6].GetFloat();
            }
            else
            {
                TC_LOG_ERROR("sql.sql", "Table `battleground_template` for id %u have non-existed WorldSafeLocs.dbc id %u in field `AllianceStartLoc`. BG not created.", data.bgTypeId, startId);
                continue;
            }

            startId = fields[7].GetUInt32();
            if (WorldSafeLocsEntry const* start = sWorldSafeLocsStore.LookupEntry(startId))
            {
                data.Team2StartLocX = start->x;
                data.Team2StartLocY = start->y;
                data.Team2StartLocZ = start->z;
                data.Team2StartLocO = fields[8].GetFloat();
            }
            else
            {
                TC_LOG_ERROR("sql.sql", "Table `battleground_template` for id %u have non-existed WorldSafeLocs.dbc id %u in field `HordeStartLoc`. BG not created.", data.bgTypeId, startId);
                continue;
            }
        }

        if (!CreateBattleground(data))
            continue;

        if (data.IsArena)
        {
            if (data.bgTypeId != BATTLEGROUND_AA)
                m_ArenaSelectionWeights[data.bgTypeId] = fields[10].GetUInt8();
        }
        else if (data.bgTypeId != BATTLEGROUND_RB && data.bgTypeId != BATTLEGROUND_RATED_10_VS_10)
        {
            uint8 weight = fields[10].GetUInt8();

            m_BGSelectionWeights[data.bgTypeId] = weight;

            if (IsRatedBattlegroundTemplate(data.bgTypeId))
                m_RatedBattlegroundSelectionWeights[data.bgTypeId] = weight;
        }

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u battlegrounds in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BattlegroundMgr::BuildBattlegroundListPacket(WorldPacket* data, uint64 guid, Player* player, BattlegroundTypeId bgTypeId)
{
    if (!player)
        return;

    BattlegroundDataContainer::iterator it = bgDataStore.find(bgTypeId);
    if (it == bgDataStore.end())
        return;

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(it->second.m_Battlegrounds.begin()->second->GetMapId(), player->getLevel());
    if (!bracketEntry)
        return;

    ObjectGuid BattlemasterGuid = guid;
    uint32 count = 0;
    BattlegroundBracketId bracketId = bracketEntry->GetBracketId();
    BattlegroundClientIdsContainer& clientIds = it->second.m_ClientBattlegroundIds[bracketId];

    data->Initialize(SMSG_BATTLEFIELD_LIST, 16 + 4 + 2 + 2 + clientIds.size() + 1 + 1 + 1 + 1);

    *data << BattlemasterGuid;
    *data << uint32(bgTypeId);
    *data << uint8(bracketEntry->minLevel);
    *data << uint8(bracketEntry->maxLevel);

    size_t count_pos = data->wpos();
    *data << uint32(0);
    
    for (auto Battlefields : clientIds)
    {
        *data << uint32(Battlefields);
        ++count;
    }
    data->put(count_pos, count);
                                                // these may not be in right order, but those are the names.
    data->WriteBit(0);                          // HasHolidayWinToday
    data->WriteBit(0);                          // HasRandomWinToday
    data->WriteBit(0);                          // PvpAnywhere
    data->WriteBit(0);                          // IsRandomBG
}

void BattlegroundMgr::SendToBattleground(Player* player, uint32 instanceId, BattlegroundTypeId bgTypeId)
{
    if (Battleground* bg = GetBattleground(instanceId, bgTypeId))
    {
        float x, y, z, O;
        uint32 mapid = bg->GetMapId();
        uint32 team = player->GetBGTeam();

        bg->GetTeamStartLoc(team, x, y, z, O);
        TC_LOG_DEBUG("bg.battleground", "BattlegroundMgr::SendToBattleground: Sending %s to map %u, X %f, Y %f, Z %f, O %f (bgType %u)", player->GetName().c_str(), mapid, x, y, z, O, bgTypeId);
        player->TeleportTo(mapid, x, y, z, O);
    }
    else
        TC_LOG_ERROR("bg.battleground", "BattlegroundMgr::SendToBattleground: Instance %u (bgType %u) not found while trying to teleport player %s", instanceId, bgTypeId, player->GetName().c_str());
}

void BattlegroundMgr::SendAreaSpiritHealerQueryOpcode(Player* player, Battleground* bg, ObjectGuid guid)
{
    uint32 TimeLeft = 30000 - bg->GetLastResurrectTime();

    if (TimeLeft == uint32(-1))
        TimeLeft = 0;

    WorldPacket data(SMSG_AREA_SPIRIT_HEALER_TIME, 18 + 4);

    data << guid;
    data << TimeLeft;

    player->GetSession()->SendPacket(&data);
}

void BattlegroundMgr::HandleStartTimer(Player* player, uint32 TimeRemaining, uint32 TotalTime, uint32 Type)
{
    WorldPacket data(SMSG_START_TIMER, 4 + 4 + 4);

    data << TimeRemaining;
    data << TotalTime;
    data << Type;

    player->GetSession()->SendPacket(&data);
}

bool BattlegroundMgr::IsArenaType(BattlegroundTypeId bgTypeId)
{
    return bgTypeId == BATTLEGROUND_AA || bgTypeId == BATTLEGROUND_BE || bgTypeId == BATTLEGROUND_NA ||
        bgTypeId == BATTLEGROUND_DS || bgTypeId == BATTLEGROUND_RV || bgTypeId == BATTLEGROUND_RL;
}

bool BattlegroundMgr::IsRatedBattlegroundTemplate(BattlegroundTypeId bgTypeId)
{
    return bgTypeId == BATTLEGROUND_RATED_10_VS_10 || bgTypeId == BATTLEGROUND_WS || bgTypeId == BATTLEGROUND_AB ||
        bgTypeId == BATTLEGROUND_EY || bgTypeId == BATTLEGROUND_BFG || bgTypeId == BATTLEGROUND_TP;
}

BattlegroundQueueTypeId BattlegroundMgr::BGQueueTypeId(BattlegroundTypeId bgTypeId, RatedType ratedType)
{
    switch (bgTypeId)
    {
        case BATTLEGROUND_AB:
            return BATTLEGROUND_QUEUE_AB;
        case BATTLEGROUND_AV:
            return BATTLEGROUND_QUEUE_AV;
        case BATTLEGROUND_EY:
            return BATTLEGROUND_QUEUE_EY;
        case BATTLEGROUND_IC:
            return BATTLEGROUND_QUEUE_IC;
        case BATTLEGROUND_TP:
            return BATTLEGROUND_QUEUE_TP;
        case BATTLEGROUND_BFG:
            return BATTLEGROUND_QUEUE_BFG;
        case BATTLEGROUND_RB:
            return BATTLEGROUND_QUEUE_RB;
        case BATTLEGROUND_SA:
            return BATTLEGROUND_QUEUE_SA;
        case BATTLEGROUND_TOK:
            return BATTLEGROUND_QUEUE_TOK;
        case BATTLEGROUND_SM:
            return BATTLEGROUND_QUEUE_SM;
        case BATTLEGROUND_DG:
            return BATTLEGROUND_QUEUE_DG;
        case BATTLEGROUND_WS:
            return BATTLEGROUND_QUEUE_WS;
        case BATTLEGROUND_AA:
        case BATTLEGROUND_BE:
        case BATTLEGROUND_DS:
        case BATTLEGROUND_NA:
        case BATTLEGROUND_RL:
        case BATTLEGROUND_RV:
        case BATTLEGROUND_RATED_10_VS_10:
            switch (ratedType)
            {
                case RATED_TYPE_2v2:
                    return BATTLEGROUND_QUEUE_2v2;
                case RATED_TYPE_3v3:
                    return BATTLEGROUND_QUEUE_3v3;
                case RATED_TYPE_5v5:
                    return BATTLEGROUND_QUEUE_5v5;
                case RATED_TYPE_10v10:
                    return BATTLEGROUND_QUEUE_10v10;
                default:
                    return BATTLEGROUND_QUEUE_NONE;
            }
        default:
            return BATTLEGROUND_QUEUE_NONE;
    }
}

BattlegroundTypeId BattlegroundMgr::BGTemplateId(BattlegroundQueueTypeId bgQueueTypeId)
{
    switch (bgQueueTypeId)
    {
        case BATTLEGROUND_QUEUE_WS:
            return BATTLEGROUND_WS;
        case BATTLEGROUND_QUEUE_AB:
            return BATTLEGROUND_AB;
        case BATTLEGROUND_QUEUE_AV:
            return BATTLEGROUND_AV;
        case BATTLEGROUND_QUEUE_EY:
            return BATTLEGROUND_EY;
        case BATTLEGROUND_QUEUE_SA:
            return BATTLEGROUND_SA;
        case BATTLEGROUND_QUEUE_IC:
            return BATTLEGROUND_IC;
        case BATTLEGROUND_QUEUE_TP:
            return BATTLEGROUND_TP;
        case BATTLEGROUND_QUEUE_BFG:
            return BATTLEGROUND_BFG;
        case BATTLEGROUND_QUEUE_RB:
            return BATTLEGROUND_RB;
        case BATTLEGROUND_QUEUE_TOK:
            return BATTLEGROUND_TOK;
        case BATTLEGROUND_QUEUE_SM:
            return BATTLEGROUND_SM;
        case BATTLEGROUND_QUEUE_DG:
            return BATTLEGROUND_DG;
        case BATTLEGROUND_QUEUE_2v2:
        case BATTLEGROUND_QUEUE_3v3:
        case BATTLEGROUND_QUEUE_5v5:
            return BATTLEGROUND_AA;
        case BATTLEGROUND_QUEUE_10v10:
            return BATTLEGROUND_RATED_10_VS_10;
        default:
            return BattlegroundTypeId(0);
    }
}

RatedType BattlegroundMgr::GetRatedTypeByQueue(BattlegroundQueueTypeId bgQueueTypeId)
{
    switch (bgQueueTypeId)
    {
        case BATTLEGROUND_QUEUE_2v2:
            return RATED_TYPE_2v2;
        case BATTLEGROUND_QUEUE_3v3:
            return RATED_TYPE_3v3;
        case BATTLEGROUND_QUEUE_5v5:
            return RATED_TYPE_5v5;
        case BATTLEGROUND_QUEUE_10v10:
            return RATED_TYPE_10v10;
        default:
            return RATED_TYPE_NOT_RATED;
    }
}

void BattlegroundMgr::ToggleTesting()
{
    m_Testing = !m_Testing;
    sWorld->SendWorldText(m_Testing ? LANG_DEBUG_BG_ON : LANG_DEBUG_BG_OFF);
}

void BattlegroundMgr::ToggleArenaTesting()
{
    m_ArenaTesting = !m_ArenaTesting;
    sWorld->SendWorldText(m_ArenaTesting ? LANG_DEBUG_ARENA_ON : LANG_DEBUG_ARENA_OFF);
}

void BattlegroundMgr::SetHolidayWeekends(uint32 mask)
{
    for (uint32 bgtype = 1; bgtype < MAX_BATTLEGROUND_TYPE_ID; ++bgtype)
    {
        if (Battleground* bg = GetBattlegroundTemplate(BattlegroundTypeId(bgtype)))
            bg->SetHoliday(mask & (1 << bgtype));
    }
}

void BattlegroundMgr::ScheduleQueueUpdate(uint32 arenaMatchmakerRating, RatedType ratedType, BattlegroundQueueTypeId bgQueueTypeId, BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id)
{
    uint64 const scheduleId = (uint64(arenaMatchmakerRating) << 48) | (uint64(ratedType) << 40) | (uint32(bgQueueTypeId) << 24) | (uint32(bgTypeId) << 8) | bracket_id;
    if (std::find(m_QueueUpdateScheduler.begin(), m_QueueUpdateScheduler.end(), scheduleId) == m_QueueUpdateScheduler.end())
        m_QueueUpdateScheduler.push_back(scheduleId);
}

uint32 BattlegroundMgr::GetMaxRatingDifference() const
{
    uint32 diff = sWorld->getIntConfig(CONFIG_ARENA_MAX_RATING_DIFFERENCE);
    if (diff == 0)
        diff = 5000;
    return diff;
}

uint32 BattlegroundMgr::GetRatingDiscardTimer() const
{
    return sWorld->getIntConfig(CONFIG_ARENA_RATING_DISCARD_TIMER);
}

uint32 BattlegroundMgr::GetPrematureFinishTime() const
{
    return sWorld->getIntConfig(CONFIG_BATTLEGROUND_PREMATURE_FINISH_TIMER);
}

void BattlegroundMgr::LoadBattleMastersEntry()
{
    uint32 oldMSTime = getMSTime();

    mBattleMastersMap.clear();

    QueryResult result = WorldDatabase.Query("SELECT entry, bg_template FROM battlemaster_entry");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 battlemaster entries. DB table `battlemaster_entry` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        uint32 bgTypeId = fields[1].GetUInt32();
        if (!sBattlemasterListStore.LookupEntry(bgTypeId))
        {
            TC_LOG_ERROR("sql.sql", "Table `battlemaster_entry` contain entry %u for not existed battleground type %u, ignored.", entry, bgTypeId);
            continue;
        }

        mBattleMastersMap[entry] = BattlegroundTypeId(bgTypeId);
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u battlemaster entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

HolidayIds BattlegroundMgr::BGTypeToWeekendHolidayId(BattlegroundTypeId bgTypeId)
{
    switch (bgTypeId)
    {
        case BATTLEGROUND_AV:  return HOLIDAY_CALL_TO_ARMS_AV;
        case BATTLEGROUND_EY:  return HOLIDAY_CALL_TO_ARMS_EY;
        case BATTLEGROUND_WS:  return HOLIDAY_CALL_TO_ARMS_WS;
        case BATTLEGROUND_SA:  return HOLIDAY_CALL_TO_ARMS_SA;
        case BATTLEGROUND_AB:  return HOLIDAY_CALL_TO_ARMS_AB;
        case BATTLEGROUND_IC:  return HOLIDAY_CALL_TO_ARMS_IC;
        case BATTLEGROUND_TP:  return HOLIDAY_CALL_TO_ARMS_TP;
        case BATTLEGROUND_BFG: return HOLIDAY_CALL_TO_ARMS_BFG;
        case BATTLEGROUND_TOK: return HOLIDAY_CALL_TO_ARMS_TOK;
        case BATTLEGROUND_DG:  return HOLIDAY_CALL_TO_ARMS_DG;
        case BATTLEGROUND_SM:  return HOLIDAY_CALL_TO_ARMS_SM;
        default:               return HOLIDAY_NONE;
    }
}

BattlegroundTypeId BattlegroundMgr::WeekendHolidayIdToBGType(HolidayIds holiday)
{
    switch (holiday)
    {
        case HOLIDAY_CALL_TO_ARMS_AV:  return BATTLEGROUND_AV;
        case HOLIDAY_CALL_TO_ARMS_EY:  return BATTLEGROUND_EY;
        case HOLIDAY_CALL_TO_ARMS_WS:  return BATTLEGROUND_WS;
        case HOLIDAY_CALL_TO_ARMS_SA:  return BATTLEGROUND_SA;
        case HOLIDAY_CALL_TO_ARMS_AB:  return BATTLEGROUND_AB;
        case HOLIDAY_CALL_TO_ARMS_IC:  return BATTLEGROUND_IC;
        case HOLIDAY_CALL_TO_ARMS_TP:  return BATTLEGROUND_TP;
        case HOLIDAY_CALL_TO_ARMS_BFG: return BATTLEGROUND_BFG;
        case HOLIDAY_CALL_TO_ARMS_TOK: return BATTLEGROUND_TOK;
        case HOLIDAY_CALL_TO_ARMS_DG:  return BATTLEGROUND_DG;
        case HOLIDAY_CALL_TO_ARMS_SM:  return BATTLEGROUND_SM;
        default:                       return BATTLEGROUND_TYPE_NONE;
    }
}

bool BattlegroundMgr::IsBGWeekend(BattlegroundTypeId bgTypeId)
{
    return IsHolidayActive(BGTypeToWeekendHolidayId(bgTypeId));
}

BattlegroundTypeId BattlegroundMgr::GetRandomBG(BattlegroundTypeId bgTypeId)
{
    uint32 weight = 0;
    BattlegroundTypeId returnBgTypeId = BATTLEGROUND_TYPE_NONE;
    BattlegroundSelectionWeightMap selectionWeights;

    if (bgTypeId == BATTLEGROUND_AA)
    {
        for (auto ArenaSelectionWeightsMap : m_ArenaSelectionWeights)
        {
            if (ArenaSelectionWeightsMap.second)
            {
                weight += ArenaSelectionWeightsMap.second;
                selectionWeights[ArenaSelectionWeightsMap.first] = ArenaSelectionWeightsMap.second;
            }
        }
    }

    if (bgTypeId == BATTLEGROUND_RATED_10_VS_10)
    {
        for (auto RatedBattlegroundSelectionWeightsMap : m_RatedBattlegroundSelectionWeights)
        {
            if (RatedBattlegroundSelectionWeightsMap.second)
            {
                weight += RatedBattlegroundSelectionWeightsMap.second;
                selectionWeights[RatedBattlegroundSelectionWeightsMap.first] = RatedBattlegroundSelectionWeightsMap.second;
            }
        }
    }

    else if (bgTypeId == BATTLEGROUND_RB)
    {
        for (auto BGSelectionWeightMap : m_BGSelectionWeights)
        {
            if (BGSelectionWeightMap.second)
            {
                weight += BGSelectionWeightMap.second;
                selectionWeights[BGSelectionWeightMap.first] = BGSelectionWeightMap.second;
            }
        }
    }

    if (weight)
    {
        uint32 selectedWeight = urand(0, weight - 1);
        weight = 0;
        for (auto selectionWeightsMap : selectionWeights)
        {
            weight += selectionWeightsMap.second;
            if (selectedWeight < weight)
            {
                returnBgTypeId = selectionWeightsMap.first;
                break;
            }
        }
    }

    return returnBgTypeId;
}

BGFreeSlotQueueContainer& BattlegroundMgr::GetBGFreeSlotQueueStore(BattlegroundTypeId bgTypeId)
{
    return bgDataStore[bgTypeId].BGFreeSlotQueue;
}

void BattlegroundMgr::AddToBGFreeSlotQueue(BattlegroundTypeId bgTypeId, Battleground* bg)
{
    bgDataStore[bgTypeId].BGFreeSlotQueue.push_front(bg);
}

void BattlegroundMgr::RemoveFromBGFreeSlotQueue(BattlegroundTypeId bgTypeId, uint32 instanceId)
{
    BGFreeSlotQueueContainer& queues = bgDataStore[bgTypeId].BGFreeSlotQueue;
    for (BGFreeSlotQueueContainer::iterator itr = queues.begin(); itr != queues.end(); ++itr)
        if ((*itr)->GetInstanceID() == instanceId)
        {
            queues.erase(itr);
            return;
        }
}

void BattlegroundMgr::AddBattleground(Battleground* bg)
{
    if (bg)
        bgDataStore[bg->GetTypeID()].m_Battlegrounds[bg->GetInstanceID()] = bg;
}

void BattlegroundMgr::RemoveBattleground(BattlegroundTypeId bgTypeId, uint32 instanceId)
{
    bgDataStore[bgTypeId].m_Battlegrounds.erase(instanceId);
}
