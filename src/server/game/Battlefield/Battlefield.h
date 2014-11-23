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

#ifndef BATTLEFIELD_H_
#define BATTLEFIELD_H_

#include "Utilities/Util.h"
#include "SharedDefines.h"
#include "ZoneScript.h"
#include "WorldPacket.h"
#include "GameObject.h"
#include "Battleground.h"
#include "ObjectAccessor.h"
#include "WorldStateBuilder.h"

enum BattlefieldTypes
{
    BATTLEFIELD_WG,                                         // Wintergrasp
    BATTLEFIELD_TB                                          // Tol Barad (cataclysm)
};

enum BattlefieldIDs
{
    BATTLEFIELD_BATTLEID_WG                      = 1        // Wintergrasp battle
};

enum BattlefieldObjectiveStates
{
    BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL = 0,
    BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE,
    BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE,
    BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE,
    BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE,
    BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE,
    BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE
};

enum BattlefieldSounds
{
    BF_HORDE_WINS                                = 8454,
    BF_ALLIANCE_WINS                             = 8455,
    BF_START                                     = 3439
};

enum BattlefieldTimers
{
    BATTLEFIELD_OBJECTIVE_UPDATE_INTERVAL        = 1000
};

// some class predefs
class Player;
class GameObject;
class WorldPacket;
class Creature;
class Unit;

class Battlefield;
class BfGraveyard;

typedef std::set<uint64> GuidSet;
typedef std::vector<BfGraveyard*> GraveyardVect;
typedef std::map<uint64, time_t> PlayerTimerMap;

class BfCapturePoint
{
public:
    BfCapturePoint(Battlefield* bf);

    virtual ~BfCapturePoint() { }

    virtual void FillInitialWorldStates(WorldStateBuilder& /*builder*/) { }
    void SendUpdateWorldState(uint32 field, uint32 value);
    void SendObjectiveComplete(uint32 id, uint64 guid);
    virtual bool HandlePlayerEnter(Player* player);
    virtual GuidSet::iterator HandlePlayerLeave(Player* player);
    //virtual void HandlePlayerActivityChanged(Player* player);

    bool IsInsideObjective(Player* player) const;

    virtual bool Update(uint32 diff);
    virtual void ChangeTeam(TeamId /*oldTeam*/) { }
    virtual void SendChangePhase();

    bool SetCapturePointData(GameObject* capturePoint);
    GameObject* GetCapturePointGo();
    uint32 GetCapturePointEntry() { return m_capturePointEntry; }

    TeamId GetTeamId() { return m_team; }
protected:
    bool DelCapturePoint();
    GuidSet m_activePlayers[2];
    float m_maxValue;
    float m_minValue;
    float m_maxSpeed;
    float m_value;
    TeamId m_team;
    BattlefieldObjectiveStates m_OldState;
    BattlefieldObjectiveStates m_State;
    uint32 m_neutralValuePct;
    Battlefield* m_Bf;
    uint32 m_capturePointEntry;
    uint64 m_capturePointGUID;
};

class BfGraveyard
{
public:
    BfGraveyard(Battlefield* Bf);

    void GiveControlTo(TeamId team);
    TeamId GetControlTeamId() const { return m_ControlTeam; }

    float GetDistance(Player* player);

    void Initialize(TeamId startcontrol, uint32 gy);

    void SetSpirit(Creature* spirit, TeamId team);

    void AddPlayer(uint64 player_guid);
    void RemovePlayer(uint64 player_guid);
    void Resurrect();
    void RelocateDeadPlayers();

    bool HasNpc(uint64 guid);

    bool HasPlayer(uint64 guid) { return m_ResurrectQueue.find(guid) != m_ResurrectQueue.end(); }

    uint32 GetGraveyardId() const { return m_GraveyardId; }

protected:
    TeamId m_ControlTeam;
    uint32 m_GraveyardId;
    uint64 m_SpiritGuide[2];
    GuidSet m_ResurrectQueue;
    Battlefield* m_Bf;
};

class Battlefield : public ZoneScript
{
    friend class BattlefieldMgr;

public:
    Battlefield();
    virtual ~Battlefield();

    typedef std::map<uint32 /*lowguid */, BfCapturePoint*> BfCapturePointMap;

    virtual bool SetupBattlefield() { return true; }

    void SendUpdateWorldState(uint32 field, uint32 value);

    virtual bool Update(uint32 diff);

    void InvitePlayersInZoneToQueue();
    void InvitePlayersInQueueToWar();
    void InvitePlayersInZoneToWar();

    virtual void HandleKill(Player* /*killer*/, Unit* /*killed*/) { };

    uint32 GetTypeId() { return m_TypeId; }
    uint32 GetZoneId() { return m_ZoneId; }
    uint64 GetGUID() { return m_Guid; }

    void TeamApplyBuff(TeamId team, uint32 spellId, uint32 spellId2 = 0);

    bool IsWarTime() { return m_isActive; }

    void ToggleBattlefield(bool enable) { m_IsEnabled = enable; }
    bool IsEnabled() { return m_IsEnabled; }

    void KickPlayerFromBattlefield(uint64 guid);

    void HandlePlayerEnterZone(Player* player, uint32 zone);
    void HandlePlayerLeaveZone(Player* player, uint32 zone);

    virtual uint64 GetData64(uint32 dataId) const { return m_Data64[dataId]; }
    virtual void SetData64(uint32 dataId, uint64 value) { m_Data64[dataId] = value; }

    virtual uint32 GetData(uint32 dataId) const { return m_Data32[dataId]; }
    virtual void SetData(uint32 dataId, uint32 value) { m_Data32[dataId] = value; }
    virtual void UpdateData(uint32 index, int32 pad) { m_Data32[index] += pad; }

    TeamId GetDefenderTeam() { return m_DefenderTeam; }
    TeamId GetAttackerTeam() { return TeamId(1 - m_DefenderTeam); }
    TeamId GetOtherTeam(TeamId team) { return (team == TEAM_HORDE ? TEAM_ALLIANCE : TEAM_HORDE); }
    void SetDefenderTeam(TeamId team) { m_DefenderTeam = team; }

    Group* GetFreeBfRaid(TeamId TeamId);
    Group* GetGroupPlayer(uint64 guid, TeamId TeamId);
    bool AddOrSetPlayerToCorrectBfGroup(Player* player);

    WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

    virtual void AddPlayerToResurrectQueue(uint64 npc_guid, uint64 player_guid);
    void RemovePlayerFromResurrectQueue(uint64 player_guid);
    void SetGraveyardNumber(uint32 number) { m_GraveyardList.resize(number); }
    BfGraveyard* GetGraveyardById(uint32 id) const;

    Creature* SpawnCreature(uint32 entry, float x, float y, float z, float o, TeamId team);
    Creature* SpawnCreature(uint32 entry, const Position& pos, TeamId team);
    GameObject* SpawnGameObject(uint32 entry, float x, float y, float z, float o);

    Creature* GetCreature(uint64 GUID);
    GameObject* GetGameObject(uint64 GUID);


    virtual void OnBattleStart() { }
    virtual void OnBattleEnd(bool /*endByTimer*/) { }
    virtual void OnStartGrouping() { }
    virtual void OnPlayerJoinWar(Player* /*player*/) { }
    virtual void OnPlayerLeaveWar(Player* /*player*/) { }
    virtual void OnPlayerLeaveZone(Player* /*player*/) { }
    virtual void OnPlayerEnterZone(Player* /*player*/) { }

    WorldPacket BuildWarningAnnPacket(std::string const& msg);
    void SendWarningToAllInZone(uint32 entry);
    void SendWarningToPlayer(Player* player, uint32 entry);

    void PlayerAcceptInviteToQueue(Player* player);
    void PlayerAcceptInviteToWar(Player* player);
    uint32 GetBattleId() { return m_BattleId; }
    void AskToLeaveQueue(Player* player);

    virtual void DoCompleteOrIncrementAchievement(uint32 /*achievement*/, Player* /*player*/, uint8 /*incrementNumber = 1*/) { }

    virtual void SendInitWorldStatesToAll() = 0;
    virtual void FillInitialWorldStates(WorldStateBuilder& /*builder*/) = 0;

    bool CanFlyIn() { return !m_isActive; }

    void SendAreaSpiritHealerQueryOpcode(Player* player, ObjectGuid HealerGuid);

    void StartBattle();
    void EndBattle(bool endByTimer);

    void HideNpc(Creature* creature);
    void ShowNpc(Creature* creature, bool aggressive);

    GraveyardVect GetGraveyardVector() { return m_GraveyardList; }

    uint32 GetTimer() { return m_Timer; }
    void SetTimer(uint32 timer) { m_Timer = timer; }

    void DoPlaySoundToAll(uint32 SoundID);

    void InvitePlayerToQueue(Player* player);
    void InvitePlayerToWar(Player* player);

    void InitStalker(uint32 entry, float x, float y, float z, float o);

protected:
    uint64 m_Guid;

    uint64 StalkerGuid;
    uint32 m_Timer;
    bool m_IsEnabled;
    bool m_isActive;
    TeamId m_DefenderTeam;

    BfCapturePointMap m_capturePoints;

    GuidSet m_players[BG_TEAMS_COUNT];
    GuidSet m_PlayersInQueue[BG_TEAMS_COUNT];
    GuidSet m_PlayersInWar[BG_TEAMS_COUNT];
    PlayerTimerMap m_InvitedPlayers[BG_TEAMS_COUNT];
    PlayerTimerMap m_PlayersWillBeKick[BG_TEAMS_COUNT];

    uint32 m_TypeId;                                        // See enum BattlefieldTypes
    uint32 m_BattleId;                                      // BattleID (for packet)
    uint32 m_ZoneId;                                        // ZoneID of Wintergrasp = 4197
    uint32 m_MapId;                                         // MapId where is Battlefield
    Map* m_Map;
    uint32 m_MaxPlayer;                                     // Maximum number of player that participated to Battlefield
    uint32 m_MinPlayer;                                     // Minimum number of player for Battlefield start
    uint32 m_MinLevel;                                      // Required level to participate at Battlefield
    uint32 m_BattleTime;                                    // Length of a battle
    uint32 m_NoWarBattleTime;                               // Time between two battles
    uint32 m_RestartAfterCrash;                             // Delay to restart Wintergrasp if the server crashed during a running battle.
    uint32 m_TimeForAcceptInvite;
    uint32 m_uiKickDontAcceptTimer;
    WorldLocation KickPosition;                             // Position where players are teleported if they switch to afk during the battle or if they don't accept invitation

    uint32 m_uiKickAfkPlayersTimer;                         // Timer for check Afk in war

    GraveyardVect m_GraveyardList;                          // Vector witch contain the different GY of the battle
    uint32 m_LastResurectTimer;                             // Timer for resurect player every 30 sec

    uint32 m_StartGroupingTimer;                            // Timer for invite players in area 15 minute before start battle
    bool m_StartGrouping;                                   // bool for know if all players in area has been invited

    GuidSet m_Groups[BG_TEAMS_COUNT];                       // Contain different raid group

    std::vector<uint64> m_Data64;
    std::vector<uint32> m_Data32;

    void KickAfkPlayers();

    virtual void SendRemoveWorldStates(Player* /*player*/) { }

    void BroadcastPacketToZone(WorldPacket& data) const;
    void BroadcastPacketToQueue(WorldPacket& data) const;
    void BroadcastPacketToWar(WorldPacket& data) const;

    void AddCapturePoint(BfCapturePoint* cp) { m_capturePoints[cp->GetCapturePointEntry()] = cp; }

    BfCapturePoint* GetCapturePoint(uint32 lowguid) const
    {
        Battlefield::BfCapturePointMap::const_iterator itr = m_capturePoints.find(lowguid);
        if (itr != m_capturePoints.end())
            return itr->second;
        return NULL;
    }

    void RegisterZone(uint32 zoneid);
    bool HasPlayer(Player* player) const;
    void TeamCastSpell(TeamId team, int32 spellId);
};

#endif
