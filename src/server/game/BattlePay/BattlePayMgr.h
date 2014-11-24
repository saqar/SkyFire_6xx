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

#include <ace/Singleton.h>

#include "Common.h"
#include "Opcodes.h"
#include "WorldSession.h"

#define MAX_BATTLE_PAY_PRODUCT_TITLE_SIZE       50
#define MAX_BATTLE_PAY_PRODUCT_DESCRIPTION_SIZE 500
#define MAX_BATTLE_PAY_GROUP_NAME_SIZE          16

enum BattlePayCurrency
{
    BATTLE_PAY_CURRENCY_DOLLAR              = 1,
    BATTLE_PAY_CURRENCY_POUND               = 2,
    BATTLE_PAY_CURRENCY_BATTLE_COIN         = 3,
    BATTLE_PAY_CURRENCY_EURO                = 4,
    BATTLE_PAY_CURRENCY_TEST                = 16,
    BATTLE_PAY_CURRENCY_END
};

enum BattlePayGroupType
{
    BATTLE_PAY_GROUP_TYPE_GRID              = 0,
    BATTLE_PAY_GROUP_TYPE_SINGLE            = 1,
    BATTLE_PAY_GROUP_TYPE_END
};

enum BattlePayBannerType
{
    BATTLE_PAY_BANNER_TYPE_FEATURED         = 0,
    BATTLE_PAY_BANNER_TYPE_NEW              = 2,
    BATTLE_PAY_BANNER_TYPE_END
};

struct BattlePayProduct
{
    BattlePayProduct(uint32 id, std::string title, std::string description, uint64 normalPrice, uint64 currentPrice, uint32 itemId,
        uint32 quantity, uint32 displayId, uint8 type, uint8 choiceType, uint32 flags)
    : Id(id), Title(title), Description(description), NormalPrice(normalPrice), CurrentPrice(currentPrice), ItemId(itemId), Quantity(quantity),
        DisplayId(displayId), Type(type), ChoiceType(choiceType), Flags(flags) { }

    uint32 Id;
    std::string Title;
    std::string Description;
    uint64 NormalPrice;
    uint32 CurrentPrice;
    uint32 ItemId;
    uint32 Quantity;
    uint32 DisplayId;
    uint8 Type;
    uint8 ChoiceType;
    uint32 Flags;
};

struct BattlePayGroup
{
    BattlePayGroup(uint32 id, uint32 order, std::string name, uint32 icon, uint8 type)
        : Id(id), Order(order), Name(name), Icon(icon), Type(type) { }

public:
    uint32 Id;
    uint32 Order;
    std::string Name;
    uint32 Icon;
    uint8 Type;
};

struct BattlePayShopEntry
{
    BattlePayShopEntry(uint32 id, uint32 order, uint32 groupId, uint32 productId, uint32 flags, uint8 banner)
        : Id(id), Order(order), GroupId(groupId), ProductId(productId), Flags(flags), Banner(banner) { }

public:
    uint32 Id;
    uint32 Order;
    uint32 GroupId;
    uint32 ProductId;
    uint32 Flags;
    uint8 Banner;
};

typedef std::set<BattlePayProduct*> BattlePayProductSet;
typedef std::set<BattlePayGroup*> BattlePayGroupSet;
typedef std::set<BattlePayShopEntry*> BattlePayShopEntryset;

class BattlePayMgr
{
    friend class ACE_Singleton<BattlePayMgr, ACE_Null_Mutex>;

public:
    BattlePayMgr()
        : m_enabled(false), m_currency(BATTLE_PAY_CURRENCY_DOLLAR) { }

    ~BattlePayMgr();

    bool IsStoreEnabled() { return m_enabled; }
    void SetEnableState(bool enabled) { m_enabled = enabled; }
    uint32 GetStoreCurrency() { return m_currency; }
    void SetStoreCurrency(uint32 currency) { m_currency = currency; }

    void LoadFromDb();

    void SendBattlePayDistributionList(WorldSession* session);
    void SendBattlePayPurchaseList(WorldSession* session);
    void SendBattlePayProductList(WorldSession* session);

private:
    BattlePayProductSet m_productStore;
    BattlePayGroupSet m_groupStore;
    BattlePayShopEntryset m_shopEntryStore;

    bool m_enabled;
    uint32 m_currency;

    bool HasProductId(uint32 productId);

    bool HasGroupId(uint32 groupId);
    bool HasGroupName(std::string name);

    void LoadProductsFromDb();
    void LoadGroupsFromDb();
    void LoadEntriesFromDb();
};

#define sBattlePayMgr ACE_Singleton<BattlePayMgr, ACE_Null_Mutex>::instance()

#endif
