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
	Followers();
public:

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

typedef std::set <Garrisons*> GarrisonsSet;
typedef std::set <Followers*> FollowerSet;
typedef std::set <GarrisonInfo*> GarrisonInfoSet;
typedef std::set <GarrisonBuildingInfo*> BuildingSet;
typedef std::set <PlotInfo*> PlotInfoSet;

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

	void LoadFromDB(PreparedQueryResult result) { };

	void SendGarrisonGetInfo(WorldSession* session);
	void HandleGetGarrisonInfoResult();

	void GarrisonCreate();

private:
	GarrisonsSet m_GarrisonsStore;
	FollowerSet m_FollowerStore;
	GarrisonInfoSet m_GarrisonInfoStore;
	BuildingSet m_BuildingSet;
	PlotInfoSet m_PlotInfoStore;

	void LoadFollowersFromDb();
	void LoadBuildingsFromDb();
	void LoadMissionsFromDb();
	void LoadWorkOrdersFromDb();
	void LoadAbilitiesFromDb();
};

#define sGarrisonMgr GarrisonMgr::instance()

#endif
