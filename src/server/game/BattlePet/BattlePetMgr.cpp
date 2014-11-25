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

#include "BattlePetMgr.h"
#include "ByteBuffer.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "DB2Enums.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

BattlePetMgr::~BattlePetMgr()
{
    for (auto &battlePet : m_battlePetSet)
        delete battlePet;

    m_battlePetSet.clear();
}

void BattlePetMgr::LoadFromDb(PreparedQueryResult result)
{
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint64 id            = fields[0].GetUInt64();
        uint16 speciesId     = fields[1].GetUInt16();
        std::string nickname = fields[2].GetString();
        uint32 timestamp     = fields[3].GetUInt32();
        uint8 level          = fields[4].GetUInt8();
        uint16 xp            = fields[5].GetUInt16();
        uint16 health        = fields[6].GetUInt16();
        uint16 maxHealth     = fields[7].GetUInt16();
        uint16 power         = fields[8].GetUInt16();
        uint16 speed         = fields[9].GetUInt16();
        uint8 quality        = fields[10].GetUInt8();
        uint8 breedId        = fields[11].GetUInt8();
        uint16 flags         = fields[12].GetUInt16();

        if (!sBattlePetSpeciesStore.LookupEntry(speciesId))
        {
            TC_LOG_ERROR("sql.sql", "Species %u defined in `account_battle_pet` for Battle Pet " UI64FMTD "  does not exist, skipped.",
                speciesId, (uint64)id);
            continue;
        }

        if (sBattlePetBreedSet.find(breedId) == sBattlePetBreedSet.end() && breedId != 0)
        {
            TC_LOG_ERROR("sql.sql", "Breed %u defined in `account_battle_pet` for Battle Pet " UI64FMTD " does not exist, skipped.",
                breedId, (uint64)id);
            continue;
        }

        // highest quality client supports, currently players can not obtain legendary pets on retail
        if (quality > ITEM_QUALITY_LEGENDARY)
        {
            TC_LOG_ERROR("sql.sql", "Quality %u defined in `account_battle_pet` for Battle Pet " UI64FMTD " is invalid, skipped.",
                quality, (uint64)id);
            continue;
        }

        // client supports up to level 255 (uint8)
        if (level > BATTLE_PET_MAX_LEVEL)
        {
            TC_LOG_ERROR("sql.sql", "Level %u defined in `account_battle_pet` for Battle Pet " UI64FMTD " is invalid, skipped.",
                quality, (uint64)id);
            continue;
        }

        BattlePet* battlePet = new BattlePet(id, MAKE_NEW_GUID128(id, 0, GUID_TYPE_BATTLE_PET), speciesId, nickname,
            timestamp, level, xp, health, maxHealth, power, speed, quality, breedId, flags);

        m_battlePetSet.insert(battlePet);
    } 
    while (result->NextRow());
}

void BattlePetMgr::SaveToDb(SQLTransaction& trans)
{
    SaveSlotsToDb(trans);

    if (m_battlePetSet.empty())
        return;

    for (auto itr = m_battlePetSet.begin(); itr != m_battlePetSet.end(); itr++)
    {
        BattlePet* battlePet = *itr;
        switch (battlePet->GetDbState())
        {
            case BATTLE_PET_DB_STATE_DELETE:
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET);
                stmt->setUInt64(0, battlePet->GetId());
                trans->Append(stmt);

                itr = m_battlePetSet.erase(itr);
                delete battlePet;

                break;
            }
            case BATTLE_PET_DB_STATE_SAVE:
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET);
                stmt->setUInt64(0, battlePet->GetId());
                trans->Append(stmt);

                stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ACCOUNT_BATTLE_PET);
                stmt->setUInt64(0, battlePet->GetId());
                stmt->setUInt32(1, m_owner->GetSession()->GetAccountId());
                stmt->setUInt16(2, battlePet->GetSpecies());
                stmt->setString(3, battlePet->GetNickname());
                stmt->setUInt32(4, battlePet->GetTimestamp());
                stmt->setUInt8 (5, battlePet->GetLevel());
                stmt->setUInt16(6, battlePet->GetXp());
                stmt->setUInt16(7, battlePet->GetCurrentHealth());
                stmt->setUInt16(8, battlePet->GetMaxHealth());
                stmt->setUInt16(9, battlePet->GetPower());
                stmt->setUInt16(10, battlePet->GetSpeed());
                stmt->setUInt8 (11, battlePet->GetQuality());
                stmt->setUInt8 (12, battlePet->GetBreed());
                stmt->setUInt16(13, battlePet->GetFlags());
                trans->Append(stmt);

                battlePet->SetDbState(BATTLE_PET_DB_STATE_NONE);
                break;
            }
            case BATTLE_PET_DB_STATE_NONE:
                break;
        }
    }
}

void BattlePetMgr::LoadSlotsFromDb(PreparedQueryResult result)
{
    if (!result)
        return;

    Field* fields = result->Fetch();
    m_loadoutFlags = fields[3].GetUInt8();

    // update flag and spell state for new alt characters
    if (m_loadoutFlags != BATTLE_PET_LOADOUT_SLOT_FLAG_NONE && !m_owner->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_BATTLE_PET_ENABLED))
    {
        m_owner->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_BATTLE_PET_ENABLED);
        m_owner->learnSpell(SPELL_BATTLE_PET_TRAINING_PASSIVE, false);
        m_owner->learnSpell(SPELL_TRACK_PETS, false);
        m_owner->learnSpell(SPELL_REVIVE_BATTLE_PETS, false);
    }

    for (uint8 i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; i++)
    {
        uint64 id = fields[i].GetUInt64();
        if ((!HasLoadoutSlot(i) || !HasBattlePet(id)) && id != 0)
        {
            TC_LOG_ERROR("sql.sql", "Battle Pet slot %u in `account_battle_pet_slots` for account %u is invalid!",
                i, m_owner->GetSession()->GetAccountId());

            SetLoadoutSlot(i, ObjectGuid(0));
            m_loadoutSave = true;
        }
        else
            SetLoadoutSlot(i, MAKE_NEW_GUID128(id, 0, GUID_TYPE_BATTLE_PET));
    }
}

void BattlePetMgr::SaveSlotsToDb(SQLTransaction& trans)
{
    if (!m_loadoutSave)
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET_SLOTS);
    stmt->setUInt32(0, m_owner->GetSession()->GetAccountId());
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ACCOUNT_BATTLE_PET_SLOTS);
    stmt->setUInt32(0, m_owner->GetSession()->GetAccountId());

    for (uint8 i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; i++)
    {
        BattlePet* battlePet = GetBattlePet(GetLoadoutSlot(i));
        stmt->setUInt64(i + 1, battlePet ? battlePet->GetId() : 0);
    }

    stmt->setUInt8(4, GetLoadoutFlags());
    trans->Append(stmt);

    m_loadoutSave = false;
}

bool BattlePetMgr::HasBattlePet(uint64 id) const
{
    for (const auto &battlePet : m_battlePetSet)
        if (battlePet->GetId() == id)
            return true;

    return false;
}

BattlePet* BattlePetMgr::GetBattlePet(ObjectGuid guid) const
{
    for (const auto &battlePet : m_battlePetSet)
        if (battlePet->GetGuid() == guid)
            return battlePet;

    return nullptr;
}

uint8 BattlePetMgr::GetBattlePetCount(uint16 speciesId) const
{
    uint8 battlePetSpeciesCount = 0u;

    for (const auto &battlePet : m_battlePetSet)
        if (battlePet->GetSpecies() == speciesId)
            battlePetSpeciesCount++;

    return battlePetSpeciesCount;
}

void BattlePetMgr::UnSummonCurrentBattlePet(bool temporary)
{
    if (!m_summon || !m_summonGuid)
        return;

    m_owner->SetUInt32Value(UNIT_FIELD_WILD_BATTLE_PET_LEVEL, 0);
    m_owner->SetGuidValue(PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID, ObjectGuid(0));
    m_owner->SetUInt32Value(PLAYER_FIELD_CURRENT_BATTLE_PET_BREED_QUALITY, 0);

    m_summonLastGuid = temporary ? m_summonGuid : ObjectGuid(0);
    m_summonGuid = ObjectGuid(0);

    m_summon->UnSummon();
    m_summon = nullptr;
}

void BattlePetMgr::ResummonLastBattlePet()
{
    if (!m_summonLastGuid)
        return;

    m_summonGuid = m_summonLastGuid;
    m_owner->CastSpell(m_owner, BattlePetGetSummonSpell(GetBattlePet(m_summonGuid)->GetSpecies()), true);

    m_summonLastGuid = ObjectGuid(0);
}

uint8 BattlePetMgr::GetLoadoutSlotForBattlePet(ObjectGuid guid) const
{
    for (uint8 i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; i++)
        if (GetLoadoutSlot(i) == guid)
            return i;

    return BATTLE_PET_LOADOUT_SLOT_NONE;
}

void BattlePetMgr::UnlockLoadoutSlot(uint8 slot)
{
    if (HasLoadoutSlot(slot))
        return;

    if (slot >= BATTLE_PET_MAX_LOADOUT_SLOTS)
        return;

    switch (slot)
    {
        case BATTLE_PET_LOADOUT_SLOT_1:
            SetLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_1);
            break;
        case BATTLE_PET_LOADOUT_SLOT_2:
            SetLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_2);
            break;
        case BATTLE_PET_LOADOUT_SLOT_3:
            SetLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_3);
            break;
    }

    SetLoadoutSlot(slot, ObjectGuid(0));

    // alert client of new Battle Pet loadout slot
    SendBattlePetSlotUpdate(slot, true);
}

void BattlePetMgr::SetLoadoutSlot(uint8 slot, ObjectGuid guid, bool save)
{
    if (!HasLoadoutSlot(slot))
        return;

    m_loadout[slot] = guid;

    if (save)
        m_loadoutSave = true;
}

ObjectGuid BattlePetMgr::GetLoadoutSlot(uint8 slot) const
{
    if (!HasLoadoutSlot(slot))
        return ObjectGuid(0);

    return m_loadout[slot];
}

bool BattlePetMgr::HasLoadoutSlot(uint8 slot) const
{
    if (!m_loadoutFlags || slot >= BATTLE_PET_MAX_LOADOUT_SLOTS)
        return false;

    switch (slot)
    {
        case BATTLE_PET_LOADOUT_SLOT_1:
            return HasLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_1);
        case BATTLE_PET_LOADOUT_SLOT_2:
            return HasLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_2);
        case BATTLE_PET_LOADOUT_SLOT_3:
            return HasLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_3);
    }

    return false;
}

void BattlePetMgr::SetLoadoutFlag(uint8 flag)
{
    if (HasLoadoutFlag(flag))
        return;

    m_loadoutFlags |= flag;
    m_loadoutSave = true;
}

void BattlePetMgr::Create(uint16 speciesId)
{
    if (!sBattlePetSpeciesStore.LookupEntry(speciesId))
        return;

    if (m_battlePetSet.size() > BATTLE_PET_MAX_JOURNAL_PETS)
        return;

    uint8 speciesCount = GetBattlePetCount(speciesId);
    if (BattlePetSpeciesHasFlag(speciesId, BATTLE_PET_FLAG_UNIQUE) && speciesCount >= 1)
        return;

    if (speciesCount >= BATTLE_PET_MAX_JOURNAL_SPECIES)
        return;

    uint64 id       = sObjectMgr->BattlePetGetNewId();
    ObjectGuid guid = MAKE_NEW_GUID128(id, 0, GUID_TYPE_BATTLE_PET);
    uint8 breed     = sObjectMgr->BattlePetGetRandomBreed(speciesId);
    uint8 quality   = sObjectMgr->BattlePetGetRandomQuality(speciesId);
    uint8 level     = sWorld->getIntConfig(CONFIG_BATTLE_PET_INITIAL_LEVEL);

    BattlePet* battlePet = new BattlePet(id, guid, speciesId, level, quality, breed);
    m_battlePetSet.insert(battlePet);

    // alert the client of a new Battle Pet
    SendBattlePetUpdate(battlePet, true);
}

void BattlePetMgr::Delete(BattlePet* battlePet)
{
    if (!battlePet)
        return;

    if (battlePet->GetDbState() == BATTLE_PET_DB_STATE_DELETE)
        return;

    // this shouldn't happen since the client doesn't allow releasing of slotted Battle Pets
    uint8 srcSlot = GetLoadoutSlotForBattlePet(battlePet->GetGuid());
    if (srcSlot != BATTLE_PET_LOADOUT_SLOT_NONE)
    {
        SetLoadoutSlot(srcSlot, ObjectGuid(0), true);
        SendBattlePetSlotUpdate(srcSlot, false);
    }

    battlePet->SetDbState(BATTLE_PET_DB_STATE_DELETE);

    // alert client of deleted pet
    SendBattlePetDeleted(battlePet->GetGuid());
}

void BattlePetMgr::SendBattlePetDeleted(ObjectGuid guid)
{
    WorldPacket data(SMSG_BATTLE_PET_DELETED, 2 + 16);
    data << guid;
    m_owner->GetSession()->SendPacket(&data);
}

void BattlePetMgr::SendBattlePetJournalLock()
{
    WorldPacket data(SMSG_BATTLE_PET_JOURNAL_LOCK_ACQUIRED, 0);
    m_owner->GetSession()->SendPacket(&data);
}

void BattlePetMgr::SendBattlePetJournal()
{
    uint32 battlePetCount = 0u;

    WorldPacket data(SMSG_BATTLE_PET_JOURNAL, 2 + 4 + 4 + 3 * (2 + 16 + 4 + 1 + 1) + m_battlePetSet.size() *
        (2 + 16 + 4 + 4 + 4 + 2 + 2 + 2 + 2 + 4 + 4 + 4 + 4 + 8 + 2 + 16) + 1);

    data << uint16(0);                                      // trapLevel
    data << uint32(BATTLE_PET_MAX_LOADOUT_SLOTS);

    size_t battlePetCountPos = data.wpos();
    data << uint32(battlePetCount);                         // placeholder

    for (uint8 i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; i++)
        AppendBattlePetSlotData(data, i);
    
    for (const auto &battlePet : m_battlePetSet)
    {
        if (battlePet->GetDbState() == BATTLE_PET_DB_STATE_DELETE)
            continue;

        AppendBattlePetData(data, battlePet);
        battlePetCount++;
    }

    data.put<uint32>(battlePetCountPos, battlePetCount);

    data.WriteBit(0);                                       // journalLock
    data.FlushBits();

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePetMgr::SendBattlePetSlotUpdate(uint8 slot, bool notification)
{
    WorldPacket data(SMSG_BATTLE_PET_SLOT_UPDATE, 4 + 2 + 16 + 4 + 1 + 1 + 1);
    data << uint32(1);                                      // slotCount
    AppendBattlePetSlotData(data, slot);

    data.WriteBit(notification);
    data.WriteBit(0);                                       // autoSlotted
    data.FlushBits();

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePetMgr::SendBattlePetUpdate(BattlePet* battlePet, bool notification)
{
    WorldPacket data(SMSG_BATTLE_PET_UPDATE, 4 + (2 + 16 + 4 + 4 + 4 + 2 + 2 + 2 +
        2 + 4 + 4 + 4 + 4 + 1 + 2 + battlePet->GetNickname().size() + 1));

    data << uint32(1);                                      // battlePetCount
    AppendBattlePetData(data, battlePet);

    data.WriteBit(notification);
    data.FlushBits();

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePetMgr::AppendBattlePetData(ByteBuffer &data, BattlePet* battlePet)
{
    CreatureTemplate const* creatureTemplate = sObjectMgr->GetCreatureTemplate(sBattlePetSpeciesStore.LookupEntry(battlePet->GetSpecies())->NpcId);

    data << battlePet->GetGuid();
    data << uint32(battlePet->GetSpecies());
    data << uint32(creatureTemplate ? creatureTemplate->Entry : 0);
    data << uint32(creatureTemplate ? creatureTemplate->Modelid1 : 0);
    data << uint16(battlePet->GetBreed());
    data << uint16(battlePet->GetLevel());
    data << uint16(battlePet->GetXp());
    data << uint16(battlePet->GetFlags());
    data << uint32(battlePet->GetPower());
    data << uint32(battlePet->GetCurrentHealth());
    data << uint32(battlePet->GetMaxHealth());
    data << uint32(battlePet->GetSpeed());
    data << uint8(battlePet->GetQuality());

    data.WriteBits(battlePet->GetNickname().size(), 7);
    data.WriteBit(0);                                       // ownerInfo
    data.WriteBit(0);                                       // noRename
    data.FlushBits();

    data.WriteString(battlePet->GetNickname());
}

void BattlePetMgr::AppendBattlePetSlotData(ByteBuffer &data, uint8 slot)
{
    data << GetLoadoutSlot(slot);
    data << uint32(0);                                      // collarId
    data << uint8(slot);

    data.WriteBit(!HasLoadoutSlot(slot));
    data.FlushBits();
}
