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
#include "ByteBuffer.h"
#include "WorldPacket.h"
#include "UpdateData.h"
#include "Log.h"
#include "Opcodes.h"
#include "World.h"
#include "zlib.h"
#include "Object.h"

UpdateData::UpdateData(uint16 map) : m_map(map), m_blockCount(0) { }

void UpdateData::AddOutOfRangeGUID(std::set<ObjectGuid>& guids)
{
    m_outOfRangeGUIDs.insert(guids.begin(), guids.end());
}

void UpdateData::AddOutOfRangeGUID(ObjectGuid guid)
{
    m_outOfRangeGUIDs.insert(guid);
}

void UpdateData::AddUpdateBlock(const ByteBuffer &block)
{
    m_data.append(block);
    ++m_blockCount;
}

bool UpdateData::BuildPacket(WorldPacket* packet)
{
    ASSERT(packet->empty());                                // shouldn't happen
    packet->Initialize(SMSG_UPDATE_OBJECT, 2 + 4 + (m_outOfRangeGUIDs.empty() ? 0 : 1 + 4 + 9 * m_outOfRangeGUIDs.size()) + m_data.wpos());

    *packet << uint32(m_blockCount);
    *packet << uint16(m_map);

    packet->WriteBit(!m_outOfRangeGUIDs.empty());
    packet->FlushBits();

    if (!m_outOfRangeGUIDs.empty())
    {
        *packet << uint16(0); // not used 
        *packet << uint32(m_outOfRangeGUIDs.size());    

        for (std::set<ObjectGuid>::const_iterator itr = m_outOfRangeGUIDs.begin(); itr != m_outOfRangeGUIDs.end(); ++itr)
             *packet << (*itr); // not sure why this happens ask Sovak asap.
    }

    m_sizePos = packet->wpos();
    *packet << uint32(0);
    packet->append(m_data);

    if (uint32 size = packet->wpos() - m_sizePos - 4)
        packet->put(m_sizePos, size);    
    return true;
}

void UpdateData::Clear()
{
    m_data.clear();
    m_outOfRangeGUIDs.clear();
    m_blockCount = 0;
    m_map = 0;
}

