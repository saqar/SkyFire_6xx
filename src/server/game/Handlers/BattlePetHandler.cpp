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

#include "BattlePet.h"
#include "BattlePetMgr.h"
#include "Common.h"
#include "DB2Enums.h"
#include "DB2Stores.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "TemporarySummon.h"
#include "WorldSession.h"
#include "WorldPacket.h"

void WorldSession::HandleBattlePetDelete(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_DELETE");

    ObjectGuid battlePetGuid;
    recvData >> battlePetGuid;

    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(battlePetGuid);
    if (!battlePet)
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_DELETE - Player %u tryed to release Battle Pet " UI64FMTD " which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)battlePetGuid);
        return;
    }

    if (BattlePetSpeciesHasFlag(battlePet->GetSpecies(), BATTLE_PET_FLAG_NOT_RELEASABLE))
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_DELETE - Player %u tryed to release Battle Pet " UI64FMTD " which isn't releasable!",
            GetPlayer()->GetGUIDLow(), (uint64)battlePetGuid);
        return;
    }

    // unsummon battle pet unit if in world
    if (ObjectGuid summonedGuid = battlePetMgr->GetCurrentSummonGuid())
        if (summonedGuid == battlePetGuid)
            battlePetMgr->UnSummonCurrentBattlePet(false);

    battlePetMgr->Delete(battlePet);
}

#define BATTLE_PET_MAX_DECLINED_NAMES 5

void WorldSession::HandleBattlePetModifyName(WorldPacket& recvData)
{
    // TODO: finish this...
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_MODIFY_NAME");

    ObjectGuid battlePetGuid;
    uint8 nicknameLen;
    std::string nickname;
    bool hasDeclinedNames;

    uint8 declinedNameLen[BATTLE_PET_MAX_DECLINED_NAMES];
    std::string declinedNames[BATTLE_PET_MAX_DECLINED_NAMES];

    recvData >> battlePetGuid;

    nicknameLen = recvData.ReadBits(7);
    hasDeclinedNames = recvData.ReadBit();
    nickname = recvData.ReadString(nicknameLen);

    if (hasDeclinedNames)
        for (uint8 i = 0; i < BATTLE_PET_MAX_DECLINED_NAMES; i++)
            declinedNameLen[i] = recvData.ReadBits(7);

    if (hasDeclinedNames)
        for (uint8 i = 0; i < BATTLE_PET_MAX_DECLINED_NAMES; i++)
            declinedNames[i] = recvData.ReadString(declinedNameLen[i]);

    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(battlePetGuid);
    if (!battlePet)
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_MODIFY_NAME - Player %u tryed to set the name for Battle Pet " UI64FMTD " which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)battlePetGuid);
        return;
    }

    if (nickname.size() > BATTLE_PET_MAX_NAME_LENGTH)
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_MODIFY_NAME - Player %u tryed to set the name for Battle Pet " UI64FMTD " with an invalid length!",
            GetPlayer()->GetGUIDLow(), (uint64)battlePetGuid);
        return;
    }

    // TODO: check for invalid characters, ect...
    // ...

    battlePet->SetNickname(nickname);
    battlePet->SetTimestamp((uint32)time(nullptr));

    if (battlePetMgr->GetCurrentSummonGuid())
        battlePetMgr->GetCurrentSummon()->SetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP, battlePet->GetTimestamp());
}

void WorldSession::HandleBattlePetQueryName(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_QUERY_NAME");

    ObjectGuid battlePetGuid;
    ObjectGuid unitGuid;

    recvData >> battlePetGuid;
    recvData >> unitGuid;

    Unit* battlePetUnit = sObjectAccessor->FindUnit(unitGuid);
    if (!battlePetUnit)
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_QUERY_NAME - Player %u queried Battle Pet unit " UI64FMTD " which doesnt't exist in world!",
            GetPlayer()->GetGUIDLow(), (uint64)unitGuid);
        return;
    }

    Unit* ownerUnit = battlePetUnit->ToTempSummon()->GetSummoner();
    if (!ownerUnit)
        return;

    BattlePetMgr* battlePetMgr = ownerUnit->ToPlayer()->GetBattlePetMgr();
    BattlePet* battlePet = battlePetMgr->GetBattlePet(battlePetMgr->GetCurrentSummonGuid());
    if (!battlePet)
        return;

    if (battlePet->GetGuid() != battlePetGuid)
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_QUERY_NAME - Player %u queried Battle Pet unit " UI64FMTD "but the supplied Battle Pet Guid " UI64FMTD " doesn't match the units!",
            GetPlayer()->GetGUIDLow(), (uint64)unitGuid, (uint64)battlePetGuid);
        return;
    }

    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(battlePet->GetSpecies());

    WorldPacket data(SMSG_BATTLE_PET_QUERY_NAME_RESPONSE, 2 + 16 + 4 + 4 + 5 + battlePet->GetNickname().size());
    data << battlePetGuid;
    data << uint32(speciesEntry->NpcId);
    data << uint32(battlePet->GetTimestamp());
    data.WriteBit(1);                                               // hasDeclined

    {
        data.WriteBits(battlePet->GetNickname().size(), 8);
        data.WriteBit(0);                                           // allow

        // TODO: finish declined names
        for (uint8 i = 0; i < BATTLE_PET_MAX_DECLINED_NAMES; i++)
            data.WriteBits(0, 7);

        data.FlushBits();
    }

    data.WriteString(battlePet->GetNickname());

    SendPacket(&data);
}

void WorldSession::HandleBattlePetSetBattleSlot(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_SET_BATTLE_SLOT");

    uint8 slot;
    ObjectGuid battlePetGuid;

    recvData >> battlePetGuid;
    recvData >> slot;

    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(battlePetGuid);
    if (!battlePet)
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_BATTLE_SLOT - Player %u tryed to add Battle Pet " UI64FMTD " to loadout which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)battlePetGuid);
        return;
    }

    if (!battlePetMgr->HasLoadoutSlot(slot))
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_BATTLE_SLOT - Player %u tryed to add Battle Pet " UI64FMTD " into slot %u which is locked!",
            GetPlayer()->GetGUIDLow(), (uint64)battlePetGuid, slot);
        return;
    }

    // this check is also done clientside
    if (BattlePetSpeciesHasFlag(battlePet->GetSpecies(), BATTLE_PET_FLAG_COMPANION))
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_BATTLE_SLOT - Player %u tryed to add a compainion Battle Pet " UI64FMTD " into slot %u!",
            GetPlayer()->GetGUIDLow(), (uint64)battlePetGuid, slot);
        return;
    }

    // sever handles slot swapping, find source slot and replace it with the destination slot
    uint8 srcSlot = battlePetMgr->GetLoadoutSlotForBattlePet(battlePetGuid);
    if (srcSlot != BATTLE_PET_LOADOUT_SLOT_NONE)
        battlePetMgr->SetLoadoutSlot(srcSlot, battlePetMgr->GetLoadoutSlot(slot), true);

    battlePetMgr->SetLoadoutSlot(slot, battlePetGuid, true);
}

enum BattlePetFlagModes
{
    BATTLE_PET_FLAG_MODE_SET         = 0,
    BATTLE_PET_FLAG_MODE_UNSET       = 2
};

void WorldSession::HandleBattlePetSetFlags(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_SET_FLAGS");

    ObjectGuid battlePetGuid;
    uint32 flags;
    uint8 controlMode;

    recvData >> battlePetGuid;
    recvData >> flags;

    controlMode = recvData.ReadBits(2);

    BattlePet* battlePet = GetPlayer()->GetBattlePetMgr()->GetBattlePet(battlePetGuid);
    if (!battlePet)
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_FLAGS - Player %u tryed to set the flags for Battle Pet " UI64FMTD " which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)battlePetGuid);
        return;
    }

    // list of flags the client can currently change
    if (flags != BATTLE_PET_JOURNAL_FLAG_FAVORITES
        && flags != BATTLE_PET_JOURNAL_FLAG_ABILITY_1
        && flags != BATTLE_PET_JOURNAL_FLAG_ABILITY_2
        && flags != BATTLE_PET_JOURNAL_FLAG_ABILITY_3)
    {
        TC_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_FLAGS - Player %u tryed to set an invalid Battle Pet flag %u!",
            GetPlayer()->GetGUIDLow(), flags);
        return;
    }

    // TODO: check if Battle Pet is correct level for ability
    // ...

    switch (controlMode)
    {
        case BATTLE_PET_FLAG_MODE_SET:
            battlePet->SetFlag(flags);
            break;
        case BATTLE_PET_FLAG_MODE_UNSET:
            battlePet->UnSetFlag(flags);
            break;
    }
}

void WorldSession::HandleBattlePetSummonCompanion(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_SUMMON_COMPANION");

    ObjectGuid battlePetGuid;
    recvData >> battlePetGuid;

    Player* player = GetPlayer();
    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(battlePetGuid);
    if (!battlePet)
    {
        TC_LOG_DEBUG("network", "CMSG_SUMMON_BATTLE_PET_COMPANION - Player %u tryed to summon battle pet companion " UI64FMTD " which it doesn't own!",
            player->GetGUIDLow(), (uint64)battlePetGuid);
        return;
    }

    if (!battlePet->GetCurrentHealth())
    {
        TC_LOG_DEBUG("network", "CMSG_SUMMON_BATTLE_PET_COMPANION - Player %u tryed to summon battle pet companion " UI64FMTD " which is dead!",
            player->GetGUIDLow(), (uint64)battlePetGuid);
        return;
    }

    if (battlePetMgr->GetCurrentSummonGuid() == battlePetGuid)
        battlePetMgr->UnSummonCurrentBattlePet(false);
    else
    {
        if (uint32 summonSpell = BattlePetGetSummonSpell(battlePet->GetSpecies()))
        {
            battlePetMgr->SetCurrentSummonGuid(battlePetGuid);
            player->CastSpell(player, summonSpell, true);
        }
    }
}
