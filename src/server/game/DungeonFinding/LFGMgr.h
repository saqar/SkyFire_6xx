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

#ifndef _LFGMGR_H
#define _LFGMGR_H

#include <ace/Singleton.h>
#include "DBCStructure.h"
#include "Field.h"
#include "LFG.h"
#include "LFGQueue.h"
#include "LFGGroupData.h"
#include "LFGPlayerData.h"

class Group;
class Player;
class Quest;

namespace lfg
{

enum LfgOptions
{
    LFG_OPTION_ENABLE_DUNGEON_FINDER             = 0x01,
    LFG_OPTION_ENABLE_RAID_BROWSER               = 0x02,
};

enum LFGMgrEnum
{
    LFG_TIME_ROLECHECK                           = 45 * IN_MILLISECONDS,
    LFG_TIME_BOOT                                = 120,
    LFG_TIME_PROPOSAL                            = 45,
    LFG_QUEUEUPDATE_INTERVAL                     = 15 * IN_MILLISECONDS,
    LFG_SPELL_DUNGEON_COOLDOWN                   = 71328,
    LFG_SPELL_DUNGEON_DESERTER                   = 71041,
    LFG_SPELL_LUCK_OF_THE_DRAW                   = 72221,
    LFG_GROUP_KICK_VOTES_NEEDED                  = 3
};

enum LfgFlags
{
    LFG_FLAG_UNK1                                = 0x1,
    LFG_FLAG_UNK2                                = 0x2,
    LFG_FLAG_SEASONAL                            = 0x4,
    LFG_FLAG_UNK3                                = 0x8
};

/// Determines the type of instance
enum LfgType
{
    LFG_TYPE_NONE                                = 0,
    LFG_TYPE_DUNGEON                             = 1,
    LFG_TYPE_RAID                                = 2,
    LFG_TYPE_HEROIC                              = 5,
    LFG_TYPE_RANDOM                              = 6
};

/// Proposal states
enum LfgProposalState
{
    LFG_PROPOSAL_INITIATING                      = 0,
    LFG_PROPOSAL_FAILED                          = 1,
    LFG_PROPOSAL_SUCCESS                         = 2
};

/// Teleport errors
enum LfgTeleportError
{
    // 7 = "You can't do that right now" | 5 = No client reaction
    LFG_TELEPORTERROR_OK                         = 0,      // Internal use
    LFG_TELEPORTERROR_PLAYER_DEAD                = 1,
    LFG_TELEPORTERROR_FALLING                    = 2,
    LFG_TELEPORTERROR_IN_VEHICLE                 = 3,
    LFG_TELEPORTERROR_FATIGUE                    = 4,
    LFG_TELEPORTERROR_INVALID_LOCATION           = 6,
    LFG_TELEPORTERROR_CHARMING                   = 8       // FIXME - It can be 7 or 8 (Need proper data)
};

/// Queue join results
enum LfgJoinResult
{
    // 3 = No client reaction | 18 = "Rolecheck failed"
    LFG_JOIN_OK                                  = 0x00,   // Joined (no client msg)
    LFG_JOIN_FAILED                              = 0x1B,   // RoleCheck Failed
    LFG_JOIN_GROUPFULL                           = 0x1C,   // Your group is full
    LFG_JOIN_INTERNAL_ERROR                      = 0x1E,   // Internal LFG Error
    LFG_JOIN_NOT_MEET_REQS                       = 0x1F,   // You do not meet the requirements for the chosen dungeons
    //LFG_JOIN_PARTY_NOT_MEET_REQS                 = 6,      // One or more party members do not meet the requirements for the chosen dungeons
    LFG_JOIN_MIXED_RAID_DUNGEON                  = 0x20,   // You cannot mix dungeons, raids, and random when picking dungeons
    LFG_JOIN_MULTI_REALM                         = 0x21,   // The dungeon you chose does not support players from multiple realms
    LFG_JOIN_DISCONNECTED                        = 0x22,   // One or more party members are pending invites or disconnected
    LFG_JOIN_PARTY_INFO_FAILED                   = 0x23,   // Could not retrieve information about some party members
    LFG_JOIN_DUNGEON_INVALID                     = 0x24,   // One or more dungeons was not valid
    LFG_JOIN_DESERTER                            = 0x25,   // You can not queue for dungeons until your deserter debuff wears off
    LFG_JOIN_PARTY_DESERTER                      = 0x26,   // One or more party members has a deserter debuff
    LFG_JOIN_RANDOM_COOLDOWN                     = 0x27,   // You can not queue for random dungeons while on random dungeon cooldown
    LFG_JOIN_PARTY_RANDOM_COOLDOWN               = 0x28,   // One or more party members are on random dungeon cooldown
    LFG_JOIN_TOO_MUCH_MEMBERS                    = 0x29,   // You can not enter dungeons with more that 5 party members
    LFG_JOIN_USING_BG_SYSTEM                     = 0x2A,   // You can not use the dungeon system while in BG or arenas
    LFG_JOIN_ROLE_CHECK_FAILED                   = 0x2B    // Role check failed, client shows special error
};

/// Role check states
enum LfgRoleCheckState
{
    LFG_ROLECHECK_DEFAULT                        = 0,      // Internal use = Not initialized.
    LFG_ROLECHECK_FINISHED                       = 1,      // Role check finished
    LFG_ROLECHECK_INITIALITING                   = 2,      // Role check begins
    LFG_ROLECHECK_MISSING_ROLE                   = 3,      // Someone didn't selected a role after 2 mins
    LFG_ROLECHECK_WRONG_ROLES                    = 4,      // Can't form a group with that role selection
    LFG_ROLECHECK_ABORTED                        = 5,      // Someone leave the group
    LFG_ROLECHECK_NO_ROLE                        = 6       // Someone selected no role
};

// Forward declaration (just to have all typedef together)
struct LFGDungeonData;
struct LfgReward;
struct LfgQueueInfo;
struct LfgRoleCheck;
struct LfgProposal;
struct LfgProposalPlayer;
struct LfgPlayerBoot;
struct LfgPlayerInfo;
struct LfgPlayerDungeonInfo;

typedef std::map<uint8, LFGQueue> LfgQueueContainer;
typedef std::multimap<uint32, LfgReward const*> LfgRewardContainer;
typedef std::pair<LfgRewardContainer::const_iterator, LfgRewardContainer::const_iterator> LfgRewardContainerBounds;
typedef std::map<uint8, LfgDungeonSet> LfgCachedDungeonContainer;
typedef std::map<uint64, LfgAnswer> LfgAnswerContainer;
typedef std::map<uint64, LfgRoleCheck> LfgRoleCheckContainer;
typedef std::map<uint32, LfgProposal> LfgProposalContainer;
typedef std::map<uint64, LfgProposalPlayer> LfgProposalPlayerContainer;
typedef std::map<uint64, LfgPlayerBoot> LfgPlayerBootContainer;
typedef std::map<uint64, LfgGroupData> LfgGroupDataContainer;
typedef std::map<uint64, LfgPlayerData> LfgPlayerDataContainer;
typedef UNORDERED_MAP<uint32, LFGDungeonData> LFGDungeonContainer;
typedef std::map<uint64, LfgPlayerInfo> LfgPlayerInfoSet;
typedef std::map<uint64, LfgPlayerDungeonInfo> LfgPlayerDungeonInfoSet;

// Data needed by SMSG_LFG_JOIN_RESULT
struct LfgJoinResultData
{
    LfgJoinResultData(LfgJoinResult _result, LfgRoleCheckState _state, uint32 _slot, uint32 _reason, uint32 _sub1, uint32 _sub2)
        : result(_result), state(_state), Slot(_slot), Reason(_reason), SubReasonOne(_sub1), SubReasonTwo(_sub2) { }
    LfgJoinResult result;
    LfgRoleCheckState state;
    LfgLockPartyMap lockmap;
    uint32 Slot;
    uint32 Reason;
    uint32 SubReasonOne;
    uint32 SubReasonTwo;
};

// Data needed by SMSG_LFG_UPDATE_STATUS
struct LfgUpdateData
{
    LfgUpdateData(LfgUpdateType _type = LFG_UPDATETYPE_DEFAULT): updateType(_type), state(LFG_STATE_NONE), comment("") { }
    LfgUpdateData(LfgUpdateType _type, LfgDungeonSet const& _dungeons, std::string const& _comment):
        updateType(_type), state(LFG_STATE_NONE), dungeons(_dungeons), comment(_comment) { }
    LfgUpdateData(LfgUpdateType _type, LfgState _state, LfgDungeonSet const& _dungeons, std::string const& _comment = ""):
        updateType(_type), state(_state), dungeons(_dungeons), comment(_comment) { }

    LfgUpdateType updateType;
    LfgState state;
    LfgDungeonSet dungeons;
    std::string comment;
};

// Data needed by SMSG_LFG_QUEUE_STATUS
struct LfgQueueStatusData
{
    LfgQueueStatusData(uint8 _queueId = 0, uint32 _dungeonId = 0, time_t _joinTime = 0, int32 _waitTime = -1, int32 _waitTimeAvg = -1, int32 _waitTimeTank = -1, int32 _waitTimeHealer = -1,
        int32 _waitTimeDps = -1, uint32 _queuedTime = 0, uint8 _tanks = 0, uint8 _healers = 0, uint8 _dps = 0, int32 _waitTimeRole = -1) :
        queueId(_queueId), dungeonId(_dungeonId), joinTime(_joinTime), waitTime(_waitTime), waitTimeAvg(_waitTimeAvg), waitTimeTank(_waitTimeTank),
        waitTimeHealer(_waitTimeHealer), waitTimeDps(_waitTimeDps), queuedTime(_queuedTime), tanks(_tanks), healers(_healers), dps(_dps), waitTimeRole(_waitTimeRole) { }

    LfgQueueStatusData(uint8 _subType, uint8 _reason, uint8 _needs, uint32 _slots, uint32 _slotsCount, ObjectGuid _suspendedPlayers, uint32 _requestedRoles,
        bool _IsParty, bool _NotifyUi, bool _Joined, bool _LfgJoined, bool _Queued, std::string _comment, uint32 _suspednedPlayersCount)
        : subType(_subType), reason(_reason), needs(_needs), slots(_slots), slotsCount(_slotsCount), suspendedPlayers(_suspendedPlayers), requestedRoles(_requestedRoles),
        IsParty(_IsParty), Notify(_NotifyUi), Joined(_Joined), LfgJoined(_LfgJoined), Queued(_Queued), Comment(_comment), SuspendedPlayersCount(_suspednedPlayersCount) { }

    uint8 queueId;
    uint32 dungeonId;
    time_t joinTime;
    int32 waitTime;
    int32 waitTimeAvg;
    int32 waitTimeTank;
    int32 waitTimeHealer;
    int32 waitTimeDps;
    uint32 queuedTime;
    uint8 tanks;
    uint8 healers;
    uint8 dps;

    uint8 subType;
    uint8 reason;
    uint8 needs;
    uint32 slots;
    uint32 slotsCount;
    ObjectGuid suspendedPlayers;
    uint32 requestedRoles;
    bool IsParty;
    bool Notify;
    bool Joined;
    bool LfgJoined;
    bool Queued;
    std::string Comment;
    int32 waitTimeRole;
    uint32 SuspendedPlayersCount;

#define QUEUE_NEEDS 3
#define QUEUE_SLOTS 5
};

struct LfgPlayerRewardData
{
    LfgPlayerRewardData(uint32 random, uint32 current, bool _done, Quest const* _quest):
        rdungeonEntry(random), sdungeonEntry(current), done(_done), quest(_quest) { }
    uint32 rdungeonEntry;
    uint32 sdungeonEntry;
    bool done;
    Quest const* quest;
};

/// Reward info
struct LfgReward
{
    LfgReward(uint32 _maxLevel = 0, uint32 _firstQuest = 0, uint32 _otherQuest = 0):
        maxLevel(_maxLevel), firstQuest(_firstQuest), otherQuest(_otherQuest) { }

    uint32 maxLevel;
    uint32 firstQuest;
    uint32 otherQuest;
};

/// Stores player data related to proposal to join
struct LfgProposalPlayer
{
    LfgProposalPlayer(): Role(0), Accepted(0), SameParty(0), Me(0), MyParty(0), Responded(0), group(0) { }

    ObjectGuid group;
    uint32 Role;
    bool Me;
    bool SameParty;
    bool MyParty;
    bool Responded;
    bool Accepted;
};

/// Stores group data related to proposal to join
struct LfgProposal
{
    LfgProposal(uint32 dungeon = 0) : InstanceID(0), dungeonId(dungeon), state(LFG_PROPOSAL_INITIATING),
        group(0), leader(0), cancelTime(0), encounters(0), isNew(true), CompletedMask(0), Slot(0), ProposalID(0)
        { }

    uint32 InstanceID;                                     ///< Proposal Id
    uint32 CompletedMask;                                  ///< Completed Mask
    uint32 Slot;                                           ///< Slot
    uint32 ProposalID;                                     ///< ProposalID
    uint32 dungeonId;                                      ///< Dungeon to join
    LfgProposalState state;                                ///< State of the proposal
    uint64 group;                                          ///< Proposal group (0 if new)
    uint64 leader;                                         ///< Leader guid.
    time_t cancelTime;                                     ///< Time when we will cancel this proposal
    uint32 encounters;                                     ///< Dungeon Encounters
    bool isNew;                                            ///< Determines if it's new group or not
    LfgGuidList queues;                                    ///< Queue Ids to remove/readd
    LfgGuidList showorder;                                 ///< Show order in update window
    LfgProposalPlayerContainer players;                    ///< Players data
};

/// Stores all rolecheck info of a group that wants to join
struct LfgRoleCheck
{
    time_t cancelTime;                                     ///< Time when the rolecheck will fail
    LfgRolesMap roles;                                     ///< Player selected roles
    LfgRoleCheckState state;                               ///< State of the rolecheck
    LfgDungeonSet dungeons;                                ///< Dungeons group is applying for (expanded random dungeons)
    uint32 rDungeonId;                                     ///< Random Dungeon Id.
    uint64 leader;                                         ///< Leader of the group
};

/// Stores information of a current vote to kick someone from a group
struct LfgPlayerBoot
{
    time_t cancelTime;                                     ///< Time left to vote
    bool inProgress;                                       ///< Vote in progress
    bool VotePassed;                                       ///< Vote has passed
    bool MyVoteComplete;                                   ///< My vote has completed
    bool MyVote;                                           ///< My vote
    uint32 TotalVotes;                                     ///< Total amount of votes
    uint32 BootVotes;                                      ///< Number of votes in favor of boot
    uint32 TimeLeft;                                       ///< Amount of time left on boot proposal
    LfgAnswerContainer votes;                              ///< Player votes (-1 not answer | 0 Not agree | 1 agree)
    uint64 victim;                                         ///< Player guid to be kicked (can't vote)
    std::string reason;                                    ///< kick reason
};

struct LFGDungeonData
{
    LFGDungeonData(): id(0), name(""), map(0), type(0), expansion(0), group(0), minlevel(0),
        maxlevel(0), difficulty(REGULAR_DIFFICULTY), seasonal(false), x(0.0f), y(0.0f), z(0.0f), o(0.0f)
        { }
    LFGDungeonData(LFGDungeonEntry const* dbc): id(dbc->ID), name(dbc->name), map(dbc->map),
        type(dbc->type), expansion(dbc->expansion), group(dbc->grouptype),
        minlevel(dbc->minlevel), maxlevel(dbc->maxlevel), difficulty(Difficulty(dbc->difficulty)),
        seasonal(dbc->flags & LFG_FLAG_SEASONAL), x(0.0f), y(0.0f), z(0.0f), o(0.0f)
        { }

    uint32 id;
    std::string name;
    uint16 map;
    uint8 type;
    uint8 expansion;
    uint8 group;
    uint8 minlevel;
    uint8 maxlevel;
    Difficulty difficulty;
    bool seasonal;
    float x, y, z, o;

    // Helpers
    uint32 Entry() const { return id + (type << 24); }
};

struct LfgPlayerInfo
{
    LfgPlayerInfo() { }
public:

};

struct LfgPlayerDungeonInfo
{
    LfgPlayerDungeonInfo(uint32 _slot, uint32 _completQuant, uint32 _completLimit, uint32 _comepletCurrency, uint32 _specQuant, uint32 _specLimit, uint32 _overQuant,
        uint32 _overLimit, uint32 _weekQuant, uint32 _, uint32 _purseQuant, uint32 _purseLimit, uint32 _quantity, uint32 _completedMask, uint32 _purseWeeklyQuant,
        uint32 _shortageRewCount, uint32 _itemCount, uint32 _currencyCount, uint32 _bonusCurrCount, uint32 _mask, uint32 _rewMoney, uint32 _rewExp, uint32 _currId,
        uint32 _itemId, bool _FirstReward, bool _ShortageEligible)
    : Slot(_slot), CompletionQuantity(_completQuant), CompletionCurrencyID(_comepletCurrency), CompletionLimit(_completLimit), SpecificQuantity(_specQuant),
    SpecificLimit(_specLimit), OverallQuantity(_overQuant), OverallLimit(_overLimit), PurseWeeklyQuantity(_purseWeeklyQuant), PurseWeeklyLimit(_purseLimit),
    PurseQuantity(_purseQuant), Quantity(_quantity), CompletedMask(_completedMask), ShortageRewardCount(_shortageRewCount), itemCount(_itemCount),
    currencyCount(_currencyCount), bonusCurrencyCount(_bonusCurrCount), Mask(_mask), RewardMoney(_rewMoney), RewardExperience(_rewExp), CurrencyID(_currId),
    ItemID(_itemId), FirstReward(_FirstReward), ShortageEligible(_ShortageEligible) { }
public:
    uint32 Slot;
    uint32 CompletionQuantity;
    uint32 CompletionLimit;
    uint32 CompletionCurrencyID;
    uint32 SpecificQuantity;
    uint32 SpecificLimit;
    uint32 OverallQuantity;
    uint32 OverallLimit;
    uint32 PurseWeeklyQuantity;
    uint32 PurseWeeklyLimit;
    uint32 PurseQuantity;
    uint32 PurseLimit;
    uint32 Quantity;
    uint32 CompletedMask;

    uint32 ShortageRewardCount;
    uint32 itemCount;
    uint32 currencyCount;
    uint32 bonusCurrencyCount;
    uint32 Mask;
    uint32 RewardMoney;
    uint32 RewardExperience;

    uint32 CurrencyID;
    uint32 ItemID;
    bool FirstReward;
    bool ShortageEligible;
};

class LFGMgr
{
    friend class ACE_Singleton<LFGMgr, ACE_Null_Mutex>;

    private:
        LFGMgr();
        ~LFGMgr();

    public:
        // Functions used outside lfg namespace
        void Update(uint32 diff);

        // World.cpp
        /// Finish the dungeon for the given group. All check are performed using internal lfg data
        void FinishDungeon(uint64 gguid, uint32 dungeonId);
        /// Loads rewards for random dungeons
        void LoadRewards();
        /// Loads dungeons from dbc and adds teleport coords
        void LoadLFGDungeons(bool reload = false);

        // Multiple files
        /// Check if given guid applied for random dungeon
        bool selectedRandomLfgDungeon(uint64 guid);
        /// Check if given guid applied for given map and difficulty. Used to know
        bool inLfgDungeonMap(uint64 guid, uint32 map, Difficulty difficulty);
        /// Get selected dungeons
        LfgDungeonSet const& GetSelectedDungeons(uint64 guid);
        /// Get current lfg state
        LfgState GetState(uint64 guid);
        /// Get current dungeon
        uint32 GetDungeon(uint64 guid, bool asId = true);
        /// Get the map id of the current dungeon
        uint32 GetDungeonMapId(uint64 guid);
        /// Get kicks left in current group
        uint8 GetKicksLeft(uint64 gguid);
        /// Load Lfg group info from DB
        void _LoadFromDB(Field* fields, uint64 guid);
        /// Initializes player data after loading group data from DB
        void SetupGroupMember(uint64 guid, uint64 gguid);
        /// Return Lfg dungeon entry for given dungeon id
        uint32 GetLFGDungeonEntry(uint32 id);

        // cs_lfg
        /// Get current player roles
        uint8 GetRoles(uint64 guid);
        /// Get current player comment (used for LFR)
        std::string const& GetComment(uint64 gguid);
        /// Gets current lfg options
        uint32 GetOptions();
        /// Sets new lfg options
        void SetOptions(uint32 options);
        /// Checks if given lfg option is enabled
        bool isOptionEnabled(uint32 option);
        /// Clears queue - Only for internal testing
        void Clean();
        /// Dumps the state of the queue - Only for internal testing
        std::string DumpQueueInfo(bool full = false);

        // LFGScripts
        /// Get leader of the group (using internal data)
        uint64 GetLeader(uint64 guid);
        /// Initializes locked dungeons for given player (called at login or level change)
        void InitializeLockedDungeons(Player* player, uint8 level = 0);
        /// Sets player team
        void SetTeam(uint64 guid, uint8 team);
        /// Sets player group
        void SetGroup(uint64 guid, uint64 group);
        /// Gets player group
        uint64 GetGroup(uint64 guid);
        /// Sets the leader of the group
        void SetLeader(uint64 gguid, uint64 leader);
        /// Removes saved group data
        void RemoveGroupData(uint64 guid);
        /// Removes a player from a group
        uint8 RemovePlayerFromGroup(uint64 gguid, uint64 guid);
        /// Adds player to group
        void AddPlayerToGroup(uint64 gguid, uint64 guid);

        // LFGHandler
        /// Get locked dungeons
        LfgLockMap const& GetLockedDungeons(uint64 guid);
        /// Returns current lfg status
        LfgUpdateData GetLfgStatus(uint64 guid);
        /// Checks if Seasonal dungeon is active
        bool IsSeasonActive(uint32 dungeonId);
        /// Gets the random dungeon reward corresponding to given dungeon and player level
        LfgReward const* GetRandomDungeonReward(uint32 dungeon, uint8 level);
        /// Returns all random and seasonal dungeons for given level and expansion
        LfgDungeonSet GetRandomAndSeasonalDungeons(uint8 level, uint8 expansion);
        /// Teleport a player to/from selected dungeon
        void TeleportPlayer(Player* player, bool out, bool fromOpcode = false);
        /// Inits new proposal to boot a player
        void InitBoot(uint64 gguid, uint64 kguid, uint64 vguid, std::string const& reason);
        /// Updates player boot proposal with new player answer
        void UpdateBoot(uint64 guid, bool accept);
        /// Updates proposal to join dungeon with player answer
        void UpdateProposal(uint32 proposalId, uint64 guid, bool accept);
        /// Updates the role check with player answer
        void UpdateRoleCheck(uint64 gguid, uint64 guid = 0, uint8 roles = PLAYER_ROLE_NONE);
        /// Sets player lfg roles
        void SetRoles(uint64 guid, uint8 roles);
        /// Sets player lfr comment
        void SetComment(uint64 guid, std::string const& comment);
        /// Join Lfg with selected roles, dungeons and comment
        void JoinLfg(Player* player, uint8 roles, LfgDungeonSet& dungeons, std::string const& comment);
        /// Leaves lfg
        void LeaveLfg(uint64 guid);

        // LfgQueue
        /// Get last lfg state (NONE, DUNGEON or FINISHED_DUNGEON)
        LfgState GetOldState(uint64 guid);
        /// Check if given group guid is lfg
        bool IsLfgGroup(uint64 guid);
        /// Gets the player count of given group
        uint8 GetPlayerCount(uint64 guid);
        /// Add a new Proposal
        uint32 AddProposal(LfgProposal& proposal);
        /// Returns queue id
        uint8 GetQueueId(uint64 guid);
        /// Checks if all players are queued
        bool AllQueued(LfgGuidList const& check);
        /// Gets queue join time
        time_t GetQueueJoinTime(uint64 guid);
        /// Checks if given roles match, modifies given roles map with new roles
        static bool CheckGroupRoles(LfgRolesMap &groles, bool removeLeaderFlag = true);
        /// Checks if given players are ignoring each other
        static bool HasIgnore(uint64 guid1, uint64 guid2);
        /// Sends queue status to player
        static void SendLfgQueueStatus(uint64 guid, LfgQueueStatusData const& data);

        void BuildRideTicket(WorldPacket data, ObjectGuid PlayerGuid, uint32 ClientInstanceID, uint32 QueueSlot, uint32 JoinTime);
        void ReadRideTicket(WorldPacket recvData, ObjectGuid PlayerGuid, uint32 ClientInstanceID, uint32 QueueSlot, uint32 JoinTime);

    private:
        uint8 GetTeam(uint64 guid);
        void RestoreState(uint64 guid, char const* debugMsg);
        void ClearState(uint64 guid, char const* debugMsg);
        void SetDungeon(uint64 guid, uint32 dungeon);
        void SetSelectedDungeons(uint64 guid, LfgDungeonSet const& dungeons);
        void SetLockedDungeons(uint64 guid, LfgLockMap const& lock);
        void DecreaseKicksLeft(uint64 guid);
        void SetState(uint64 guid, LfgState state);
        void RemovePlayerData(uint64 guid);
        void GetCompatibleDungeons(LfgDungeonSet& dungeons, LfgGuidSet const& players, LfgLockPartyMap& lockMap);
        void _SaveToDB(uint64 guid, uint32 db_guid);
        LFGDungeonData const* GetLFGDungeon(uint32 id);

        // Proposals
        void RemoveProposal(LfgProposalContainer::iterator itProposal, LfgUpdateType type);
        void MakeNewGroup(LfgProposal const& proposal);

        // Generic
        LFGQueue& GetQueue(uint64 guid);

        LfgDungeonSet const& GetDungeonsByRandom(uint32 randomdungeon);
        LfgType GetDungeonType(uint32 dungeon);

        void SendLfgBootProposalUpdate(uint64 guid, LfgPlayerBoot const& boot);
        void SendLfgJoinResult(uint64 guid, LfgJoinResultData const& data);
        void SendLfgRoleChosen(uint64 guid, uint64 pguid, uint8 roles);
        void SendLfgRoleCheckUpdate(uint64 guid, LfgRoleCheck const& roleCheck);
        void SendLfgUpdateStatus(uint64 guid, LfgUpdateData const& data, bool party);
        void SendLfgUpdateProposal(uint64 guid, LfgProposal const& proposal);

        LfgGuidSet const& GetPlayers(uint64 guid);

        // General variables
        uint32 m_QueueTimer;                               ///< used to check interval of update
        uint32 m_lfgProposalId;                            ///< used as internal counter for proposals
        uint32 m_options;                                  ///< Stores config options

        LfgQueueContainer QueuesStore;                     ///< Queues
        LfgCachedDungeonContainer CachedDungeonMapStore;   ///< Stores all dungeons by groupType
        // Reward System
        LfgRewardContainer RewardMapStore;                 ///< Stores rewards for random dungeons
        LFGDungeonContainer  LfgDungeonStore;
        // Rolecheck - Proposal - Vote Kicks
        LfgRoleCheckContainer RoleChecksStore;             ///< Current Role checks
        LfgProposalContainer ProposalsStore;               ///< Current Proposals
        LfgPlayerBootContainer BootsStore;                 ///< Current player kicks
        LfgPlayerDataContainer PlayersStore;               ///< Player data
        LfgGroupDataContainer GroupsStore;                 ///< Group data
        LfgDungeonSet m_lfgDungeonStore;
        LfgPlayerDungeonInfoSet m_lfgDungeonInfoStore;
};

} // namespace lfg

#define sLFGMgr ACE_Singleton<lfg::LFGMgr, ACE_Null_Mutex>::instance()
#endif
