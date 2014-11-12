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

#ifndef GARRISON_MGR_H
#define GARRISON_MGR_H

#include "SharedDefines.h"
#include "Common.h"
#include <set>

#include "DB2Structure.h"
#include "DB2Stores.h"
#include "DB2Enums.h"

#define MAX_BUILDING_PLOTS 3
#define MAX_FOLLOWER_SLOTS 5
#define MAX_GARRISON_LEVEL 3
#define MAX_RESOURCES      10000

enum DBStateGarr
{
	DB_STATE_GARR_NONE = 0,
	DB_STATE_GARR_SAVE = 1,
	DB_STATE_GARR_DELETE = 2
};

enum DBStateFollowers
{
	DB_STATE_FOLLOWER_NONE = 0,
	DB_STATE_FOLLOWER_SAVE = 1,
	DB_STATE_FOLLOWER_DELETE = 2
};

class GarrisonMgr
{
public:
    GarrisonMgr();
    ~GarrisonMgr();

    void HandleGetGarrisonInfoResult();

    uint32 GetGarrisonId() const { return m_garrId; }
    uint32 GetAccountId() const { return m_accountId; }
    uint32 GetCharacterId() const { return m_charId; }
    uint32 GetGarrisonLevel() const { return m_garrLevel; }
    uint32 GetGarrisonResources() const { return m_currentResources; }
    uint8 GetGarrisonSpec() const { return m_specialization; }
    uint32 GetGarrisonBuildings() const { return m_buildings; }
    uint32 GetGarrisonWorkOrders() const { return m_workOrders; }

    Player* GetOwner() const { return m_owner; }
    //Garrisons* GetGarrison(uint64 garrisonId) const;
    void GarrisonCreate();

    void Reset();
    static void DeleteFromDB(uint32 lowguid);
    void LoadFromDB(PreparedQueryResult result) {};
    void SaveToDB(SQLTransaction& trans);

    static void DeleteFollowerFromDB(uint32 garrId);
    void SaveFollowerToDB(SQLTransaction& trans);
    void LoadFollowerFromDB(PreparedQueryResult result) {};

    CreatureTemplate* GetCreatureTemplate() const { return m_creature; }

    // Garrison Functions
    GarrMissionCategory* GetMissionCategory() const { return m_category; }
    GarrMissionEntry* GetMission() const { return m_garrMission; }
    GarrAbilityEntry* GetAbility() const { return m_garrAbility; }

    void SendGarrisonGetInfo();

    uint8 GarrisonGetFollowerSlots(uint64 garrisonId);
    uint8 GetFollowerSlot(uint32 followerId);

private:
    Player* m_owner;
    Unit* m_unit;

    uint32 m_garrId;
    uint32 m_accountId;
    uint32 m_charId;
    uint32 m_garrLevel;
    uint32 m_currentResources;
    uint8 m_specialization;
    uint32 m_buildings;
    uint32 m_workOrders;

    CreatureTemplate* m_creature;
    GarrAbilityEntry* m_garrAbility;
    GarrMissionEntry* m_garrMission;
    GarrMissionCategory* m_category;

    //GarrisonSet m_garrisonSet;
};

//typedef std::set<Garrisons*> GarrisonSet;

#pragma once
class Garrisons
{
public:
	Garrisons(uint64 garrisonId, uint32 accountId, uint32 characterId, uint32 garrisonLevel, uint32 currentResources, uint8 specialization,
		uint32 garrisonBuildings, uint32 garrisonWorkOrders) : m_garrId(garrisonId), m_accountId(accountId), m_charId(characterId), m_garrLevel(garrisonLevel),
		m_currentResources(currentResources), m_specialization(specialization), m_buildings(garrisonBuildings), m_workOrders(garrisonWorkOrders) { }
	~Garrisons();

	Player* GetOwner() const { return m_owner; }

	uint64 GetGarrisonId() const { return m_garrId; }
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

	uint32 GetGarrisonPlot() const { return m_plot; }

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
	uint32 GetFollowerId() const { return m_garrFollower; }

	// Mission functions
	void SendGarrCompleteMission();
	void SendGarrStartMission();
	void GarrisonSelectFollowersForMission();
	void SendGarrOpenMissionNpc();
	void SendGarrAddMission();

	void HandleGarrisonMissionType(PreparedQueryResult result);

private:
	Player* m_owner;

	// Struct of garrison table
	uint64 m_garrId;
	uint32 m_accountId;
	uint32 m_charId;
	uint32 m_garrLevel;
	uint32 m_currentResources;
	uint8 m_specialization;
	uint32 m_buildings;
	uint32 m_workOrders;

	uint32 m_followerAbility;
	uint32 m_plot;

	CreatureTemplate* m_creature;
	GarrAbilityEntry* m_garrAbility;
	GarrMissionEntry* m_garrMission;
	GarrMissionCategory* m_category;
	uint32 m_garrFollower;

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

#endif
