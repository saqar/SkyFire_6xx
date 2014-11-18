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

#include "QuestDef.h"
#include "GossipDef.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Formulas.h"

GossipMenu::GossipMenu()
{
    _menuId = 0;
}

GossipMenu::~GossipMenu()
{
    ClearMenu();
}

void GossipMenu::AddMenuItem(int32 menuItemId, uint8 icon, std::string const& message, uint32 sender, uint32 action, std::string const& boxMessage, uint32 boxMoney, bool coded /*= false*/)
{
    ASSERT(_menuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    // Find a free new id - script case
    if (menuItemId == -1)
    {
        menuItemId = 0;
        if (!_menuItems.empty())
        {
            for (GossipMenuItemContainer::const_iterator itr = _menuItems.begin(); itr != _menuItems.end(); ++itr)
            {
                if (int32(itr->first) > menuItemId)
                    break;

                menuItemId = itr->first + 1;
            }
        }
    }

    GossipMenuItem& menuItem = _menuItems[menuItemId];

    menuItem.MenuItemIcon    = icon;
    menuItem.Message         = message;
    menuItem.IsCoded         = coded;
    menuItem.Sender          = sender;
    menuItem.OptionType      = action;
    menuItem.BoxMessage      = boxMessage;
    menuItem.BoxMoney        = boxMoney;
}

/**
 * @name AddMenuItem
 * @brief Adds a localized gossip menu item from db by menu id and menu item id.
 * @param menuId Gossip menu id.
 * @param menuItemId Gossip menu item id.
 * @param sender Identifier of the current menu.
 * @param action Custom action given to OnGossipHello.
 */
void GossipMenu::AddMenuItem(uint32 menuId, uint32 menuItemId, uint32 sender, uint32 action)
{
    /// Find items for given menu id.
    GossipMenuItemsMapBounds bounds = sObjectMgr->GetGossipMenuItemsMapBounds(menuId);
    /// Return if there are none.
    if (bounds.first == bounds.second)
        return;

    /// Iterate over each of them.
    for (GossipMenuItemsContainer::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
    {
        /// Find the one with the given menu item id.
        if (itr->second.OptionIndex != menuItemId)
            continue;

        /// Store texts for localization.
        std::string strOptionText, strBoxText;
        BroadcastText const* optionBroadcastText = sObjectMgr->GetBroadcastText(itr->second.OptionBroadcastTextId);
        BroadcastText const* boxBroadcastText = sObjectMgr->GetBroadcastText(itr->second.BoxBroadcastTextId);

        /// OptionText
        if (optionBroadcastText)
            strOptionText = optionBroadcastText->GetText(GetLocale());
        else
            strOptionText = itr->second.OptionText;

        /// BoxText
        if (boxBroadcastText)
            strBoxText = boxBroadcastText->GetText(GetLocale());
        else
            strBoxText = itr->second.BoxText;

        /// Check need of localization.
        if (GetLocale() > LOCALE_enUS)
        {
            if (!optionBroadcastText)
            {
                /// Find localizations from database.
                if (GossipMenuItemsLocale const* gossipMenuLocale = sObjectMgr->GetGossipMenuItemsLocale(MAKE_PAIR32(menuId, menuItemId)))
                    ObjectMgr::GetLocaleString(gossipMenuLocale->OptionText, GetLocale(), strOptionText);
            }

            if (!boxBroadcastText)
            {
                /// Find localizations from database.
                if (GossipMenuItemsLocale const* gossipMenuLocale = sObjectMgr->GetGossipMenuItemsLocale(MAKE_PAIR32(menuId, menuItemId)))
                    ObjectMgr::GetLocaleString(gossipMenuLocale->BoxText, GetLocale(), strBoxText);
            }
        }

        /// Add menu item with existing method. Menu item id -1 is also used in ADD_GOSSIP_ITEM macro.
        AddMenuItem(-1, itr->second.OptionIcon, strOptionText, sender, action, strBoxText, itr->second.BoxMoney, itr->second.BoxCoded);
    }
}

void GossipMenu::AddGossipMenuItemData(uint32 menuItemId, uint32 gossipActionMenuId, uint32 gossipActionPoi)
{
    GossipMenuItemData& itemData = _menuItemData[menuItemId];

    itemData.GossipActionMenuId  = gossipActionMenuId;
    itemData.GossipActionPoi     = gossipActionPoi;
}

uint32 GossipMenu::GetMenuItemSender(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.Sender;
}

uint32 GossipMenu::GetMenuItemAction(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.OptionType;
}

bool GossipMenu::IsMenuItemCoded(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return false;

    return itr->second.IsCoded;
}

void GossipMenu::ClearMenu()
{
    _menuItems.clear();
    _menuItemData.clear();
}

PlayerMenu::PlayerMenu(WorldSession* session) : _session(session)
{
    if (_session)
        _gossipMenu.SetLocale(_session->GetSessionDbLocaleIndex());
}

PlayerMenu::~PlayerMenu()
{
    ClearMenus();
}

void PlayerMenu::ClearMenus()
{
    _gossipMenu.ClearMenu();
    _questMenu.ClearMenu();
}

void PlayerMenu::SendGossipMenu(uint32 titleTextId, uint64 objectGuid) const
{
    std::string updatedQuestTitles[0x20];
    ObjectGuid guid = objectGuid;

    WorldPacket data(SMSG_GOSSIP_MESSAGE, 150);             // guess size
    data << guid;
    data << int32(_gossipMenu.GetMenuId());                 // new 2.4.0
    data << int32(0);                                       // FriendshipFactionID
    data << int32(titleTextId);
    data << uint32(_gossipMenu.GetMenuItemCount());
    data << uint32(_questMenu.GetMenuItemCount());

    for (GossipMenuItemContainer::const_iterator itr = _gossipMenu.GetMenuItems().begin(); itr != _gossipMenu.GetMenuItems().end(); ++itr)
    {
        GossipMenuItem const& item = itr->second;

        data << int32(itr->first);
        data << uint8(item.MenuItemIcon);
        data << int8(item.IsCoded);                         // makes pop up box password
        data << int32(item.BoxMoney);                       // money required to open menu, 2.0.3

        data.WriteBits(item.Message.size(), 12);
        data.WriteBits(item.BoxMessage.size(), 12);
        data.FlushBits();

        data.WriteString(item.Message);                     // text for gossip item
        data.WriteString(item.BoxMessage);                  // accept text (related to money) pop up box, 2.0
    }

    // Store this instead of checking the Singleton every loop iteration
    bool questLevelInTitle = sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS);

    for (uint8 i = 0; i < _questMenu.GetMenuItemCount(); ++i)
    {
        QuestMenuItem const& item = _questMenu.GetItem(i);
        Quest const* quest = sObjectMgr->GetQuestTemplate(item.QuestId);
        std::string title = quest->GetTitle();

        int32 locale = _session->GetSessionDbLocaleIndex();
        if (locale >= 0)
            if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(item.QuestId))
                ObjectMgr::GetLocaleString(localeData->Title, locale, title);

        if (questLevelInTitle)
            AddQuestLevelToTitle(title, quest->GetQuestLevel());

        data << int32(item.QuestId);
        data << int32(item.QuestIcon);
        data << int32(quest->GetQuestLevel());
        data << int32(quest->GetFlags());
        data << int32(quest->GetFlags2());

        data.WriteBit(quest->IsRepeatable());
        data.WriteBits(quest->GetTitle().size(), 9);
        data.FlushBits();

        data.WriteString(quest->GetTitle());
    }

    _session->SendPacket(&data);
}

void PlayerMenu::SendCloseGossip() const
{
    WorldPacket data(SMSG_GOSSIP_COMPLETE, 0);
    _session->SendPacket(&data);
}

void PlayerMenu::SendPointOfInterest(uint32 poiId) const
{
    PointOfInterest const* poi = sObjectMgr->GetPointOfInterest(poiId);
    if (!poi)
    {
        TC_LOG_ERROR("sql.sql", "Request to send non-existing POI (Id: %u), ignored.", poiId);
        return;
    }

    std::string iconText = poi->icon_name;
    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
        if (PointOfInterestLocale const* localeData = sObjectMgr->GetPointOfInterestLocale(poiId))
            ObjectMgr::GetLocaleString(localeData->IconName, locale, iconText);

    WorldPacket data(SMSG_GOSSIP_POI, 3 + 4 + 4 + 4 + 4 + iconText.size());
    data.WriteBits(poi->flags, 14);
    data.WriteBits(iconText.size(), 6);
    data.FlushBits();

    data << float(poi->x);
    data << float(poi->y);
    data << int32(poi->icon);
    data << int32(poi->data);
    data << iconText;

    _session->SendPacket(&data);
}

/*********************************************************/
/***                    QUEST SYSTEM                   ***/
/*********************************************************/

QuestMenu::QuestMenu()
{
    _questMenuItems.reserve(16);                                   // can be set for max from most often sizes to speedup push_back and less memory use
}

QuestMenu::~QuestMenu()
{
    ClearMenu();
}

void QuestMenu::AddMenuItem(uint32 QuestId, uint8 Icon)
{
    if (!sObjectMgr->GetQuestTemplate(QuestId))
        return;

    ASSERT(_questMenuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    QuestMenuItem questMenuItem;

    questMenuItem.QuestId        = QuestId;
    questMenuItem.QuestIcon      = Icon;

    _questMenuItems.push_back(questMenuItem);
}

bool QuestMenu::HasItem(uint32 questId) const
{
    for (QuestMenuItemList::const_iterator i = _questMenuItems.begin(); i != _questMenuItems.end(); ++i)
        if (i->QuestId == questId)
            return true;

    return false;
}

void QuestMenu::ClearMenu()
{
    _questMenuItems.clear();
}

void PlayerMenu::SendQuestGiverQuestList(QEmote eEmote, const std::string& Title, uint64 npcGuid)
{
    ObjectGuid guid = npcGuid;
    uint32 count = 0;

    WorldPacket data(SMSG_QUESTGIVER_QUEST_LIST, 100);      // guess size
    data << guid;
    data << int32(eEmote._Emote);
    data << int32(eEmote._Delay);

    size_t countPos = data.wpos();
    data << uint32(count);

    // Store this instead of checking the Singleton every loop iteration
    bool questLevelInTitle = sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS);

    for (uint32 i = 0; i < _questMenu.GetMenuItemCount(); ++i)
    {
        QuestMenuItem const& qmi = _questMenu.GetItem(i);

        uint32 questID = qmi.QuestId;

        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questID))
        {
            ++count;
            std::string title = quest->GetTitle();

            int32 locale = _session->GetSessionDbLocaleIndex();
            if (locale >= 0)
                if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(questID))
                    ObjectMgr::GetLocaleString(localeData->Title, locale, title);

            if (questLevelInTitle)
                AddQuestLevelToTitle(title, quest->GetQuestLevel());

            data << int32(questID);
            data << int32(qmi.QuestIcon);
            data << int32(quest->GetQuestLevel());
            data << int32(quest->GetFlags());
            data << int32(quest->GetFlags2());

            data.WriteBit(quest->IsRepeatable());
            data.WriteBits(quest->GetTitle().size(), 9);
            data.FlushBits();

            data.WriteString(quest->GetTitle());
        }
    }

    data.WriteBits(Title.size(), 11);
    data.FlushBits();

    data.WriteString(Title);

    data.put<uint32>(countPos, count);
    _session->SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_QUEST_LIST NPC Guid=%u", GUID_LOPART(npcGuid));
}

void PlayerMenu::SendQuestGiverStatus(uint32 questStatus, Object* questGiver) const
{
    // HACK FIX!
    WorldPacket data(/*SMSG_QUESTGIVER_STATUS*/SMSG_QUESTGIVER_STATUS_MULTIPLE, 4 + 16 + 4);
    data << uint32(1);
    data << questGiver->GetGUID128();
    data << uint32(questStatus);

    _session->SendPacket(&data);
    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_STATUS NPC Guid=%u, status=%u", GUID_LOPART(questGiver->GetGUID()), questStatus);
}

void PlayerMenu::SendQuestGiverQuestDetails(Quest const* quest, uint64 npcGuid, bool activateAccept) const
{
    std::string questTitle           = quest->GetTitle();
    std::string questDetails         = quest->GetDetails();
    std::string questObjectives      = quest->GetObjectives();
    std::string questGiverTextWindow = quest->GetQuestGiverTextWindow();
    std::string questGiverTargetName = quest->GetQuestGiverTargetName();
    std::string questTurnTextWindow  = quest->GetQuestTurnTextWindow();
    std::string questTurnTargetName  = quest->GetQuestTurnTargetName();

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->Details, locale, questDetails);
            ObjectMgr::GetLocaleString(localeData->Objectives, locale, questObjectives);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTextWindow, locale, questGiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTargetName, locale, questGiverTargetName);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTextWindow, locale, questTurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTargetName, locale, questTurnTargetName);
        }
    }

    uint32 xpValue = quest->XPValue(_session->GetPlayer()) * sWorld->getRate(RATE_XP_QUEST);

    if (sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS))
        AddQuestLevelToTitle(questTitle, quest->GetQuestLevel());

    ObjectGuid guid = npcGuid;

    WorldPacket data(SMSG_QUESTGIVER_QUEST_DETAILS, 100);   // guess size
    data << guid;
    data << guid;                                           // should be ender Guid
    data << int32(quest->GetQuestId());
    data << int32(quest->GetRewardPackageItemId());
    data << int32(quest->GetQuestGiverPortrait());
    data << int32(quest->GetQuestTurnInPortrait());
    data << int32(quest->GetFlags());
    data << int32(quest->GetFlags2());
    data << int32(quest->GetSuggestedPlayers());
    data << uint32(0);                                      // LearnSpells

    WriteQuestRewardsData(quest, data, xpValue);

    data << uint32(QUEST_EMOTE_COUNT);
    data << uint32(quest->m_questObjectives.size());

    /*for (uint i = 0; i < spellCount; i++)
        data << int32(spell);*/

    for (int i = 0; i < QUEST_EMOTE_COUNT; i++)
    {
        data << int32(quest->DetailsEmote[i]);
        data << uint32(quest->DetailsEmoteDelay[i]);
    }

    for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); citr++)
    {
        QuestObjective const* questObjective = *citr;

        data << int32(questObjective->Id);
        data << int32(questObjective->ObjectId);
        data << int32(questObjective->Amount);
        data << uint8(questObjective->Type);
    }

    data.WriteBits(questTitle.size(), 9);
    data.WriteBits(questObjectives.size(), 12);
    data.WriteBits(questDetails.size(), 12);
    data.WriteBits(questGiverTextWindow.size(), 10);
    data.WriteBits(questGiverTargetName.size(), 8);
    data.WriteBits(questTurnTextWindow.size(), 10);
    data.WriteBits(questTurnTargetName.size(), 8);
    data.WriteBit(0);
    data.WriteBit(0);
    data.WriteBit(0);
    data.FlushBits();

    data.WriteString(questTitle);
    data.WriteString(questObjectives);
    data.WriteString(questDetails);
    data.WriteString(questGiverTextWindow);
    data.WriteString(questGiverTargetName);
    data.WriteString(questTurnTextWindow);
    data.WriteString(questTurnTargetName);

    _session->SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_QUEST_DETAILS NPCGuid=%u, questid=%u", GUID_LOPART(npcGuid), quest->GetQuestId());
}

void PlayerMenu::SendQuestQueryResponse(Quest const* quest) const
{
    std::string questTitle = quest->GetTitle();
    std::string questDetails = quest->GetDetails();
    std::string questObjectives = quest->GetObjectives();
    std::string questEndText = quest->GetEndText();
    std::string questCompletedText = quest->GetCompletedText();
    std::string questGiverTextWindow = quest->GetQuestGiverTextWindow();
    std::string questGiverTargetName = quest->GetQuestGiverTargetName();
    std::string questTurnTextWindow = quest->GetQuestTurnTextWindow();
    std::string questTurnTargetName = quest->GetQuestTurnTargetName();

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->Details, locale, questDetails);
            ObjectMgr::GetLocaleString(localeData->Objectives, locale, questObjectives);
            ObjectMgr::GetLocaleString(localeData->EndText, locale, questEndText);
            ObjectMgr::GetLocaleString(localeData->CompletedText, locale, questCompletedText);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTextWindow, locale, questGiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTargetName, locale, questGiverTargetName);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTextWindow, locale, questTurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTargetName, locale, questTurnTargetName);
        }
    }

    if (sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS))
        AddQuestLevelToTitle(questTitle, quest->GetQuestLevel());

    bool hiddenReward = quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS);

    WorldPacket data(SMSG_QUEST_QUERY_RESPONSE, 100);   // guess size
    data << uint32(quest->GetQuestId());

    data.WriteBit(1);                                   // has data
    data.FlushBits();

    data << int32(quest->GetQuestId());
    data << int32(quest->GetType());
    data << int32(quest->GetQuestLevel());
    data << int32(quest->GetRewardPackageItemId());
    data << int32(quest->GetMinLevel());
    data << int32(quest->GetZoneOrSort());
    data << int32(0);                                   // QuestInfoID
    data << int32(quest->GetSuggestedPlayers());
    data << int32(quest->GetNextQuestId());
    data << int32(quest->GetXPId());
    data << float(0);                                   // unknown
    data << int32(quest->GetRewMoney());
    data << int32(0);                                   // RewardMoneyDifficulty
    data << float(0);                                   // unknown
    data << int32(quest->GetRewMoneyMaxLevel());
    data << int32(quest->GetRewSpell());
    data << int32(quest->GetRewSpellCast());
    data << int32(quest->GetRewHonorAddition());
    data << float(quest->GetRewHonorMultiplier());
    data << int32(quest->GetSrcItemId());
    data << int32(quest->GetFlags());
    data << int32(quest->GetFlags2());

    for (uint8 i = 0; i < QUEST_REWARDS_COUNT; i++)
    {
        data << int32(hiddenReward ? 0 : quest->RewardItemId[i]);
        data << int32(hiddenReward ? 0 : quest->RewardItemIdCount[i]);
        data << int32(quest->RequiredSourceItemId[i]);
        data << int32(quest->RequiredSourceItemCount[i]);
    }

    for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; i++)
    {
        data << int32(hiddenReward ? 0 : quest->RewardChoiceItemId[i]);
        data << int32(hiddenReward ? 0 : quest->RewardChoiceItemCount[i]);

        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardChoiceItemId[i]))
            data << int32(hiddenReward ? 0 : itemTemplate->DisplayInfoID);
        else
            data << int32(0);
    }

    data << int32(quest->GetPointMapId());
    data << float(quest->GetPointX());
    data << float(quest->GetPointY());
    data << int32(quest->GetPointOpt());
    data << int32(quest->GetCharTitleId());
    data << int32(quest->GetBonusTalents());
    data << int32(quest->GetRewArenaPoints());
    data << int32(quest->GetRewardSkillId());
    data << int32(quest->GetRewardSkillPoints());
    data << int32(quest->GetQuestGiverPortrait());
    data << int32(quest->GetQuestTurnInPortrait());

    for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; i++)
    {
        data << int32(hiddenReward ? 0 : quest->RewardFactionId[i]);
        data << int32(hiddenReward ? 0 : quest->RewardFactionValueId[i]);
        data << int32(hiddenReward ? 0 : quest->RewardFactionValueIdOverride[i]);
    }

    data << int32(quest->GetRewardReputationMask());

    for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; i++)
    {
        data << int32(hiddenReward ? 0 : quest->RewardCurrencyId[i]);
        data << int32(hiddenReward ? 0 : quest->RewardChoiceItemCount[i]);
    }

    data << int32(quest->GetSoundAccept());
    data << int32(quest->GetSoundTurnIn());
    data << int32(0);                                   // AreaGroupID
    data << int32(quest->GetLimitTime());
    data << uint32(quest->m_questObjectives.size());
    data << int32(quest->m_questObjectives.size());

    for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); citr++)
    {
        QuestObjective const* questObjective = *citr;

        std::string descriptionText = questObjective->Description;
        if (locale > 0)
            if (QuestObjectiveLocale const* questObjectiveLocale = sObjectMgr->GetQuestObjectiveLocale(questObjective->Id))
                ObjectMgr::GetLocaleString(questObjectiveLocale->Description, locale, descriptionText);

        data << int32(questObjective->Id);
        data << uint8(questObjective->Type);
        data << int8(questObjective->Index);
        data << int32(questObjective->ObjectId);
        data << int32(questObjective->Amount);
        data << int32(questObjective->Flags);
        data << float(0);                               // unknown
        data << uint32(questObjective->VisualEffects.size());

        for (VisualEffectVec::const_iterator citrEffects = questObjective->VisualEffects.begin(); citrEffects != questObjective->VisualEffects.end(); citrEffects++)
            data << int32(*citrEffects);

        data.WriteBits(descriptionText.size(), 8);
        data.WriteString(descriptionText);
    }

    data.WriteBits(questTitle.size(), 9);
    data.WriteBits(questObjectives.size(), 12);
    data.WriteBits(questDetails.size(), 12);
    data.WriteBits(questEndText.size(), 9);
    data.WriteBits(questGiverTextWindow.size(), 10);
    data.WriteBits(questGiverTargetName.size(), 8);
    data.WriteBits(questTurnTextWindow.size(), 10);
    data.WriteBits(questTurnTargetName.size(), 8);
    data.WriteBits(questCompletedText.size(), 11);
    data.FlushBits();

    data.WriteString(questTitle);
    data.WriteString(questObjectives);
    data.WriteString(questDetails);
    data.WriteString(questEndText);
    data.WriteString(questGiverTextWindow);
    data.WriteString(questGiverTargetName);
    data.WriteString(questTurnTextWindow);
    data.WriteString(questTurnTargetName);
    data.WriteString(questCompletedText);

    _session->SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUEST_QUERY_RESPONSE questid=%u", quest->GetQuestId());
}

void PlayerMenu::SendQuestGiverOfferReward(Quest const* quest, uint64 npcGuid, bool enableNext) const
{
    std::string questTitle = quest->GetTitle();
    std::string questOfferRewardText = quest->GetOfferRewardText();
    std::string questGiverTextWindow = quest->GetQuestGiverTextWindow();
    std::string questGiverTargetName = quest->GetQuestGiverTargetName();
    std::string questTurnTextWindow = quest->GetQuestTurnTextWindow();
    std::string questTurnTargetName = quest->GetQuestTurnTargetName();

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->OfferRewardText, locale, questOfferRewardText);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTextWindow, locale, questGiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTargetName, locale, questGiverTargetName);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTextWindow, locale, questTurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTargetName, locale, questTurnTargetName);
        }
    }

    uint32 xpValue = quest->XPValue(_session->GetPlayer()) * sWorld->getRate(RATE_XP_QUEST);

    if (sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS))
        AddQuestLevelToTitle(questTitle, quest->GetQuestLevel());

    ObjectGuid guid = npcGuid;

    WorldPacket data(SMSG_QUESTGIVER_OFFER_REWARD, 100);    // guess size
    data << guid;
    data << int32(0);                                       // QuestGiverCreatureID
    data << int32(quest->GetQuestId());
    data << int32(quest->GetFlags());
    data << int32(quest->GetFlags2());
    data << int32(quest->GetSuggestedPlayers());

    WriteQuestRewardsData(quest, data, xpValue);

    data << uint32(QUEST_EMOTE_COUNT);

    for (int i = 0; i < QUEST_EMOTE_COUNT; i++)
    {
        data << int32(quest->OfferRewardEmote[i]);
        data << uint32(quest->OfferRewardEmoteDelay[i]);
    }

    data.WriteBit(enableNext);
    data.FlushBits();

    data << int32(quest->GetQuestTurnInPortrait());
    data << int32(quest->GetQuestGiverPortrait());
    data << int32(quest->GetRewardPackageItemId());

    data.WriteBits(questTitle.size(), 9);
    data.WriteBits(questOfferRewardText.size(), 12);
    data.WriteBits(questGiverTextWindow.size(), 10);
    data.WriteBits(questGiverTargetName.size(), 8);
    data.WriteBits(questTurnTextWindow.size(), 10);
    data.WriteBits(questTurnTargetName.size(), 8);
    data.FlushBits();

    data.WriteString(questTitle);
    data.WriteString(questOfferRewardText);
    data.WriteString(questGiverTextWindow);
    data.WriteString(questGiverTargetName);
    data.WriteString(questTurnTextWindow);
    data.WriteString(questTurnTargetName);

    _session->SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_OFFER_REWARD NPCGuid=%u, questid=%u", GUID_LOPART(npcGuid), quest->GetQuestId());
}

void PlayerMenu::SendQuestGiverRequestItems(Quest const* quest, uint64 npcGuid, bool canComplete, bool closeOnCancel) const
{
    // We can always call to RequestItems, but this packet only goes out if there are actually
    // items.  Otherwise, we'll skip straight to the OfferReward

    uint32 itemCounter = quest->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_ITEM);
    if (!itemCounter && canComplete)
    {
        SendQuestGiverOfferReward(quest, npcGuid, true);
        return;
    }

    std::string questTitle = quest->GetTitle();
    std::string requestItemsText = quest->GetRequestItemsText();

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->RequestItemsText, locale, requestItemsText);
        }
    }

    uint32 requiredMoney = 0, itemCount = 0, currencyCount = 0;
    ByteBuffer currencyData, itemData;

    for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); citr++)
    {
        QuestObjective const* questObjective = *citr;
        switch (questObjective->Type)
        {
            case QUEST_OBJECTIVE_TYPE_ITEM:
            {
                itemData << int32(questObjective->ObjectId);
                itemData << int32(questObjective->Amount);

                itemCount++;
                break;
            }
            case QUEST_OBJECTIVE_TYPE_CURRENCY:
            {
                currencyData << int32(questObjective->ObjectId);
                currencyData << int32(questObjective->Amount);

                currencyCount++;
                break;
            }
            case QUEST_OBJECTIVE_TYPE_MONEY:
            {
                requiredMoney = questObjective->Amount;
                break;
            }
            default:
                break;
        }
    }

    if (sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS))
        AddQuestLevelToTitle(questTitle, quest->GetQuestLevel());

    ObjectGuid guid = npcGuid;
    uint32 currencyCounter = quest->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_CURRENCY);

    WorldPacket data(SMSG_QUESTGIVER_REQUEST_ITEMS, 16 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
        4 + 4 + 4 + 4 + itemCount * (4 + 4) + currencyCount * (4 + 4));

    data << guid;
    data << int32(0);                               // QuestGiverCreatureID
    data << int32(quest->GetQuestId());
    data << int32(0);                               // CompEmoteDelay
    data << int32(canComplete ? quest->GetCompleteEmote() : quest->GetIncompleteEmote());
    data << int32(quest->GetFlags());
    data << int32(quest->GetFlags2());
    data << int32(quest->GetSuggestedPlayers());
    data << int32(requiredMoney);
    data << uint32(itemCount);
    data << uint32(currencyCount);
    data << int32(canComplete ? 0xDF : 0xDB);       // StatusFlags
    data.append(itemData);
    data.append(currencyData);

    data.WriteBit(closeOnCancel);
    data.FlushBits();

    data.WriteBits(questTitle.size(), 9);
    data.WriteBits(requestItemsText.size(), 12);
    data.FlushBits();

    data.WriteString(questTitle);
    data.WriteString(requestItemsText);

    _session->SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS NPCGuid=%u, questid=%u", GUID_LOPART(npcGuid), quest->GetQuestId());
}

void PlayerMenu::AddQuestLevelToTitle(std::string &title, int32 level)
{
    // Adds the quest level to the front of the quest title
    // example: [13] Westfall Stew

    std::stringstream questTitlePretty;
    questTitlePretty << "[" << level << "] " << title;
    title = questTitlePretty.str();
}

void PlayerMenu::WriteQuestRewardsData(Quest const* quest, ByteBuffer &data, uint32 xpValue)
{
    data << int32(quest->GetRewChoiceItemsCount());
    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; i++)
    {
        data << int32(quest->RewardChoiceItemId[i]);
        data << int32(quest->RewardChoiceItemCount[i]);
    }

    data << int32(quest->GetRewItemsCount());
    for (int i = 0; i < QUEST_REWARDS_COUNT; i++)
    {
        data << int32(quest->RewardItemId[i]);
        data << int32(quest->RewardItemIdCount[i]);
    }

    data << int32(quest->GetRewMoney());
    data << int32(xpValue);
    data << int32(quest->GetCharTitleId());
    data << int32(quest->GetBonusTalents());

    data << int32(quest->GetRewardReputationMask());
    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; i++)
    {
        data << int32(quest->RewardFactionId[i]);
        data << int32(quest->RewardFactionValueId[i]);
        data << int32(quest->RewardFactionValueIdOverride[i]);
    }

    data << int32(quest->GetRewSpell());
    data << int32(quest->GetRewSpellCast());

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; i++)
    {
        data << int32(quest->RewardCurrencyId[i]);
        data << int32(quest->RewardCurrencyCount[i]);
    }

    data << int32(quest->GetRewardSkillId());
    data << int32(quest->GetRewardSkillPoints());

    data.WriteBit(0);                                        // unknown
    data.FlushBits();
}
