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

#include "Opcodes.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "Config.h"

void WorldSession::SendAuthResponse(uint8 code, bool queued, uint32 queuePos)
{
    std::map<uint32, std::string> realmNamesToSend;

    QueryResult classResult = LoginDatabase.PQuery("SELECT class, expansion FROM realm_classes WHERE realmId = %u", realmID);
    QueryResult raceResult = LoginDatabase.PQuery("SELECT race, expansion FROM realm_races WHERE realmId = %u", realmID);

    if (!classResult || !raceResult)
    {
        TC_LOG_ERROR("network", "Unable to retrieve class or race data.");
        return;
    }

    RealmNameMap::const_iterator iter = realmNameStore.find(realmID);
    if (iter != realmNameStore.end()) // Add local realm
        realmNamesToSend[realmID] = iter->second;

    TC_LOG_ERROR("network", "SMSG_AUTH_RESPONSE");
    WorldPacket packet(SMSG_AUTH_RESPONSE, 80);

    packet << uint8(code);                             // Auth response ?
    packet.WriteBit(code == AUTH_OK);
    packet.WriteBit(queued);

    if (code == AUTH_OK)
    {
        packet << uint32(realmID);
        packet << uint32(realmNamesToSend.size());     // Send current realmId
        packet << uint32(0);
        packet << uint32(0);
        packet << uint32(0);
        packet << uint8(Expansion());
        packet << uint8(Expansion());
        packet << uint32(0);
        packet << uint32(raceResult->GetRowCount());
        packet << uint32(classResult->GetRowCount());
        packet << uint32(0);
        packet << uint32(0); // Currency ???

        for (std::map<uint32, std::string>::const_iterator itr = realmNamesToSend.begin(); itr != realmNamesToSend.end(); itr++)
        {
            packet << uint32(itr->first);
            packet.WriteBit(itr->first == realmID); // Home realm
            packet.WriteBit(0); // Unk 18967
            packet.WriteBits(itr->second.size(), 8);
            packet.WriteBits(itr->second.size(), 8);
            packet.FlushBits();
            packet.WriteString(itr->second);
            packet.WriteString(itr->second);
        }

        do
        {
            Field* fields = raceResult->Fetch();

            packet << fields[0].GetUInt8();
            packet << fields[1].GetUInt8();
        }
        while (raceResult->NextRow());

        do
        {
            Field* fields = classResult->Fetch();

            packet << fields[0].GetUInt8();
            packet << fields[1].GetUInt8();
        }
        while (classResult->NextRow());

        packet.WriteBit(0); // Trial
        packet.WriteBit(0); // ForTemplate
        packet.WriteBit(0);
        packet.WriteBit(0); // IsVeteranTrial
    }

    if (queued)
    {
        packet << uint32(0);                            // Unknown
        packet.WriteBit(1);                             // Unknown
    }

    packet.FlushBits();
    SendPacket(&packet);
}

void WorldSession::SendClientCacheVersion(uint32 version)
{
    WorldPacket data(SMSG_CLIENTCACHE_VERSION, 4);
    data << uint32(version);
    SendPacket(&data);
}
