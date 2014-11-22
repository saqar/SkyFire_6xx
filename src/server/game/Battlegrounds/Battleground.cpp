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

#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "RatedMgr.h"
#include "RatedInfo.h"
#include "BattlePetMgr.h"
#include "Creature.h"
#include "Chat.h"
#include "Formulas.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "MapManager.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "Util.h"
#include "World.h"
#include "WorldPacket.h"

namespace Trinity
{
    class BattlegroundChatBuilder
    {
    public:
        BattlegroundChatBuilder(ChatMsg msgtype, int32 textId, Player const* source, va_list* args = NULL) : _msgtype(msgtype), _textId(textId), _source(source), _args(args) { }

        void operator()(WorldPacket& data, LocaleConstant loc_idx)
        {
            char const* text = sObjectMgr->GetTrinityString(_textId, loc_idx);
            if (_args)
            {
                // we need copy va_list before use or original va_list will corrupted
                va_list ap;
                va_copy(ap, *_args);

                char str[2048];
                vsnprintf(str, 2048, text, ap);
                va_end(ap);
                do_helper(data, &str[0]);
            }
            else
                do_helper(data, text);
        }
    private:
        void do_helper(WorldPacket& data, char const* text)
        {
            uint64 target_guid = _source ? _source->GetGUID() : 0;
            ChatHandler::BuildChatPacket(data, _msgtype, LANG_UNIVERSAL, _source, _source, text);
        }
        ChatMsg _msgtype;
        int32 _textId;
        Player const* _source;
        va_list* _args;
    };
    
    class Battleground2ChatBuilder
    {
    public:
        Battleground2ChatBuilder(ChatMsg msgtype, int32 textId, Player const* source, int32 arg1, int32 arg2) : _msgtype(msgtype), _textId(textId), _source(source), _arg1(arg1), _arg2(arg2) { }

        void operator()(WorldPacket& data, LocaleConstant loc_idx)
        {
            char const* text = sObjectMgr->GetTrinityString(_textId, loc_idx);
            char const* arg1str = _arg1 ? sObjectMgr->GetTrinityString(_arg1, loc_idx) : "";
            char const* arg2str = _arg2 ? sObjectMgr->GetTrinityString(_arg2, loc_idx) : "";

            char str[2048];
            snprintf(str, 2048, text, arg1str, arg2str);
            uint64 target_guid = _source  ? _source->GetGUID() : 0;
            ChatHandler::BuildChatPacket(data, _msgtype, LANG_UNIVERSAL, _source, _source, str);
        }
    private:
        ChatMsg _msgtype;
        int32 _textId;
        Player const* _source;
        int32 _arg1;
        int32 _arg2;
    };
} // namespace Trinity

template<class Do>
void Battleground::BroadcastWorker(Do& _do)
{
    for (auto players : m_Players)
        if (Player* player = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(players.first, 0, HIGHGUID_PLAYER)))
            _do(player);
}

Battleground::Battleground()
{
    m_Guid                          = 0;
    m_TypeID                        = BATTLEGROUND_TYPE_NONE;
    m_RandomTypeID                  = BATTLEGROUND_TYPE_NONE;
    m_InstanceID                    = 0;
    m_Status                        = STATUS_NONE;
    m_ClientInstanceID              = 0;
    m_EndTime                       = 0;
    m_LastResurrectTime             = 0;
    m_BracketId                     = BG_BRACKET_ID_FIRST;
    m_InvitedAlliance               = 0;
    m_InvitedHorde                  = 0;
    m_RatedType                     = RATED_TYPE_NOT_RATED;
    m_IsArena                       = false;
    m_Winner                        = 2;
    m_StartTime                     = 0;
    m_CountdownTimer                = 0;
    m_ResetStatTimer                = 0;
    m_ValidStartPositionTimer       = 0;
    m_Events                        = 0;
    m_IsRated                       = false;
    m_BuffChange                    = false;
    m_IsRandom                      = false;
    m_Name                          = "";
    m_LevelMin                      = 0;
    m_LevelMax                      = 0;
    m_InBGFreeSlotQueue             = false;
    m_SetDeleteThis                 = false;

    m_MaxPlayersPerTeam             = 0;
    m_MaxPlayers                    = 0;
    m_MinPlayersPerTeam             = 0;
    m_MinPlayers                    = 0;

    m_MapId                         = 0;
    m_Map                           = NULL;
    m_StartMaxDist                  = 0.0f;

    m_TeamStartLocX[TEAM_ALLIANCE]  = 0;
    m_TeamStartLocX[TEAM_HORDE]     = 0;

    m_TeamStartLocY[TEAM_ALLIANCE]  = 0;
    m_TeamStartLocY[TEAM_HORDE]     = 0;

    m_TeamStartLocZ[TEAM_ALLIANCE]  = 0;
    m_TeamStartLocZ[TEAM_HORDE]     = 0;

    m_TeamStartLocO[TEAM_ALLIANCE]  = 0;
    m_TeamStartLocO[TEAM_HORDE]     = 0;

    m_BgRaids[TEAM_ALLIANCE]        = NULL;
    m_BgRaids[TEAM_HORDE]           = NULL;

    m_PlayersCount[TEAM_ALLIANCE]   = 0;
    m_PlayersCount[TEAM_HORDE]      = 0;

    m_TeamScores[TEAM_ALLIANCE]     = 0;
    m_TeamScores[TEAM_HORDE]        = 0;

    m_PrematureCountDown            = false;

    m_HonorMode                     = BG_NORMAL;

    StartDelayTimes[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_2M;
    StartDelayTimes[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_1M;
    StartDelayTimes[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_30S;
    StartDelayTimes[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;
    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_WS_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_WS_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_WS_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_WS_HAS_BEGUN;
}

Battleground::~Battleground()
{
    uint32 size = uint32(BgCreatures.size());
    for (uint32 i = 0; i < size; ++i)
        DelCreature(i);

    size = uint32(BgObjects.size());
    for (uint32 i = 0; i < size; ++i)
        DelObject(i);

    sBattlegroundMgr->RemoveBattleground(GetTypeID(), GetInstanceID());

    if (m_Map)
    {
        m_Map->SetUnload();
        m_Map->SetBG(NULL);
        m_Map = NULL;
    }

    RemoveFromBGFreeSlotQueue();

    for (auto itr : PlayerScores)
        delete itr.second;
}

void Battleground::Update(uint32 diff)
{
    if (!PreUpdateImpl(diff))
        return;

    if (!GetPlayersSize())
    {
        if (!GetInvitedCount(HORDE) && !GetInvitedCount(ALLIANCE))
            m_SetDeleteThis = true;
        return;
    }

    switch (GetStatus())
    {
        case STATUS_WAIT_JOIN:
            if (GetPlayersSize())
            {
                _ProcessJoin(diff);
                _CheckSafePositions(diff);
            }
            break;
        case STATUS_IN_PROGRESS:
            _ProcessOfflineQueue();
            if (IsArena())
            {
                if (GetElapsedTime() >= 47 * MINUTE*IN_MILLISECONDS)
                {
                    UpdateArenaWorldState();
                    CheckArenaAfterTimerConditions();
                    return;
                }
            }
            else
            {
                _ProcessRessurect(diff);
                if (sBattlegroundMgr->GetPrematureFinishTime() && (GetPlayersCountByTeam(ALLIANCE) < GetMinPlayersPerTeam() || GetPlayersCountByTeam(HORDE) < GetMinPlayersPerTeam()))
                    _ProcessProgress(diff);
                else if (m_PrematureCountDown)
                    m_PrematureCountDown = false;
            }
            break;
        case STATUS_WAIT_LEAVE:
            _ProcessLeave(diff);
            break;
        default:
            break;
    }

    SetElapsedTime(GetElapsedTime() + diff);
    if (GetStatus() == STATUS_WAIT_JOIN)
    {
        m_ResetStatTimer += diff;
        m_CountdownTimer += diff;
    }

    PostUpdateImpl(diff);
}

inline void Battleground::_CheckSafePositions(uint32 diff)
{
    float maxDist = GetStartMaxDist();
    if (!maxDist)
        return;

    m_ValidStartPositionTimer += diff;
    if (m_ValidStartPositionTimer >= CHECK_PLAYER_POSITION_INVERVAL)
    {
        m_ValidStartPositionTimer = 0;

        Position pos;
        float x, y, z, o;
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                player->GetPosition(&pos);
                GetTeamStartLoc(player->GetBGTeam(), x, y, z, o);
                if (pos.GetExactDistSq(x, y, z) > maxDist)
                {
                    TC_LOG_DEBUG("bg.battleground", "BATTLEGROUND: Sending %s back to start location (map: %u) (possible exploit)", player->GetName().c_str(), GetMapId());
                    player->TeleportTo(GetMapId(), x, y, z, o);
                }
            }
    }
}

inline void Battleground::_ProcessOfflineQueue()
{
    if (!m_OfflineQueue.empty())
    {
        BattlegroundPlayerMap::iterator itr = m_Players.find(*(m_OfflineQueue.begin()));
        if (itr != m_Players.end())
        {
            if (itr->second.OfflineRemoveTime <= sWorld->GetGameTime())
            {
                RemovePlayerAtLeave(itr->first, true, true);
                m_OfflineQueue.pop_front();
            }
        }
    }
}

inline void Battleground::_ProcessRessurect(uint32 diff)
{
    // *********************************************************
    // ***        BATTLEGROUND RESSURECTION SYSTEM           ***
    // *********************************************************

    m_LastResurrectTime += diff;
    if (m_LastResurrectTime >= RESURRECTION_INTERVAL)
    {
        if (GetReviveQueueSize())
        {
            for (std::map<uint64, std::vector<uint64> >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
            {
                Creature* sh = NULL;
                for (std::vector<uint64>::const_iterator itr2 = (itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
                {
                    Player* player = ObjectAccessor::FindPlayer(*itr2);
                    if (!player)
                        continue;

                    if (!sh && player->IsInWorld())
                    {
                        sh = player->GetMap()->GetCreature(itr->first);
                        if (sh)
                            sh->CastSpell(sh, SPELL_SPIRIT_HEAL, true);
                    }

                    player->CastSpell(player, SPELL_RESURRECTION_VISUAL, true);
                    m_ResurrectQueue.push_back(*itr2);
                }
                (itr->second).clear();
            }

            m_ReviveQueue.clear();
            m_LastResurrectTime = 0;
        }
        else
            m_LastResurrectTime = 0;
    }
    else if (m_LastResurrectTime > 500)
    {
        for (std::vector<uint64>::const_iterator itr = m_ResurrectQueue.begin(); itr != m_ResurrectQueue.end(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(*itr);
            if (!player)
                continue;
            player->ResurrectPlayer(1.0f);
            player->CastSpell(player, 6962, true);
            player->CastSpell(player, SPELL_SPIRIT_HEAL_MANA, true);
            sObjectAccessor->ConvertCorpseForPlayer(*itr);
        }
        m_ResurrectQueue.clear();
    }
}

uint32 Battleground::GetPrematureWinner()
{
    uint32 winner = 0;
    if (GetPlayersCountByTeam(ALLIANCE) >= GetMinPlayersPerTeam())
        winner = ALLIANCE;
    else if (GetPlayersCountByTeam(HORDE) >= GetMinPlayersPerTeam())
        winner = HORDE;

    return winner;
}

inline void Battleground::_ProcessProgress(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND BALLANCE SYSTEM            ***
    // *********************************************************
    if (!m_PrematureCountDown)
    {
        m_PrematureCountDown = true;
        m_PrematureCountDownTimer = sBattlegroundMgr->GetPrematureFinishTime();
    }
    else if (m_PrematureCountDownTimer < diff)
    {
        EndBattleground(GetPrematureWinner());
        m_PrematureCountDown = false;
    }
    else if (!sBattlegroundMgr->isTesting())
    {
        uint32 newtime = m_PrematureCountDownTimer - diff;
        if (newtime > (MINUTE * IN_MILLISECONDS))
        {
            if (newtime / (MINUTE * IN_MILLISECONDS) != m_PrematureCountDownTimer / (MINUTE * IN_MILLISECONDS))
                PSendMessageToAll(LANG_BATTLEGROUND_PREMATURE_FINISH_WARNING, CHAT_MSG_SYSTEM, NULL, (uint32)(m_PrematureCountDownTimer / (MINUTE * IN_MILLISECONDS)));
        }
        else
        {
            if (newtime / (15 * IN_MILLISECONDS) != m_PrematureCountDownTimer / (15 * IN_MILLISECONDS))
                PSendMessageToAll(LANG_BATTLEGROUND_PREMATURE_FINISH_WARNING_SECS, CHAT_MSG_SYSTEM, NULL, (uint32)(m_PrematureCountDownTimer / IN_MILLISECONDS));
        }
        m_PrematureCountDownTimer = newtime;
    }
}

inline void Battleground::_ProcessJoin(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND STARTING SYSTEM            ***
    // *********************************************************
    ModifyStartDelayTime(diff);

    if (!IsArena())
        SetRemainingTime(300000);

    if (m_ResetStatTimer > 5000)
    {
        m_ResetStatTimer = 0;
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                player->ResetAllPowers();
    }

    if (m_CountdownTimer >= 10000)
    {
        uint32 countdownMaxForBGType = IsArena() ? ARENA_COUNTDOWN_MAX : BATTLEGROUND_COUNTDOWN_MAX;
        uint32 tTime = (countdownMaxForBGType - (GetElapsedTime() / 1000));

        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                sBattlegroundMgr->HandleStartTimer(player, countdownMaxForBGType, tTime, 0);

        m_CountdownTimer = 0;
    }

    if (!(m_Events & BG_STARTING_EVENT_1))
    {
        m_Events |= BG_STARTING_EVENT_1;

        if (!FindBgMap())
        {
            TC_LOG_ERROR("bg.battleground", "Battleground::_ProcessJoin: map (map id: %u, instance id: %u) is not created!", GetMapId(), GetInstanceID());
            EndNow();
            return;
        }

        if (!SetupBattleground())
        {
            EndNow();
            return;
        }

        StartingEventCloseDoors();
        SetStartDelayTime(StartDelayTimes[BG_STARTING_EVENT_FIRST]);
        SendMessageToAll(StartMessageIds[BG_STARTING_EVENT_FIRST], CHAT_MSG_BG_SYSTEM_NEUTRAL);
    }
    else if (GetStartDelayTime() <= StartDelayTimes[BG_STARTING_EVENT_SECOND] && !(m_Events & BG_STARTING_EVENT_2))
    {
        m_Events |= BG_STARTING_EVENT_2;
        SendMessageToAll(StartMessageIds[BG_STARTING_EVENT_SECOND], CHAT_MSG_BG_SYSTEM_NEUTRAL);
    }
    else if (GetStartDelayTime() <= StartDelayTimes[BG_STARTING_EVENT_THIRD] && !(m_Events & BG_STARTING_EVENT_3))
    {
        m_Events |= BG_STARTING_EVENT_3;
        SendMessageToAll(StartMessageIds[BG_STARTING_EVENT_THIRD], CHAT_MSG_BG_SYSTEM_NEUTRAL);
    }
    else if (GetStartDelayTime() <= 0 && !(m_Events & BG_STARTING_EVENT_4))
    {
        m_Events |= BG_STARTING_EVENT_4;

        StartingEventOpenDoors();

        SendWarningToAll(StartMessageIds[BG_STARTING_EVENT_FOURTH]);
        SetStatus(STATUS_IN_PROGRESS);
        SetStartDelayTime(StartDelayTimes[BG_STARTING_EVENT_FOURTH]);

        if (IsArena())
        {
            /// @todo add arena sound PlaySoundToAll(SOUND_ARENA_START);
            for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    WorldPacket status;
                    BattlegroundQueueTypeId bgQueueTypeId = sBattlegroundMgr->BGQueueTypeId(GetTypeID(), GetRatedType());
                    uint32 queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);

                    sBattlegroundMgr->BuildBattlegroundStatusPacket(&status, this, player, queueSlot, STATUS_IN_PROGRESS, player->GetBattlegroundQueueJoinTime(GetTypeID()), GetElapsedTime(), GetRatedType());
                    player->GetSession()->SendPacket(&status);
                    player->SetByteValue(PLAYER_FIELD_PVP_INFO, 3, player->GetBGTeam());
                    player->RemoveAurasDueToSpell(SPELL_ARENA_PREPARATION);
                    player->ResetAllPowers();
                    if (!player->IsGameMaster())
                    {
                        Unit::AuraApplicationMap & auraMap = player->GetAppliedAuras();
                        for (Unit::AuraApplicationMap::iterator iter = auraMap.begin(); iter != auraMap.end();)
                        {
                            AuraApplication * aurApp = iter->second;
                            Aura* aura = aurApp->GetBase();
                            if (!aura->IsPermanent() && aura->GetDuration() <= 30 * IN_MILLISECONDS && aurApp->IsPositive() &&
                                (!(aura->GetSpellInfo()->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY)) &&
                                (!aura->HasEffectType(SPELL_AURA_MOD_INVISIBILITY)))
                                player->RemoveAura(iter);
                            else
                                ++iter;
                        }
                    }
                }
            CheckArenaWinConditions();
        }
        else
        {
            PlaySoundToAll(SOUND_BG_START);

            for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    player->RemoveAurasDueToSpell(SPELL_PREPARATION);
                    player->ResetAllPowers();
                }

            if (sWorld->getBoolConfig(CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_ENABLE))
                sWorld->SendWorldText(LANG_BG_STARTED_ANNOUNCE_WORLD, GetName(), GetMinLevel(), GetMaxLevel());
        }
    }

    if (IsRated())
    {
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                RatedInfo* rInfo = sRatedMgr->GetRatedInfo(player->GetGUID());
                const StatsBySlot* stats = rInfo->GetStatsBySlot(GetRatedType());
                UpdatePlayerScore(player, SCORE_PRE_MATCH_MMR, rInfo->GetMatchMakerRating());
                UpdatePlayerScore(player, SCORE_PRE_MATCH_PERSONAL_RATING, stats->PersonalRating);
            }
        }
    }

    if (GetRemainingTime() > 0 && (m_EndTime -= diff) > 0)
        SetRemainingTime(GetRemainingTime() - diff);
}

inline void Battleground::_ProcessLeave(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND ENDING SYSTEM              ***
    // *********************************************************
    SetRemainingTime(GetRemainingTime() - diff);
    if (GetRemainingTime() <= 0)
    {
        SetRemainingTime(0);
        BattlegroundPlayerMap::iterator itr, next;
        for (itr = m_Players.begin(); itr != m_Players.end(); itr = next)
        {
            next = itr;
            ++next;
            RemovePlayerAtLeave(itr->first, true, true);
        }
    }
}

inline Player* Battleground::_GetPlayer(uint64 guid, bool offlineRemove, char const* context) const
{
    Player* player = NULL;
    if (!offlineRemove)
    {
        player = ObjectAccessor::FindPlayer(guid);
        if (!player)
            TC_LOG_ERROR("bg.battleground", "Battleground::%s: player (GUID: %u) not found for BG (map: %u, instance id: %u)!",
                context, GUID_LOPART(guid), GetMapId(), GetInstanceID());
    }
    return player;
}

inline Player* Battleground::_GetPlayer(BattlegroundPlayerMap::iterator itr, char const* context)
{
    return _GetPlayer(itr->first, itr->second.OfflineRemoveTime, context);
}

inline Player* Battleground::_GetPlayer(BattlegroundPlayerMap::const_iterator itr, char const* context) const
{
    return _GetPlayer(itr->first, itr->second.OfflineRemoveTime, context);
}

inline Player* Battleground::_GetPlayerForTeam(uint32 teamId, BattlegroundPlayerMap::const_iterator itr, char const* context) const
{
    Player* player = _GetPlayer(itr, context);
    if (player)
    {
        uint32 team = itr->second.Team;
        if (!team)
            team = player->GetTeam();
        if (team != teamId)
            player = NULL;
    }
    return player;
}

void Battleground::SetTeamStartLoc(uint32 TeamID, float X, float Y, float Z, float O)
{
    TeamId idx = GetTeamIndexByTeamId(TeamID);
    m_TeamStartLocX[idx] = X;
    m_TeamStartLocY[idx] = Y;
    m_TeamStartLocZ[idx] = Z;
    m_TeamStartLocO[idx] = O;
}

void Battleground::SendPacketToAll(WorldPacket* packet)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayer(itr, "SendPacketToAll"))
            player->SendDirectMessage(packet);
}

void Battleground::SendPacketToTeam(uint32 TeamID, WorldPacket* packet, Player* sender, bool self)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        if (Player* player = _GetPlayerForTeam(TeamID, itr, "SendPacketToTeam"))
        {
            if (self || sender != player)
                player->SendDirectMessage(packet);
        }
    }
}

void Battleground::PlaySoundToAll(uint32 SoundID)
{
    WorldPacket data;
    sBattlegroundMgr->BuildPlaySoundPacket(&data, SoundID);
    SendPacketToAll(&data);
}

void Battleground::PlaySoundToTeam(uint32 SoundID, uint32 TeamID)
{
    WorldPacket data;
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayerForTeam(TeamID, itr, "PlaySoundToTeam"))
        {
            sBattlegroundMgr->BuildPlaySoundPacket(&data, SoundID);
            player->SendDirectMessage(&data);
        }
}

void Battleground::CastSpellOnTeam(uint32 SpellID, uint32 TeamID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayerForTeam(TeamID, itr, "CastSpellOnTeam"))
            player->CastSpell(player, SpellID, true);
}

void Battleground::RemoveAuraOnTeam(uint32 SpellID, uint32 TeamID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayerForTeam(TeamID, itr, "RemoveAuraOnTeam"))
            player->RemoveAura(SpellID);
}

void Battleground::YellToAll(Creature* creature, char const* text, uint32 language)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayer(itr, "YellToAll"))
        {
            WorldPacket data;
            ChatHandler::BuildChatPacket(data, CHAT_MSG_MONSTER_YELL, Language(language), creature, player, text);
            player->SendDirectMessage(&data);
        }
}

void Battleground::RewardHonorToTeam(uint32 Honor, uint32 TeamID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayerForTeam(TeamID, itr, "RewardHonorToTeam"))
            UpdatePlayerScore(player, SCORE_BONUS_HONOR, Honor);
}

void Battleground::RewardReputationToTeam(uint32 faction_id, uint32 Reputation, uint32 TeamID)
{
    if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction_id))
        for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
            if (Player* player = _GetPlayerForTeam(TeamID, itr, "RewardReputationToTeam"))
                player->GetReputationMgr().ModifyReputation(factionEntry, Reputation);
}

void Battleground::UpdateWorldState(uint32 Field, uint32 Value, bool Hidden/*= false*/)
{
    WorldPacket data;
    sBattlegroundMgr->BuildUpdateWorldStatePacket(&data, Field, Value, Hidden);
    SendPacketToAll(&data);
}

void Battleground::UpdateWorldStateForPlayer(uint32 field, uint32 value, Player* player, bool Hidden/*= false*/)
{
    WorldPacket data;
    sBattlegroundMgr->BuildUpdateWorldStatePacket(&data, field, value, Hidden);
    player->SendDirectMessage(&data);
}

void Battleground::EndBattleground(uint32 winner)
{
    RemoveFromBGFreeSlotQueue();

    int32 winmsg_id = 0;

    if (winner == ALLIANCE)
    {
        winmsg_id = IsBattleground() ? LANG_BG_A_WINS : LANG_ARENA_GOLD_WINS;
        PlaySoundToAll(SOUND_ALLIANCE_WINS);        
        SetWinner(WINNER_ALLIANCE);
    }
    else if (winner == HORDE)
    {
        winmsg_id = IsBattleground() ? LANG_BG_H_WINS : LANG_ARENA_GREEN_WINS;
        PlaySoundToAll(SOUND_HORDE_WINS);
        SetWinner(WINNER_HORDE);
    }
    else
    {
        SetWinner(3);
    }

    SetStatus(STATUS_WAIT_LEAVE);
    SetRemainingTime(TIME_AUTOCLOSE_BATTLEGROUND);

    std::set<uint32> guildIDs;
    WorldPacket pvpLogData;
    sBattlegroundMgr->BuildPvpLogDataPacket(&pvpLogData, this);

    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(GetTypeID(), GetRatedType());

    uint8 aliveWinners = GetAlivePlayersCountByTeam(winner);
    for (BattlegroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        Player* player = _GetPlayer(itr, "EndBattleground");
        uint32 team = itr->second.Team;

        RatedInfo* rInfo = sRatedMgr->GetRatedInfo(itr->first);
        ASSERT(rInfo && "RatedInfo must not be nullptr");

        int16 personalRatingChange = 0, matchmakerRatingChange = 0;
        uint16 winnerMatchmakerRating = GetTeamMatchmakerRating(winner);
        uint16 loserMatchmakerRating = GetTeamMatchmakerRating(GetOtherTeam(winner));

        if (itr->second.OfflineRemoveTime)
        {
            if (IsRated())
            {
                if (team == winner)
                    rInfo->UpdateStats(GetRatedType(), loserMatchmakerRating, personalRatingChange, matchmakerRatingChange, true, true);
                else
                    rInfo->UpdateStats(GetRatedType(), winnerMatchmakerRating, personalRatingChange, matchmakerRatingChange, false, true);
            }
            continue;
        }

        if (!player)
            continue;

        if (player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
            player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

        if (team == winner && GetRatedType() == RATED_TYPE_5v5 && player->IsAlive() && aliveWinners == 1)
            player->CastSpell(player, SPELL_THE_LAST_STANDING, true);

        if (!player->IsAlive())
        {
            player->ResurrectPlayer(1.0f);
            player->SpawnCorpseBones();
        }
        else
        {
            player->CombatStop();
            player->getHostileRefManager().deleteReferences();
        }

        if (IsRated())
        {
            if (team == winner)
            {
                uint8 ratedSlot = RatedInfo::GetRatedSlotByType(GetRatedType());
                uint32 rating = player->GetArenaPersonalRating(ratedSlot);

                if (IsArena())
                {
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA, GetMapId());
                    //player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_META_ARENA, sWorld->getIntConfig(CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD));
                }
                else
                {
                    //player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_META_RBG, sWorld->getIntConfig(CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD)); 
                }
                rInfo->UpdateStats(GetRatedType(), loserMatchmakerRating, personalRatingChange, matchmakerRatingChange, true, false);
            }
            else
            {
                if (IsArena())
                    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA, ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE);
                else
                {
                    // Rated Battleground placeholder
                }
                rInfo->UpdateStats(GetRatedType(), winnerMatchmakerRating, personalRatingChange, matchmakerRatingChange, false, false);                
            }
        }

        UpdatePlayerScore(player, SCORE_PERSONAL_RATING_CHANGE, personalRatingChange);
        UpdatePlayerScore(player, SCORE_MMR_CHANGE, matchmakerRatingChange);

        auto updateAverageMMRforTeam = [this](uint32 team) -> void
        {
            GroupRatedStats statsByGroup = GetBgRaid(team)->GetRatedStats(GetRatedType());
            SetTeamMatchmakerRating(team, statsByGroup.averageMMR);
        };

        updateAverageMMRforTeam(winner);
        updateAverageMMRforTeam(GetOtherTeam(winner));

        uint32 winnerKills = player->GetRandomWinner() ? sWorld->getIntConfig(CONFIG_BG_REWARD_WINNER_HONOR_LAST) : sWorld->getIntConfig(CONFIG_BG_REWARD_WINNER_HONOR_FIRST);
        uint32 loserKills = player->GetRandomWinner() ? sWorld->getIntConfig(CONFIG_BG_REWARD_LOSER_HONOR_LAST) : sWorld->getIntConfig(CONFIG_BG_REWARD_LOSER_HONOR_FIRST);

        player->RemoveAura(SPELL_HONORABLE_DEFENDER_25Y);
        player->RemoveAura(SPELL_HONORABLE_DEFENDER_60Y);

        if (team == winner)
        {
            if (IsRandom() || BattlegroundMgr::IsBGWeekend(GetTypeID()))
            {
                UpdatePlayerScore(player, SCORE_BONUS_HONOR, GetBonusHonorFromKill(winnerKills));
                if (!player->GetRandomWinner())
                {
                    player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_ARENA_META, sWorld->getIntConfig(CONFIG_BG_REWARD_WINNER_CONQUEST_FIRST));
                    player->SetRandomWinner(true);
                }
            }
            else
                player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_ARENA_META, sWorld->getIntConfig(CONFIG_BG_REWARD_WINNER_CONQUEST_LAST));

            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_BG, 1);

            if (player->GetGuildId() && (guildIDs.find(player->GetGuildId()) == guildIDs.end()))
            {
                guildIDs.insert(player->GetGuildId());
                if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
                {
                    guild->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_BG, 1, 0, 0, NULL, player);

                    if (IsRated())
                    {
                        if (IsBattleground())
                            guild->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_BATTLEGROUND, 1, 0, 0, NULL, player);
                        else
                        {
                            guild->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA, 1, 0, 0, NULL, player);
                            if (rInfo)
                            {
                                StatsBySlot const *stats = rInfo->GetStatsBySlot(GetRatedType());
                                guild->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING, stats->PersonalRating, 0, 0, NULL, player);
                            }
                        }
                    }
                }
            }                                 
        }
        else
        {
            if (IsRandom() || BattlegroundMgr::IsBGWeekend(GetTypeID()))
                UpdatePlayerScore(player, SCORE_BONUS_HONOR, GetBonusHonorFromKill(loserKills));
        }

        player->ResetAllPowers();
        player->CombatStopWithPets(true);

        BlockMovement(player);

        player->SendDirectMessage(&pvpLogData);

        WorldPacket data;
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, this, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), STATUS_IN_PROGRESS, player->GetBattlegroundQueueJoinTime(GetTypeID()), GetElapsedTime(), GetRatedType());
        player->SendDirectMessage(&data);

        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND, 1);
    }

    if (winmsg_id)
        SendMessageToAll(winmsg_id, CHAT_MSG_BG_SYSTEM_NEUTRAL);
}

uint32 Battleground::GetBonusHonorFromKill(uint32 kills) const
{
    uint32 maxLevel = std::min<uint32>(GetMaxLevel(), 80U);
    return Trinity::Honor::hk_honor_at_level(maxLevel, float(kills));
}

void Battleground::BlockMovement(Player* player)
{
    player->SetClientControl(player, false);    
}

void Battleground::RemovePlayerAtLeave(uint64 guid, bool Transport, bool SendPacket)
{
    uint32 team = GetPlayerTeam(guid);
    bool participant = false;
    BattlegroundPlayerMap::iterator itr = m_Players.find(guid);
    if (itr != m_Players.end())
    {
        UpdatePlayersCountByTeam(team, true);
        m_Players.erase(itr);
        participant = true;
    }

    BattlegroundScoreMap::iterator itr2 = PlayerScores.find(guid);
    if (itr2 != PlayerScores.end())
    {
        delete itr2->second;
        PlayerScores.erase(itr2);
    }

    RemovePlayerFromResurrectQueue(guid);

    Player* player = ObjectAccessor::FindPlayer(guid);

    if (player)
    {
        if (player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
            player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

        if (!player->IsAlive())
        {
            player->ResurrectPlayer(1.0f);
            player->SpawnCorpseBones();
        }
    }

    RemovePlayer(player, guid, team);                           

    BattlegroundTypeId bgTypeId = GetTypeID();
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(GetTypeID(), GetRatedType());

    if (participant)
    {
        if (player)
        {
            player->ClearAfkReports();

            if (!team) team = player->GetTeam();

            if (IsArena())
            {
                bgTypeId = BATTLEGROUND_AA; 

                player->RemovePet(NULL, PET_SAVE_NOT_IN_SLOT);
                player->ResummonPetTemporaryUnSummonedIfAny();

                if (IsRated() && GetStatus() == STATUS_IN_PROGRESS)
                {
                    RatedInfo* rInfo = sRatedMgr->GetRatedInfo(player->GetGUID());
                    int16 ratingChange, matchmakerRatingChange;
                    rInfo->UpdateStats(GetRatedType(), GetTeamMatchmakerRating(GetOtherTeam(team)), ratingChange, matchmakerRatingChange, false, false);
                }
            }
            if (SendPacket)
            {
                WorldPacket data;
                sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, this, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), STATUS_NONE, player->GetBattlegroundQueueJoinTime(bgTypeId), 0, RATED_TYPE_NOT_RATED);
                player->SendDirectMessage(&data);
            }

            player->RemoveBattlegroundQueueId(bgQueueTypeId);
        }
        else
        {
            if (IsRated() && GetStatus() == STATUS_IN_PROGRESS)
            {
                RatedInfo* rInfo = sRatedMgr->GetRatedInfo(guid);
                int16 ratingChange, matchmakerRatingChange;
                rInfo->UpdateStats(GetRatedType(), GetTeamMatchmakerRating(GetOtherTeam(team)), ratingChange, matchmakerRatingChange, false, true);
            }
        }

        if (Group* group = GetBgRaid(team))
        {
            if (!group->RemoveMember(guid))
                SetBgRaid(team, NULL);
        }
        DecreaseInvitedCount(team);
        if (IsBattleground() && GetStatus() < STATUS_WAIT_LEAVE)
        {
            AddToBGFreeSlotQueue();
            sBattlegroundMgr->ScheduleQueueUpdate(0, RATED_TYPE_NOT_RATED, bgQueueTypeId, bgTypeId, GetBracketId());
        }
        WorldPacket data;
        sBattlegroundMgr->BuildPlayerLeftBattlegroundPacket(&data, guid);
        SendPacketToTeam(team, &data, player, false);
    }

    if (player)
    {
        player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);
        player->SetBGTeam(0);
        player->RemoveBattlegroundQueueJoinTime(bgTypeId);

        if (Transport)
            player->TeleportToBGEntryPoint();

        TC_LOG_DEBUG("bg.battleground", "Removed player %s from Battleground.", player->GetName().c_str());
    }
}

void Battleground::Reset()
{
    SetWinner(WINNER_NONE);
    SetStatus(STATUS_WAIT_QUEUE);
    SetElapsedTime(0);
    SetRemainingTime(0);
    SetLastResurrectTime(0);
    m_Events = 0;

    if (m_InvitedAlliance > 0 || m_InvitedHorde > 0)
        TC_LOG_ERROR("bg.battleground", "Battleground::Reset: one of the counters is not 0 (alliance: %u, horde: %u) for BG (map: %u, instance id: %u)!",
            m_InvitedAlliance, m_InvitedHorde, GetMapId(), GetInstanceID());

    m_InvitedAlliance = 0;
    m_InvitedHorde = 0;
    m_InBGFreeSlotQueue = false;

    m_Players.clear();

    for (auto scores : PlayerScores)
        delete scores.second;

    PlayerScores.clear();
    ResetBGSubclass();
}

void Battleground::StartBattleground()
{
    SetElapsedTime(0);
    SetLastResurrectTime(0);
    AddToBGFreeSlotQueue();

    sBattlegroundMgr->AddBattleground(this);

    if (IsRated())
        TC_LOG_DEBUG("bg.arena", "Rated match type: %u started.", GetRatedType());
}

void Battleground::AddPlayer(Player* player)
{
    if (player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_AFK))
        player->ToggleAFK();

    uint64 guid = player->GetGUID();
    uint32 team = player->GetBGTeam();

    BattlegroundPlayer bp;
    bp.OfflineRemoveTime = 0;
    bp.Team = team;

    m_Players[guid] = bp;

    UpdatePlayersCountByTeam(team, false);

    WorldPacket data;
    sBattlegroundMgr->BuildPlayerJoinedBattlegroundPacket(&data, player->GetGUID());
    SendPacketToTeam(team, &data, player, false);

    BattlegroundQueueTypeId bgQueueTypeId = sBattlegroundMgr->BGQueueTypeId(GetTypeID(), GetRatedType());
    uint32 queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);

    sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, this, player, queueSlot, STATUS_IN_PROGRESS, player->GetBattlegroundQueueJoinTime(GetTypeID()), GetElapsedTime(), GetRatedType());
    player->GetSession()->SendPacket(&data);

    player->RemoveAurasByType(SPELL_AURA_MOUNTED);

    if (IsArena())
    {
        player->RemoveArenaEnchantments(TEMP_ENCHANTMENT_SLOT);
        if (team == ALLIANCE)
        {
            if (player->GetTeam() == HORDE)
                player->CastSpell(player, SPELL_HORDE_GOLD_FLAG, true);
            else
                player->CastSpell(player, SPELL_ALLIANCE_GOLD_FLAG, true);
        }
        else
        {
            if (player->GetTeam() == HORDE)
                player->CastSpell(player, SPELL_HORDE_GREEN_FLAG, true);
            else
                player->CastSpell(player, SPELL_ALLIANCE_GREEN_FLAG, true);
        }

        player->DestroyConjuredItems(true);
        player->UnsummonPetTemporaryIfAny();
        player->GetBattlePetMgr()->UnSummonCurrentBattlePet(false);

        if (GetStatus() == STATUS_WAIT_JOIN)
        {
            player->CastSpell(player, SPELL_ARENA_PREPARATION, true);
            player->ResetAllPowers();
        }
    }
    else
    {
        if (GetStatus() == STATUS_WAIT_JOIN)
        {
            player->CastSpell(player, SPELL_PREPARATION, true);

            int32 countdownMaxForBGType = IsArena() ? ARENA_COUNTDOWN_MAX : BATTLEGROUND_COUNTDOWN_MAX;
            uint32 tTime = (countdownMaxForBGType - (GetElapsedTime() / 1000));
            sBattlegroundMgr->HandleStartTimer(player, countdownMaxForBGType, tTime, 0);
        }
    }

    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,            GetMapId(), true);
    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_BG, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,                   GetMapId(), true);
    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,              GetMapId(), true);
    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,          GetMapId(), true);
    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,               GetMapId(), true);
    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,     GetMapId(), true);
    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,   GetMapId(), true);
    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,           GetMapId(), true);
    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,             GetMapId(), true);
    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,        GetMapId(), true);
    player->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP,         GetMapId(), true);

    PlayerAddedToBGCheckIfBGIsRunning(player);
    AddOrSetPlayerToCorrectBgGroup(player, team);
}

void Battleground::AddOrSetPlayerToCorrectBgGroup(Player* player, uint32 team)
{
    uint64 playerGuid = player->GetGUID();
    Group* group = GetBgRaid(team);
    if (!group)
    {
        group = new Group;
        SetBgRaid(team, group);
        group->Create(player);
    }
    else
    {
        if (group->IsMember(playerGuid))
        {
            uint8 subgroup = group->GetMemberGroup(playerGuid);
            player->SetBattlegroundOrBattlefieldRaid(group, subgroup);
        }
        else
        {
            group->AddMember(player);
            if (Group* originalGroup = player->GetOriginalGroup())
                if (originalGroup->IsLeader(playerGuid))
                {
                    group->ChangeLeader(playerGuid);
                    group->SendUpdate();
                }
        }
    }
}

void Battleground::EventPlayerLoggedIn(Player* player)
{
    uint64 guid = player->GetGUID();
    for (std::deque<uint64>::iterator itr = m_OfflineQueue.begin(); itr != m_OfflineQueue.end(); ++itr)
    {
        if (*itr == guid)
        {
            m_OfflineQueue.erase(itr);
            break;
        }
    }
    m_Players[guid].OfflineRemoveTime = 0;
    PlayerAddedToBGCheckIfBGIsRunning(player);
}

void Battleground::EventPlayerLoggedOut(Player* player)
{
    uint64 guid = player->GetGUID();
    if (!IsPlayerInBattleground(guid))
        return;

    m_OfflineQueue.push_back(player->GetGUID());
    m_Players[guid].OfflineRemoveTime = sWorld->GetGameTime() + MAX_OFFLINE_TIME;
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        RemovePlayer(player, guid, GetPlayerTeam(guid));

        if (IsArena())
            if (GetAlivePlayersCountByTeam(player->GetBGTeam()) <= 1 && GetPlayersCountByTeam(GetOtherTeam(player->GetBGTeam())))
                EndBattleground(GetOtherTeam(player->GetBGTeam()));
    }
}

void Battleground::AddToBGFreeSlotQueue()
{
    if (!m_InBGFreeSlotQueue && IsBattleground())
    {
        sBattlegroundMgr->AddToBGFreeSlotQueue(GetTypeID(), this);
        m_InBGFreeSlotQueue = true;
    }
}

void Battleground::RemoveFromBGFreeSlotQueue()
{
    if (m_InBGFreeSlotQueue)
    {
        sBattlegroundMgr->RemoveFromBGFreeSlotQueue(GetTypeID(), GetInstanceID());
        m_InBGFreeSlotQueue = false;
    }
}

uint32 Battleground::GetFreeSlotsForTeam(uint32 Team) const
{
    if (GetStatus() == STATUS_WAIT_JOIN)
        return (GetInvitedCount(Team) < GetMaxPlayersPerTeam()) ? GetMaxPlayersPerTeam() - GetInvitedCount(Team) : 0;

    uint32 otherTeam;
    uint32 otherIn;
    if (Team == ALLIANCE)
    {
        otherTeam = GetInvitedCount(HORDE);
        otherIn = GetPlayersCountByTeam(HORDE);
    }
    else
    {
        otherTeam = GetInvitedCount(ALLIANCE);
        otherIn = GetPlayersCountByTeam(ALLIANCE);
    }
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        uint32 diff = 0;
        if (otherTeam == GetInvitedCount(Team))
            diff = 1;
        else if (otherTeam > GetInvitedCount(Team))
            diff = otherTeam - GetInvitedCount(Team);

        uint32 diff2 = (GetInvitedCount(Team) < GetMaxPlayersPerTeam()) ? GetMaxPlayersPerTeam() - GetInvitedCount(Team) : 0;
        uint32 diff3 = 0;
        if (otherIn == GetPlayersCountByTeam(Team))
            diff3 = 1;
        else if (otherIn > GetPlayersCountByTeam(Team))
            diff3 = otherIn - GetPlayersCountByTeam(Team);
        else if (GetInvitedCount(Team) <= GetMinPlayersPerTeam())
            diff3 = GetMinPlayersPerTeam() - GetInvitedCount(Team) + 1;

        diff = std::min(diff, diff2);
        return std::min(diff, diff3);
    }
    return 0;
}

bool Battleground::HasFreeSlots() const
{
    return GetPlayersSize() < GetMaxPlayers();
}

void Battleground::UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor)
{
    BattlegroundScoreMap::const_iterator itr = PlayerScores.find(Source->GetGUID());
    if (itr == PlayerScores.end())
        return;

    switch (type)
    {
        case SCORE_KILLING_BLOWS:
            itr->second->KillingBlows += value;
            break;
        case SCORE_DEATHS:
            itr->second->Deaths += value;
            break;
        case SCORE_HONORABLE_KILLS:
            itr->second->HonorableKills += value;
            break;
        case SCORE_BONUS_HONOR:
            if (IsBattleground())
            {
                if (doAddHonor)
                    Source->RewardHonor(NULL, 1, value);
                else
                    itr->second->BonusHonor += value;
            }
            break;
        case SCORE_DAMAGE_DONE:
            itr->second->DamageDone += value;
            break;
        case SCORE_HEALING_DONE:
            itr->second->HealingDone += value;
            break;
        case SCORE_PRE_MATCH_PERSONAL_RATING:
            itr->second->PreMatchPersonalRating = value;
            break;
        case SCORE_PRE_MATCH_MMR:
            itr->second->PreMatchMatchmakerRating = value;
            break;
        case SCORE_PERSONAL_RATING_CHANGE:
            itr->second->PersonalRatingChange = value;
            break;
        case SCORE_MMR_CHANGE:
            itr->second->MatchmakerRatingChange = value;
            break;
        default:
            TC_LOG_ERROR("bg.battleground", "Battleground::UpdatePlayerScore: unknown score type (%u) for BG (map: %u, instance id: %u)!", type, GetMapId(), GetInstanceID());
            break;
    }
}

void Battleground::AddPlayerToResurrectQueue(uint64 npc_guid, uint64 player_guid)
{
    m_ReviveQueue[npc_guid].push_back(player_guid);

    Player* player = ObjectAccessor::FindPlayer(player_guid);
    if (!player)
        return;

    player->CastSpell(player, SPELL_WAITING_FOR_RESURRECT, true);
}

void Battleground::RemovePlayerFromResurrectQueue(uint64 player_guid)
{
    for (std::map<uint64, std::vector<uint64> >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
    {
        for (std::vector<uint64>::iterator itr2 = (itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
        {
            if (*itr2 == player_guid)
            {
                (itr->second).erase(itr2);
                if (Player* player = ObjectAccessor::FindPlayer(player_guid))
                    player->RemoveAurasDueToSpell(SPELL_WAITING_FOR_RESURRECT);
                return;
            }
        }
    }
}

void Battleground::RelocateDeadPlayers(uint64 queueIndex)
{
    std::vector<uint64>& ghostList = m_ReviveQueue[queueIndex];
    if (!ghostList.empty())
    {
        WorldSafeLocsEntry const* closestGrave = NULL;
        for (std::vector<uint64>::const_iterator itr = ghostList.begin(); itr != ghostList.end(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(*itr);
            if (!player)
                continue;

            if (!closestGrave)
                closestGrave = GetClosestGraveYard(player);

            if (closestGrave)
                player->TeleportTo(GetMapId(), closestGrave->x, closestGrave->y, closestGrave->z, player->GetOrientation());
        }
        ghostList.clear();
    }
}

bool Battleground::AddObject(uint32 type, uint32 entry, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3, uint32 /*respawnTime*/)
{
    ASSERT(type < BgObjects.size());

    Map* map = FindBgMap();
    if (!map)
        return false;

    GameObject* go = new GameObject;
    if (!go->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT), entry, GetBgMap(), PHASEMASK_NORMAL, x, y, z, o, rotation0, rotation1, rotation2, rotation3, 100, GO_STATE_READY))
    {
        TC_LOG_ERROR("sql.sql", "Battleground::AddObject: cannot create gameobject (entry: %u) for BG (map: %u, instance id: %u)!", entry, GetMapId(), GetInstanceID());
        TC_LOG_ERROR("bg.battleground", "Battleground::AddObject: cannot create gameobject (entry: %u) for BG (map: %u, instance id: %u)!", entry, GetMapId(), GetInstanceID());
        delete go;
        return false;
    }

    if (!map->AddToMap(go))
    {
        delete go;
        return false;
    }
    BgObjects[type] = go->GetGUID();
    return true;
}

bool Battleground::AddTransport(uint32 type, uint32 entry, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3, uint32 /*respawnTime*/)
{
    ASSERT(type < BgObjects.size());

    Map* map = FindBgMap();
    if (!map)
        return false;

    GameObject* go = new GameObject;
    if (!go->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_MO_TRANSPORT), entry, GetBgMap(), PHASEMASK_NORMAL, x, y, z, o, rotation0, rotation1, rotation2, rotation3, 0, GO_STATE_PREPARE_TRANSPORT))
    {
        TC_LOG_ERROR("sql.sql", "Battleground::AddObject: cannot create transport (entry: %u) for BG (map: %u, instance id: %u)!", entry, GetMapId(), GetInstanceID());
        TC_LOG_ERROR("bg.battleground", "Battleground::AddObject: cannot create transport (entry: %u) for BG (map: %u, instance id: %u)!", entry, GetMapId(), GetInstanceID());
        delete go;
        return false;
    }
    if (!map->AddToMap(go))
    {
        delete go;
        return false;
    }
    BgObjects[type] = go->GetGUID();
    return true;
}

void Battleground::DoorClose(uint32 type)
{
    if (GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]))
    {
        if (obj->getLootState() == GO_ACTIVATED && obj->GetGoState() != GO_STATE_READY)
        {
            obj->SetLootState(GO_READY);
            obj->SetGoState(GO_STATE_READY);
        }
    }
    else
        TC_LOG_ERROR("bg.battleground", "Battleground::DoorClose: door gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!", type, GUID_LOPART(BgObjects[type]), GetMapId(), GetInstanceID());
}

void Battleground::DoorOpen(uint32 type)
{
    if (GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]))
    {
        obj->SetLootState(GO_ACTIVATED);
        obj->SetGoState(GO_STATE_ACTIVE);
    }
    else
        TC_LOG_ERROR("bg.battleground", "Battleground::DoorOpen: door gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!", type, GUID_LOPART(BgObjects[type]), GetMapId(), GetInstanceID());
}

GameObject* Battleground::GetBGObject(uint32 type)
{
    GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]);
    if (!obj)
        TC_LOG_ERROR("bg.battleground", "Battleground::GetBGObject: gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!", type, GUID_LOPART(BgObjects[type]), GetMapId(), GetInstanceID());
    return obj;
}

Creature* Battleground::GetBGCreature(uint32 type)
{
    Creature* creature = GetBgMap()->GetCreature(BgCreatures[type]);
    if (!creature)
        TC_LOG_ERROR("bg.battleground", "Battleground::GetBGCreature: creature (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
            type, GUID_LOPART(BgCreatures[type]), GetMapId(), GetInstanceID());
    return creature;
}

void Battleground::SpawnBGObject(uint32 type, uint32 respawntime)
{
    if (Map* map = FindBgMap())
        if (GameObject* obj = map->GetGameObject(BgObjects[type]))
        {
            if (respawntime)
                obj->SetLootState(GO_JUST_DEACTIVATED);
            else
                if (obj->getLootState() == GO_JUST_DEACTIVATED)
                    obj->SetLootState(GO_READY);
            obj->SetRespawnTime(respawntime);
            map->AddToMap(obj);
        }
}

Creature* Battleground::AddCreature(uint32 entry, uint32 type, uint32 teamval, float x, float y, float z, float o, uint32 respawntime)
{
    ASSERT(type < BgCreatures.size());

    Map* map = FindBgMap();
    if (!map)
        return NULL;

    Creature* creature = new Creature;
    if (!creature->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_UNIT), map, PHASEMASK_NORMAL, entry, 0, teamval, x, y, z, o))
    {
        TC_LOG_ERROR("bg.battleground", "Battleground::AddCreature: cannot create creature (entry: %u) for BG (map: %u, instance id: %u)!", entry, GetMapId(), GetInstanceID());
        delete creature;
        return NULL;
    }

    creature->SetHomePosition(x, y, z, o);

    CreatureTemplate const* cinfo = sObjectMgr->GetCreatureTemplate(entry);
    if (!cinfo)
    {
        TC_LOG_ERROR("bg.battleground", "Battleground::AddCreature: creature template (entry: %u) does not exist for BG (map: %u, instance id: %u)!", entry, GetMapId(), GetInstanceID());
        delete creature;
        return NULL;
    }
    creature->SetSpeed(MOVE_WALK, cinfo->speed_walk);
    creature->SetSpeed(MOVE_RUN, cinfo->speed_run);

    if (!map->AddToMap(creature))
    {
        delete creature;
        return NULL;
    }

    BgCreatures[type] = creature->GetGUID();

    if (respawntime)
        creature->SetRespawnDelay(respawntime);

    return  creature;
}

bool Battleground::DelCreature(uint32 type)
{
    if (!BgCreatures[type])
        return true;

    if (Creature* creature = GetBgMap()->GetCreature(BgCreatures[type]))
    {
        creature->AddObjectToRemoveList();
        BgCreatures[type] = 0;
        return true;
    }

    TC_LOG_ERROR("bg.battleground", "Battleground::DelCreature: creature (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!", type, GUID_LOPART(BgCreatures[type]), GetMapId(), GetInstanceID());
    BgCreatures[type] = 0;
    return false;
}

bool Battleground::DelObject(uint32 type)
{
    if (!BgObjects[type])
        return true;

    if (GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]))
    {
        obj->SetRespawnTime(0);
        obj->Delete();
        BgObjects[type] = 0;
        return true;
    }
    TC_LOG_ERROR("bg.battleground", "Battleground::DelObject: gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!", type, GUID_LOPART(BgObjects[type]), GetMapId(), GetInstanceID());
    BgObjects[type] = 0;
    return false;
}

bool Battleground::AddSpiritGuide(uint32 type, float x, float y, float z, float o, uint32 team)
{
    uint32 entry = (team == ALLIANCE) ? BG_CREATURE_ENTRY_A_SPIRITGUIDE : BG_CREATURE_ENTRY_H_SPIRITGUIDE;

    if (Creature* creature = AddCreature(entry, type, team, x, y, z, o))
    {
        creature->setDeathState(DEAD);
        creature->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, creature->GetGUID());
        creature->SetUInt32Value(UNIT_FIELD_CHANNEL_SPELL, SPELL_SPIRIT_HEAL_CHANNEL);
        creature->SetFloatValue(UNIT_FIELD_MOD_CASTING_SPEED, 1.0f);
        creature->SetFloatValue(UNIT_FIELD_MOD_SPELL_HASTE, 1.0f);
        return true;
    }
    TC_LOG_ERROR("bg.battleground", "Battleground::AddSpiritGuide: cannot create spirit guide (type: %u, entry: %u) for BG (map: %u, instance id: %u)!", type, entry, GetMapId(), GetInstanceID());
    EndNow();
    return false;
}

void Battleground::SendMessageToAll(int32 entry, ChatMsg type, Player const* source)
{
    if (!entry)
        return;

    Trinity::BattlegroundChatBuilder bg_builder(type, entry, source);
    Trinity::LocalizedPacketDo<Trinity::BattlegroundChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);
}

void Battleground::PSendMessageToAll(int32 entry, ChatMsg type, Player const* source, ...)
{
    if (!entry)
        return;

    va_list ap;
    va_start(ap, source);

    Trinity::BattlegroundChatBuilder bg_builder(type, entry, source, &ap);
    Trinity::LocalizedPacketDo<Trinity::BattlegroundChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);

    va_end(ap);
}

void Battleground::SendWarningToAll(int32 entry, ...)
{
    if (!entry)
        return;

    std::map<uint32, WorldPacket> localizedPackets;

    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(itr->first, 0, HIGHGUID_PLAYER)))
        {
            if (localizedPackets.find(player->GetSession()->GetSessionDbLocaleIndex()) == localizedPackets.end())
            {
                char const* format = sObjectMgr->GetTrinityString(entry, player->GetSession()->GetSessionDbLocaleIndex());

                char str[1024];
                va_list ap;
                va_start(ap, entry);
                vsnprintf(str, 1024, format, ap);
                va_end(ap);

                ChatHandler::BuildChatPacket(localizedPackets[player->GetSession()->GetSessionDbLocaleIndex()], CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL, NULL, NULL, str);
            }
            player->SendDirectMessage(&localizedPackets[player->GetSession()->GetSessionDbLocaleIndex()]);
        }
}

void Battleground::SendMessage2ToAll(int32 entry, ChatMsg type, Player const* source, int32 arg1, int32 arg2)
{
    Trinity::Battleground2ChatBuilder bg_builder(type, entry, source, arg1, arg2);
    Trinity::LocalizedPacketDo<Trinity::Battleground2ChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);
}

void Battleground::EndNow()
{
    RemoveFromBGFreeSlotQueue();
    SetStatus(STATUS_WAIT_LEAVE);
    SetRemainingTime(0);
}

// To be removed
char const* Battleground::GetTrinityString(int32 entry)
{
    return sObjectMgr->GetTrinityStringForDBCLocale(entry);
}

void Battleground::HandleTriggerBuff(uint64 go_guid)
{
    GameObject* obj = GetBgMap()->GetGameObject(go_guid);
    if (!obj || obj->GetGoType() != GAMEOBJECT_TYPE_TRAP || !obj->isSpawned())
        return;

    int32 index = BgObjects.size() - 1;
    while (index >= 0 && BgObjects[index] != go_guid)
        index--;
    if (index < 0)
    {
        TC_LOG_ERROR("bg.battleground", "Battleground::HandleTriggerBuff: cannot find buff gameobject (GUID: %u, entry: %u, type: %u) in internal data for BG (map: %u, instance id: %u)!",
            GUID_LOPART(go_guid), obj->GetEntry(), obj->GetGoType(), GetMapId(), GetInstanceID());
        return;
    }

    uint8 buff = urand(0, 2);
    uint32 entry = obj->GetEntry();
    if (m_BuffChange && entry != Buff_Entries[buff])
    {
        SpawnBGObject(index, RESPAWN_ONE_DAY);
        for (uint8 currBuffTypeIndex = 0; currBuffTypeIndex < 3; ++currBuffTypeIndex)
            if (entry == Buff_Entries[currBuffTypeIndex])
            {
                index -= currBuffTypeIndex;
                index += buff;
            }
    }

    SpawnBGObject(index, BUFF_RESPAWN_TIME);
}

void Battleground::HandleKillPlayer(Player* victim, Player* killer)
{
    UpdatePlayerScore(victim, SCORE_DEATHS, 1);
    if (killer)
    {
        if (killer == victim)
            return;

        UpdatePlayerScore(killer, SCORE_HONORABLE_KILLS, 1);
        UpdatePlayerScore(killer, SCORE_KILLING_BLOWS, 1);

        for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        {
            Player* creditedPlayer = ObjectAccessor::FindPlayer(itr->first);
            if (!creditedPlayer || creditedPlayer == killer)
                continue;

            if (creditedPlayer->GetTeam() == killer->GetTeam() && creditedPlayer->IsAtGroupRewardDistance(victim))
                UpdatePlayerScore(creditedPlayer, SCORE_HONORABLE_KILLS, 1);
        }
    }

    if (!IsArena())
    {
        victim->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
        RewardXPAtKill(killer, victim);
    }
}

uint32 Battleground::GetPlayerTeam(uint64 guid) const
{
    BattlegroundPlayerMap::const_iterator itr = m_Players.find(guid);
    if (itr != m_Players.end())
        return itr->second.Team;
    return 0;
}

uint32 Battleground::GetOtherTeam(uint32 teamId) const
{
    return teamId ? ((teamId == ALLIANCE) ? HORDE : ALLIANCE) : 0;
}

bool Battleground::IsPlayerInBattleground(uint64 guid) const
{
    BattlegroundPlayerMap::const_iterator itr = m_Players.find(guid);
    if (itr != m_Players.end())
        return true;
    return false;
}

void Battleground::PlayerAddedToBGCheckIfBGIsRunning(Player* player)
{
    if (GetStatus() != STATUS_WAIT_LEAVE)
        return;

    WorldPacket data;
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(GetTypeID(), GetRatedType());

    BlockMovement(player);

    sBattlegroundMgr->BuildPvpLogDataPacket(&data, this);
    player->SendDirectMessage(&data);

    sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, this, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), STATUS_IN_PROGRESS, player->GetBattlegroundQueueJoinTime(GetTypeID()), GetElapsedTime(), GetRatedType());
    player->SendDirectMessage(&data);
}

uint32 Battleground::GetAlivePlayersCountByTeam(uint32 Team) const
{
    int count = 0;
    for (auto players : m_Players)
    {
        if (players.second.Team == Team)
        {
            Player* player = ObjectAccessor::FindPlayer(players.first);
            if (player && player->IsAlive() && !player->HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 3, FORM_SPIRITOFREDEMPTION))
                ++count;
        }
    }
    return count;
}

void Battleground::SetHoliday(bool is_holiday)
{
    m_HonorMode = is_holiday ? BG_HOLIDAY : BG_NORMAL;
}

int32 Battleground::GetObjectType(uint64 guid)
{
    for (uint32 i = 0; i < BgObjects.size(); ++i)
        if (BgObjects[i] == guid)
            return i;

    TC_LOG_ERROR("bg.battleground", "Battleground::GetObjectType: player used gameobject (GUID: %u) which is not in internal data for BG (map: %u, instance id: %u), cheating?",
        GUID_LOPART(guid), GetMapId(), GetInstanceID());

    return -1;
}

void Battleground::HandleKillUnit(Creature* /*victim*/, Player* /*killer*/) { }

void Battleground::CheckArenaAfterTimerConditions()
{
    EndBattleground(WINNER_NONE);
}

void Battleground::CheckArenaWinConditions()
{
    if (!GetAlivePlayersCountByTeam(ALLIANCE) && GetPlayersCountByTeam(HORDE))
        EndBattleground(HORDE);
    else if (GetPlayersCountByTeam(ALLIANCE) && !GetAlivePlayersCountByTeam(HORDE))
        EndBattleground(ALLIANCE);
}

void Battleground::UpdateArenaWorldState()
{
    UpdateWorldState(0xe10, GetAlivePlayersCountByTeam(HORDE));
    UpdateWorldState(0xe11, GetAlivePlayersCountByTeam(ALLIANCE));
}

void Battleground::SetBgRaid(uint32 TeamID, Group* bg_raid)
{
    Group*& old_raid = TeamID == ALLIANCE ? m_BgRaids[TEAM_ALLIANCE] : m_BgRaids[TEAM_HORDE];
    if (old_raid)
        old_raid->SetBattlegroundGroup(NULL);
    if (bg_raid)
        bg_raid->SetBattlegroundGroup(this);
    old_raid = bg_raid;
}

WorldSafeLocsEntry const* Battleground::GetClosestGraveYard(Player* player)
{
    return sObjectMgr->GetClosestGraveYard(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetMapId(), player->GetTeam());
}

void Battleground::StartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            player->StartTimedAchievement(type, entry);
}

void Battleground::SetBracket(PvPDifficultyEntry const* bracketEntry)
{
    m_BracketId = bracketEntry->GetBracketId();
    SetLevelRange(bracketEntry->minLevel, bracketEntry->maxLevel);
}

void Battleground::RewardXPAtKill(Player* killer, Player* victim)
{
    if (sWorld->getBoolConfig(CONFIG_BG_XP_FOR_KILL) && killer && victim)
        killer->RewardPlayerAndGroupAtKill(victim, true);
}

uint32 Battleground::GetTeamScore(uint32 teamId) const
{
    if (teamId == TEAM_ALLIANCE || teamId == TEAM_HORDE)
        return m_TeamScores[teamId];

    TC_LOG_ERROR("bg.battleground", "GetTeamScore with wrong Team %u for BG %u", teamId, GetTypeID());
    return 0;
}

void Battleground::HandleAreaTrigger(Player* player, uint32 trigger)
{
    TC_LOG_DEBUG("bg.battleground", "Unhandled AreaTrigger %u in Battleground %u. Player coords (x: %f, y: %f, z: %f)", trigger, player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());
}

bool Battleground::CheckAchievementCriteriaMeet(uint32 criteriaId, Player const* /*source*/, Unit const* /*target*/, uint32 /*miscvalue1*/)
{
    TC_LOG_ERROR("bg.battleground", "Battleground::CheckAchievementCriteriaMeet: No implementation for criteria %u", criteriaId);
    return false;
}
