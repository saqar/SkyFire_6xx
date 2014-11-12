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

#include "BattlePayMgr.h"

void WorldSession::HandletBattlePayGetProductListResponseData()
{
    uint32 BattlePayDistributionObjectCount;
    uint32 BattlePayProductGroupCount;
    uint32 BattlePayShopEntryCount;

    uint32 Ordering;
    uint32 IconFileDataID;
    uint32 EntryID;
    uint32 GroupID;
    uint32 ProductID;
    uint8 DisplayType;
    uint32 Flags;
    uint8 NameSize;
    uint8 BannerType;
    bool HasBattlepayDisplayInfo;
    bool HasCreatureDisplayInfoID;

    WorldPacket data(SMSG_BATTLE_PAY_GET_PRODUCT_LIST_RESPONSE, 250); // TODO: fix this shit :S

    data << uint32(0); // Result
    data << uint32(0); // CurrencyID

    data << BattlePayDistributionObjectCount;
    data << BattlePayProductGroupCount;
    data << BattlePayShopEntryCount;

    //for (uint8 i = 0; i < BattlePayDistributionObjectCount; i++)
    //    BattlePayMgr::GetBattlePayProduct(data);

    for (uint8 i = 0; i < BattlePayProductGroupCount; i++)
    {
        data << GroupID;
        data << IconFileDataID;
        data << DisplayType;
        data << Ordering;
        data.WriteBits(NameSize, 8);
        data.WriteString("Name"); // bits4
    }

    for (uint8 i = 0; i < BattlePayShopEntryCount; i++)
    {
        data << EntryID;
        data << GroupID;
        data << ProductID;
        data << Ordering;
        data << Flags;
        data << BannerType;
        data.WriteBit(HasBattlepayDisplayInfo);
        //if (HasBattlepayDisplayInfo)
        //    BattlePayMgr::ReadBattlepayDisplayInfo(data);
    }
}

void BattlePayMgr::GetBattlePayProduct(WorldPacket& data)
{
    data << ProductID;
    data << NormalPriceFixedPoint;
    data << CurrentPriceFixedPoint;
    data << BattlepayProductItemCount;
    data << Type;
    data << Flags;

    for (int j = 0; j < BattlepayProductItemCount; j++)
    {
        data << uint32(0); // ID
        data << uint32(0); // ItemID
        data << uint32(0); // Quantity
        data.WriteBit(HasBattlepayDisplayInfo);
        data.WriteBit(HasPet);
        data.WriteBit(HasBATTLEPETRESULT);
        if (HasBATTLEPETRESULT)
            data.WriteBits(0, 4); // PetResult
        if (HasBattlepayDisplayInfo)
            BattlePayMgr::ReadBattlepayDisplayInfo(data);
    }

    data.WriteBits(0, 2); // ChoiceType
    data.WriteBit(HasBattlepayDisplayInfo);
    if (HasBattlepayDisplayInfo)
        BattlePayMgr::ReadBattlepayDisplayInfo(data);
}

void BattlePayMgr::ReadBattlepayDisplayInfo(WorldPacket& data)
{
    data.WriteBit(HasCreatureDisplayInfoID);
    data.WriteBit(HasFileDataID);
    data.WriteBits(0, 10); // bits16
    data.WriteBits(0, 10); // bits529
    data.WriteBits(0, 13); // bits1042
    data.WriteBit(HasFlags);
    if (HasCreatureDisplayInfoID)
        data << CreatureDisplayInfoID;
    if (HasFileDataID)
        data << FileDataID;
    data.WriteString("Name1"); // bits16
    data.WriteString("Name2"); // bits529
    data.WriteString("Name3"); // bits1042
    if (HasFlags)
        data << Flags;
}