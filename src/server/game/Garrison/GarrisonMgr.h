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

#define MAX_GARRISON_LEVEL 3
#define MAX_RESOURCES      10000

#pragma once
struct Garrisons
{
	Garrisons();
public:

};

struct Followers
{
	Followers(uint64 DbId, uint32 followerId, uint32 creatureId, uint32 givenNameId, uint32 familyNameId, uint32 gender, 
		uint32 spec, uint32 race, uint32 quality, uint32 level, uint32 iLevelW, uint32 iLevelA, uint32 buildingId, uint32 missionId, uint32 abilityId)
        : npcEntry(DbId), GarrFollowerID(followerId), CreatureID(creatureId), GarrGivenNameID(givenNameId), GarrFamilyNameID(familyNameId), _Gender(gender),
		_Spec(spec), _Race(race), Quality(quality), FollowerLevel(level), ItemLevelArmor(iLevelA), ItemLevelWeapon(iLevelW), CurrentBuildingID(buildingId),
		CurrentMissionID(missionId), AbilityID(abilityId) { }
public:
	uint64 npcEntry;
	uint32 GarrFollowerID;
	uint32 CreatureID;
	uint32 GarrGivenNameID;
	uint32 GarrFamilyNameID;
	uint32 _Gender;
	uint32 _Spec;
	uint32 _Race;
	uint32 Quality;
	uint32 FollowerLevel;
	uint32 ItemLevelWeapon;
	uint32 ItemLevelArmor;
	uint32 CurrentBuildingID;
	uint32 CurrentMissionID;
	uint32 AbilityID;
};

struct GarrisonInfo
{
	GarrisonInfo(uint32 infoCount, uint32 plotInfoCount, uint32 followerCount, uint32 mission, uint32 archivedMissions, uint32 abilityCount)
		: GarrisonBuildingInfoCount(infoCount), GarrisonPlotInfoCount(plotInfoCount), GarrisonFollowerCount(followerCount), GarrisonMission(mission),
		ArchivedMissions(archivedMissions), AbilityIDCount(abilityCount) { }
		
public:
	uint32 GarrisonBuildingInfoCount;
	uint32 GarrisonPlotInfoCount;
	uint32 GarrisonFollowerCount;
	uint32 GarrisonMission;
	uint32 ArchivedMissions;
	uint32 AbilityIDCount;
};

struct GarrisonBuildingInfo
{
	GarrisonBuildingInfo(uint32 plotId, uint32 buildingId, uint32 time, uint32 specId, bool active)
		: GarrPlotInstanceID(plotId), GarrBuildingID(buildingId), Time(time), CurrentGarSpecID(specId), Active(active) { }

public:
	uint32 GarrPlotInstanceID;
	uint32 GarrBuildingID;
	uint32 Time;
	uint32 CurrentGarSpecID;
	bool Active;
};

struct PlotInfo 
{
	PlotInfo(uint32 Id, float X, uint32 Y, uint32 Z, uint32 plotType)
	: GarrPlotInstanceID(Id), PosX(X), PosY(Y), PosZ(Z), PlotType(plotType) { }

public:
	uint32 GarrPlotInstanceID;
	float PosX;
	float PosY;
	float PosZ;
	uint32 PlotType;
};

struct Missions
{
	Missions(uint64 dbId, uint32 missionId, uint32 offerTime, uint32 offerDuration, uint32 startTime, uint32 travelDuration, uint32 missionDuration, uint32 missionState)
        : missionEntry(dbId), MissionRecID(missionId), OfferTime(offerTime), OfferDuration(offerDuration), StartTime(startTime), TravelDuration(travelDuration),
		MissionDuration(missionDuration), MissionState(missionState) { }

public:
	uint64 missionEntry;
	uint32 MissionRecID;
	uint32 OfferTime;
	uint32 OfferDuration;
	uint32 StartTime;
	uint32 TravelDuration;
	uint32 MissionDuration;
	uint32 MissionState;
};

struct Abilities
{
	Abilities(uint32 _id, uint32 _type, std::string _name, std::string _description, uint32 _spellId, uint32 _data1, uint32 _data2)
		: Id(_id), Type(_type), Name(_name), Description(_description), SpellId(_spellId), Data1(_data1), Data2(_data2) { }
public:
	uint32 Id;
	uint32 Type;
	std::string Name;
	std::string Description;
	uint32 SpellId;
	uint32 Data1;
	uint32 Data2;
};

struct Buildings
{
	Buildings(uint32 _ID, uint32 _SpawnIDH, uint32 _SpawnIDA, uint32 _Data0, uint32 _Data1, uint32 _BuildingRank, std::string _NameH, std::string _NameA,
		std::string _Description, std::string _Effect, uint32 _BuildTime, uint32 _field11, uint32 _ResourcesCost, uint32 _field13, uint32 _field14, uint32 _DisplayId,
		uint32 _field16, uint32 _field17, uint32 _field18, uint32 _field19, uint32 _WorkOrders, uint32 _field21, uint32 _field22, uint32 _GoldCost)
		: ID(_ID), SpawnIDH(_SpawnIDH), SpawnIDA(_SpawnIDA), Data0(_Data0), Data1(_Data1), BuildingRank(_BuildingRank), NameH(_NameH), NameA(_NameA), Description(_Description),
		Effect(_Effect), BuildTime(_BuildTime), field11(_field11), ResourcesCost(_ResourcesCost), field13(_field13), field14(_field14), DisplayId(_DisplayId), field16(_field16),
		field17(_field17), field18(_field18), field19(_field19), WorkOrders(_WorkOrders), field21(_field21), field22(_field22), GoldCost(_GoldCost) { }

public:
	uint32 ID;
	uint32 SpawnIDH;
	uint32 SpawnIDA;
	uint32 Data0;
	uint32 Data1;
	uint32 BuildingRank;
	std::string NameH;
	std::string NameA;
	std::string Description;
	std::string Effect;
	uint32 BuildTime;
	uint32 field11;
	uint32 ResourcesCost;
	uint32 field13;
	uint32 field14;
	uint32 DisplayId;
	uint32 field16;
	uint32 field17;
	uint32 field18;
	uint32 field19;
	uint32 WorkOrders;
	uint32 field21;
	uint32 field22;
	uint32 GoldCost;
};

typedef std::set <Abilities*> AbilitiesSet;
typedef std::set <Buildings*> GarrBuildingSet;
typedef std::set <Garrisons*> GarrisonsSet;
typedef std::set <Followers*> FollowerSet;
typedef std::set <GarrisonInfo*> GarrisonInfoSet;
typedef std::set <GarrisonBuildingInfo*> BuildingSet;
typedef std::set <PlotInfo*> PlotInfoSet;
typedef std::set <Missions*> MissionSet;

class GarrisonMgr
{
public:

	static GarrisonMgr* instance()
	{
		static GarrisonMgr instance;
		return &instance;
	}

	GarrisonMgr();
	~GarrisonMgr();

	// Database Methods
    void LoadFromDB();

	// Garrison Methods
	void BuildingActivate();
	void GarrisonCreate();
	void PlaceBuilding();
	void SendArchitectWindow();

	// Garrison Opcodes
	void SendGarrisonActivateBuilding(WorldSession* session);
	void SendGarrisonArchitect(WorldSession* session);
    void SendGarrisonCompleteMission(WorldSession* session);
	void SendGarrisonGetInfo(WorldSession* session);
	void SendGarrisonPlotPlacedResult(WorldSession* session);

private:
	AbilitiesSet m_AbilitiesStore;
	GarrBuildingSet m_buildingStore;
	BuildingSet m_BuildingSet;
	GarrisonsSet m_GarrisonsStore;
	GarrisonInfoSet m_GarrisonInfoStore;
	FollowerSet m_FollowerStore;
	MissionSet m_MissionStore;
	PlotInfoSet m_PlotInfoStore;

	bool HasAbilityId(uint32 id);
	bool HasBuildingId(uint32 id);
	bool HasFollowerId(uint32 id);
	bool HasMissionId(uint32 id);

	void LoadAbilitiesFromDb();
	void LoadBuildingsFromDb();
	void LoadFollowersFromDb();
	void LoadMissionsFromDb();
	void LoadWorkOrdersFromDb();
};

#define sGarrisonMgr GarrisonMgr::instance()

#endif
