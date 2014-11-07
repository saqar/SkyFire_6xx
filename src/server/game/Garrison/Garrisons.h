#include "SharedDefines.h"
#include "Common.h"
#include <set>

#include "DB2Structure.h"
#include "DB2Stores.h"
#include "DB2Enums.h"
#include "GarrisonMgr.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

enum DBStateGarr
{
    DB_STATE_GARR_NONE      = 0,
    DB_STATE_GARR_SAVE      = 1,
    DB_STATE_GARR_DELETE    = 2
};

enum DBStateFollowers
{
    DB_STATE_FOLLOWER_NONE = 0,
    DB_STATE_FOLLOWER_SAVE = 1,
    DB_STATE_FOLLOWER_DELETE = 2
};

#pragma once
class Garrisons
{
public:
    Garrisons(uint32 garrisonId, uint32 accountId, uint32 characterId, uint32 garrisonLevel, uint32 currentResources, uint8 specialization,
        uint32 garrisonBuildings, uint32 garrisonWorkOrders) : m_garrId(garrisonId), m_accountId(accountId), m_charId(characterId), m_garrLevel(garrisonLevel),
        m_currentResources(currentResources), m_specialization(specialization), m_buildings(garrisonBuildings), m_workOrders(garrisonWorkOrders) { }
    ~Garrisons();

    uint32 GetGarrisonId() const { return m_garrId; }
    uint32 GetAccountId() const { return m_accountId; }
    uint32 GetCharacterId() const { return m_charId; }
    uint32 GetGarrisonLevel() const { return m_garrLevel; }
    uint32 GetGarrisonResources() const { return m_currentResources; }
    uint8 GetGarrisonSpec() const { return m_specialization; }
    uint32 GetGarrisonBuildings() const { return m_buildings; }
    uint32 GetGarrisonWorkOrders() const { return m_workOrders; }

    DBStateGarr GetDbState() const { return m_dbState; }
    void SetDbState(DBStateGarr state) { m_dbState = state; }

    // Garrison Functions
    void SendArchitectWindow(ObjectGuid npcGuid);
    void SendGarrisonUpgrade();
    void SendGarrisonLevelUp();  

    void CreateGarrison();
    void DeleteGarrison();

    // Building Functions
    void SendGarrisonBuildingActivate();
    void GarrisonPlaceBuilding();
    void GarrisonDeleteBuilding();
    void GarrisonSelectBuildingSpecialization();
    void GarrisonSwapBuilding();

    void SendGarrisonPlotPlaced();
    void SendGarrisonPlotCleared();

    // Follower Functions
    void SendGarrisonAddFollower();
    void SendGarrisonRemoveFollower();
    void HandleGarrisonFollowerLevel();
    void HandleGarrisonFollowerItemLevelChange();
    uint32 GetFollowerAbility() const { return m_followerAbility; }
    GarrFollowerEntry* GetFollowerId() const { return m_garrFollower; }

    // Mission functions
    void SendGarrCompleteMission();
    void SendGarrStartMission();
    void GarrisonSelectFollowersForMission();
    void SendGarrOpenMissionNpc();
    void SendGarrAddMission();

    void HandleGarrisonMissionType(PreparedQueryResult result);

private:
    // Struct of garrison table
    uint32 m_garrId;
    uint32 m_accountId;
    uint32 m_charId;
    uint32 m_garrLevel;
    uint32 m_currentResources;
    uint8 m_specialization;
    uint32 m_buildings;
    uint32 m_workOrders;

    uint32 m_followerAbility;

    CreatureTemplate* m_creature;
    GarrAbilityEntry* m_garrAbility;
    GarrMissionEntry* m_garrMission;
    GarrMissionCategory* m_category;
    GarrFollowerEntry* m_garrFollower;

    DBStateGarr m_dbState;
};

class Followers
{
public:
    Followers(uint32 garrisonId, uint32 slot1, uint32 slot2, uint32 slot3, uint32 slot4, uint32 slot5, uint32 slotLevel1, uint32 slotLevel2, uint32 slotLevel3,
        uint32 slotLevel4, uint32 slotLevel5, uint32 slot1XpToLevel, uint32 slot2XpToLevel, uint32 slot3XpToLevel, uint32 slot4XpToLevel, uint32 slot5XpToLevel)
        : m_garrisonId(garrisonId), m_slot1(slot1), m_slot2(slot2), m_slot3(slot3), m_slot4(slot4), m_slot5(slot5), m_slotLevel1(slotLevel1), m_slotLevel2(slotLevel2),
        m_slotLevel3(slotLevel3), m_slotLevel4(slotLevel4), m_slotLevel5(slotLevel5), m_slot1XpToLevel(slot1XpToLevel), m_slot2XpToLevel(slot2XpToLevel),
        m_slot3XpToLevel(slot3XpToLevel), m_slot4XpToLevel(slot4XpToLevel), m_slot5XpToLevel(slot5XpToLevel) { }
    ~Followers();

    DBStateFollowers GetDbState() const { return m_dbState; }
    void SetDbState(DBStateFollowers state) { m_dbState = state; }

private:
    uint32 m_garrisonId;
    uint32 m_slot1;
    uint32 m_slot2;
    uint32 m_slot3;
    uint32 m_slot4;
    uint32 m_slot5;
    uint32 m_slotLevel1;
    uint32 m_slotLevel2;
    uint32 m_slotLevel3;
    uint32 m_slotLevel4;
    uint32 m_slotLevel5;
    uint32 m_slot1XpToLevel;
    uint32 m_slot2XpToLevel;
    uint32 m_slot3XpToLevel;
    uint32 m_slot4XpToLevel;
    uint32 m_slot5XpToLevel;

    DBStateFollowers m_dbState;
};