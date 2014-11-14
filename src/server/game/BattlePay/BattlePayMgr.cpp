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
#include "Common.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

BattlePayMgr::~BattlePayMgr()
{
    for (BattlePayProductSet::iterator itr = m_productStore.begin(); itr != m_productStore.end(); itr++)
        delete *itr;

    for (BattlePayGroupSet::iterator itr = m_groupStore.begin(); itr != m_groupStore.end(); itr++)
        delete *itr;

    for (BattlePayShopEntryset::iterator itr = m_shopEntryStore.begin(); itr != m_shopEntryStore.end(); itr++)
        delete *itr;

    m_productStore.clear();
    m_groupStore.clear();
    m_shopEntryStore.clear();
}

void BattlePayMgr::LoadFromDb()
{
    LoadProductsFromDb();
    LoadGroupsFromDb();
    LoadEntriesFromDb();
}

void BattlePayMgr::LoadProductsFromDb()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT id, title, description, normalPrice, currentPrice, itemId, quantity, displayId, type, choiceType, flags FROM battle_pay_product");
    if (!result)
    {
        TC_LOG_INFO("sql.sql", ">> Loaded 0 Battle Pay store products, table `battle_pay_produce` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 id               = fields[0].GetUInt32();
        std::string title       = fields[1].GetString();
        std::string description = fields[2].GetString();
        uint64 normalPrice      = fields[3].GetUInt32();
        uint64 currentPrice     = fields[4].GetUInt64();
        uint32 itemId           = fields[5].GetUInt32();
        uint32 quantity         = fields[6].GetUInt32();
        uint32 displayId        = fields[7].GetUInt32();
        uint8 type              = fields[8].GetUInt8();
        uint8 choiceType        = fields[9].GetUInt8();
        uint32 flags            = fields[10].GetUInt32();

        if (HasProductId(id))
        {
            TC_LOG_ERROR("sql.sql", "Product id %u defined in `battle_pay_product` already exists, skipped!", id);
            continue;
        }

        if (title.size() > MAX_BATTLE_PAY_PRODUCT_TITLE_SIZE)
        {
            TC_LOG_ERROR("sql.sql", "Title for product id %u defined in `battle_pay_product` is too large (max %d), skipped!",
                id, MAX_BATTLE_PAY_PRODUCT_TITLE_SIZE);

            continue;
        }

        if (description.size() > MAX_BATTLE_PAY_PRODUCT_DESCRIPTION_SIZE)
        {
            TC_LOG_ERROR("sql.sql", "Description for product id %u defined in `battle_pay_product` is too large (max %d), skipped!",
                id, MAX_BATTLE_PAY_PRODUCT_DESCRIPTION_SIZE);

            continue;
        }

        if (currentPrice > normalPrice)
        {
            TC_LOG_ERROR("sql.sql", "Current price of product id %u defined in `battle_pay_product` is larger then the normal price. "
                "Current price has been updated to the normal price", id);

            currentPrice = normalPrice;
        }

        if (!sObjectMgr->GetItemTemplate(itemId))
        {
            TC_LOG_ERROR("sql.sql", "Item id %u for product id %u defined in `battle_pay_product` doesn't exist, skipped!", itemId, id);
            continue;
        }

        BattlePayProduct* product = new BattlePayProduct(id, title, description, normalPrice, currentPrice,
            itemId, quantity, displayId, type, choiceType, flags);

        m_productStore.insert(product);
        count++;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Battle Pay store products in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BattlePayMgr::LoadGroupsFromDb()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT id, `order`, name, icon, type FROM battle_pay_group");
    if (!result)
    {
        TC_LOG_INFO("sql.sql", ">> Loaded 0 Battle Pay store groups, table `battle_pay_group` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 id        = fields[0].GetUInt32();
        uint32 order     = fields[1].GetUInt32();
        std::string name = fields[2].GetString();
        uint32 icon      = fields[3].GetUInt32();
        uint8 type       = fields[4].GetUInt8();

        if (HasGroupId(id))
        {
            TC_LOG_ERROR("sql.sql", "Group id %u defined in `battle_pay_group` already exists, skipped!", id);
            continue;
        }

        if (HasGroupName(name))
        {
            TC_LOG_ERROR("sql.sql", "Group name %s defined in `battle_pay_group` is invalid because a group of the same name already exists, skipped!", name, id);
            continue;
        }

        if (name.size() > MAX_BATTLE_PAY_GROUP_NAME_SIZE)
        {
            TC_LOG_ERROR("sql.sql", "Name for group id %u defined in `battle_pay_group` is too large (max %d), skipped!",
                id, MAX_BATTLE_PAY_GROUP_NAME_SIZE);

            continue;
        }

        if (type >= BATTLE_PAY_GROUP_TYPE_END)
        {
            TC_LOG_ERROR("sql.sql", "Group id %u defined in `battle_pay_group` has invalid group type %u, skipped!", id, type);
            continue;
        }

        m_groupStore.insert(new BattlePayGroup(id, order, name, icon, type));
        count++;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Battle Pay store groups in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BattlePayMgr::LoadEntriesFromDb()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT id, `order`, groupId, productId, flags, banner FROM battle_pay_entry");
    if (!result)
    {
        TC_LOG_INFO("sql.sql", ">> Loaded 0 Battle Pay store entries, table `battle_pay_entry` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 id        = fields[0].GetUInt32();
        uint32 order     = fields[1].GetUInt32();
        uint32 groupId   = fields[2].GetUInt32();
        uint32 productId = fields[3].GetUInt32();
        uint32 flags     = fields[4].GetUInt32();
        uint32 banner    = fields[5].GetUInt32();

        if (!id)
        {
            TC_LOG_ERROR("sql.sql", "Id defined in `battle_pay_entry` is invalid because it has a value of 0, skipped!");
            continue;
        }

        if (!HasGroupId(groupId))
        {
            TC_LOG_ERROR("sql.sql", "Group id %u for entry id %u defined in `battle_pay_entry` is invalid because the group doesn't exists, skipped!", groupId, id);
            continue;
        }

        if (!HasProductId(productId))
        {
            TC_LOG_ERROR("sql.sql", "Product id %u for entry id %u defined in `battle_pay_entry` is invalid because the group doesn't exists, skipped!", productId, id);
            continue;
        }

        if (banner >= BATTLE_PAY_BANNER_TYPE_END)
        {
            TC_LOG_ERROR("sql.sql", "Entry id %u defined in `battle_pay_entry` has invalid banner type %u, skipped!", id, banner);
            continue;
        }

        m_shopEntryStore.insert(new BattlePayShopEntry(id, order, groupId, productId, flags, banner));
        count++;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Battle Pay store entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool BattlePayMgr::HasProductId(uint32 id)
{
    for (BattlePayProductSet::const_iterator citr = m_productStore.begin(); citr != m_productStore.end(); citr++)
        if ((*citr)->Id == id)
            return true;

    return false;
}

bool BattlePayMgr::HasGroupId(uint32 id)
{
    for (BattlePayGroupSet::const_iterator citr = m_groupStore.begin(); citr != m_groupStore.end(); citr++)
        if ((*citr)->Id == id)
            return true;

    return false;
}

bool BattlePayMgr::HasGroupName(std::string name)
{
    for (BattlePayGroupSet::const_iterator citr = m_groupStore.begin(); citr != m_groupStore.end(); citr++)
        if ((*citr)->Name == name)
            return true;

    return false;
}

void BattlePayMgr::SendBattlePayDistributionList(WorldSession* session)
{
    // TODO: finish this
    WorldPacket data(SMSG_BATTLE_PAY_GET_DISTRIBUTION_LIST_RESPONSE, 4 + 4);
    data << uint32(0);
    data << uint32(0);
    session->SendPacket(&data);
}

void BattlePayMgr::SendBattlePayPurchaseList(WorldSession* session)
{
    // TODO: finish this
    WorldPacket data(SMSG_BATTLE_PAY_GET_PURCHASE_LIST_RESPONSE, 4 + 4);
    data << uint32(0);
    data << uint32(0);
    session->SendPacket(&data);
}

void BattlePayMgr::SendBattlePayProductList(WorldSession* session)
{
    SendBattlePayDistributionList(session);
    SendBattlePayPurchaseList(session);

    WorldPacket data(SMSG_BATTLE_PAY_GET_PRODUCT_LIST_RESPONSE, 500);   // guess size
    data << uint32(0);
    data << uint32(m_currency);

    data << uint32(m_productStore.size());
    data << uint32(m_groupStore.size());
    data << uint32(m_shopEntryStore.size());

    for (BattlePayProductSet::const_iterator citr = m_productStore.begin(); citr != m_productStore.end(); citr++)
    {
        const BattlePayProduct* product = *citr;

        data << uint32(product->Id);
        data << uint64(product->NormalPrice);
        data << uint64(product->CurrentPrice);
        data << uint32(1);                                              // battlepayProductItemCount
        data << uint8(product->Type);
        data << uint32(product->Flags);

        {
            data << uint32(product->Id);
            data << uint32(product->ItemId);
            data << uint32(product->Quantity);

            data.WriteBit(0);                                           // hasBattlepayDisplayInfo
            data.WriteBit(0);                                           // hasPet
            data.WriteBit(0);                                           // hasBattlePetResult
            data.WriteBit(0);                                           // hasMount
            data.FlushBits();
        }

        data.WriteBits(product->ChoiceType, 2);
        data.WriteBit(1);                                               // hasBattlepayDisplayInfo
        data.FlushBits();

        {
            data.WriteBit(1);                                           // hasCreatureDisplayInfoID
            data.WriteBit(0);                                           // hasFileDataID
            data.WriteBits(product->Title.size(), 10);
            data.WriteBits(0, 10);
            data.WriteBits(product->Description.size(), 13);
            data.WriteBit(0);                                           // hasDisplayFlags
            data.FlushBits();

            data << uint32(product->DisplayId);

            data.WriteString(product->Title);
            data.WriteString(product->Description);
        }
    }

    for (BattlePayGroupSet::const_iterator citr = m_groupStore.begin(); citr != m_groupStore.end(); citr++)
    {
        const BattlePayGroup* group = *citr;

        data << uint32(group->Id);
        data << uint32(group->Icon);
        data << uint8(group->Type);
        data << uint32(group->Order);

        data.WriteBits(group->Name.size(), 8);          // client reads size as 8 bits
        data.WriteString(group->Name);
    }

    for (BattlePayShopEntryset::const_iterator citr = m_shopEntryStore.begin(); citr != m_shopEntryStore.end(); citr++)
    {
        const BattlePayShopEntry* entry = *citr;

        data << uint32(entry->Id);
        data << uint32(entry->GroupId);
        data << uint32(entry->ProductId);
        data << uint32(entry->Order);
        data << uint32(entry->Flags);                   // more research required
        data << uint8(entry->Banner);

        data.WriteBit(0);                               // entry overide display data currently ignored
        data.FlushBits();
    }

    session->SendPacket(&data);
}
