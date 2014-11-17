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

#ifndef __UPDATEDATA_H
#define __UPDATEDATA_H

#include "ByteBuffer.h"
#include "Object.h"
#include <set>
#include <string>

class WorldPacket;

enum OBJECT_UPDATE_TYPE
{
    UPDATETYPE_VALUES               = 0,
    UPDATETYPE_CREATE_OBJECT        = 1,
    UPDATETYPE_CREATE_OBJECT2       = 2,
    UPDATETYPE_OUT_OF_RANGE_OBJECTS = 3,
};

enum OBJECT_UPDATE_FLAGS
{
    UPDATEFLAG_NONE                     = 0x00000,
    UPDATEFLAG_SELF                     = 0x00001,
    UPDATEFLAG_TRANSPORT                = 0x00002,
    UPDATEFLAG_HAS_TARGET               = 0x00004,
    UPDATEFLAG_LIVING                   = 0x00008,
    UPDATEFLAG_STATIONARY_POSITION      = 0x00010,
    UPDATEFLAG_VEHICLE                  = 0x00020,
    UPDATEFLAG_TRANSPORT_POSITION       = 0x00040,
    UPDATEFLAG_ROTATION                 = 0x00080,
    UPDATEFLAG_ANIMKITS                 = 0x00100,
    UPDATEFLAG_AREATRIGGER              = 0x00200,
    UPDATEFLAG_GAMEOBJECT               = 0x00400,
    UPDATEFLAG_REPLACE_ACTIVE           = 0x00800,
    UPDATEFLAG_NO_BIRTH_ANIM            = 0x01000,
    UPDATEFLAG_ENABLE_PORTALS           = 0x02000,
    UPDATEFLAG_PLAY_HOVER_ANIM          = 0x04000,
    UPDATEFLAG_IS_SUPPRESSING_GREETINGS = 0x08000,
    UPDATEFLAG_SCENEOBJECT              = 0x10000,
    UPDATEFLAG_SCENE_PENDING_INSTANCE   = 0x20000
};

class UpdateData
{
    public:
        UpdateData(uint16 map);

        void AddOutOfRangeGUID(std::set<ObjectGuid>& guids);
        void AddOutOfRangeGUID(ObjectGuid guid);
        void AddUpdateBlock(const ByteBuffer &block);
        bool BuildPacket(WorldPacket* packet);
        bool HasData() const { return m_blockCount > 0 || !m_outOfRangeGUIDs.empty(); }
        void Clear();
        size_t GetSizePosition() { return m_sizePos; }



        std::set<ObjectGuid> const& GetOutOfRangeGUIDs() const { return m_outOfRangeGUIDs; }

    protected:
        uint16 m_map;
        uint32 m_blockCount;
        std::set<ObjectGuid> m_outOfRangeGUIDs;
        ByteBuffer m_data;
        size_t m_sizePos;
};
#endif

