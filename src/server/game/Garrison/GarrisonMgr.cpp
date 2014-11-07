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
#include "DatabaseEnv.h"
#include "DB2Enums.h"
#include "Field.h"
#include "GarrisonMgr.h"
#include "Garrisons.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

//GarrisonMgr::~GarrisonMgr()
//{
//    for (GarrisonSet::iterator itr = m_garrisonSet.begin(); itr != m_garrisonSet.end(); itr++)
//        delete *itr;
//
//    m_garrisonSet.clear();
//}
//
//void GarrisonMgr::LoadFromDB(PreparedQueryResult result)
//{
//    if (!result)
//        return;
//    
//    do
//    {
//        Field* fields = result->Fetch();
//
//        uint64 garrisonId = fields[0].&Field::GetUInt64;
//        uint32 accountId = fields[1].GetUInt32;
//        uint32 characterId = fields[2].GetUInt32;
//        uint32 garrisonLevel = fields[3].GetUInt32;
//        uint32 currentResources = fields[4].GetUInt32;
//        uint8  specialization = fields[5].GetUInt8;
//        uint32 buildings = fields[6].GetUInt32;
//        uint32 workOrders = fields[7].GetUInt32;
//
//        Garrisons* garrison = new Garrisons(garrisonId, accountId, characterId, garrisonLevel, currentResources,
//            specialization, buildings, workOrders);
//
//        m_garrisonSet.insert(garrison);
//
//    } while (result->NextRow());
//}
//
//void GarrisonMgr::SaveToDB(SQLTransaction& trans)
//{
//    SaveToDB(trans);
//
//    if (m_garrisonSet.empty())
//        return;
//
//    GarrisonSet::iterator itr = m_garrisonSet.begin();
//    while (itr != m_garrisonSet.end())
//    {
//        Garrisons* garrison = *itr++;
//        switch (garrison->GetDbState())
//        {
//        case DB_STATE_GARR_NONE:
//            break;
//        case DB_STATE_GARR_DELETE:
//        {
//            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON);
//            stmt->setUInt64(0, garrison->GetGarrisonId());
//            trans->Append(stmt);
//
//            m_garrisonSet.erase(itr);
//            delete garrison;
//
//            break;
//        }
//        case DB_STATE_GARR_SAVE:
//        {
//            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON);
//            stmt->setUInt64(0, garrison->GetGarrisonId());
//            trans->Append(stmt);
//
//            stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON);
//            stmt->setUInt64(0, garrison->GetGarrisonId());
//            stmt->setUInt32(1, garrison->GetAccountId());
//            stmt->setUInt32(2, garrison->GetCharacterId());
//            stmt->setUInt32(3, garrison->GetGarrisonLevel());
//            stmt->setUInt32(4, garrison->GetGarrisonResources());
//            stmt->setUInt8(5, garrison->GetGarrisonSpec());
//            stmt->setUInt32(6, garrison->GetGarrisonBuildings());
//            stmt->setUInt32(7, garrison->GetGarrisonWorkOrders());
//            trans->Append(stmt);
//
//            garrison->SetDbState(DB_STATE_GARR_NONE);
//            break;
//        }
//        default:
//            break;
//        }
//    }
//}
//
//void GarrisonMgr::LoadFollowerFromDB(PreparedQueryResult result)
//{
//    if (!result)
//        return;
//
//    do
//    {
//        Field* field = result->Fetch();
//
//        uint64 garrisonId = field[0].GetUInt64();
//        uint32 followerSlot1 = field[1].GetUInt32();
//        uint32 followerSlot2 = field[2].GetUInt32();
//        uint32 followerSlot3 = field[3].GetUInt32();
//        uint32 followerSlot4 = field[4].GetUInt32();
//        uint32 followerSlot5 = field[5].GetUInt32();
//        uint32 slot1Level    = field[6].GetUInt32();
//        uint32 slot2Level = field[7].GetUInt32();
//        uint32 slot3Level = field[8].GetUInt32();
//        uint32 slot4Level = field[9].GetUInt32();
//        uint32 slot5Level = field[10].GetUInt32();
//        uint32 slot1XpToLevel = field[11].GetUInt32();
//        uint32 slot2XpToLevel = field[12].GetUInt32();
//        uint32 slot3XpToLevel = field[13].GetUInt32();
//        uint32 slot4XpToLevel = field[14].GetUInt32();
//        uint32 slot5XpToLevel = field[15].GetUInt32();
//
//        Followers* follower = new Followers(garrisonId, followerSlot1, followerSlot2, followerSlot3, followerSlot4, followerSlot5,
//            slot1Level, slot2Level, slot3Level, slot4Level, slot5Level, slot1XpToLevel, slot2XpToLevel, slot3XpToLevel, slot4XpToLevel, slot5XpToLevel);
//    } while (result->NextRow());
//}
//
//Garrisons* GarrisonMgr::GetGarrison(uint64 garrisonId) const
//{
//    for (GarrisonSet::iterator itr = m_garrisonSet.begin(); itr != m_garrisonSet.end(); itr++)
//        if ((*itr)->GetGarrisonId() == garrisonId)
//            return *itr;
//
//    return NULL;
//}
//
//uint8 GarrisonMgr::GarrisonGetFollowerSlots(uint64 garrisonId)
//{
//    for (uint8 i = 0; i < MAX_FOLLOWER_SLOTS; i++)
//        if (GetFollowerSlot(i) == garrisonId)
//            return i;
//
//    return 0;
//}
//
//void GarrisonMgr::SendGarrisonGetInfo()
//{
//    TC_LOG_DEBUG("network", "World: Sent SMSG_GET_GARR_INFO_RESULT");
//    WorldPacket data(SMSG_GET_GARR_INFO_RESULT);
//    ObjectGuid guid;
//    bool active;
//
//    Unit const* m_unit = 0;
//    GarrPlotEntry const* plotEntry = 0;
//
//    data << uint32(0);                              // Unk
//    data << uint32(0);                              // GarrSiteID
//    data << uint32(0);                              // FactionIndex
//    data << uint32(0);                              // GarrSiteLevelID
//    data << uint32(0);                              // Unk
//    data << uint32(0);                              // Unk
//    data << uint32(0);                              // Unk
//    data << uint32(0);                              // Unk
//    data << uint32(0);                              // Unk
//
//
//        data << uint64(0);                              // DBID
//        data << uint32(0);                                        // MissionRecID
//        data << uint32(0);                                        // OfferTime
//        data << uint32(0);                                        // OfferDuration
//        data << uint32(0);                                        // StartTime
//        data << uint32(0);                                        // TravelDuration
//        data << uint32(0);                                          // MissionDuration
//        data << uint32(0);                            // MissionState
//
//        data << uint32(0);                              // GarrPlotInstanceID
//        data << uint32(0);
//        data << uint32(0);
//        data << uint32(0);
//        data << uint32(0);                               // GarrBuildingID
//        active = data.WriteBit(0);                      // Active
//
//        data << uint64(0);        // DBID
//        data << uint32(0);      // GarrFollowerID
//        data << uint32(0);                              // CreatureID
//        data << uint32(0);                              // Gender
//        data << uint32(0);                              // Spec
//        data << uint32(0);                              // Race
//        data << uint32(0);                              // Quality
//        data << uint32(0);                              // FollowerLevel
//        data << uint32(0);                              // ItemLevelWeapon
//        data << uint32(0);                              // ItemLevelArmor
//        data << uint32(0);                              // Xp
//
//        uint32 abilityID = 0;
//        for (uint32 i = 0; i < abilityID; i++)
//            uint32(0);                                  // AbilityID
//
//        data << uint32(plotEntry->ID);                  // GarrPlotInstanceID
//        data << float(m_unit->GetPositionX());          // PositionX
//        data << float(m_unit->GetPositionY());          // PositionY
//        data << float(m_unit->GetPositionZ());          // PositionZ
//        data << uint32(plotEntry->PlotSize);            // PlotType
//
//    uint32 archivedMissions = 0;
//    for (uint32 i = 0; i < archivedMissions; i++)
//        uint32(0);                                  // ArchivedMissions
//
//    m_owner->GetSession()->SendPacket(&data);
//}