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
#include "GarrisonMgr.h"
#include "Garrison.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

GarrisonMgr::~GarrisonMgr()
{
    for (GarrisonSet::const_iterator itr = m_garrisonSet.begin(); itr != m_garrisonSet.end(); itr++)
        delete *itr;

    m_garrisonSet.clear();
}

void GarrisonMgr::LoadFromDB(PreparedQueryResult result)
{
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint64 id                   = fields[0].GetUInt64();
        uint32 accountId            = fields[1].GetUInt32();
        uint32 garrisonId           = fields[2].GetUInt32();
        uint32 garrisonLevel        = fields[3].GetUInt32();
        uint32 currentResources     = fields[4].GetUInt32();
        uint8 specialization        = fields[5].GetUInt8();
        uint32 buildings            = fields[6].GetUInt32();
        uint32 followers            = fields[7].GetUInt32();
        uint32 workOrders           = fields[8].GetUInt32();

        if (!sGarrFollowerStore.LookupEntry(workOrders))
        {
            TC_LOG_ERROR("sql.sql", "Work Orders %u defined in `character_garrison` for GarrisonID %lu does not exist, skipped.", workOrders, (uint64)id);
            continue;
        }

        // Error for when the garrison's level is higher than 3
        if (garrisonLevel > MAX_GARRISON_LEVEL)
        {
            TC_LOG_ERROR("sql.sql", "Level %u defined in `account_battle_pet` for Battle Pet %lu is invalid, skipped.", garrisonLevel, (uint64)id);
            continue;
        }

        Garrison* garrison = new Garrison(id, accountId, garrisonId, garrisonLevel, currentResources, specialization,
            buildings, followers, workOrders);
    
        //m_GarrisonSet.insert(garrison);// -- For some reason, this isn't working.
    } 
    while (result->NextRow());
}

void GarrisonMgr::SaveToDB(SQLTransaction& trans)
{
    // SaveFollowerSlots to DB

    if (m_garrisonSet.empty())
        return;

    GarrisonSet::const_iterator itr = m_garrisonSet.begin();
    while (itr != m_garrisonSet.end())
    {
        Garrison* garr = *itr++; // Why is pointer giving error?
        switch (garr->GetDBState())
        {
            case GARR_DB_STATE_NONE:
                break;
            case GARR_DB_STATE_DELETE:
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON);
                stmt->setUInt64(0, garr->GetId());
                trans->Append(stmt);

                m_garrisonSet.erase(itr);
                delete garr;

                break;
            }
            case GARR_DB_STATE_SAVE:
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON);
                stmt->setUInt64(0, garr->GetId());
                trans->Append(stmt);

                stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON);
                stmt->setUInt64(0, garr->GetId());
                stmt->setUInt32(1, m_owner->GetSession()->GetAccountId());
                stmt->setUInt32(2, garr->GetGarrisonId());
                stmt->setUInt32(3, garr->GetGarrisonLevel());
                stmt->setUInt32(4, garr->GetResources());
                stmt->setUInt8(5,  garr->GetGarrisonSpec());
                stmt->setUInt32(6, garr->GetGarrisonBuildings());
                stmt->setUInt32(7, garr->GetGarrisonFollowers());
                stmt->setUInt32(8, garr->GetWorkOrders());

                trans->Append(stmt);

                //garr->SetDBState(GARR_DB_STATE_NONE);
                break;
            }
            default:
                break;
        }
    }
}

