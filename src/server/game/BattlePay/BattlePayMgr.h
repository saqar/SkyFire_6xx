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

#ifndef BATTLE_PAY_MGR_H
#define BATTLE_PAY_MGR_H

class BattlePayMgr
{
public:
    BattlePayMgr();
    ~BattlePayMgr();

    void GetBattlePayProduct(WorldPacket& data);
    void HandletBattlePayGetProductListResponse();
    void ReadBattlepayDisplayInfo(WorldPacket& data);

private:
    uint32 BattlePayDistributionObjectCount;
    uint32 BattlePayProductGroupCount;
    uint32 BattlePayShopEntryCount;
    uint32 BattlepayProductItemCount;
    uint64 NormalPriceFixedPoint;
    uint64 CurrentPriceFixedPoint;

    uint32 Ordering;
    uint32 IconFileDataID;
    uint32 EntryID;
    uint32 GroupID;
    uint32 ProductID;
    uint8 DisplayType;
    uint8 Type;
    uint32 Flags;
    uint8 NameSize;
    uint8 BannerType;
    uint32 CreatureDisplayInfoID;
    uint32 FileDataID;
    bool HasFileDataID;
    bool HasFlags;
    bool HasPet;
    bool HasBattlepayDisplayInfo;
    bool HasBATTLEPETRESULT;
    bool HasCreatureDisplayInfoID;
};

#endif
