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

#ifndef TRINITY_OBJECTDEFINES_H
#define TRINITY_OBJECTDEFINES_H

#include "Define.h"

enum HighGuid
{
    HIGHGUID_ITEM = 0x400,                       // blizz 4000
    HIGHGUID_CONTAINER = 0x400,                       // blizz 4000
    HIGHGUID_PLAYER = 0x000,                       // blizz 0000
    HIGHGUID_GAMEOBJECT = 0xF11,                       // blizz F110
    HIGHGUID_TRANSPORT = 0xF12,                       // blizz F120 (for GAMEOBJECT_TYPE_TRANSPORT)
    HIGHGUID_UNIT = 0xF13,                       // blizz F130
    HIGHGUID_PET = 0xF14,                       // blizz F140
    HIGHGUID_VEHICLE = 0xF15,                       // blizz F550
    HIGHGUID_DYNAMICOBJECT = 0xF10,                       // blizz F100
    HIGHGUID_CORPSE = 0xF101,                      // blizz F100
    HIGHGUID_AREATRIGGER = 0xF102,                      // blizz F100
    HIGHGUID_BATTLEGROUND = 0x1F1,                       // new 4.x
    HIGHGUID_MO_TRANSPORT = 0x1FC,                       // blizz 1FC0 (for GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT)
    HIGHGUID_GROUP = 0x1F5,
    HIGHGUID_GUILD = 0x1FF                        // new 4.x
};

enum GuidType
{
    GUID_TYPE_NONE                  = 0,
    GUID_TYPE_UNIQUE                = 1,
    GUID_TYPE_PLAYER                = 2,
    GUID_TYPE_ITEM                  = 3,
    GUID_TYPE_STATIC_DOOR           = 4,
    GUID_TYPE_TRANSPORT             = 5,
    GUID_TYPE_CONVERSATION          = 6,
    GUID_TYPE_CREATURE              = 7,
    GUID_TYPE_VEHICLE               = 8,
    GUID_TYPE_PET                   = 9,
    GUID_TYPE_GAMEOBJECT            = 10,
    GUID_TYPE_DYNAMICOBJECT         = 11,
    GUID_TYPE_AREATRIGGER           = 12,
    GUID_TYPE_CORPSE                = 13,
    GUID_TYPE_LOOT_OBJECT           = 14,
    GUID_TYPE_SCENE_OBJECT          = 15,
    GUID_TYPE_SCENARIO              = 16,
    GUID_TYPE_AI_GROUP              = 17,
    GUID_TYPE_DYNAMIC_DOOR          = 18,
    GUID_TYPE_CLIENT_ACTOR          = 19,
    GUID_TYPE_VIGNETTE              = 20,
    GUID_TYPE_CALL_FOR_HELP         = 21,
    GUID_TYPE_AI_RESOURCE           = 22,
    GUID_TYPE_AI_LOCK               = 23,
    GUID_TYPE_AI_LOCK_TICKET        = 24,
    GUID_TYPE_CHAT_CHANNEL          = 25,
    GUID_TYPE_PARTY                 = 26,
    GUID_TYPE_GUILD                 = 27,
    GUID_TYPE_WOWACCOUNT            = 28,
    GUID_TYPE_BNET_ACCOUNT          = 29,
    GUID_TYPE_GM_TASK               = 30,
    GUID_TYPE_MOBILE_SESSION        = 31,
    GUID_TYPE_RAID_GROUP            = 32,
    GUID_TYPE_SPELL                 = 33,
    GUID_TYPE_MAIL                  = 34,
    GUID_TYPE_WEB_OBJECT            = 35,
    GUID_TYPE_LFG_OBJECT            = 36,
    GUID_TYPE_LFG_LIST              = 37,
    GUID_TYPE_USER_ROUTER           = 38,
    GUID_TYPE_BATTLEGROUND          = 39,
    GUID_TYPE_USER_CLIENT           = 40,
    GUID_TYPE_PET_BATTLE            = 41,
    GUID_TYPE_UNIQUE_USER_CLIENT    = 42,
    GUID_TYPE_BATTLE_PET            = 43
};

struct Guid128
{
    uint64 loGuid;
    uint64 hiGuid;
};

// used for creating values for respawn for example
inline uint64 MAKE_PAIR64(uint32 l, uint32 h);
inline uint32 PAIR64_HIPART(uint64 x);
inline uint32 PAIR64_LOPART(uint64 x);
inline uint16 MAKE_PAIR16(uint8 l, uint8 h);
inline uint32 MAKE_PAIR32(uint16 l, uint16 h);
inline uint16 PAIR32_HIPART(uint32 x);
inline uint16 PAIR32_LOPART(uint32 x);

inline bool IS_EMPTY_GUID(uint64 guid);
inline bool IS_CREATURE_GUID(uint64 guid);
inline bool IS_PET_GUID(uint64 guid);
inline bool IS_VEHICLE_GUID(uint64 guid);
inline bool IS_CRE_OR_VEH_GUID(uint64 guid);
inline bool IS_CRE_OR_VEH_OR_PET_GUID(uint64 guid);
inline bool IS_PLAYER_GUID(uint64 guid);
inline bool IS_GUILD_GUID(uint64 guid);
inline bool IS_UNIT_GUID(uint64 guid);
inline bool IS_ITEM_GUID(uint64 guid);
inline bool IS_GAMEOBJECT_GUID(uint64 guid);
inline bool IS_DYNAMICOBJECT_GUID(uint64 guid);
inline bool IS_CORPSE_GUID(uint64 guid);
inline bool IS_TRANSPORT_GUID(uint64 guid);
inline bool IS_MO_TRANSPORT_GUID(uint64 guid);
inline bool IS_GROUP_GUID(uint64 guid);
inline bool IS_AREATRIGGER_GUID(uint64 guid);

// l - OBJECT_FIELD_GUID
// e - OBJECT_FIELD_ENTRY_ID for GO (except GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT) and creatures or UNIT_FIELD_PET_NUMBER for pets
// h - OBJECT_FIELD_GUID + 1
inline uint64 MAKE_NEW_GUID(uint32 l, uint32 e, uint32 h);

static Guid128 MAKE_NEW_GUID128(uint64 l, uint32 e, uint32 t)
{
    Guid128 guid = {0, 0};
    guid.hiGuid |= ((uint64(t) << 58) | (uint64(e) << 6));
    guid.loGuid |= l;
    return guid;
}

//#define GUID_HIPART(x)   (uint32)((uint64(x) >> 52)) & 0x0000FFFF)
inline uint32 GUID_HIPART(uint64 guid);
inline uint32 GUID_ENPART(uint64 x);
inline uint32 GUID_LOPART(uint64 x);

inline bool IsGuidHaveEnPart(uint64 guid);
inline char const* GetLogNameForGuid(uint64 guid);

uint64 MAKE_PAIR64(uint32 l, uint32 h)
{
    return uint64(l | (uint64(h) << 32));
}

uint32 PAIR64_HIPART(uint64 x)
{
    return (uint32)((x >> 32) & UI64LIT(0x00000000FFFFFFFF));
}

uint32 PAIR64_LOPART(uint64 x)
{
    return (uint32)(x & UI64LIT(0x00000000FFFFFFFF));
}

uint16 MAKE_PAIR16(uint8 l, uint8 h)
{
    return uint16(l | (uint16(h) << 8));
}

uint32 MAKE_PAIR32(uint16 l, uint16 h)
{
    return uint32(l | (uint32(h) << 16));
}

uint16 PAIR32_HIPART(uint32 x)
{
    return (uint16)((x >> 16) & 0x0000FFFF);
}

uint16 PAIR32_LOPART(uint32 x)
{
    return (uint16)(x & 0x0000FFFF);
}

bool IS_EMPTY_GUID(uint64 guid)
{
    return guid == 0;
}

bool IS_CREATURE_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_UNIT;
}

bool IS_PET_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_PET;
}

bool IS_VEHICLE_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_VEHICLE;
}

bool IS_CRE_OR_VEH_GUID(uint64 guid)
{
    return IS_CREATURE_GUID(guid) || IS_VEHICLE_GUID(guid);
}

bool IS_CRE_OR_VEH_OR_PET_GUID(uint64 guid)
{
    return IS_CRE_OR_VEH_GUID(guid) || IS_PET_GUID(guid);
}

bool IS_PLAYER_GUID(uint64 guid)
{
    return guid != 0 && GUID_HIPART(guid) == HIGHGUID_PLAYER;
}

bool IS_GUILD_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_GUILD;
}

bool IS_UNIT_GUID(uint64 guid)
{
    return IS_CRE_OR_VEH_OR_PET_GUID(guid) || IS_PLAYER_GUID(guid);
}

bool IS_ITEM_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_ITEM;
}

bool IS_GAMEOBJECT_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_GAMEOBJECT;
}

bool IS_DYNAMICOBJECT_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_DYNAMICOBJECT;
}

bool IS_CORPSE_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_CORPSE;
}

bool IS_TRANSPORT_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_TRANSPORT;
}

bool IS_MO_TRANSPORT_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_MO_TRANSPORT;
}

bool IS_GROUP_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_GROUP;
}

bool IS_AREATRIGGER_GUID(uint64 guid)
{
    return GUID_HIPART(guid) == HIGHGUID_AREATRIGGER;
}

uint64 MAKE_NEW_GUID(uint32 l, uint32 e, uint32 h)
{
    return uint64(uint64(l) | (uint64(e) << 32) | (uint64(h) << ((h == HIGHGUID_CORPSE || h == HIGHGUID_AREATRIGGER) ? 48 : 52)));
}

uint32 GUID_HIPART(uint64 guid)
{
    uint32 t = ((uint64(guid) >> 48) & 0x0000FFFF);
    return (t == HIGHGUID_CORPSE || t == HIGHGUID_AREATRIGGER) ? t : ((t >> 4) & 0x00000FFF);
}

uint32 GUID_ENPART(uint64 x)
{
    return IsGuidHaveEnPart(x)
            ? ((uint32)((x >> 32) & UI64LIT(0x00000000000FFFFF)))
            : 0;
}

uint32 GUID_LOPART(uint64 x)
{
    // _GUID_LOPART_3 and _GUID_LOPART_2 were both equal to PAIR64_LOPART
    return PAIR64_LOPART(x);
}

bool IsGuidHaveEnPart(uint64 guid)
{
    switch (GUID_HIPART(guid))
    {
        case HIGHGUID_ITEM:
        case HIGHGUID_PLAYER:
        case HIGHGUID_DYNAMICOBJECT:
        case HIGHGUID_CORPSE:
        case HIGHGUID_GROUP:
        case HIGHGUID_GUILD:
            return false;
        case HIGHGUID_GAMEOBJECT:
        case HIGHGUID_TRANSPORT:
        case HIGHGUID_UNIT:
        case HIGHGUID_PET:
        case HIGHGUID_VEHICLE:
        case HIGHGUID_MO_TRANSPORT:
        case HIGHGUID_AREATRIGGER:
        default:
            return true;
    }
}

char const* GetLogNameForGuid(uint64 guid)
{
    switch (GUID_HIPART(guid))
    {
        case HIGHGUID_ITEM:         return "item";
        case HIGHGUID_PLAYER:       return guid ? "player" : "none";
        case HIGHGUID_GAMEOBJECT:   return "gameobject";
        case HIGHGUID_TRANSPORT:    return "transport";
        case HIGHGUID_UNIT:         return "creature";
        case HIGHGUID_PET:          return "pet";
        case HIGHGUID_VEHICLE:      return "vehicle";
        case HIGHGUID_DYNAMICOBJECT:return "dynobject";
        case HIGHGUID_CORPSE:       return "corpse";
        case HIGHGUID_MO_TRANSPORT: return "mo_transport";
        case HIGHGUID_GROUP:        return "group";
        case HIGHGUID_GUILD:        return "guild";
        case HIGHGUID_AREATRIGGER:  return "areatrigger";
        default:
            return "<unknown>";
    }
}

#endif
