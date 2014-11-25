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

#ifndef DB2_ENUMS_H
#define DB2_ENUMS_H

enum BattlePetSpeciesFlags
{
    BATTLE_PET_FLAG_NONE                = 0x0000,
    BATTLE_PET_FLAG_LIMITED_ABILITIES   = 0x0001, // battle pets with less than 6 abilites have this flag
    BATTLE_PET_FLAG_NOT_TRADABLE        = 0x0002,
    BATTLE_PET_FLAG_NOT_ACCOUNT_BOUND   = 0x0004,
    BATTLE_PET_FLAG_NOT_RELEASABLE      = 0x0008,
    BATTLE_PET_FLAG_NOT_CAGEABLE        = 0x0010,
    BATTLE_PET_FLAG_NOT_TAMEABLE        = 0x0020,
    BATTLE_PET_FLAG_UNIQUE              = 0x0040,
    BATTLE_PET_FLAG_COMPANION           = 0x0080,
    BATTLE_PET_FLAG_UNKNOWN_2           = 0x0100,
    BATTLE_PET_FLAG_UNKNOWN_3           = 0x0200,
    BATTLE_PET_FLAG_ELITE               = 0x0400,
    BATTLE_PET_FLAG_UNKNOWN_4           = 0x0800, // battle pets 89, 169 and 218, these also have not tradable flag but don't display message in game
    BATTLE_PET_FLAG_UNKNOWN_5           = 0x1000  // only seen on battle pet 316 (Cenarion Hatchling)
};

enum GarrAbilityCategory
{
    GARR_ABILITY_CATEGORY_NONE              = 0,
    GARR_ABILITY_CATEGORY_SLAYER            = 1,
    GARR_ABILITY_CATEGORY_RACIAL_PREFERENCE = 2,
    GARR_ABILITY_CATEGORY_PROFESSION        = 3,
    GARR_ABILITY_CATEGORY_OTHER             = 4,
    GARR_ABILITY_CATEGORY_INCREASED_REWARDS = 5,
    GARR_ABILITY_CATEGORY_MISSION_DURATION  = 6,
    GARR_ABILITY_CATEGORY_ENV_PREFERENCE    = 7
};

enum GarrBuildingCategory
{
    GARR_BUILDING_SMALL                     = 0,
    GARR_BUILDING_MEDIUM                    = 1,
    GARR_BUILDING_LARGE                     = 2,
    GARR_BUILDING_SPECIAL                   = 3
};

enum GarrPlotCategory
{
    GARR_PLOT_SMALL                         = 0,
    GARR_PLOT_MEDIUM                        = 1,
    GARR_PLOT_LARGE                         = 2,
    GARR_PLOT_MINE                          = 3,
    GARR_PLOT_FARM                          = 4,
    GARR_PLOT_FISHING_HUT                   = 5,
    GARR_PLOT_PET_MENAGERIE                 = 6
};

enum GarrMissionCategory
{
    GARR_MISSION_COMBAT                     = 0,
    GARR_MISSION_GENERIC                    = 1,
    GARR_MISSION_SALVAGE                    = 2,
    GARR_MISSION_LOGISTICS                  = 3,
    GARR_MISSION_WILDLIFE                   = 4,
    GARR_MISSION_TRADING                    = 5,
    GARR_MISSION_CONSTRUCTION               = 6,
    GARR_MISSION_PROVISION                  = 7,
    GARR_MISSION_RECRUITMENT                = 8,
    GARR_MISSION_TRAINING                   = 9,
    GARR_MISSION_PATROL                     = 10,
    GARR_MISSION_RESEARCH                   = 11,
    GARR_MISSION_DEFENSE                    = 12,
    GARR_MISSION_EXPLORATION                = 13,
    GARR_MISSION_SIEGE                      = 14,
    GARR_MISSION_ALCHEMY                    = 15,
    GARR_MISSION_BLACKSMITHING              = 16,
    GARR_MISSION_ENCHANTING                 = 17,
    GARR_MISSION_ENGINEERING                = 18,
    GARR_MISSION_INSRIPTION                 = 19,
    GARR_MISSION_JEWELCRAFTING              = 20,
    GARR_MISSION_LEATHERWORKING             = 21,
    GARR_MISSION_TAILORING                  = 22,
    GARR_MISSION_TREASURE                   = 23,
    GARR_MISSION_PET_BATTLE                 = 24
};

#endif
