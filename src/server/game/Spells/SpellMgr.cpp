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

#include "SpellMgr.h"
#include "SpellInfo.h"
#include "ObjectMgr.h"
#include "SpellAuras.h"
#include "SpellAuraDefines.h"
#include "SharedDefines.h"
#include "DBCStores.h"
#include "World.h"
#include "Chat.h"
#include "Spell.h"
#include "BattlegroundMgr.h"
#include "MapManager.h"
#include "BattlefieldWG.h"
#include "BattlefieldMgr.h"
#include "Player.h"

bool IsPrimaryProfessionSkill(uint32 skill)
{
    SkillLineEntry const* pSkill = sSkillLineStore.LookupEntry(skill);
    return pSkill && pSkill->categoryId == SKILL_CATEGORY_PROFESSION;
}

bool IsWeaponSkill(uint32 skill)
{
    SkillLineEntry const* pSkill = sSkillLineStore.LookupEntry(skill);
    return pSkill && pSkill->categoryId == SKILL_CATEGORY_WEAPON;
}

bool IsPartOfSkillLine(uint32 skillId, uint32 spellId)
{
    SkillLineAbilityMapBounds skillBounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
    for (SkillLineAbilityMap::const_iterator itr = skillBounds.first; itr != skillBounds.second; ++itr)
        if (itr->second->skillId == skillId)
            return true;

    return false;
}

DiminishingGroup GetDiminishingReturnsGroupForSpell(SpellInfo const* spellproto, bool triggered)
{
    if (spellproto->IsPositive())
        return DIMINISHING_NONE;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (spellproto->Effects[i].ApplyAuraName == SPELL_AURA_MOD_TAUNT)
            return DIMINISHING_TAUNT;
    }

    // Lastly - Set diminishing depending on mechanic
    uint32 mechanic = spellproto->GetAllEffectsMechanicMask();
    if (mechanic & (1 << MECHANIC_CHARM))
        return DIMINISHING_MIND_CONTROL;
    if (mechanic & (1 << MECHANIC_SILENCE))
        return DIMINISHING_SILENCE;
    if (mechanic & (1 << MECHANIC_SLEEP))
        return DIMINISHING_SLEEP;
    if (mechanic & ((1 << MECHANIC_SAPPED) | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_SHACKLE)))
        return DIMINISHING_DISORIENT;
    // Mechanic Knockout, except Blast Wave
    if (mechanic & (1 << MECHANIC_KNOCKOUT) && spellproto->SpellIconID != 292)
        return DIMINISHING_DISORIENT;
    if (mechanic & (1 << MECHANIC_DISARM))
        return DIMINISHING_DISARM;
    if (mechanic & (1 << MECHANIC_FEAR))
        return DIMINISHING_FEAR;
    if (mechanic & (1 << MECHANIC_STUN))
        return triggered ? DIMINISHING_STUN : DIMINISHING_CONTROLLED_STUN;
    if (mechanic & (1 << MECHANIC_BANISH))
        return DIMINISHING_BANISH;
    if (mechanic & (1 << MECHANIC_ROOT))
        return triggered ? DIMINISHING_ROOT : DIMINISHING_CONTROLLED_ROOT;
    if (mechanic & (1 << MECHANIC_HORROR))
        return DIMINISHING_HORROR;

    return DIMINISHING_NONE;
}

DiminishingReturnsType GetDiminishingReturnsGroupType(DiminishingGroup group)
{
    switch (group)
    {
        case DIMINISHING_TAUNT:
        case DIMINISHING_CONTROLLED_STUN:
        case DIMINISHING_STUN:
        case DIMINISHING_OPENING_STUN:
        case DIMINISHING_CYCLONE:
        case DIMINISHING_CHARGE:
            return DRTYPE_ALL;
        case DIMINISHING_LIMITONLY:
        case DIMINISHING_NONE:
            return DRTYPE_NONE;
        default:
            return DRTYPE_PLAYER;
    }
}

DiminishingLevels GetDiminishingReturnsMaxLevel(DiminishingGroup group)
{
    switch (group)
    {
        case DIMINISHING_TAUNT:
            return DIMINISHING_LEVEL_TAUNT_IMMUNE;
        default:
            return DIMINISHING_LEVEL_IMMUNE;
    }
}

int32 GetDiminishingReturnsLimitDuration(DiminishingGroup group, SpellInfo const* spellproto)
{
    if (!IsDiminishingReturnsGroupDurationLimited(group))
        return 0;

    return 8 * IN_MILLISECONDS;
}

bool IsDiminishingReturnsGroupDurationLimited(DiminishingGroup group)
{
    switch (group)
    {
        case DIMINISHING_BANISH:
        case DIMINISHING_CONTROLLED_STUN:
        case DIMINISHING_CONTROLLED_ROOT:
        case DIMINISHING_CYCLONE:
        case DIMINISHING_DISORIENT:
        case DIMINISHING_ENTRAPMENT:
        case DIMINISHING_FEAR:
        case DIMINISHING_HORROR:
        case DIMINISHING_MIND_CONTROL:
        case DIMINISHING_OPENING_STUN:
        case DIMINISHING_ROOT:
        case DIMINISHING_STUN:
        case DIMINISHING_SLEEP:
        case DIMINISHING_LIMITONLY:
            return true;
        default:
            return false;
    }
}

SpellMgr::SpellMgr() { }

SpellMgr::~SpellMgr()
{
    UnloadSpellInfoStore();
}

/// Some checks for spells, to prevent adding deprecated/broken spells for trainers, spell book, etc
bool SpellMgr::IsSpellValid(SpellInfo const* spellInfo, Player* player, bool msg)
{
    // not exist
    if (!spellInfo)
        return false;

    bool needCheckReagents = false;

    // check effects
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch (spellInfo->Effects[i].Effect)
        {
            case 0:
                continue;

            // craft spell for crafting non-existed item (break client recipes list show)
            case SPELL_EFFECT_CREATE_ITEM:
            case SPELL_EFFECT_CREATE_ITEM_2:
            {
                if (spellInfo->Effects[i].ItemType == 0)
                {
                    // skip auto-loot crafting spells, its not need explicit item info (but have special fake items sometime)
                    if (!spellInfo->IsLootCrafting())
                    {
                        if (msg)
                        {
                            if (player)
                                ChatHandler(player->GetSession()).PSendSysMessage("Craft spell %u not have create item entry.", spellInfo->Id);
                            else
                                TC_LOG_ERROR("sql.sql", "Craft spell %u not have create item entry.", spellInfo->Id);
                        }
                        return false;
                    }

                }
                // also possible IsLootCrafting case but fake item must exist anyway
                else if (!sObjectMgr->GetItemTemplate(spellInfo->Effects[i].ItemType))
                {
                    if (msg)
                    {
                        if (player)
                            ChatHandler(player->GetSession()).PSendSysMessage("Craft spell %u create not-exist in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Effects[i].ItemType);
                        else
                            TC_LOG_ERROR("sql.sql", "Craft spell %u create not-exist in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Effects[i].ItemType);
                    }
                    return false;
                }

                needCheckReagents = true;
                break;
            }
            case SPELL_EFFECT_LEARN_SPELL:
            {
                SpellInfo const* spellInfo2 = sSpellMgr->GetSpellInfo(spellInfo->Effects[i].TriggerSpell);
                if (!IsSpellValid(spellInfo2, player, msg))
                {
                    if (msg)
                    {
                        if (player)
                            ChatHandler(player->GetSession()).PSendSysMessage("Spell %u learn to broken spell %u, and then...", spellInfo->Id, spellInfo->Effects[i].TriggerSpell);
                        else
                            TC_LOG_ERROR("sql.sql", "Spell %u learn to invalid spell %u, and then...", spellInfo->Id, spellInfo->Effects[i].TriggerSpell);
                    }
                    return false;
                }
                break;
            }
        }
    }

    if (needCheckReagents)
    {
        for (uint8 j = 0; j < MAX_SPELL_REAGENTS; ++j)
        {
            if (spellInfo->Reagent[j] > 0 && !sObjectMgr->GetItemTemplate(spellInfo->Reagent[j]))
            {
                if (msg)
                {
                    if (player)
                        ChatHandler(player->GetSession()).PSendSysMessage("Craft spell %u have not-exist reagent in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Reagent[j]);
                    else
                        TC_LOG_ERROR("sql.sql", "Craft spell %u have not-exist reagent in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Reagent[j]);
                }
                return false;
            }
        }
    }

    return true;
}

uint32 SpellMgr::GetSpellDifficultyId(uint32 spellId) const
{
    SpellDifficultySearcherMap::const_iterator i = mSpellDifficultySearcherMap.find(spellId);
    return i == mSpellDifficultySearcherMap.end() ? 0 : i->second;
}

void SpellMgr::SetSpellDifficultyId(uint32 spellId, uint32 id)
{
    mSpellDifficultySearcherMap[spellId] = id;
}

uint32 SpellMgr::GetSpellIdForDifficulty(uint32 spellId, Unit const* caster) const
{
    if (!GetSpellInfo(spellId))
        return spellId;

    if (!caster || !caster->GetMap() || !caster->GetMap()->IsDungeon())
        return spellId;

    uint32 mode = uint32(caster->GetMap()->GetSpawnMode());
    if (mode >= MAX_DIFFICULTY)
    {
        TC_LOG_ERROR("spells", "SpellMgr::GetSpellIdForDifficulty: Incorrect Difficulty for spell %u.", spellId);
        return spellId; //return source spell
    }

    uint32 difficultyId = GetSpellDifficultyId(spellId);
    if (!difficultyId)
        return spellId; //return source spell, it has only REGULAR_DIFFICULTY

    SpellDifficultyEntry const* difficultyEntry = sSpellDifficultyStore.LookupEntry(difficultyId);
    if (!difficultyEntry)
    {
        TC_LOG_DEBUG("spells", "SpellMgr::GetSpellIdForDifficulty: SpellDifficultyEntry not found for spell %u. This should never happen.", spellId);
        return spellId; //return source spell
    }

    if (difficultyEntry->SpellID[mode] <= 0 && mode > DUNGEON_DIFFICULTY_HEROIC)
    {
        TC_LOG_DEBUG("spells", "SpellMgr::GetSpellIdForDifficulty: spell %u mode %u spell is NULL, using mode %u", spellId, mode, mode - 2);
        mode -= 2;
    }

    if (difficultyEntry->SpellID[mode] <= 0)
    {
        TC_LOG_ERROR("sql.sql", "SpellMgr::GetSpellIdForDifficulty: spell %u mode %u spell is 0. Check spelldifficulty_dbc!", spellId, mode);
        return spellId;
    }

    TC_LOG_DEBUG("spells", "SpellMgr::GetSpellIdForDifficulty: spellid for spell %u in mode %u is %d", spellId, mode, difficultyEntry->SpellID[mode]);
    return uint32(difficultyEntry->SpellID[mode]);
}

SpellInfo const* SpellMgr::GetSpellForDifficultyFromSpell(SpellInfo const* spell, Unit const* caster) const
{
    if (!spell)
        return NULL;

    uint32 newSpellId = GetSpellIdForDifficulty(spell->Id, caster);
    SpellInfo const* newSpell = GetSpellInfo(newSpellId);
    if (!newSpell)
    {
        TC_LOG_DEBUG("spells", "SpellMgr::GetSpellForDifficultyFromSpell: spell %u not found. Check spelldifficulty_dbc!", newSpellId);
        return spell;
    }

    TC_LOG_DEBUG("spells", "SpellMgr::GetSpellForDifficultyFromSpell: Spell id for instance mode is %u (original %u)", newSpell->Id, spell->Id);
    return newSpell;
}

SpellChainNode const* SpellMgr::GetSpellChainNode(uint32 spell_id) const
{
    SpellChainMap::const_iterator itr = mSpellChains.find(spell_id);
    if (itr == mSpellChains.end())
        return NULL;

    return &itr->second;
}

uint32 SpellMgr::GetFirstSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        return node->first->Id;

    return spell_id;
}

uint32 SpellMgr::GetLastSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        return node->last->Id;

    return spell_id;
}

uint32 SpellMgr::GetNextSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        if (node->next)
            return node->next->Id;

    return 0;
}

uint32 SpellMgr::GetPrevSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        if (node->prev)
            return node->prev->Id;

    return 0;
}

uint8 SpellMgr::GetSpellRank(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        return node->rank;

    return 0;
}

uint32 SpellMgr::GetSpellWithRank(uint32 spell_id, uint32 rank, bool strict) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
    {
        if (rank != node->rank)
            return GetSpellWithRank(node->rank < rank ? node->next->Id : node->prev->Id, rank, strict);
    }
    else if (strict && rank > 1)
        return 0;
    return spell_id;
}

SpellRequiredMapBounds SpellMgr::GetSpellsRequiredForSpellBounds(uint32 spell_id) const
{
    return mSpellReq.equal_range(spell_id);
}

SpellsRequiringSpellMapBounds SpellMgr::GetSpellsRequiringSpellBounds(uint32 spell_id) const
{
    return mSpellsReqSpell.equal_range(spell_id);
}

bool SpellMgr::IsSpellRequiringSpell(uint32 spellid, uint32 req_spellid) const
{
    SpellsRequiringSpellMapBounds spellsRequiringSpell = GetSpellsRequiringSpellBounds(req_spellid);
    for (SpellsRequiringSpellMap::const_iterator itr = spellsRequiringSpell.first; itr != spellsRequiringSpell.second; ++itr)
    {
        if (itr->second == spellid)
            return true;
    }
    return false;
}

SpellLearnSkillNode const* SpellMgr::GetSpellLearnSkill(uint32 spell_id) const
{
    SpellLearnSkillMap::const_iterator itr = mSpellLearnSkills.find(spell_id);
    if (itr != mSpellLearnSkills.end())
        return &itr->second;
    else
        return NULL;
}

SpellLearnSpellMapBounds SpellMgr::GetSpellLearnSpellMapBounds(uint32 spell_id) const
{
    return mSpellLearnSpells.equal_range(spell_id);
}

bool SpellMgr::IsSpellLearnSpell(uint32 spell_id) const
{
    return mSpellLearnSpells.find(spell_id) != mSpellLearnSpells.end();
}

bool SpellMgr::IsSpellLearnToSpell(uint32 spell_id1, uint32 spell_id2) const
{
    SpellLearnSpellMapBounds bounds = GetSpellLearnSpellMapBounds(spell_id1);
    for (SpellLearnSpellMap::const_iterator i = bounds.first; i != bounds.second; ++i)
        if (i->second.spell == spell_id2)
            return true;
    return false;
}

SpellTargetPosition const* SpellMgr::GetSpellTargetPosition(uint32 spell_id, SpellEffIndex effIndex) const
{
    SpellTargetPositionMap::const_iterator itr = mSpellTargetPositions.find(std::make_pair(spell_id, effIndex));
    if (itr != mSpellTargetPositions.end())
        return &itr->second;
    return NULL;
}

SpellSpellGroupMapBounds SpellMgr::GetSpellSpellGroupMapBounds(uint32 spell_id) const
{
    spell_id = GetFirstSpellInChain(spell_id);
    return mSpellSpellGroup.equal_range(spell_id);
}

bool SpellMgr::IsSpellMemberOfSpellGroup(uint32 spellid, SpellGroup groupid) const
{
    SpellSpellGroupMapBounds spellGroup = GetSpellSpellGroupMapBounds(spellid);
    for (SpellSpellGroupMap::const_iterator itr = spellGroup.first; itr != spellGroup.second; ++itr)
    {
        if (itr->second == groupid)
            return true;
    }
    return false;
}

SpellGroupSpellMapBounds SpellMgr::GetSpellGroupSpellMapBounds(SpellGroup group_id) const
{
    return mSpellGroupSpell.equal_range(group_id);
}

void SpellMgr::GetSetOfSpellsInSpellGroup(SpellGroup group_id, std::set<uint32>& foundSpells) const
{
    std::set<SpellGroup> usedGroups;
    GetSetOfSpellsInSpellGroup(group_id, foundSpells, usedGroups);
}

void SpellMgr::GetSetOfSpellsInSpellGroup(SpellGroup group_id, std::set<uint32>& foundSpells, std::set<SpellGroup>& usedGroups) const
{
    if (usedGroups.find(group_id) != usedGroups.end())
        return;
    usedGroups.insert(group_id);

    SpellGroupSpellMapBounds groupSpell = GetSpellGroupSpellMapBounds(group_id);
    for (SpellGroupSpellMap::const_iterator itr = groupSpell.first; itr != groupSpell.second; ++itr)
    {
        if (itr->second < 0)
        {
            SpellGroup currGroup = (SpellGroup)abs(itr->second);
            GetSetOfSpellsInSpellGroup(currGroup, foundSpells, usedGroups);
        }
        else
        {
            foundSpells.insert(itr->second);
        }
    }
}

bool SpellMgr::AddSameEffectStackRuleSpellGroups(SpellInfo const* spellInfo, int32 amount, std::map<SpellGroup, int32>& groups) const
{
    uint32 spellId = spellInfo->GetFirstRankSpell()->Id;
    SpellSpellGroupMapBounds spellGroup = GetSpellSpellGroupMapBounds(spellId);
    // Find group with SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT if it belongs to one
    for (SpellSpellGroupMap::const_iterator itr = spellGroup.first; itr != spellGroup.second; ++itr)
    {
        SpellGroup group = itr->second;
        SpellGroupStackMap::const_iterator found = mSpellGroupStack.find(group);
        if (found != mSpellGroupStack.end())
        {
            if (found->second == SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT)
            {
                // Put the highest amount in the map
                if (groups.find(group) == groups.end())
                    groups[group] = amount;
                else
                {
                    int32 curr_amount = groups[group];
                    // Take absolute value because this also counts for the highest negative aura
                    if (abs(curr_amount) < abs(amount))
                        groups[group] = amount;
                }
                // return because a spell should be in only one SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT group
                return true;
            }
        }
    }
    // Not in a SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT group, so return false
    return false;
}

SpellGroupStackRule SpellMgr::CheckSpellGroupStackRules(SpellInfo const* spellInfo1, SpellInfo const* spellInfo2) const
{
    uint32 spellid_1 = spellInfo1->GetFirstRankSpell()->Id;
    uint32 spellid_2 = spellInfo2->GetFirstRankSpell()->Id;
    if (spellid_1 == spellid_2)
        return SPELL_GROUP_STACK_RULE_DEFAULT;
    // find SpellGroups which are common for both spells
    SpellSpellGroupMapBounds spellGroup1 = GetSpellSpellGroupMapBounds(spellid_1);
    std::set<SpellGroup> groups;
    for (SpellSpellGroupMap::const_iterator itr = spellGroup1.first; itr != spellGroup1.second; ++itr)
    {
        if (IsSpellMemberOfSpellGroup(spellid_2, itr->second))
        {
            bool add = true;
            SpellGroupSpellMapBounds groupSpell = GetSpellGroupSpellMapBounds(itr->second);
            for (SpellGroupSpellMap::const_iterator itr2 = groupSpell.first; itr2 != groupSpell.second; ++itr2)
            {
                if (itr2->second < 0)
                {
                    SpellGroup currGroup = (SpellGroup)abs(itr2->second);
                    if (IsSpellMemberOfSpellGroup(spellid_1, currGroup) && IsSpellMemberOfSpellGroup(spellid_2, currGroup))
                    {
                        add = false;
                        break;
                    }
                }
            }
            if (add)
                groups.insert(itr->second);
        }
    }

    SpellGroupStackRule rule = SPELL_GROUP_STACK_RULE_DEFAULT;

    for (std::set<SpellGroup>::iterator itr = groups.begin(); itr!= groups.end(); ++itr)
    {
        SpellGroupStackMap::const_iterator found = mSpellGroupStack.find(*itr);
        if (found != mSpellGroupStack.end())
            rule = found->second;
        if (rule)
            break;
    }
    return rule;
}

SpellProcEventEntry const* SpellMgr::GetSpellProcEvent(uint32 spellId) const
{
    SpellProcEventMap::const_iterator itr = mSpellProcEventMap.find(spellId);
    if (itr != mSpellProcEventMap.end())
        return &itr->second;
    return NULL;
}

bool SpellMgr::IsSpellProcEventCanTriggeredBy(SpellProcEventEntry const* spellProcEvent, uint32 EventProcFlag, SpellInfo const* procSpell, uint32 procFlags, uint32 procExtra, bool active) const
{
    // No extra req need
    uint32 procEvent_procEx = PROC_EX_NONE;

    // check prockFlags for condition
    if ((procFlags & EventProcFlag) == 0)
        return false;

    bool hasFamilyMask = false;

    /* Check Periodic Auras

    *Dots can trigger if spell has no PROC_FLAG_SUCCESSFUL_NEGATIVE_MAGIC_SPELL
        nor PROC_FLAG_TAKEN_POSITIVE_MAGIC_SPELL

    *Only Hots can trigger if spell has PROC_FLAG_TAKEN_POSITIVE_MAGIC_SPELL

    *Only dots can trigger if spell has both positivity flags or PROC_FLAG_SUCCESSFUL_NEGATIVE_MAGIC_SPELL

    *Aura has to have PROC_FLAG_TAKEN_POSITIVE_MAGIC_SPELL or spellfamily specified to trigger from Hot

    */

    if (procFlags & PROC_FLAG_DONE_PERIODIC)
    {
        if (EventProcFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG)
        {
            if (!(procExtra & PROC_EX_INTERNAL_DOT))
                return false;
        }
        else if (procExtra & PROC_EX_INTERNAL_HOT)
            procExtra |= PROC_EX_INTERNAL_REQ_FAMILY;
        else if (EventProcFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS)
            return false;
    }

    if (procFlags & PROC_FLAG_TAKEN_PERIODIC)
    {
        if (EventProcFlag & PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS)
        {
            if (!(procExtra & PROC_EX_INTERNAL_DOT))
                return false;
        }
        else if (procExtra & PROC_EX_INTERNAL_HOT)
            procExtra |= PROC_EX_INTERNAL_REQ_FAMILY;
        else if (EventProcFlag & PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS)
            return false;
    }
    // Trap casts are active by default
    if (procFlags & PROC_FLAG_DONE_TRAP_ACTIVATION)
        active = true;

    // Always trigger for this
    if (procFlags & (PROC_FLAG_KILLED | PROC_FLAG_KILL | PROC_FLAG_DEATH))
        return true;

    if (spellProcEvent)     // Exist event data
    {
        // Store extra req
        procEvent_procEx = spellProcEvent->procEx;

        // For melee triggers
        if (procSpell == NULL)
        {
            // Check (if set) for school (melee attack have Normal school)
            if (spellProcEvent->schoolMask && (spellProcEvent->schoolMask & SPELL_SCHOOL_MASK_NORMAL) == 0)
                return false;
        }
        else // For spells need check school/spell family/family mask
        {
            // Check (if set) for school
            if (spellProcEvent->schoolMask && (spellProcEvent->schoolMask & procSpell->SchoolMask) == 0)
                return false;

            // Check (if set) for spellFamilyName
            if (spellProcEvent->spellFamilyName && (spellProcEvent->spellFamilyName != procSpell->SpellFamilyName))
                return false;

            // spellFamilyName is Ok need check for spellFamilyMask if present
            if (spellProcEvent->spellFamilyMask)
            {
                if (!(spellProcEvent->spellFamilyMask & procSpell->SpellFamilyFlags))
                    return false;
                hasFamilyMask = true;
                // Some spells are not considered as active even with have spellfamilyflags
                if (!(procEvent_procEx & PROC_EX_ONLY_ACTIVE_SPELL))
                    active = true;
            }
        }
    }

    if (procExtra & (PROC_EX_INTERNAL_REQ_FAMILY))
    {
        if (!hasFamilyMask)
            return false;
    }

    // Check for extra req (if none) and hit/crit
    if (procEvent_procEx == PROC_EX_NONE)
    {
        // No extra req, so can trigger only for hit/crit - spell has to be active
        if ((procExtra & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT)) && active)
            return true;
    }
    else // Passive spells hits here only if resist/reflect/immune/evade
    {
        if (procExtra & AURA_SPELL_PROC_EX_MASK)
        {
            // if spell marked as procing only from not active spells
            if (active && procEvent_procEx & PROC_EX_NOT_ACTIVE_SPELL)
                return false;
            // if spell marked as procing only from active spells
            if (!active && procEvent_procEx & PROC_EX_ONLY_ACTIVE_SPELL)
                return false;
            // Exist req for PROC_EX_EX_TRIGGER_ALWAYS
            if (procEvent_procEx & PROC_EX_EX_TRIGGER_ALWAYS)
                return true;
            // PROC_EX_NOT_ACTIVE_SPELL and PROC_EX_ONLY_ACTIVE_SPELL flags handle: if passed checks before
            if ((procExtra & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT)) && ((procEvent_procEx & (AURA_SPELL_PROC_EX_MASK)) == 0))
                return true;
        }
        // Check Extra Requirement like (hit/crit/miss/resist/parry/dodge/block/immune/reflect/absorb and other)
        if (procEvent_procEx & procExtra)
            return true;
    }
    return false;
}

SpellProcEntry const* SpellMgr::GetSpellProcEntry(uint32 spellId) const
{
    SpellProcMap::const_iterator itr = mSpellProcMap.find(spellId);
    if (itr != mSpellProcMap.end())
        return &itr->second;
    return NULL;
}

bool SpellMgr::CanSpellTriggerProcOnEvent(SpellProcEntry const& procEntry, ProcEventInfo& eventInfo) const
{
    // proc type doesn't match
    if (!(eventInfo.GetTypeMask() & procEntry.typeMask))
        return false;

    // check XP or honor target requirement
    if (procEntry.attributesMask & PROC_ATTR_REQ_EXP_OR_HONOR)
        if (Player* actor = eventInfo.GetActor()->ToPlayer())
            if (eventInfo.GetActionTarget() && !actor->isHonorOrXPTarget(eventInfo.GetActionTarget()))
                return false;

    // always trigger for these types
    if (eventInfo.GetTypeMask() & (PROC_FLAG_KILLED | PROC_FLAG_KILL | PROC_FLAG_DEATH))
        return true;

    // check school mask (if set) for other trigger types
    if (procEntry.schoolMask && !(eventInfo.GetSchoolMask() & procEntry.schoolMask))
        return false;

    // check spell family name/flags (if set) for spells
    if (eventInfo.GetTypeMask() & (PERIODIC_PROC_FLAG_MASK | SPELL_PROC_FLAG_MASK | PROC_FLAG_DONE_TRAP_ACTIVATION))
    {
        if (procEntry.spellFamilyName && (procEntry.spellFamilyName != eventInfo.GetSpellInfo()->SpellFamilyName))
            return false;

        if (procEntry.spellFamilyMask && !(procEntry.spellFamilyMask & eventInfo.GetSpellInfo()->SpellFamilyFlags))
            return false;
    }

    // check spell type mask (if set)
    if (eventInfo.GetTypeMask() & (SPELL_PROC_FLAG_MASK | PERIODIC_PROC_FLAG_MASK))
    {
        if (procEntry.spellTypeMask && !(eventInfo.GetSpellTypeMask() & procEntry.spellTypeMask))
            return false;
    }

    // check spell phase mask
    if (eventInfo.GetTypeMask() & REQ_SPELL_PHASE_PROC_FLAG_MASK)
    {
        if (!(eventInfo.GetSpellPhaseMask() & procEntry.spellPhaseMask))
            return false;
    }

    // check hit mask (on taken hit or on done hit, but not on spell cast phase)
    if ((eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK) || ((eventInfo.GetTypeMask() & DONE_HIT_PROC_FLAG_MASK) && !(eventInfo.GetSpellPhaseMask() & PROC_SPELL_PHASE_CAST)))
    {
        uint32 hitMask = procEntry.hitMask;
        // get default values if hit mask not set
        if (!hitMask)
        {
            // for taken procs allow normal + critical hits by default
            if (eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK)
                hitMask |= PROC_HIT_NORMAL | PROC_HIT_CRITICAL;
            // for done procs allow normal + critical + absorbs by default
            else
                hitMask |= PROC_HIT_NORMAL | PROC_HIT_CRITICAL | PROC_HIT_ABSORB;
        }
        if (!(eventInfo.GetHitMask() & hitMask))
            return false;
    }

    return true;
}

SpellBonusEntry const* SpellMgr::GetSpellBonusData(uint32 spellId) const
{
    // Lookup data
    SpellBonusMap::const_iterator itr = mSpellBonusMap.find(spellId);
    if (itr != mSpellBonusMap.end())
        return &itr->second;
    // Not found, try lookup for 1 spell rank if exist
    if (uint32 rank_1 = GetFirstSpellInChain(spellId))
    {
        SpellBonusMap::const_iterator itr2 = mSpellBonusMap.find(rank_1);
        if (itr2 != mSpellBonusMap.end())
            return &itr2->second;
    }
    return NULL;
}

SpellThreatEntry const* SpellMgr::GetSpellThreatEntry(uint32 spellID) const
{
    SpellThreatMap::const_iterator itr = mSpellThreatMap.find(spellID);
    if (itr != mSpellThreatMap.end())
        return &itr->second;
    else
    {
        uint32 firstSpell = GetFirstSpellInChain(spellID);
        itr = mSpellThreatMap.find(firstSpell);
        if (itr != mSpellThreatMap.end())
            return &itr->second;
    }
    return NULL;
}

SkillLineAbilityMapBounds SpellMgr::GetSkillLineAbilityMapBounds(uint32 spell_id) const
{
    return mSkillLineAbilityMap.equal_range(spell_id);
}

PetAura const* SpellMgr::GetPetAura(uint32 spell_id, uint8 eff) const
{
    SpellPetAuraMap::const_iterator itr = mSpellPetAuraMap.find((spell_id<<8) + eff);
    if (itr != mSpellPetAuraMap.end())
        return &itr->second;
    else
        return NULL;
}

SpellEnchantProcEntry const* SpellMgr::GetSpellEnchantProcEvent(uint32 enchId) const
{
    SpellEnchantProcEventMap::const_iterator itr = mSpellEnchantProcEventMap.find(enchId);
    if (itr != mSpellEnchantProcEventMap.end())
        return &itr->second;
    return NULL;
}

bool SpellMgr::IsArenaAllowedEnchancment(uint32 ench_id) const
{
    return mEnchantCustomAttr[ench_id];
}

const std::vector<int32>* SpellMgr::GetSpellLinked(int32 spell_id) const
{
    SpellLinkedMap::const_iterator itr = mSpellLinkedMap.find(spell_id);
    return itr != mSpellLinkedMap.end() ? &(itr->second) : NULL;
}

PetLevelupSpellSet const* SpellMgr::GetPetLevelupSpellList(uint32 petFamily) const
{
    PetLevelupSpellMap::const_iterator itr = mPetLevelupSpellMap.find(petFamily);
    if (itr != mPetLevelupSpellMap.end())
        return &itr->second;
    else
        return NULL;
}

PetDefaultSpellsEntry const* SpellMgr::GetPetDefaultSpellsEntry(int32 id) const
{
    PetDefaultSpellsMap::const_iterator itr = mPetDefaultSpellsMap.find(id);
    if (itr != mPetDefaultSpellsMap.end())
        return &itr->second;
    return NULL;
}

SpellAreaMapBounds SpellMgr::GetSpellAreaMapBounds(uint32 spell_id) const
{
    return mSpellAreaMap.equal_range(spell_id);
}

SpellAreaForQuestMapBounds SpellMgr::GetSpellAreaForQuestMapBounds(uint32 quest_id) const
{
    return mSpellAreaForQuestMap.equal_range(quest_id);
}

SpellAreaForQuestMapBounds SpellMgr::GetSpellAreaForQuestEndMapBounds(uint32 quest_id) const
{
    return mSpellAreaForQuestEndMap.equal_range(quest_id);
}

SpellAreaForAuraMapBounds SpellMgr::GetSpellAreaForAuraMapBounds(uint32 spell_id) const
{
    return mSpellAreaForAuraMap.equal_range(spell_id);
}

SpellAreaForAreaMapBounds SpellMgr::GetSpellAreaForAreaMapBounds(uint32 area_id) const
{
    return mSpellAreaForAreaMap.equal_range(area_id);
}

bool SpellArea::IsFitToRequirements(Player const* player, uint32 newZone, uint32 newArea) const
{
    if (gender != GENDER_NONE)                   // not in expected gender
        if (!player || gender != player->getGender())
            return false;

    if (raceMask)                                // not in expected race
        if (!player || !(raceMask & player->getRaceMask()))
            return false;

    if (areaId)                                  // not in expected zone
        if (newZone != areaId && newArea != areaId)
            return false;

    if (questStart)                              // not in expected required quest state
        if (!player || (((1 << player->GetQuestStatus(questStart)) & questStartStatus) == 0))
            return false;

    if (questEnd)                                // not in expected forbidden quest state
        if (!player || (((1 << player->GetQuestStatus(questEnd)) & questEndStatus) == 0))
            return false;

    if (auraSpell)                               // not have expected aura
        if (!player || (auraSpell > 0 && !player->HasAura(auraSpell)) || (auraSpell < 0 && player->HasAura(-auraSpell)))
            return false;

    if (player)
    {
        if (Battleground* bg = player->GetBattleground())
            return bg->IsSpellAllowed(spellId, player);
    }

    // Extra conditions
    switch (spellId)
    {
        case 91604: // No fly Zone - Wintergrasp
        {
            if (!player)
                return false;

            Battlefield* Bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId());
            if (!Bf || Bf->CanFlyIn() || (!player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) && !player->HasAuraType(SPELL_AURA_FLY)))
                return false;
            break;
        }
        case 56618: // Horde Controls Factory Phase Shift
        case 56617: // Alliance Controls Factory Phase Shift
        {
            if (!player)
                return false;

            Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId());

            if (!bf || bf->GetTypeId() != BATTLEFIELD_WG)
                return false;

            // team that controls the workshop in the specified area
            uint32 team = bf->GetData(newArea);

            if (team == TEAM_HORDE)
                return spellId == 56618;
            else if (team == TEAM_ALLIANCE)
                return spellId == 56617;
            break;
        }
        case 57940: // Essence of Wintergrasp - Northrend
        case 58045: // Essence of Wintergrasp - Wintergrasp
        {
            if (!player)
                return false;

            if (Battlefield* battlefieldWG = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG))
                return battlefieldWG->IsEnabled() && (player->GetTeamId() == battlefieldWG->GetDefenderTeam()) && !battlefieldWG->IsWarTime();
            break;
        }
        case 74411: // Battleground - Dampening
        {
            if (!player)
                return false;

            if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId()))
                return bf->IsWarTime();
            break;
        }

    }

    return true;
}

void SpellMgr::UnloadSpellInfoChains()
{
    for (SpellChainMap::iterator itr = mSpellChains.begin(); itr != mSpellChains.end(); ++itr)
        mSpellInfoMap[itr->first]->ChainEntry = NULL;

    mSpellChains.clear();
}

void SpellMgr::LoadSpellTalentRanks()
{
    // Remove Me
}

void SpellMgr::LoadSpellRanks()
{
    // cleanup data and load spell ranks for talents from dbc
    LoadSpellTalentRanks();

    uint32 oldMSTime = getMSTime();

    //                                                     0             1      2
    QueryResult result = WorldDatabase.Query("SELECT first_spell_id, spell_id, rank from spell_ranks ORDER BY first_spell_id, rank");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell rank records. DB table `spell_ranks` is empty.");
        return;
    }

    uint32 count = 0;
    bool finished = false;

    do
    {
                        // spellid, rank
        std::list < std::pair < int32, int32 > > rankChain;
        int32 currentSpell = -1;
        int32 lastSpell = -1;

        // fill one chain
        while (currentSpell == lastSpell && !finished)
        {
            Field* fields = result->Fetch();

            currentSpell = fields[0].GetUInt32();
            if (lastSpell == -1)
                lastSpell = currentSpell;
            uint32 spell_id = fields[1].GetUInt32();
            uint32 rank = fields[2].GetUInt8();

            // don't drop the row if we're moving to the next rank
            if (currentSpell == lastSpell)
            {
                rankChain.push_back(std::make_pair(spell_id, rank));
                if (!result->NextRow())
                    finished = true;
            }
            else
                break;
        }
        // check if chain is made with valid first spell
        SpellInfo const* first = GetSpellInfo(lastSpell);
        if (!first)
        {
            TC_LOG_ERROR("sql.sql", "Spell rank identifier(first_spell_id) %u listed in `spell_ranks` does not exist!", lastSpell);
            continue;
        }
        // check if chain is long enough
        if (rankChain.size() < 2)
        {
            TC_LOG_ERROR("sql.sql", "There is only 1 spell rank for identifier(first_spell_id) %u in `spell_ranks`, entry is not needed!", lastSpell);
            continue;
        }
        int32 curRank = 0;
        bool valid = true;
        // check spells in chain
        for (std::list<std::pair<int32, int32> >::iterator itr = rankChain.begin(); itr!= rankChain.end(); ++itr)
        {
            SpellInfo const* spell = GetSpellInfo(itr->first);
            if (!spell)
            {
                TC_LOG_ERROR("sql.sql", "Spell %u (rank %u) listed in `spell_ranks` for chain %u does not exist!", itr->first, itr->second, lastSpell);
                valid = false;
                break;
            }
            ++curRank;
            if (itr->second != curRank)
            {
                TC_LOG_ERROR("sql.sql", "Spell %u (rank %u) listed in `spell_ranks` for chain %u does not have proper rank value(should be %u)!", itr->first, itr->second, lastSpell, curRank);
                valid = false;
                break;
            }
        }
        if (!valid)
            continue;
        int32 prevRank = 0;
        // insert the chain
        std::list<std::pair<int32, int32> >::iterator itr = rankChain.begin();
        do
        {
            ++count;
            int32 addedSpell = itr->first;

            if (mSpellInfoMap[addedSpell]->ChainEntry)
                TC_LOG_ERROR("sql.sql", "Spell %u (rank: %u, first: %u) listed in `spell_ranks` has already ChainEntry from dbc.", addedSpell, itr->second, lastSpell);

            mSpellChains[addedSpell].first = GetSpellInfo(lastSpell);
            mSpellChains[addedSpell].last = GetSpellInfo(rankChain.back().first);
            mSpellChains[addedSpell].rank = itr->second;
            mSpellChains[addedSpell].prev = GetSpellInfo(prevRank);
            mSpellInfoMap[addedSpell]->ChainEntry = &mSpellChains[addedSpell];
            prevRank = addedSpell;
            ++itr;

            if (itr == rankChain.end())
            {
                mSpellChains[addedSpell].next = NULL;
                break;
            }
            else
                mSpellChains[addedSpell].next = GetSpellInfo(itr->first);
        }
        while (true);
    }
    while (!finished);

    TC_LOG_INFO("server.loading", ">> Loaded %u spell rank records in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellRequired()
{
    uint32 oldMSTime = getMSTime();

    mSpellsReqSpell.clear();                                   // need for reload case
    mSpellReq.clear();                                         // need for reload case

    //                                                   0        1
    QueryResult result = WorldDatabase.Query("SELECT spell_id, req_spell from spell_required");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell required records. DB table `spell_required` is empty.");

        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell_id = fields[0].GetUInt32();
        uint32 spell_req = fields[1].GetUInt32();

        // check if chain is made with valid first spell
        SpellInfo const* spell = GetSpellInfo(spell_id);
        if (!spell)
        {
            TC_LOG_ERROR("sql.sql", "spell_id %u in `spell_required` table is not found in dbcs, skipped", spell_id);
            continue;
        }

        SpellInfo const* reqSpell = GetSpellInfo(spell_req);
        if (!reqSpell)
        {
            TC_LOG_ERROR("sql.sql", "req_spell %u in `spell_required` table is not found in dbcs, skipped", spell_req);
            continue;
        }

        if (spell->IsRankOf(reqSpell))
        {
            TC_LOG_ERROR("sql.sql", "req_spell %u and spell_id %u in `spell_required` table are ranks of the same spell, entry not needed, skipped", spell_req, spell_id);
            continue;
        }

        if (IsSpellRequiringSpell(spell_id, spell_req))
        {
            TC_LOG_ERROR("sql.sql", "duplicated entry of req_spell %u and spell_id %u in `spell_required`, skipped", spell_req, spell_id);
            continue;
        }

        mSpellReq.insert (std::pair<uint32, uint32>(spell_id, spell_req));
        mSpellsReqSpell.insert (std::pair<uint32, uint32>(spell_req, spell_id));
        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u spell required records in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void SpellMgr::LoadSpellLearnSkills()
{
    uint32 oldMSTime = getMSTime();

    mSpellLearnSkills.clear();                              // need for reload case

    // search auto-learned skills and add its to map also for use in unlearn spells/talents
    uint32 dbc_count = 0;
    for (uint32 spell = 0; spell < GetSpellInfoStoreSize(); ++spell)
    {
        SpellInfo const* entry = GetSpellInfo(spell);

        if (!entry)
            continue;

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (entry->Effects[i].Effect == SPELL_EFFECT_SKILL)
            {
                SpellLearnSkillNode dbc_node;
                dbc_node.skill = entry->Effects[i].MiscValue;
                dbc_node.step  = entry->Effects[i].CalcValue();
                if (dbc_node.skill != SKILL_RIDING)
                    dbc_node.value = 1;
                else
                    dbc_node.value = dbc_node.step * 75;
                dbc_node.maxvalue = dbc_node.step * 75;
                mSpellLearnSkills[spell] = dbc_node;
                ++dbc_count;
                break;
            }
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Spell Learn Skills from DBC in %u ms", dbc_count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellLearnSpells()
{
    uint32 oldMSTime = getMSTime();

    mSpellLearnSpells.clear();                              // need for reload case

    //                                                  0      1        2
    QueryResult result = WorldDatabase.Query("SELECT entry, SpellID, Active FROM spell_learn_spell");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell learn spells. DB table `spell_learn_spell` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell_id = fields[0].GetUInt32();

        SpellLearnSpellNode node;
        node.spell       = fields[1].GetUInt32();
        node.active      = fields[2].GetBool();
        node.autoLearned = false;

        if (!GetSpellInfo(spell_id))
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_learn_spell` does not exist", spell_id);
            continue;
        }

        if (!GetSpellInfo(node.spell))
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_learn_spell` learning not existed spell %u", spell_id, node.spell);
            continue;
        }

        if (GetTalentSpellCost(node.spell))
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_learn_spell` attempt learning talent spell %u, skipped", spell_id, node.spell);
            continue;
        }

        mSpellLearnSpells.insert(SpellLearnSpellMap::value_type(spell_id, node));

        ++count;
    } while (result->NextRow());

    // copy state loaded from db
    SpellLearnSpellMap dbSpellLearnSpells = mSpellLearnSpells;

    // search auto-learned spells and add its to map also for use in unlearn spells/talents
    uint32 dbc_count = 0;
    for (uint32 spell = 0; spell < GetSpellInfoStoreSize(); ++spell)
    {
        SpellInfo const* entry = GetSpellInfo(spell);

        if (!entry)
            continue;

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (entry->Effects[i].Effect == SPELL_EFFECT_LEARN_SPELL)
            {
                SpellLearnSpellNode dbc_node;
                dbc_node.spell = entry->Effects[i].TriggerSpell;
                dbc_node.active = true;                     // all dbc based learned spells is active (show in spell book or hide by client itself)

                // ignore learning not existed spells (broken/outdated/or generic learnig spell 483
                if (!GetSpellInfo(dbc_node.spell))
                    continue;

                // talent or passive spells or skill-step spells auto-casted and not need dependent learning,
                // pet teaching spells must not be dependent learning (casted)
                // other required explicit dependent learning
                dbc_node.autoLearned = entry->Effects[i].TargetA.GetTarget() == TARGET_UNIT_PET || GetTalentSpellCost(spell) > 0 || entry->IsPassive() || entry->HasEffect(SPELL_EFFECT_SKILL_STEP);

                SpellLearnSpellMapBounds db_node_bounds = dbSpellLearnSpells.equal_range(spell);

                bool found = false;
                for (SpellLearnSpellMap::const_iterator itr = db_node_bounds.first; itr != db_node_bounds.second; ++itr)
                {
                    if (itr->second.spell == dbc_node.spell)
                    {
                        TC_LOG_ERROR("sql.sql", "Spell %u auto-learn spell %u in spell.dbc then the record in `spell_learn_spell` is redundant, please fix DB.",
                            spell, dbc_node.spell);
                        found = true;
                        break;
                    }
                }

                if (!found)                                  // add new spell-spell pair if not found
                {
                    mSpellLearnSpells.insert(SpellLearnSpellMap::value_type(spell, dbc_node));
                    ++dbc_count;
                }
            }
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u spell learn spells, %u found in Spell.dbc and %u from TalentTab.dbc in %u ms", count, dbc_count, 0, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellTargetPositions()
{
    uint32 oldMSTime = getMSTime();

    mSpellTargetPositions.clear();                                // need for reload case

    //                                                0      1          2             3                  4                  5                   6
    QueryResult result = WorldDatabase.Query("SELECT id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation FROM spell_target_position");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell target coordinates. DB table `spell_target_position` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 Spell_ID = fields[0].GetUInt32();
        SpellEffIndex effIndex = SpellEffIndex(fields[1].GetUInt8());

        SpellTargetPosition st;

        st.target_mapId       = fields[2].GetUInt16();
        st.target_X           = fields[3].GetFloat();
        st.target_Y           = fields[4].GetFloat();
        st.target_Z           = fields[5].GetFloat();
        st.target_Orientation = fields[6].GetFloat();

        MapEntry const* mapEntry = sMapStore.LookupEntry(st.target_mapId);
        if (!mapEntry)
        {
            TC_LOG_ERROR("sql.sql", "Spell (Id: %u, effIndex: %u) target map (ID: %u) does not exist in `Map.dbc`.", Spell_ID, effIndex, st.target_mapId);
            continue;
        }

        if (st.target_X==0 && st.target_Y==0 && st.target_Z==0)
        {
            TC_LOG_ERROR("sql.sql", "Spell (Id: %u, effIndex: %u) target coordinates not provided.", Spell_ID, effIndex);
            continue;
        }

        SpellInfo const* spellInfo = GetSpellInfo(Spell_ID);
        if (!spellInfo)
        {
            TC_LOG_ERROR("sql.sql", "Spell (Id: %u) listed in `spell_target_position` does not exist.", Spell_ID);
            continue;
        }

        if (spellInfo->Effects[effIndex].TargetA.GetTarget() == TARGET_DEST_DB || spellInfo->Effects[effIndex].TargetB.GetTarget() == TARGET_DEST_DB)
        {
            std::pair<uint32, SpellEffIndex> key = std::make_pair(Spell_ID, effIndex);
            mSpellTargetPositions[key] = st;
            ++count;
        }
        else
        {
            TC_LOG_ERROR("sql.sql", "Spell (Id: %u, effIndex: %u) listed in `spell_target_position` does not have target TARGET_DEST_DB (17).", Spell_ID, effIndex);
            continue;
        }

    } while (result->NextRow());

    /*
    // Check all spells
    for (uint32 i = 1; i < GetSpellInfoStoreSize; ++i)
    {
        SpellInfo const* spellInfo = GetSpellInfo(i);
        if (!spellInfo)
            continue;

        bool found = false;
        for (int j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            switch (spellInfo->Effects[j].TargetA)
            {
                case TARGET_DEST_DB:
                    found = true;
                    break;
            }
            if (found)
                break;
            switch (spellInfo->Effects[j].TargetB)
            {
                case TARGET_DEST_DB:
                    found = true;
                    break;
            }
            if (found)
                break;
        }
        if (found)
        {
            if (!sSpellMgr->GetSpellTargetPosition(i))
                TC_LOG_DEBUG("spells", "Spell (ID: %u) does not have record in `spell_target_position`", i);
        }
    }*/

    TC_LOG_INFO("server.loading", ">> Loaded %u spell teleport coordinates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellGroups()
{
    uint32 oldMSTime = getMSTime();

    mSpellSpellGroup.clear();                                  // need for reload case
    mSpellGroupSpell.clear();

    //                                                0     1
    QueryResult result = WorldDatabase.Query("SELECT id, spell_id FROM spell_group");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell group definitions. DB table `spell_group` is empty.");
        return;
    }

    std::set<uint32> groups;
    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 group_id = fields[0].GetUInt32();
        if (group_id <= SPELL_GROUP_DB_RANGE_MIN && group_id >= SPELL_GROUP_CORE_RANGE_MAX)
        {
            TC_LOG_ERROR("sql.sql", "SpellGroup id %u listed in `spell_group` is in core range, but is not defined in core!", group_id);
            continue;
        }
        int32 spell_id = fields[1].GetInt32();

        groups.insert(std::set<uint32>::value_type(group_id));
        mSpellGroupSpell.insert(SpellGroupSpellMap::value_type((SpellGroup)group_id, spell_id));

    } while (result->NextRow());

    for (SpellGroupSpellMap::iterator itr = mSpellGroupSpell.begin(); itr!= mSpellGroupSpell.end();)
    {
        if (itr->second < 0)
        {
            if (groups.find(abs(itr->second)) == groups.end())
            {
                TC_LOG_ERROR("sql.sql", "SpellGroup id %u listed in `spell_group` does not exist", abs(itr->second));
                mSpellGroupSpell.erase(itr++);
            }
            else
                ++itr;
        }
        else
        {
            SpellInfo const* spellInfo = GetSpellInfo(itr->second);

            if (!spellInfo)
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_group` does not exist", itr->second);
                mSpellGroupSpell.erase(itr++);
            }
            else if (spellInfo->GetRank() > 1)
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_group` is not first rank of spell", itr->second);
                mSpellGroupSpell.erase(itr++);
            }
            else
                ++itr;
        }
    }

    for (std::set<uint32>::iterator groupItr = groups.begin(); groupItr != groups.end(); ++groupItr)
    {
        std::set<uint32> spells;
        GetSetOfSpellsInSpellGroup(SpellGroup(*groupItr), spells);

        for (std::set<uint32>::iterator spellItr = spells.begin(); spellItr != spells.end(); ++spellItr)
        {
            ++count;
            mSpellSpellGroup.insert(SpellSpellGroupMap::value_type(*spellItr, SpellGroup(*groupItr)));
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u spell group definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellGroupStackRules()
{
    uint32 oldMSTime = getMSTime();

    mSpellGroupStack.clear();                                  // need for reload case

    //                                                       0         1
    QueryResult result = WorldDatabase.Query("SELECT group_id, stack_rule FROM spell_group_stack_rules");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell group stack rules. DB table `spell_group_stack_rules` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 group_id = fields[0].GetUInt32();
        uint8 stack_rule = fields[1].GetInt8();
        if (stack_rule >= SPELL_GROUP_STACK_RULE_MAX)
        {
            TC_LOG_ERROR("sql.sql", "SpellGroupStackRule %u listed in `spell_group_stack_rules` does not exist", stack_rule);
            continue;
        }

        SpellGroupSpellMapBounds spellGroup = GetSpellGroupSpellMapBounds((SpellGroup)group_id);

        if (spellGroup.first == spellGroup.second)
        {
            TC_LOG_ERROR("sql.sql", "SpellGroup id %u listed in `spell_group_stack_rules` does not exist", group_id);
            continue;
        }

        mSpellGroupStack[(SpellGroup)group_id] = (SpellGroupStackRule)stack_rule;

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u spell group stack rules in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellProcEvents()
{
    uint32 oldMSTime = getMSTime();

    mSpellProcEventMap.clear();                             // need for reload case

    //                                                0      1           2                3                 4                 5                6                 7          8        9       10            11
    QueryResult result = WorldDatabase.Query("SELECT entry, SchoolMask, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, SpellFamilyMask3, procFlags, procEx, ppmRate, CustomChance, Cooldown FROM spell_proc_event");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell proc event conditions. DB table `spell_proc_event` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        int i = 0;

        int32 spellId = fields[i++].GetInt32();

        bool allRanks = false;
        if (spellId < 0)
        {
            allRanks = true;
            spellId = -spellId;
        }

        SpellInfo const* spellInfo = GetSpellInfo(spellId);
        if (!spellInfo)
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_proc_event` does not exist", spellId);
            continue;
        }

        if (allRanks)
        {
            if (!spellInfo->IsRanked())
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_proc_event` with all ranks, but spell has no ranks.", spellId);

            if (spellInfo->GetFirstRankSpell()->Id != uint32(spellId))
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_proc_event` is not first rank of spell.", spellId);
                continue;
            }
        }

        SpellProcEventEntry spellProcEvent;

        spellProcEvent.schoolMask         = fields[i++].GetInt8();
        spellProcEvent.spellFamilyName    = fields[i++].GetUInt16();
        spellProcEvent.spellFamilyMask[0] = fields[i++].GetUInt32();
        spellProcEvent.spellFamilyMask[1] = fields[i++].GetUInt32();
        spellProcEvent.spellFamilyMask[2] = fields[i++].GetUInt32();
        spellProcEvent.spellFamilyMask[3] = fields[i++].GetUInt32();
        spellProcEvent.procFlags          = fields[i++].GetUInt32();
        spellProcEvent.procEx             = fields[i++].GetUInt32();
        spellProcEvent.ppmRate            = fields[i++].GetFloat();
        spellProcEvent.customChance       = fields[i++].GetFloat();
        spellProcEvent.cooldown           = fields[i++].GetUInt32();

        while (spellInfo)
        {
            if (mSpellProcEventMap.find(spellInfo->Id) != mSpellProcEventMap.end())
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_proc_event` already has its first rank in table.", spellInfo->Id);
                break;
            }

            if (!spellInfo->ProcFlags && !spellProcEvent.procFlags)
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_proc_event` probably not triggered spell", spellInfo->Id);

            mSpellProcEventMap[spellInfo->Id] = spellProcEvent;

            if (allRanks)
                spellInfo = spellInfo->GetNextRankSpell();
            else
                break;
        }

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u extra spell proc event conditions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellProcs()
{
    uint32 oldMSTime = getMSTime();

    mSpellProcMap.clear();                             // need for reload case

    //                                                 0        1           2                3                 4                 5                 6         7              8               9        10              11             12      13        14
    QueryResult result = WorldDatabase.Query("SELECT spellId, schoolMask, spellFamilyName, spellFamilyMask0, spellFamilyMask1, spellFamilyMask2, typeMask, spellTypeMask, spellPhaseMask, hitMask, attributesMask, ratePerMinute, chance, cooldown, charges FROM spell_proc");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell proc conditions and data. DB table `spell_proc` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 spellId = fields[0].GetInt32();

        bool allRanks = false;
        if (spellId < 0)
        {
            allRanks = true;
            spellId = -spellId;
        }

        SpellInfo const* spellInfo = GetSpellInfo(spellId);
        if (!spellInfo)
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_proc` does not exist", spellId);
            continue;
        }

        if (allRanks)
        {
            if (!spellInfo->IsRanked())
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_proc` with all ranks, but spell has no ranks.", spellId);

            if (spellInfo->GetFirstRankSpell()->Id != uint32(spellId))
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_proc` is not first rank of spell.", spellId);
                continue;
            }
        }

        SpellProcEntry baseProcEntry;

        baseProcEntry.schoolMask      = fields[1].GetInt8();
        baseProcEntry.spellFamilyName = fields[2].GetUInt16();
        baseProcEntry.spellFamilyMask[0] = fields[3].GetUInt32();
        baseProcEntry.spellFamilyMask[1] = fields[4].GetUInt32();
        baseProcEntry.spellFamilyMask[2] = fields[5].GetUInt32();
        baseProcEntry.typeMask        = fields[6].GetUInt32();
        baseProcEntry.spellTypeMask   = fields[7].GetUInt32();
        baseProcEntry.spellPhaseMask  = fields[8].GetUInt32();
        baseProcEntry.hitMask         = fields[9].GetUInt32();
        baseProcEntry.attributesMask  = fields[10].GetUInt32();
        baseProcEntry.ratePerMinute   = fields[11].GetFloat();
        baseProcEntry.chance          = fields[12].GetFloat();
        float cooldown                = fields[13].GetFloat();
        baseProcEntry.cooldown        = uint32(cooldown);
        baseProcEntry.charges         = fields[14].GetUInt32();

        while (spellInfo)
        {
            if (mSpellProcMap.find(spellInfo->Id) != mSpellProcMap.end())
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_proc` already has its first rank in table.", spellInfo->Id);
                break;
            }

            SpellProcEntry procEntry = SpellProcEntry(baseProcEntry);

            // take defaults from dbcs
            if (!procEntry.typeMask)
                procEntry.typeMask = spellInfo->ProcFlags;
            if (!procEntry.charges)
                procEntry.charges = spellInfo->ProcCharges;
            if (!procEntry.chance && !procEntry.ratePerMinute)
                procEntry.chance = float(spellInfo->ProcChance);

            // validate data
            if (procEntry.schoolMask & ~SPELL_SCHOOL_MASK_ALL)
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has wrong `schoolMask` set: %u", spellInfo->Id, procEntry.schoolMask);
            if (procEntry.spellFamilyName && (procEntry.spellFamilyName < 3 || procEntry.spellFamilyName > 17 || procEntry.spellFamilyName == 14 || procEntry.spellFamilyName == 16))
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has wrong `spellFamilyName` set: %u", spellInfo->Id, procEntry.spellFamilyName);
            if (procEntry.chance < 0)
            {
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has negative value in `chance` field", spellInfo->Id);
                procEntry.chance = 0;
            }
            if (procEntry.ratePerMinute < 0)
            {
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has negative value in `ratePerMinute` field", spellInfo->Id);
                procEntry.ratePerMinute = 0;
            }
            if (cooldown < 0)
            {
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has negative value in `cooldown` field", spellInfo->Id);
                procEntry.cooldown = 0;
            }
            if (procEntry.chance == 0 && procEntry.ratePerMinute == 0)
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u doesn't have `chance` and `ratePerMinute` values defined, proc will not be triggered", spellInfo->Id);
            if (procEntry.charges > 99)
            {
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has too big value in `charges` field", spellInfo->Id);
                procEntry.charges = 99;
            }
            if (!procEntry.typeMask)
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u doesn't have `typeMask` value defined, proc will not be triggered", spellInfo->Id);
            if (procEntry.spellTypeMask & ~PROC_SPELL_TYPE_MASK_ALL)
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has wrong `spellTypeMask` set: %u", spellInfo->Id, procEntry.spellTypeMask);
            if (procEntry.spellTypeMask && !(procEntry.typeMask & (SPELL_PROC_FLAG_MASK | PERIODIC_PROC_FLAG_MASK)))
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has `spellTypeMask` value defined, but it won't be used for defined `typeMask` value", spellInfo->Id);
            if (!procEntry.spellPhaseMask && procEntry.typeMask & REQ_SPELL_PHASE_PROC_FLAG_MASK)
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u doesn't have `spellPhaseMask` value defined, but it's required for defined `typeMask` value, proc will not be triggered", spellInfo->Id);
            if (procEntry.spellPhaseMask & ~PROC_SPELL_PHASE_MASK_ALL)
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has wrong `spellPhaseMask` set: %u", spellInfo->Id, procEntry.spellPhaseMask);
            if (procEntry.spellPhaseMask && !(procEntry.typeMask & REQ_SPELL_PHASE_PROC_FLAG_MASK))
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has `spellPhaseMask` value defined, but it won't be used for defined `typeMask` value", spellInfo->Id);
            if (procEntry.hitMask & ~PROC_HIT_MASK_ALL)
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has wrong `hitMask` set: %u", spellInfo->Id, procEntry.hitMask);
            if (procEntry.hitMask && !(procEntry.typeMask & TAKEN_HIT_PROC_FLAG_MASK || (procEntry.typeMask & DONE_HIT_PROC_FLAG_MASK && (!procEntry.spellPhaseMask || procEntry.spellPhaseMask & (PROC_SPELL_PHASE_HIT | PROC_SPELL_PHASE_FINISH)))))
                TC_LOG_ERROR("sql.sql", "`spell_proc` table entry for spellId %u has `hitMask` value defined, but it won't be used for defined `typeMask` and `spellPhaseMask` values", spellInfo->Id);

            mSpellProcMap[spellInfo->Id] = procEntry;

            if (allRanks)
                spellInfo = spellInfo->GetNextRankSpell();
            else
                break;
        }
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u spell proc conditions and data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellBonusess()
{
    uint32 oldMSTime = getMSTime();

    mSpellBonusMap.clear();                             // need for reload case

    //                                                0      1             2          3         4
    QueryResult result = WorldDatabase.Query("SELECT entry, direct_bonus, dot_bonus, ap_bonus, ap_dot_bonus FROM spell_bonus_data");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell bonus data. DB table `spell_bonus_data` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 entry = fields[0].GetUInt32();

        SpellInfo const* spell = GetSpellInfo(entry);
        if (!spell)
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_bonus_data` does not exist", entry);
            continue;
        }

        SpellBonusEntry& sbe = mSpellBonusMap[entry];
        sbe.direct_damage = fields[1].GetFloat();
        sbe.dot_damage    = fields[2].GetFloat();
        sbe.ap_bonus      = fields[3].GetFloat();
        sbe.ap_dot_bonus   = fields[4].GetFloat();

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u extra spell bonus data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellThreats()
{
    uint32 oldMSTime = getMSTime();

    mSpellThreatMap.clear();                                // need for reload case

    //                                                0      1        2       3
    QueryResult result = WorldDatabase.Query("SELECT entry, flatMod, pctMod, apPctMod FROM spell_threat");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 aggro generating spells. DB table `spell_threat` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        if (!GetSpellInfo(entry))
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_threat` does not exist", entry);
            continue;
        }

        SpellThreatEntry ste;
        ste.flatMod  = fields[1].GetInt32();
        ste.pctMod   = fields[2].GetFloat();
        ste.apPctMod = fields[3].GetFloat();

        mSpellThreatMap[entry] = ste;
        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u SpellThreatEntries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSkillLineAbilityMap()
{
    uint32 oldMSTime = getMSTime();

    mSkillLineAbilityMap.clear();

    uint32 count = 0;

    for (uint32 i = 0; i < sSkillLineAbilityStore.GetNumRows(); ++i)
    {
        SkillLineAbilityEntry const* SkillInfo = sSkillLineAbilityStore.LookupEntry(i);
        if (!SkillInfo)
            continue;

        mSkillLineAbilityMap.insert(SkillLineAbilityMap::value_type(SkillInfo->spellId, SkillInfo));
        ++count;
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u SkillLineAbility MultiMap Data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellPetAuras()
{
    uint32 oldMSTime = getMSTime();

    mSpellPetAuraMap.clear();                                  // need for reload case

    //                                                  0       1       2    3
    QueryResult result = WorldDatabase.Query("SELECT spell, effectId, pet, aura FROM spell_pet_auras");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell pet auras. DB table `spell_pet_auras` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell = fields[0].GetUInt32();
        uint8 eff = fields[1].GetUInt8();
        uint32 pet = fields[2].GetUInt32();
        uint32 aura = fields[3].GetUInt32();

        SpellPetAuraMap::iterator itr = mSpellPetAuraMap.find((spell<<8) + eff);
        if (itr != mSpellPetAuraMap.end())
            itr->second.AddAura(pet, aura);
        else
        {
            SpellInfo const* spellInfo = GetSpellInfo(spell);
            if (!spellInfo)
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_pet_auras` does not exist", spell);
                continue;
            }
            if (spellInfo->Effects[eff].Effect != SPELL_EFFECT_DUMMY &&
               (spellInfo->Effects[eff].Effect != SPELL_EFFECT_APPLY_AURA ||
                spellInfo->Effects[eff].ApplyAuraName != SPELL_AURA_DUMMY))
            {
                TC_LOG_ERROR("spells", "Spell %u listed in `spell_pet_auras` does not have dummy aura or dummy effect", spell);
                continue;
            }

            SpellInfo const* spellInfo2 = GetSpellInfo(aura);
            if (!spellInfo2)
            {
                TC_LOG_ERROR("sql.sql", "Aura %u listed in `spell_pet_auras` does not exist", aura);
                continue;
            }

            PetAura pa(pet, aura, spellInfo->Effects[eff].TargetA.GetTarget() == TARGET_UNIT_PET, spellInfo->Effects[eff].CalcValue());
            mSpellPetAuraMap[(spell<<8) + eff] = pa;
        }

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u spell pet auras in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

// Fill custom data about enchancments
void SpellMgr::LoadEnchantCustomAttr()
{
    uint32 oldMSTime = getMSTime();

    uint32 size = sSpellItemEnchantmentStore.GetNumRows();
    mEnchantCustomAttr.resize(size);

    for (uint32 i = 0; i < size; ++i)
       mEnchantCustomAttr[i] = 0;

    uint32 count = 0;
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellInfo = GetSpellInfo(i);
        if (!spellInfo)
            continue;

        /// @todo find a better check
        if (!(spellInfo->AttributesEx2 & SPELL_ATTR2_PRESERVE_ENCHANT_IN_ARENA) || !(spellInfo->Attributes & SPELL_ATTR0_NOT_SHAPESHIFT))
            continue;

        for (uint32 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            if (spellInfo->Effects[j].Effect == SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY)
            {
                uint32 enchId = spellInfo->Effects[j].MiscValue;
                SpellItemEnchantmentEntry const* ench = sSpellItemEnchantmentStore.LookupEntry(enchId);
                if (!ench)
                    continue;
                mEnchantCustomAttr[enchId] = true;
                ++count;
                break;
            }
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u custom enchant attributes in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellEnchantProcData()
{
    uint32 oldMSTime = getMSTime();

    mSpellEnchantProcEventMap.clear();                             // need for reload case

    //                                                  0         1           2         3
    QueryResult result = WorldDatabase.Query("SELECT entry, customChance, PPMChance, procEx FROM spell_enchant_proc_data");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell enchant proc event conditions. DB table `spell_enchant_proc_data` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 enchantId = fields[0].GetUInt32();

        SpellItemEnchantmentEntry const* ench = sSpellItemEnchantmentStore.LookupEntry(enchantId);
        if (!ench)
        {
            TC_LOG_ERROR("sql.sql", "Enchancment %u listed in `spell_enchant_proc_data` does not exist", enchantId);
            continue;
        }

        SpellEnchantProcEntry spe;

        spe.customChance = fields[1].GetUInt32();
        spe.PPMChance = fields[2].GetFloat();
        spe.procEx = fields[3].GetUInt32();

        mSpellEnchantProcEventMap[enchantId] = spe;

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u enchant proc data definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellLinked()
{
    uint32 oldMSTime = getMSTime();

    mSpellLinkedMap.clear();    // need for reload case

    //                                                0              1             2
    QueryResult result = WorldDatabase.Query("SELECT spell_trigger, spell_effect, type FROM spell_linked_spell");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 linked spells. DB table `spell_linked_spell` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 trigger = fields[0].GetInt32();
        int32 effect = fields[1].GetInt32();
        int32 type = fields[2].GetUInt8();

        SpellInfo const* spellInfo = GetSpellInfo(abs(trigger));
        if (!spellInfo)
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_linked_spell` does not exist", abs(trigger));
            continue;
        }
        spellInfo = GetSpellInfo(abs(effect));
        if (!spellInfo)
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_linked_spell` does not exist", abs(effect));
            continue;
        }

        if (type) //we will find a better way when more types are needed
        {
            if (trigger > 0)
                trigger += SPELL_LINKED_MAX_SPELLS * type;
            else
                trigger -= SPELL_LINKED_MAX_SPELLS * type;
        }
        mSpellLinkedMap[trigger].push_back(effect);

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u linked spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadPetLevelupSpellMap()
{
    uint32 oldMSTime = getMSTime();

    mPetLevelupSpellMap.clear();                                   // need for reload case

    uint32 count = 0;
    uint32 family_count = 0;

    for (uint32 i = 0; i < sCreatureFamilyStore.GetNumRows(); ++i)
    {
        CreatureFamilyEntry const* creatureFamily = sCreatureFamilyStore.LookupEntry(i);
        if (!creatureFamily)                                     // not exist
            continue;

        for (uint8 j = 0; j < 2; ++j)
        {
            if (!creatureFamily->skillLine[j])
                continue;

            for (uint32 k = 0; k < sSkillLineAbilityStore.GetNumRows(); ++k)
            {
                SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(k);
                if (!skillLine)
                    continue;

                //if (skillLine->skillId != creatureFamily->skillLine[0] &&
                //    (!creatureFamily->skillLine[1] || skillLine->skillId != creatureFamily->skillLine[1]))
                //    continue;

                if (skillLine->skillId != creatureFamily->skillLine[j])
                    continue;

                if (skillLine->learnOnGetSkill != ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL)
                    continue;

                SpellInfo const* spell = GetSpellInfo(skillLine->spellId);
                if (!spell) // not exist or triggered or talent
                    continue;

                if (!spell->SpellLevel)
                    continue;

                PetLevelupSpellSet& spellSet = mPetLevelupSpellMap[creatureFamily->ID];
                if (spellSet.empty())
                    ++family_count;

                spellSet.insert(PetLevelupSpellSet::value_type(spell->SpellLevel, spell->Id));
                ++count;
            }
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u pet levelup and default spells for %u families in %u ms", count, family_count, GetMSTimeDiffToNow(oldMSTime));
}

bool LoadPetDefaultSpells_helper(CreatureTemplate const* cInfo, PetDefaultSpellsEntry& petDefSpells)
{
    // skip empty list;
    bool have_spell = false;
    for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
    {
        if (petDefSpells.spellid[j])
        {
            have_spell = true;
            break;
        }
    }
    if (!have_spell)
        return false;

    // remove duplicates with levelupSpells if any
    if (PetLevelupSpellSet const* levelupSpells = cInfo->family ? sSpellMgr->GetPetLevelupSpellList(cInfo->family) : NULL)
    {
        for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
        {
            if (!petDefSpells.spellid[j])
                continue;

            for (PetLevelupSpellSet::const_iterator itr = levelupSpells->begin(); itr != levelupSpells->end(); ++itr)
            {
                if (itr->second == petDefSpells.spellid[j])
                {
                    petDefSpells.spellid[j] = 0;
                    break;
                }
            }
        }
    }

    // skip empty list;
    have_spell = false;
    for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
    {
        if (petDefSpells.spellid[j])
        {
            have_spell = true;
            break;
        }
    }

    return have_spell;
}

void SpellMgr::LoadPetDefaultSpells()
{
    uint32 oldMSTime = getMSTime();

    mPetDefaultSpellsMap.clear();

    uint32 countCreature = 0;
    uint32 countData = 0;

    CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
    for (CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
    {

        if (!itr->second.PetSpellDataId)
            continue;

        // for creature with PetSpellDataId get default pet spells from dbc
        CreatureSpellDataEntry const* spellDataEntry = sCreatureSpellDataStore.LookupEntry(itr->second.PetSpellDataId);
        if (!spellDataEntry)
            continue;

        int32 petSpellsId = -int32(itr->second.PetSpellDataId);
        PetDefaultSpellsEntry petDefSpells;
        for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
            petDefSpells.spellid[j] = spellDataEntry->spellId[j];

        if (LoadPetDefaultSpells_helper(&itr->second, petDefSpells))
        {
            mPetDefaultSpellsMap[petSpellsId] = petDefSpells;
            ++countData;
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded addition spells for %u pet spell data entries in %u ms", countData, GetMSTimeDiffToNow(oldMSTime));

    TC_LOG_INFO("server.loading", "Loading summonable creature templates...");
    oldMSTime = getMSTime();

    // different summon spells
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellEntry = GetSpellInfo(i);
        if (!spellEntry)
            continue;

        for (uint8 k = 0; k < MAX_SPELL_EFFECTS; ++k)
        {
            if (spellEntry->Effects[k].Effect == SPELL_EFFECT_SUMMON || spellEntry->Effects[k].Effect == SPELL_EFFECT_SUMMON_PET)
            {
                uint32 creature_id = spellEntry->Effects[k].MiscValue;
                CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(creature_id);
                if (!cInfo)
                    continue;

                // already loaded
                if (cInfo->PetSpellDataId)
                    continue;

                // for creature without PetSpellDataId get default pet spells from creature_template
                int32 petSpellsId = cInfo->Entry;
                if (mPetDefaultSpellsMap.find(cInfo->Entry) != mPetDefaultSpellsMap.end())
                    continue;

                PetDefaultSpellsEntry petDefSpells;
                for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
                    petDefSpells.spellid[j] = cInfo->spells[j];

                if (LoadPetDefaultSpells_helper(cInfo, petDefSpells))
                {
                    mPetDefaultSpellsMap[petSpellsId] = petDefSpells;
                    ++countCreature;
                }
            }
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u summonable creature templates in %u ms", countCreature, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellAreas()
{
    uint32 oldMSTime = getMSTime();

    mSpellAreaMap.clear();                                  // need for reload case
    mSpellAreaForQuestMap.clear();
    mSpellAreaForQuestEndMap.clear();
    mSpellAreaForAuraMap.clear();

    //                                                  0     1         2              3               4                 5          6          7       8         9
    QueryResult result = WorldDatabase.Query("SELECT spell, area, quest_start, quest_start_status, quest_end_status, quest_end, aura_spell, racemask, gender, autocast FROM spell_area");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell area requirements. DB table `spell_area` is empty.");

        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell = fields[0].GetUInt32();
        SpellArea spellArea;
        spellArea.spellId             = spell;
        spellArea.areaId              = fields[1].GetUInt32();
        spellArea.questStart          = fields[2].GetUInt32();
        spellArea.questStartStatus    = fields[3].GetUInt32();
        spellArea.questEndStatus      = fields[4].GetUInt32();
        spellArea.questEnd            = fields[5].GetUInt32();
        spellArea.auraSpell           = fields[6].GetInt32();
        spellArea.raceMask            = fields[7].GetUInt32();
        spellArea.gender              = Gender(fields[8].GetUInt8());
        spellArea.autocast            = fields[9].GetBool();

        if (SpellInfo const* spellInfo = GetSpellInfo(spell))
        {
            if (spellArea.autocast)
                const_cast<SpellInfo*>(spellInfo)->Attributes |= SPELL_ATTR0_CANT_CANCEL;
        }
        else
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` does not exist", spell);
            continue;
        }

        {
            bool ok = true;
            SpellAreaMapBounds sa_bounds = GetSpellAreaMapBounds(spellArea.spellId);
            for (SpellAreaMap::const_iterator itr = sa_bounds.first; itr != sa_bounds.second; ++itr)
            {
                if (spellArea.spellId != itr->second.spellId)
                    continue;
                if (spellArea.areaId != itr->second.areaId)
                    continue;
                if (spellArea.questStart != itr->second.questStart)
                    continue;
                if (spellArea.auraSpell != itr->second.auraSpell)
                    continue;
                if ((spellArea.raceMask & itr->second.raceMask) == 0)
                    continue;
                if (spellArea.gender != itr->second.gender)
                    continue;

                // duplicate by requirements
                ok = false;
                break;
            }

            if (!ok)
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` already listed with similar requirements.", spell);
                continue;
            }
        }

        if (spellArea.areaId && !GetAreaEntryByAreaID(spellArea.areaId))
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` have wrong area (%u) requirement", spell, spellArea.areaId);
            continue;
        }

        if (spellArea.questStart && !sObjectMgr->GetQuestTemplate(spellArea.questStart))
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` have wrong start quest (%u) requirement", spell, spellArea.questStart);
            continue;
        }

        if (spellArea.questEnd)
        {
            if (!sObjectMgr->GetQuestTemplate(spellArea.questEnd))
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` have wrong end quest (%u) requirement", spell, spellArea.questEnd);
                continue;
            }
        }

        if (spellArea.auraSpell)
        {
            SpellInfo const* spellInfo = GetSpellInfo(abs(spellArea.auraSpell));
            if (!spellInfo)
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` have wrong aura spell (%u) requirement", spell, abs(spellArea.auraSpell));
                continue;
            }

            if (uint32(abs(spellArea.auraSpell)) == spellArea.spellId)
            {
                TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` have aura spell (%u) requirement for itself", spell, abs(spellArea.auraSpell));
                continue;
            }

            // not allow autocast chains by auraSpell field (but allow use as alternative if not present)
            if (spellArea.autocast && spellArea.auraSpell > 0)
            {
                bool chain = false;
                SpellAreaForAuraMapBounds saBound = GetSpellAreaForAuraMapBounds(spellArea.spellId);
                for (SpellAreaForAuraMap::const_iterator itr = saBound.first; itr != saBound.second; ++itr)
                {
                    if (itr->second->autocast && itr->second->auraSpell > 0)
                    {
                        chain = true;
                        break;
                    }
                }

                if (chain)
                {
                    TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` have aura spell (%u) requirement that itself autocast from aura", spell, spellArea.auraSpell);
                    continue;
                }

                SpellAreaMapBounds saBound2 = GetSpellAreaMapBounds(spellArea.auraSpell);
                for (SpellAreaMap::const_iterator itr2 = saBound2.first; itr2 != saBound2.second; ++itr2)
                {
                    if (itr2->second.autocast && itr2->second.auraSpell > 0)
                    {
                        chain = true;
                        break;
                    }
                }

                if (chain)
                {
                    TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` have aura spell (%u) requirement that itself autocast from aura", spell, spellArea.auraSpell);
                    continue;
                }
            }
        }

        if (spellArea.raceMask && (spellArea.raceMask & RACEMASK_ALL_PLAYABLE) == 0)
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` have wrong race mask (%u) requirement", spell, spellArea.raceMask);
            continue;
        }

        if (spellArea.gender != GENDER_NONE && spellArea.gender != GENDER_FEMALE && spellArea.gender != GENDER_MALE)
        {
            TC_LOG_ERROR("sql.sql", "Spell %u listed in `spell_area` have wrong gender (%u) requirement", spell, spellArea.gender);
            continue;
        }

        SpellArea const* sa = &mSpellAreaMap.insert(SpellAreaMap::value_type(spell, spellArea))->second;

        // for search by current zone/subzone at zone/subzone change
        if (spellArea.areaId)
            mSpellAreaForAreaMap.insert(SpellAreaForAreaMap::value_type(spellArea.areaId, sa));

        // for search at quest start/reward
        if (spellArea.questStart)
            mSpellAreaForQuestMap.insert(SpellAreaForQuestMap::value_type(spellArea.questStart, sa));

        // for search at quest start/reward
        if (spellArea.questEnd)
            mSpellAreaForQuestEndMap.insert(SpellAreaForQuestMap::value_type(spellArea.questEnd, sa));

        // for search at aura apply
        if (spellArea.auraSpell)
            mSpellAreaForAuraMap.insert(SpellAreaForAuraMap::value_type(abs(spellArea.auraSpell), sa));

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u spell area requirements in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

// Temporary structure to hold spell effect entries for faster loading
struct SpellEffectArray
{
    SpellEffectArray()
    {
        for (int i = 0; i < MAX_SPELL_EFFECTS; i++)
            effects[i] = NULL;
    }

    SpellEffectEntry const* effects[MAX_SPELL_EFFECTS];
};

void SpellMgr::LoadSpellInfoStore()
{
    uint32 oldMSTime = getMSTime();

    UnloadSpellInfoStore();
    mSpellInfoMap.resize(sSpellStore.GetNumRows(), NULL);

    std::map<uint32, SpellEffectArray> effectsBySpell;

    for (uint32 i = 0; i < sSpellEffectStore.GetNumRows(); ++i)
    {
        SpellEffectEntry const* effect = sSpellEffectStore.LookupEntry(i);
        if (!effect)
            continue;

        effectsBySpell[effect->EffectSpellId].effects[effect->EffectIndex] = effect;
    }

    for (uint32 i = 0; i < sSpellStore.GetNumRows(); ++i)
        if (SpellEntry const* spellEntry = sSpellStore.LookupEntry(i))
            mSpellInfoMap[i] = new SpellInfo(spellEntry, effectsBySpell[i].effects);

    TC_LOG_INFO("server.loading", ">> Loaded SpellInfo store in %u ms", GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::UnloadSpellInfoStore()
{
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
        delete mSpellInfoMap[i];

    mSpellInfoMap.clear();
}

void SpellMgr::UnloadSpellInfoImplicitTargetConditionLists()
{
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
        if (mSpellInfoMap[i])
            mSpellInfoMap[i]->_UnloadImplicitTargetConditionLists();
}

void SpellMgr::LoadSpellInfoCustomAttributes()
{
    uint32 oldMSTime = getMSTime();

    SpellInfo* spellInfo = NULL;
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        spellInfo = mSpellInfoMap[i];
        if (!spellInfo)
            continue;

        for (uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            switch (spellInfo->Effects[j].ApplyAuraName)
            {
                case SPELL_AURA_MOD_POSSESS:
                case SPELL_AURA_MOD_CONFUSE:
                case SPELL_AURA_MOD_CHARM:
                case SPELL_AURA_AOE_CHARM:
                case SPELL_AURA_MOD_FEAR:
                case SPELL_AURA_MOD_STUN:
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_AURA_CC;
                    break;
                case SPELL_AURA_PERIODIC_HEAL:
                case SPELL_AURA_PERIODIC_DAMAGE:
                case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
                case SPELL_AURA_PERIODIC_LEECH:
                case SPELL_AURA_PERIODIC_MANA_LEECH:
                case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
                case SPELL_AURA_PERIODIC_ENERGIZE:
                case SPELL_AURA_OBS_MOD_HEALTH:
                case SPELL_AURA_OBS_MOD_POWER:
                case SPELL_AURA_POWER_BURN:
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_NO_INITIAL_THREAT;
                    break;
            }

            switch (spellInfo->Effects[j].Effect)
            {
                case SPELL_EFFECT_SCHOOL_DAMAGE:
                case SPELL_EFFECT_WEAPON_DAMAGE:
                case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
                case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                case SPELL_EFFECT_HEAL:
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_DIRECT_DAMAGE;
                    break;
                case SPELL_EFFECT_POWER_DRAIN:
                case SPELL_EFFECT_POWER_BURN:
                case SPELL_EFFECT_HEAL_MAX_HEALTH:
                case SPELL_EFFECT_HEALTH_LEECH:
                case SPELL_EFFECT_HEAL_PCT:
                case SPELL_EFFECT_ENERGIZE_PCT:
                case SPELL_EFFECT_ENERGIZE:
                case SPELL_EFFECT_HEAL_MECHANICAL:
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_NO_INITIAL_THREAT;
                    break;
                case SPELL_EFFECT_CHARGE:
                case SPELL_EFFECT_CHARGE_DEST:
                case SPELL_EFFECT_JUMP:
                case SPELL_EFFECT_JUMP_DEST:
                case SPELL_EFFECT_LEAP_BACK:
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_CHARGE;
                    break;
                case SPELL_EFFECT_PICKPOCKET:
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_PICKPOCKET;
                    break;
                case SPELL_EFFECT_ENCHANT_ITEM:
                case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
                case SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC:
                case SPELL_EFFECT_ENCHANT_HELD_ITEM:
                {
                    // only enchanting profession enchantments procs can stack
                    if (IsPartOfSkillLine(SKILL_ENCHANTING, i))
                    {
                        uint32 enchantId = spellInfo->Effects[j].MiscValue;
                        SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(enchantId);
                        if (!enchant)
                            break;

                        for (uint8 s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
                        {
                            if (enchant->type[s] != ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
                                continue;

                            SpellInfo* procInfo = _GetSpellInfo(enchant->spellid[s]);
                            if (!procInfo)
                                continue;

                            // if proced directly from enchantment, not via proc aura
                            // NOTE: Enchant Weapon - Blade Ward also has proc aura spell and is proced directly
                            // however its not expected to stack so this check is good
                            if (procInfo->HasAura(SPELL_AURA_PROC_TRIGGER_SPELL))
                                continue;

                            procInfo->AttributesCu |= SPELL_ATTR0_CU_ENCHANT_PROC;
                        }
                    }
                    break;
                }
            }
        }

        if (!spellInfo->_IsPositiveEffect(EFFECT_0, false))
            spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF0;

        if (!spellInfo->_IsPositiveEffect(EFFECT_1, false))
            spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF1;

        if (!spellInfo->_IsPositiveEffect(EFFECT_2, false))
            spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF2;

        if (spellInfo->SpellVisual[0] == 3879)
            spellInfo->AttributesCu |= SPELL_ATTR0_CU_CONE_BACK;

        spellInfo->_InitializeExplicitTargetMask();
    }

    TC_LOG_INFO("server.loading", ">> Loaded SpellInfo custom attributes in %u ms", GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellInfoCorrections()
{
    uint32 oldMSTime = getMSTime();

    SpellInfo* spellInfo = NULL;
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        spellInfo = (SpellInfo*)mSpellInfoMap[i];
        if (!spellInfo)
            continue;

        for (uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            switch (spellInfo->Effects[j].Effect)
            {
                case SPELL_EFFECT_CHARGE:
                case SPELL_EFFECT_CHARGE_DEST:
                case SPELL_EFFECT_JUMP:
                case SPELL_EFFECT_JUMP_DEST:
                case SPELL_EFFECT_LEAP_BACK:
                    if (!spellInfo->Speed && !spellInfo->SpellFamilyName)
                        spellInfo->Speed = SPEED_CHARGE;
                    break;
            }
        }

        if (spellInfo->ActiveIconID == 2158)  // flight
            spellInfo->Attributes |= SPELL_ATTR0_PASSIVE;
    }

    if (SummonPropertiesEntry* properties = const_cast<SummonPropertiesEntry*>(sSummonPropertiesStore.LookupEntry(121)))
        properties->Type = SUMMON_TYPE_TOTEM;
    if (SummonPropertiesEntry* properties = const_cast<SummonPropertiesEntry*>(sSummonPropertiesStore.LookupEntry(647))) // 52893
        properties->Type = SUMMON_TYPE_TOTEM;

    TC_LOG_INFO("server.loading", ">> Loaded SpellInfo corrections in %u ms", GetMSTimeDiffToNow(oldMSTime));
}
