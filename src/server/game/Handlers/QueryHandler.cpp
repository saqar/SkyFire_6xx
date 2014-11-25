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

#include "AccountMgr.h"
#include "BattlenetAccountMgr.h"
#include "Common.h"
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"
#include "Config.h"

void WorldSession::SendNameQueryOpcode(ObjectGuid guid, uint32 VirtualRealmAddress)
{
    uint32 AccountID = sObjectMgr->GetPlayerAccountIdByGUID(guid);
    uint32 BnetAccountID = Battlenet::AccountMgr::GetIdByGameAccount(AccountID);

    Player* player = ObjectAccessor::FindPlayer(guid);
    CharacterNameData const* nameData = sWorld->GetCharacterNameData(GUID_LOPART(guid));

    WorldPacket data(SMSG_NAME_QUERY_RESPONSE, 500);
    data << uint8(!nameData);
    data << guid;

    if (!nameData)
    {
        SendPacket(&data);
        return;
    }

    data.WriteBit(0); //isDeleted ? Wod ?
    data.WriteBits(nameData->m_name.size(), 6);

    DeclinedName const* names = (player ? player->GetDeclinedNames() : NULL);
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        data.WriteBits(names ? names->name[i].size() : 0, 7);

    if (names)
        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data.WriteString(names->name[i]);

    data << BnetAccountID;
    data << AccountID;
    data << guid;

    data << uint32(VirtualRealmAddress);
    data << uint8(nameData->m_race);
    data << uint8(nameData->m_gender);
    data << uint8(nameData->m_class);
    data << uint8(nameData->m_level);
    data.WriteString(nameData->m_name);

    SendPacket(&data);
}

void WorldSession::HandleNameQueryOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    bool HasVirtualRealmAddress, HasNativeRealmAddress;
    uint32 VirtualRealmAddress, NativeRealmAddress;

    recvData >> guid;

    HasVirtualRealmAddress = recvData.ReadBit();
    if (HasVirtualRealmAddress)
        recvData >> VirtualRealmAddress;
    
    HasNativeRealmAddress = recvData.ReadBit();
    if (HasNativeRealmAddress)
        recvData >> NativeRealmAddress;

    // TC_LOG_INFO("network", "CMSG_NAME_QUERY %u", guid);

    SendNameQueryOpcode(guid, VirtualRealmAddress);
}

void WorldSession::SendRealmNameQueryOpcode(uint32 realmId)
{
    RealmNameMap::const_iterator iter = realmNameStore.find(realmId);

    bool found = iter != realmNameStore.end();
    std::string realmName = found ? iter->second : "";

    WorldPacket data(SMSG_REALM_NAME_QUERY_RESPONSE, 50);

    data << uint32(realmId);
    data << uint8(!found);
    if (found)
    {
        data.WriteBit(realmId == realmID);
        data.WriteBit(realmId == realmID);
        data.WriteBits(realmName.length(), 8);
        data.WriteBits(realmName.length(), 8);
        data.FlushBits();

        data.WriteString(realmName);
        data.WriteString(realmName);
    }

    SendPacket(&data);
}

void WorldSession::HandleRealmNameQueryOpcode(WorldPacket& recvPacket)
{
    uint32 realmId;
    recvPacket >> realmId;
    SendRealmNameQueryOpcode(realmId);
}

void WorldSession::HandleQueryTimeOpcode(WorldPacket & /*recvData*/)
{
    SendQueryTimeResponse();
}

void WorldSession::SendQueryTimeResponse()
{
    WorldPacket data(SMSG_QUERY_TIME_RESPONSE, 4+4);
    data << uint32(time(NULL));
    data << uint32(sWorld->GetNextDailyQuestsResetTime() - time(NULL));
    SendPacket(&data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleCreatureQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    recvData >> entry;

    WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 500);

    CreatureTemplate const* info = sObjectMgr->GetCreatureTemplate(entry);

    data << uint32(entry);
    data.WriteBit(info != 0);                                    // Has data
    data.FlushBits();

    if (info)
    {
        std::string Name, NameAlt, SubName, SubNameAlt;
        Name = info->Name;
        NameAlt = info->NameAlt;
        SubName = info->SubName;
        SubNameAlt = info->SubNameAlt;

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (CreatureLocale const* cl = sObjectMgr->GetCreatureLocale(entry))
            {
                ObjectMgr::GetLocaleString(cl->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(cl->NameAlt, loc_idx, NameAlt);
                ObjectMgr::GetLocaleString(cl->SubName, loc_idx, SubName);
                ObjectMgr::GetLocaleString(cl->SubNameAlt, loc_idx, SubNameAlt);
            }
        }

        TC_LOG_DEBUG("network", "WORLD: CMSG_CREATURE_QUERY '%s' - Entry: %u.", info->Name.c_str(), entry);
        data.WriteBits(SubName.length() ? SubName.length() + 1 : 0, 11);
        data.WriteBits(SubNameAlt.length() ? SubNameAlt.length() + 1 : 0, 11);
        data.WriteBits(info->IconName.length() ? info->IconName.length() + 1 : 0, 6);
        data.WriteBit(info->RacialLeader);

        for (int i = 0; i < 4; i++)
        {
            data.WriteBits(i == 0 ? Name.length() + 1 : 0, 11);
            data.WriteBits(i == 0 ? NameAlt.length() + 1 : 0, 11);
        }

        data.FlushBits();

        if (Name != "")
            data << Name;

        if (NameAlt != "")
            data << NameAlt;

        data << uint32(info->type_flags);                     // Flags
        data << uint32(info->type_flags2);                    // Flags2
        data << uint32(info->type);                           // CreatureType.dbc
        data << uint32(info->family);                         // CreatureFamily.dbc
        data << uint32(info->rank);                           // Creature Rank (elite, boss, etc)
        data << uint32(info->KillCredit[0]);                  // New in 3.1, kill credit
        data << uint32(info->KillCredit[1]);                  // New in 3.1, kill credit
        data << uint32(info->Modelid1);                       // Modelid1
        data << uint32(info->Modelid2);                       // Modelid2
        data << uint32(info->Modelid3);                       // Modelid3
        data << uint32(info->Modelid4);                       // Modelid4
        data << float(info->ModHealth);                       // Hp modifier
        data << float(info->ModMana);                         // Mana modifier
        data << uint32(MAX_CREATURE_QUEST_ITEMS);             // Quest items
        data << uint32(info->movementId);                     // CreatureMovementInfo.dbc
        data << uint32(info->expansion);                      // Expansion Required
        data << uint32(info->FlagQuest);                      // FlagQuest 6.x

        if (SubName != "")
            data << SubName;                                  // Subname

        if (SubNameAlt != "")
            data << SubNameAlt;                               // SubnameAlt

        if (info->IconName != "")
            data << info->IconName;                           // "Directions" for guard, string for Icons 2.3.0

        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            data << uint32(info->questItems[i]);              // ItemId[6], quest drop


        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
    else
        TC_LOG_DEBUG("network", "WORLD: CMSG_CREATURE_QUERY - NO CREATURE INFO! (ENTRY: %u)", entry);
    SendPacket(&data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    ObjectGuid guid;

    recvData >> entry;
    recvData >> guid;

    const GameObjectTemplate* info = sObjectMgr->GetGameObjectTemplate(entry);

    WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 150);
    data << uint32(entry);
    data.WriteBit(info != NULL);
    data.FlushBits();
    size_t pos = data.wpos();
    data << uint32(0);

    if (info)
    {
        std::string Name;
        std::string IconName;
        std::string CastBarCaption;

        Name = info->name;
        IconName = info->IconName;
        CastBarCaption = info->castBarCaption;

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (GameObjectLocale const* gl = sObjectMgr->GetGameObjectLocale(entry))
            {
                ObjectMgr::GetLocaleString(gl->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(gl->CastBarCaption, loc_idx, CastBarCaption);
            }
        }

        TC_LOG_DEBUG("network", "WORLD: CMSG_GAMEOBJECT_QUERY '%s' - Entry: %u. ", info->name.c_str(), entry);

        data << uint32(info->type);
        data << uint32(info->displayId);
        data << Name;
        data << uint8(0) << uint8(0) << uint8(0);           // name2, name3, name4
        data << IconName;                                   // 2.0.3, string. Icon name to use instead of default icon for go's (ex: "Attack" makes sword)
        data << CastBarCaption;                             // 2.0.3, string. Text will appear in Cast Bar when using GO (ex: "Collecting")
        data << info->unk1;                                 // 2.0.3, string

        data.append(info->raw.data, MAX_GAMEOBJECT_DATA);
        data << float(info->size);                          // go size

        data << uint8(MAX_GAMEOBJECT_QUEST_ITEMS);

        for (uint32 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
            data << uint32(info->questItems[i]);            // itemId[6], quest drop

        data << int32(info->unkInt32);                      // 4.x, unknown

        data.put(pos, uint32(data.wpos() - (pos + 4)));
        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
    else
    {
        TC_LOG_DEBUG("network", "WORLD: CMSG_GAMEOBJECT_QUERY - Missing gameobject info for (GUID: %u, ENTRY: %u)",
            GUID_LOPART((uint64)guid), entry);
        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }

    SendPacket(&data);
}

void WorldSession::HandleCorpseQueryOpcode(WorldPacket& /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_CORPSE_QUERY");

    Corpse* corpse = GetPlayer()->GetCorpse();

    if (!corpse)
    {
        WorldPacket data(SMSG_CORPSE_QUERY, 2 + (5 * 4) + 8);
        data.WriteBit(0); // Corpse not Found
        data.FlushBits();

        data << uint32(0);
        data << float(0);
        data << float(0);
        data << float(0);
        data << uint32(0);
        data << ObjectGuid(0);
        SendPacket(&data);
        return;
    }

    uint32 mapId = corpse->GetMapId();
    float x = corpse->GetPositionX();
    float y = corpse->GetPositionY();
    float z = corpse->GetPositionZ();
    uint32 corpseMapId = mapId;

    // if corpse at different map
    if (mapId != _player->GetMapId())
    {
        // search entrance map for proper show entrance
        if (MapEntry const* corpseMapEntry = sMapStore.LookupEntry(mapId))
        {
            if (corpseMapEntry->IsDungeon() && corpseMapEntry->entrance_map >= 0)
            {
                // if corpse map have entrance
                if (Map const* entranceMap = sMapMgr->CreateBaseMap(corpseMapEntry->entrance_map))
                {
                    mapId = corpseMapEntry->entrance_map;
                    x = corpseMapEntry->entrance_x;
                    y = corpseMapEntry->entrance_y;
                    z = entranceMap->GetHeight(GetPlayer()->GetPhaseMask(), x, y, MAX_HEIGHT);
                }
            }
        }
    }

    _player->SendCorpseReclaimDelay();

    ObjectGuid corpseGuid = corpse->GetGUID();

    WorldPacket data(SMSG_CORPSE_QUERY, 9 + 1 + (4 * 5));
    data.WriteBit(1); // Corpse Found
    data.FlushBits();
    
    data << mapId;
    data << x;
    data << y;
    data << z;
    data << corpseMapId;
    data << corpse->GetGUID128();
    SendPacket(&data);
}

void WorldSession::HandleNpcTextQueryOpcode(WorldPacket& recvData)
{
    uint32 textId;
    ObjectGuid guid;

    recvData >> textId;
    recvData >> guid;

    TC_LOG_DEBUG("network", "WORLD: CMSG_NPC_TEXT_QUERY ID '%u'", textId);

    GossipText const* pGossip = sObjectMgr->GetGossipText(textId);

    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 4 + 1 + 4 + 64);
    data << textId;

    data.WriteBit(1);                                   // has data
    data.FlushBits();

    data << uint32(64);                                 // size (8 * 4) * 2

    for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; i++)
        data << float(pGossip ? pGossip->Options[i].Probability : 0);

    for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; i++)
        data << uint32(pGossip ? pGossip->Options[i].BroadcastTextID : 0);

    SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_NPC_TEXT_UPDATE");
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandlePageTextQueryOpcode(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_PAGE_TEXT_QUERY");

    uint32 pageID;
    recvData >> pageID;
    recvData.read_skip<uint64>();                          // guid

    while (pageID)
    {
        PageText const* pageText = sObjectMgr->GetPageText(pageID);
                                                            // guess size
        WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 50);
        data << pageID;

        if (!pageText)
        {
            data << "Item page missing.";
            data << uint32(0);
            pageID = 0;
        }
        else
        {
            std::string Text = pageText->Text;

            int loc_idx = GetSessionDbLocaleIndex();
            if (loc_idx >= 0)
                if (PageTextLocale const* player = sObjectMgr->GetPageTextLocale(pageID))
                    ObjectMgr::GetLocaleString(player->Text, loc_idx, Text);

            data << Text;
            data << uint32(pageText->NextPage);
            pageID = pageText->NextPage;
        }
        SendPacket(&data);

        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_PAGE_TEXT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseMapPositionQuery(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Recv CMSG_CORPSE_MAP_POSITION_QUERY");

    ObjectGuid transportGuid;
    recvData >> transportGuid;

    WorldPacket data(SMSG_CORPSE_MAP_POSITION_QUERY_RESPONSE, 4+4+4+4);
    data << float(0);
    data << float(0);
    data << float(0);
    data << float(0);
    SendPacket(&data);
}

void WorldSession::HandleQuestPOIQuery(WorldPacket& recvData)
{
    uint32 count;
    recvData >> count;

    if (count > MAX_QUEST_LOG_SIZE)
    {
        recvData.rfinish();
        return;
    }

    WorldPacket data(SMSG_QUEST_POI_QUERY_RESPONSE, 300);       // guess size

    // for some reason Blizzard is including the counts twice throughout this packet
    data << int32(count);
    data << uint32(count);

    for (uint8 i = 0; i < count; ++i)
    {
        uint32 questId;
        recvData >> questId;

        bool questOk = false;

        uint16 questSlot = _player->FindQuestSlot(questId);
        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk =_player->GetQuestSlotQuestId(questSlot) == questId;

        if (questOk)
        {
            if (QuestPOIVector const* poi = sObjectMgr->GetQuestPOIVector(questId))
            {
                data << uint32(questId);
                data << int32(poi->size());
                data << uint32(poi->size());

                for (QuestPOIVector::const_iterator itr = poi->begin(); itr != poi->end(); ++itr)
                {
                    // work out the names for itr->Unk3 and itr->Unk4
                    data << int32(itr->Id);
                    data << int32(itr->ObjectiveIndex);
                    data << int32(0);                           // QuestObjectiveID
                    data << int32(0);                           // QuestObjectID
                    data << int32(itr->MapId);
                    data << int32(itr->AreaId);
                    data << int32(itr->FloorId);
                    data << int32(0);                           // Priority
                    data << int32(0);                           // Flags
                    data << int32(0);                           // WorldEffectID
                    data << int32(0);                           // PlayerConditionID
                    data << int32(itr->points.size());
                    data << int32(0);                           // Unknown
                    data << uint32(itr->points.size());

                    for (std::vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
                    {
                        data << int32(itr2->x);
                        data << int32(itr2->y);
                    }
                } 
            }
            else
            {
                data << uint32(questId);
                data << int32(0);                               // POI Count
                data << uint32(0);                              // ...
            }
        }
        else
        {
            data << uint32(questId);
            data << int32(0);                                   // POI Count
            data << uint32(0);                                  // ...
        }
    }

    SendPacket(&data);
}
