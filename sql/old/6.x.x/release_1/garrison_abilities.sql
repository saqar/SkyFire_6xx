/*
Navicat MySQL Data Transfer

Source Server         : Skyfire6xx
Source Server Version : 50540
Source Host           : localhost:3306
Source Database       : world

Target Server Type    : MYSQL
Target Server Version : 50540
File Encoding         : 65001

Date: 2014-11-06 05:13:12
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for garrison_abilities
-- ----------------------------
DROP TABLE IF EXISTS `garrison_abilities`;
CREATE TABLE `garrison_abilities` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `Type` int(11) NOT NULL DEFAULT '0',
  `Name` text NOT NULL,
  `Description` text NOT NULL,
  `SpellID` int(11) NOT NULL DEFAULT '0',
  `Data1` int(11) NOT NULL DEFAULT '0',
  `Data2` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Export of GarrAbility.db2';

-- ----------------------------
-- Records of garrison_abilities
-- ----------------------------
INSERT INTO `garrison_abilities` VALUES ('4', '1', 'Orcslayer', 'Increases success chance when fighting orcs.', '897144', '0', '1');
INSERT INTO `garrison_abilities` VALUES ('5', '0', 'Blizzard', 'Cast by a mage to deal damage to many enemies.', '135857', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('6', '0', 'Shield Wall', 'Used by a warrior to greatly reduce damage.', '132362', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('7', '1', 'Mountaineer', 'Increases success chance when fighting in the mountains.', '236848', '0', '6');
INSERT INTO `garrison_abilities` VALUES ('8', '1', 'Cold-Blooded', 'Increases success chance when fighting in the snow.', '236833', '0', '6');
INSERT INTO `garrison_abilities` VALUES ('9', '1', 'Wastelander', 'Increases success chance when fighting in the desert.', '236846', '0', '6');
INSERT INTO `garrison_abilities` VALUES ('10', '0', 'Leap of Faith', 'Cast by a priest to help the target escape harm\'s way (or just to mess with them).', '463835', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('11', '0', 'Prayer of Healing', 'Cast by a priest to heal a group of allies.', '135943', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('29', '33', 'Fast Learner', 'Increases the experience this follower gains from a mission by 50%.', '236225', '0', '7');
INSERT INTO `garrison_abilities` VALUES ('30', '3', 'Natural Leader', 'Increases the XP gained for all followers on a mission.', '236362', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('36', '1', 'Demonslayer', 'Increases success chance when fighting demons.', '254651', '0', '1');
INSERT INTO `garrison_abilities` VALUES ('37', '1', 'Beastslayer', 'Increases success chance when fighting beasts.', '298659', '0', '1');
INSERT INTO `garrison_abilities` VALUES ('38', '1', 'Ogreslayer', 'Increases success chance when fighting ogres.', '236695', '0', '1');
INSERT INTO `garrison_abilities` VALUES ('39', '1', 'Primalslayer', 'Increases success chance when fighting primals and their minions.', '254105', '0', '1');
INSERT INTO `garrison_abilities` VALUES ('40', '1', 'Gronnslayer', 'Increases success chance when fighting breakers and their minions.', '236412', '0', '1');
INSERT INTO `garrison_abilities` VALUES ('41', '1', 'Furyslayer', 'Increases success chance when fighting elemental furies.', '254652', '0', '1');
INSERT INTO `garrison_abilities` VALUES ('42', '1', 'Voidslayer', 'Increases success chance when fighting aberrations and undead.', '298680', '0', '1');
INSERT INTO `garrison_abilities` VALUES ('43', '1', 'Talonslayer', 'Increases success chance when fighting arakkoa and their minions.', '952507', '0', '1');
INSERT INTO `garrison_abilities` VALUES ('44', '1', 'Naturalist', 'Increases success chance when fighting in a forest.', '236830', '0', '6');
INSERT INTO `garrison_abilities` VALUES ('45', '1', 'Cave Dweller', 'Increases success chance when fighting underground.', '409595', '0', '6');
INSERT INTO `garrison_abilities` VALUES ('46', '1', 'Guerilla Fighter', 'Increases success chance when fighting in the jungle.', '236850', '0', '6');
INSERT INTO `garrison_abilities` VALUES ('47', '3', 'Urbanite', 'Increases success chance when fighting in a town.', '462338', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('48', '1', 'Marshwalker', 'Increases success chance when fighting in a swamp.', '236819', '0', '6');
INSERT INTO `garrison_abilities` VALUES ('49', '1', 'Plainsrunner', 'Increases success chance when fighting on plains.', '236712', '0', '6');
INSERT INTO `garrison_abilities` VALUES ('52', '33', 'Mining', 'Unlocks mining carts and boosts work order production when assigned to the Mine.', '136248', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('53', '33', 'Herbalism', 'Grants herbalism benefits and boosts work orders when assigned to the Herb Garden.', '136246', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('54', '33', 'Alchemy', 'Offers various Alchemy benefits when assigned to the Alchemy Lab.', '136240', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('55', '33', 'Blacksmithing', 'Can reduce your durability damage and increase work order output when assigned to the Forge.', '136241', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('56', '33', 'Enchanting', 'Offers a variety of enchanting benefits when assigned to the Enchanter\'s Study.', '136244', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('57', '33', 'Engineering', 'Sells engineering gadgets and boosts work orders when assigned to the Engineering Works.', '136243', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('58', '33', 'Inscription', 'Allows you create special documents and boosts work orders when assigned to the Scribe\'s Quarters.', '237171', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('59', '33', 'Jewelcrafting', 'Offers a variety of Jewelcrafting benefits when assigned to the Gem Boutique.', '134071', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('60', '33', 'Leatherworking', 'Can craft tents and boost work orders when assigned to the Tannery.', '136247', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('61', '33', 'Tailoring', 'Offers a variety of tailoring benefits when assigned to the Tailoring Emporium.', '136249', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('62', '33', 'Skinning', 'Boosts work order production based on follower level while assigned to the Barn.', '134366', '0', '5');
INSERT INTO `garrison_abilities` VALUES ('63', '9', 'Gnome-Lover', 'Increases success chance when on a mission with a gnome.', '236446', '73', '4');
INSERT INTO `garrison_abilities` VALUES ('64', '9', 'Humanist', 'Increases success chance when on a mission with a human.', '236447', '71', '4');
INSERT INTO `garrison_abilities` VALUES ('65', '9', 'Dwarvenborn', 'Increases success chance when on a mission with a dwarf.', '236443', '70', '4');
INSERT INTO `garrison_abilities` VALUES ('66', '9', 'Child of the Moon', 'Increases success chance when on a mission with a night elf.', '236450', '74', '4');
INSERT INTO `garrison_abilities` VALUES ('67', '9', 'Ally of Argus', 'Increases success chance when on a mission with a draenei.', '236442', '72', '4');
INSERT INTO `garrison_abilities` VALUES ('68', '9', 'Canine Companion', 'Increases success chance when on a mission with a worgen.', '528692', '75', '4');
INSERT INTO `garrison_abilities` VALUES ('69', '1', 'Brew Aficionado', 'Increases success chance when on a mission with a pandaren.', '626190', '0', '4');
INSERT INTO `garrison_abilities` VALUES ('70', '5', 'Child of Draenor', 'Increases success chance when on a mission with an orc.', '236451', '65', '4');
INSERT INTO `garrison_abilities` VALUES ('71', '5', 'Death Fascination', 'Increases success chance when on a mission with an undead.', '236457', '64', '4');
INSERT INTO `garrison_abilities` VALUES ('72', '5', 'Totemist', 'Increases success chance when on a mission with a tauren.', '236454', '67', '4');
INSERT INTO `garrison_abilities` VALUES ('73', '5', 'Voodoo Zealot', 'Increases success chance when on a mission with a troll.', '236456', '63', '4');
INSERT INTO `garrison_abilities` VALUES ('74', '5', 'Elvenkind', 'Increases success chance when on a mission with a blood elf.', '236440', '66', '4');
INSERT INTO `garrison_abilities` VALUES ('75', '5', 'Economist', 'Increases success chance when on a mission with a goblin.', '632354', '68', '4');
INSERT INTO `garrison_abilities` VALUES ('76', '1', 'High Stamina', 'Increases success chance on missions with duration longer than 7 hours.', '135987', '0', '8');
INSERT INTO `garrison_abilities` VALUES ('77', '1', 'Burst of Power', 'Increases success chance on missions with duration shorter than 7 hours.', '136088', '0', '8');
INSERT INTO `garrison_abilities` VALUES ('78', '3', 'Lone Wolf', 'Increases success chance when on a mission alone.', '538568', '0', '9');
INSERT INTO `garrison_abilities` VALUES ('79', '1', 'Scavenger', 'Increases the garrison resources gained from a mission by 200%.', '413571', '0', '7');
INSERT INTO `garrison_abilities` VALUES ('80', '1', 'Extra Training', 'Increases the experience gained from a mission for all followers by 35%.', '975743', '0', '7');
INSERT INTO `garrison_abilities` VALUES ('81', '3', 'Advanced Training', 'Increases the experience gained from a mission for all followers by 100%.', '615342', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('82', '3', 'Combat Experience', 'Increases success chance for all bonus treasures on a mission.', '413592', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('83', '3', 'Combat Expertise', 'Increases success chance for all bonus treasures on a mission.', '413593', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('84', '3', 'Teleport', 'Reduces travel time by 50%.', '135736', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('85', '3', 'Mass Teleport', 'Reduces travel time by 100%.', '136054', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('87', '3', 'Well-Equipped', 'Increases the garrison resources gained from a mission by 50%.', '458725', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('88', '3', 'Heavily-Equipped', 'Increases the garrison resources gained from a mission by 100%.', '132965', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('89', '3', 'Bronze Key', 'Increases the chance to obtain treasure from a bronze chest.', '134241', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('90', '3', 'Silver Key', 'Increases the chance to obtain treasure from a silver chest.', '134245', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('91', '3', 'Gold Key', 'Increases the chance to obtain treasure from a gold chest.', '135828', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('92', '3', 'Enchanted Key', 'Increases the chance to obtain treasure from all chests.', '134236', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('93', '3', 'Unstable Portal', 'Reduces travel time by 25%.', '607512', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('94', '3', 'Unstable Rift', 'Reduces travel time by 75%.', '607513', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('95', '3', 'Quality Provisions', 'Reduces mission duration by 25%.', '136000', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('96', '3', 'Epic Provisions', 'Reduces mission duration by 50%.', '250626', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('97', '3', 'Manual of Tactics', 'Increases the experience gained from a mission for all followers by 25%.', '133735', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('98', '3', 'Manual of Strategy', 'Increases the experience gained from a mission for all followers by 75%.', '133737', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('99', '3', 'Manual of War', 'Increases the experience gained from a mission for all followers by 150%.', '442732', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('100', '0', 'Taunt', 'Forces an enemy to attack the warrior.', '136080', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('101', '0', 'Multi-Shot', 'Used by a hunter to damage many enemies.', '132330', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('102', '0', 'Heroic Leap', 'Used by a warrior to quickly escape harm\'s way.', '236171', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('103', '0', 'Rapid Fire', 'Significantly increases the hunter\'s burst damage.', '132208', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('104', '0', 'Sap', 'Used by a rogue to neutralize a dangerous enemy (or just to annoy you).', '132310', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('105', '0', 'Kick', 'Used by a rogue to interrupt a spell (super frustrating).', '132219', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('106', '0', 'Chain Heal', 'Cast by a shaman to heal a group of allies.', '136042', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('107', '0', 'Purify Spirit', 'Cast by a shaman to remove a harmful magic effect.', '236288', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('108', '0', 'Growl', 'Forces the target to attack the druid.', '132270', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('114', '0', 'Dark Command', 'Forces the target to attack the death knight.', '136088', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('115', '0', 'Bone Shield', 'Protects the death knight with a shield of bone.', '458717', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('116', '0', 'Death and Decay', 'Cast by a death knight to damage many enemies.', '136144', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('117', '0', 'Mind Freeze', 'Freezes the opponent\'s mind, interrupting a spell.', '237527', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('118', '0', 'Empower Rune Weapon', 'Significantly increases the death knight\'s burst damage.', '135372', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('119', '0', 'Anti-Magic Shell', 'Protects the death knight from harmful magic effects.', '136120', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('120', '0', 'Cleave', 'Used by a warrior to damage multiple enemies.  Also used to describe every arena team.', '132338', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('121', '0', 'Pummel', 'Used by a warrior to interrupt a spell.', '132938', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('122', '0', 'Recklessness', 'Used by a warrior to greatly increase burst damage.', '603532', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('123', '0', 'Reckoning', 'Forces the target to attack the paladin.', '135984', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('124', '0', 'Divine Shield', 'Cast by a paladin to prevent damage.  Also used to escape in a cowardly manner (shameful).', '524354', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('125', '0', 'Cleanse', 'Cast by a paladin to remove a harmful magic effect.', '135949', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('126', '0', 'Rebuke', 'Cast by a paladin to interrupt a spell.', '523893', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('127', '0', 'Repentance', 'Cast by a paladin to neutralize a dangerous enemy.', '135942', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('128', '0', 'Holy Radiance', 'Cast by a paladin to heal a group of allies.', '457654', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('129', '2', 'Divine Plea', 'Cast by a paladin to quickly regenerate mana.', '237537', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('130', '0', 'Divine Storm', 'Cast by a paladin to deal damage to many enemies.', '236250', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('131', '0', 'Avenging Wrath', 'Cast by a paladin to greatly increase burst damage.', '135875', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('132', '0', 'Barkskin', 'The druid takes on a hardened skin, significantly reducing damage taken.', '136097', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('133', '2', 'Innervate', 'Cast by a druid to rapidly regenerate the target\'s mana.', '136048', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('134', '0', 'Entangling Roots', 'Cast by a druid to neutralize a dangerous enemy.', '136100', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('135', '0', 'Wild Growth', 'Cast by a druid to heal a group of allies.', '236153', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('136', '0', 'Nature\'s Cure', 'Cast by a druid to remove a harmful magic effect.', '236288', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('137', '0', 'Hurricane', 'Cast by the druid to damage many enemies.', '136018', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('138', '0', 'Berserk', 'Significantly increases the druid\'s burst damage.', '236149', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('139', '0', 'Celestial Alignment', 'Significantly increases the druid\'s burst damage.', '136060', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('140', '0', 'Provoke', 'Forces the enemy to attack the monk.', '620830', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('141', '0', 'Guard', 'Used by a monk to significantly reduce damage.', '611417', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('142', '0', 'Chi Wave', 'Cast by a monk to heal a group of allies.', '606541', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('143', '0', 'Roll', 'Used by a monk to quickly move out of harm\'s way.', '574574', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('144', '0', 'Paralysis', 'Used by a monk to neutralize a dangerous enemy.', '629534', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('145', '0', 'Detox', 'Cast by a monk to remove a harmful magic effect.', '460692', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('146', '2', 'Mana Tea', 'Cast by a monk to quickly regenerate the target\'s mana.', '608949', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('147', '0', 'Spear Hand Strike', 'Used by a monk to interrupt a spell.', '608940', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('148', '0', 'Dispel Magic', 'Cast by a priest to remove a harmful magic effect.', '135894', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('149', '2', 'Shadowfiend', 'Cast by a priest to greatly increase burst damage and mana regeneration.', '136199', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('150', '0', 'Mind Sear', 'Cast by a priest to damage many enemies.', '237565', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('151', '0', 'Dominate Mind', 'Cast by a priest to neutralize a dangerous enemy.  Wildly popular in AB and EotS.', '136206', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('152', '0', 'Power Infusion', 'Cast by a priest to greatly increase burst damage.', '135939', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('153', '2', 'Water Shield', 'Cast by a shaman to greatly increase mana regeneration.', '132315', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('154', '0', 'Chain Lightning', 'Cast by a shaman to damage many enemies.', '136015', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('155', '0', 'Wind Shear', 'Cast by a shaman to interrupt a spell.', '463565', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('156', '0', 'Ghost Wolf', 'Cast by a shaman to quickly move out of harm\'s way.', '136095', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('157', '0', 'Hex', 'Cast by a shaman to neutralize a dangerous enemy.  Also makes them feel dumb.', '237579', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('158', '0', 'Ascendance', 'Cast by a shaman to greatly increase burst damage.  Seems unfair at times.', '135791', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('159', '0', 'Evasion', 'Used by a rogue to avoid significant damage.', '136205', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('160', '0', 'Sprint', 'Used by a rogue to escape harm\'s way.  Or, to move quickly between the mailbox and auction house.', '132307', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('161', '0', 'Fan of Knives', 'Used by a rogue to damage many enemies.', '236273', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('162', '0', 'Marked for Death', 'Used by a rogue to greatly increase burst damage.', '236364', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('163', '0', 'Feign Death', 'Used by a hunter to force the enemy to attack a different target.', '132293', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('164', '0', 'Deterrence', 'Used by a hunter to prevent damage.', '132369', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('165', '0', 'Disengage', 'Used by a hunter to quickly move out of harm\'s way.', '132294', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('166', '0', 'Counter Shot', 'Used by a hunter to interrupt a spell.', '249170', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('167', '0', 'Freezing Trap', 'Used by a hunter to neutralize a dangerous enemy.  Watch your step!', '135834', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('168', '0', 'Ice Block', 'Cast by a mage to prevent massive damage.  For a while.', '135841', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('169', '2', 'Conjure Food', 'Your body needs energy to perform to its fullest!', '609812', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('170', '0', 'Blink', 'Cast by a mage to quickly teleport out of harm\'s way.  Hax.', '135736', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('171', '0', 'Counterspell', 'Cast by a mage to interrupt a spell.', '135856', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('172', '0', 'Polymorph', 'Cast by a mage to neutralize a dangerous enemy.', '136071', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('173', '0', 'Time Warp', 'Cast by a mage to greatly increase burst damage.', '458224', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('174', '0', 'Unending Resolve', 'Cast by a warlock to prevent significant damage.', '136150', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('175', '0', 'Drain Life', 'Cast by a warlock to regain life.  Duh.', '136169', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('176', '0', 'Singe Magic', 'Cast by an annoying imp to remove a harmful magic effect.', '841219', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('177', '0', 'Metamorphosis', 'Cast by a warlock pretending to be a demon hunter. Greatly increases burst damage.', '236293', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('178', '0', 'Rain of Fire', 'Cast by a warlock to damage many enemies.', '135804', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('179', '0', 'Spell Lock', 'Cast by a warlock to interrupt a spell.', '136174', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('180', '0', 'Fear', 'Used to neutralize a dangerous opponent.  Sometimes causes them to violate basic laws of physics.  Odd.', '136183', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('181', '0', 'Summon Infernal', 'Cast by a warlock to feel powerful and deal significant damage.  Makes nearby innocents very nervous.', '136219', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('182', '0', 'Dash', 'Used by a druid to quickly dash out of harm\'s way, as if they didn\'t already move fast enough.', '132120', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('183', '0', 'Energizing Brew', 'Used by a monk to significantly increase burst damage.  Also tastes fantastic.', '608938', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('185', '3', 'Wanderlust', 'Increases success chance on missions with travel time longer than 6 hours.', '879931', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('186', '3', 'Sprinter', 'Increases success chance on missions with travel time shorter than 6 hours.', '132296', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('199', '2', 'Extra Training', 'Grants a bonus to XP upon mission completion.', '236221', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('200', '2', 'Extra Training', 'Grants a bonus to XP upon mission completion.', '236221', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('201', '51', 'Combat Experience', 'Grants a bonus to mission success chance.', '458725', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('202', '2', 'Combat Experience', 'Grants a bonus to mission success chance.', '236221', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('203', '2', 'Teleport', 'Reduces travel time of missions.', '236221', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('204', '2', 'Teleport', 'Reduces travel time of missions.', '236221', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('205', '2', 'Salvage Arms', 'Awards a bonus to garrison resources upon mission completion.', '236221', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('206', '2', 'Salvage Arms', 'Awards a bonus to garrison resources upon mission completion.', '236221', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('221', '1', 'Epic Mount', 'Reduces mission time by 50%.', '631718', '0', '8');
INSERT INTO `garrison_abilities` VALUES ('222', '3', 'Powered By Death', 'Reduces mission duration by 25%.', '575534', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('227', '19', 'Angler', 'With Nat\'s knowledge your fishing skill in Draenor allows you to catch lunkers without bait.', '236574', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('228', '19', 'Evergreen', 'When assigned to the Lumber Mill, can more efficiently and humanely processes timber.', '236160', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('231', '19', 'Bodyguard', 'Can accompany you as a guardian in Draenor when assigned to a level 2 or 3 barracks.', '1037260', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('232', '3', 'Dancer', 'Your lithe moves help you skip out of danger zones just a bit more quickly.', '132171', '0', '1');
INSERT INTO `garrison_abilities` VALUES ('236', '3', 'Hearthstone Pro', 'A master tactician, and a must-watch.  Party members gain experience 35% more quickly.', '1061040', '0', '0');
INSERT INTO `garrison_abilities` VALUES ('244', '19', 'Orcslayer', 'Increases success chance when fighting orcs.', '897144', '0', '1');

-- ----------------------------
-- Table structure for garrison_ability_category
-- ----------------------------
DROP TABLE IF EXISTS `garrison_ability_category`;
CREATE TABLE `garrison_ability_category` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `Category` text NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Export of GarrAbilityCategory.db2';

-- ----------------------------
-- Records of garrison_ability_category
-- ----------------------------
INSERT INTO `garrison_ability_category` VALUES ('1', 'Slayer');
INSERT INTO `garrison_ability_category` VALUES ('4', 'Racial Preference');
INSERT INTO `garrison_ability_category` VALUES ('5', 'Profession');
INSERT INTO `garrison_ability_category` VALUES ('6', 'Environment Preference');
INSERT INTO `garrison_ability_category` VALUES ('7', 'Increased Rewards');
INSERT INTO `garrison_ability_category` VALUES ('8', 'Mission Duration');
INSERT INTO `garrison_ability_category` VALUES ('9', 'Other');

-- ----------------------------
-- Table structure for garrison_encounter
-- ----------------------------
DROP TABLE IF EXISTS `garrison_encounter`;
CREATE TABLE `garrison_encounter` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `CreatureID` int(11) NOT NULL DEFAULT '0',
  `Name` text NOT NULL,
  `Data0` float NOT NULL DEFAULT '0',
  `Data1` float NOT NULL DEFAULT '0',
  `Data2` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Export of GarrEncounter.db2';

-- ----------------------------
-- Records of garrison_encounter
-- ----------------------------
INSERT INTO `garrison_encounter` VALUES ('1', '80693', 'Frostfire Gronnling', '0.75', '0.55', '1067373');
INSERT INTO `garrison_encounter` VALUES ('2', '87430', 'Thunderlord Scout', '1.4', '0.5', '1067284');
INSERT INTO `garrison_encounter` VALUES ('4', '78009', 'Orc Encounter 1', '1.5', '0.5', '1067281');
INSERT INTO `garrison_encounter` VALUES ('5', '79359', 'Orc Encounter 2', '1.5', '0.5', '1067297');
INSERT INTO `garrison_encounter` VALUES ('7', '79378', 'Ogre Encounter 1', '1.5', '0.55', '1067321');
INSERT INTO `garrison_encounter` VALUES ('29', '0', 'Chris\' Mission Set Test Encounter', '1', '0.4', '0');
INSERT INTO `garrison_encounter` VALUES ('30', '80704', 'Frostfire Gronnling', '0.75', '0.55', '1067391');
INSERT INTO `garrison_encounter` VALUES ('31', '80693', 'Frostfire Gronnling', '0.75', '0.55', '1067373');
INSERT INTO `garrison_encounter` VALUES ('32', '80705', 'Icemaw Rylak', '0.55', '0.45', '1067265');
INSERT INTO `garrison_encounter` VALUES ('33', '80192', 'Thunderlord Scout', '0.6', '0.9', '1067335');
INSERT INTO `garrison_encounter` VALUES ('34', '80172', 'Thunderlord Scout', '1.2', '0.5', '1067334');
INSERT INTO `garrison_encounter` VALUES ('35', '79380', 'Ogre Encounter 2', '0.9', '0.55', '1067230');
INSERT INTO `garrison_encounter` VALUES ('36', '79380', 'Undead Encounter 1', '1', '0.5', '1067230');
INSERT INTO `garrison_encounter` VALUES ('37', '79383', 'Aberration Encounter 1', '0.8', '0.5', '1067191');
INSERT INTO `garrison_encounter` VALUES ('38', '79387', 'Arakkoa Encounter 1', '0.5', '0.5', '1067348');
INSERT INTO `garrison_encounter` VALUES ('39', '81338', 'Arakkoa Encounter 2', '0.9', '0.5', '1067462');
INSERT INTO `garrison_encounter` VALUES ('40', '79384', 'Beast Encounter 1', '1.1', '0.5', '1067296');
INSERT INTO `garrison_encounter` VALUES ('41', '79386', 'Beast Encounter 2', '1.4', '0.5', '1067246');
INSERT INTO `garrison_encounter` VALUES ('42', '79381', 'Demon Encounter 1', '1.1', '0.6', '1067240');
INSERT INTO `garrison_encounter` VALUES ('43', '81339', 'Demon Encounter 2', '0.6', '0.6', '1067193');
INSERT INTO `garrison_encounter` VALUES ('44', '79382', 'Breaker Encounter 1', '1', '0.5', '1067290');
INSERT INTO `garrison_encounter` VALUES ('45', '79385', 'Breaker Encounter 2', '1.5', '0.6', '1067311');
INSERT INTO `garrison_encounter` VALUES ('46', '81340', 'Fury Encounter 1', '1.2', '0.5', '1067223');
INSERT INTO `garrison_encounter` VALUES ('47', '81341', 'Fury Encounter 2', '1', '0.5', '1067207');
INSERT INTO `garrison_encounter` VALUES ('48', '81342', 'Primal Encounter 1', '1', '0.5', '1067224');
INSERT INTO `garrison_encounter` VALUES ('49', '81343', 'Primal Encounter 2', '1', '0.5', '1067208');
INSERT INTO `garrison_encounter` VALUES ('50', '19787', 'Wretched Slugs', '1', '0.4', '0');
INSERT INTO `garrison_encounter` VALUES ('51', '81538', 'Skulking Ravager', '0.9', '0.45', '1067344');
INSERT INTO `garrison_encounter` VALUES ('52', '81562', 'Thunderlord Crag-Leaper', '1.55', '0.5', '1067236');
INSERT INTO `garrison_encounter` VALUES ('53', '81565', 'Thunderlord Windreader', '1.55', '0.5', '1067286');
INSERT INTO `garrison_encounter` VALUES ('54', '81568', 'Cursed Wolf', '1.5', '0.5', '1067253');
INSERT INTO `garrison_encounter` VALUES ('55', '81580', 'Grimfrost Drudge', '1.2', '0.55', '1067268');
INSERT INTO `garrison_encounter` VALUES ('56', '81583', 'Frostbeak Scavenger', '1.3', '0.9', '1067178');
INSERT INTO `garrison_encounter` VALUES ('57', '81587', 'Bladespire Raider', '1.2', '0.5', '1067336');
INSERT INTO `garrison_encounter` VALUES ('58', '82405', 'Pale Devourer', '1', '0.45', '1067305');
INSERT INTO `garrison_encounter` VALUES ('59', '82410', 'Corrupted Toad', '1', '0.45', '1067273');
INSERT INTO `garrison_encounter` VALUES ('60', '82409', 'Lunarfall Raven', '0.8', '0.5', '1067466');
INSERT INTO `garrison_encounter` VALUES ('61', '82413', 'Shadowmoon Necrolyte', '1.55', '0.5', '1067287');
INSERT INTO `garrison_encounter` VALUES ('62', '82414', 'Shadowmoon Darkcaster', '1.55', '0.5', '1067282');
INSERT INTO `garrison_encounter` VALUES ('63', '83059', 'Thundering Plainswalker', '0.75', '0.55', '1067312');
INSERT INTO `garrison_encounter` VALUES ('64', '83067', 'Gorian Arcanist', '1.2', '0.5', '1067413');
INSERT INTO `garrison_encounter` VALUES ('65', '83071', 'Gorian Cohort', '1.2', '0.5', '1067409');
INSERT INTO `garrison_encounter` VALUES ('66', '83128', 'Iron Flameseer', '1.4', '0.5', '1067339');
INSERT INTO `garrison_encounter` VALUES ('67', '83126', 'Iron Flameseer', '1.4', '0.5', '1067270');
INSERT INTO `garrison_encounter` VALUES ('68', '83236', 'Beremus the Overseer', '0.9', '0.53', '1067399');
INSERT INTO `garrison_encounter` VALUES ('69', '83236', 'Tarina the Ancient', '0.9', '0.53', '1067399');
INSERT INTO `garrison_encounter` VALUES ('70', '83357', 'Skyreach Stormcaller', '0.9', '0.55', '1067403');
INSERT INTO `garrison_encounter` VALUES ('71', '83363', 'Adherent Harrier', '0.9', '0.55', '1067405');
INSERT INTO `garrison_encounter` VALUES ('72', '83379', 'Felwrought Annihilator', '0.75', '0.55', '1067240');
INSERT INTO `garrison_encounter` VALUES ('73', '83074', 'Bloodmane Matriarch', '0.8', '0.55', '1067433');
INSERT INTO `garrison_encounter` VALUES ('74', '83347', 'Shade Prowler', '1.2', '0.5', '1067465');
INSERT INTO `garrison_encounter` VALUES ('75', '83170', 'Felflame Infernal', '0.75', '0.55', '1067195');
INSERT INTO `garrison_encounter` VALUES ('76', '83382', 'Flaming Pillager', '0.9', '0.55', '1067237');
INSERT INTO `garrison_encounter` VALUES ('77', '83256', 'Essence of Darkness', '1.4', '0.55', '1067191');
INSERT INTO `garrison_encounter` VALUES ('78', '83251', 'Essence of the Void', '1.4', '0.55', '1067184');
INSERT INTO `garrison_encounter` VALUES ('79', '83098', 'Gor', '0.75', '0.55', '1067407');
INSERT INTO `garrison_encounter` VALUES ('80', '83193', 'Ravenous Ironchewer', '0.75', '0.55', '1067290');
INSERT INTO `garrison_encounter` VALUES ('81', '83043', 'Bladespire Brute', '1.2', '0.5', '1067419');
INSERT INTO `garrison_encounter` VALUES ('82', '83042', 'Bladespire Mauler', '1.2', '0.5', '1067420');
INSERT INTO `garrison_encounter` VALUES ('83', '83045', 'Bladespire Geomancer', '1.2', '0.5', '1067422');
INSERT INTO `garrison_encounter` VALUES ('84', '83120', 'Thunderlord Windreader', '1.5', '0.5', '1067286');
INSERT INTO `garrison_encounter` VALUES ('85', '83225', 'Yggdrel', '0.55', '0.55', '1067302');
INSERT INTO `garrison_encounter` VALUES ('86', '83118', 'Thunderlord Talon', '1.5', '0.5', '1067285');
INSERT INTO `garrison_encounter` VALUES ('87', '83122', 'Thunderlord Beast-Rider', '1.5', '0.5', '1067284');
INSERT INTO `garrison_encounter` VALUES ('88', '83226', 'Moonbark Ancient', '0.55', '0.55', '1067303');
INSERT INTO `garrison_encounter` VALUES ('89', '83135', 'Nagrand Raider', '1.5', '0.5', '1067388');
INSERT INTO `garrison_encounter` VALUES ('90', '83140', 'Warsong Earthcaller', '1.5', '0.5', '1067358');
INSERT INTO `garrison_encounter` VALUES ('91', '83084', 'Sabermaw Mystic', '0.8', '0.55', '1067432');
INSERT INTO `garrison_encounter` VALUES ('92', '83074', 'Sabermaw Scarfang', '0.8', '0.55', '1067433');
INSERT INTO `garrison_encounter` VALUES ('93', '83081', 'Sabermaw Mauler', '0.8', '0.55', '1067431');
INSERT INTO `garrison_encounter` VALUES ('94', '83353', 'Adherent Bladewing', '0.9', '0.55', '1067381');
INSERT INTO `garrison_encounter` VALUES ('95', '83354', 'Adherent Bladewing', '0.9', '0.55', '1067401');
INSERT INTO `garrison_encounter` VALUES ('96', '83356', 'Adherent Sun-Priest', '0.9', '0.55', '1067404');
INSERT INTO `garrison_encounter` VALUES ('97', '83241', 'Sargerai Vigilant', '1.3', '0.5', '1067367');
INSERT INTO `garrison_encounter` VALUES ('98', '83242', 'Sargerai Channeler', '1.3', '0.5', '1067368');
INSERT INTO `garrison_encounter` VALUES ('99', '83243', 'Sargerai Soul-Twister', '1.3', '0.5', '1067366');
INSERT INTO `garrison_encounter` VALUES ('100', '83045', 'Stonemaul Slaver', '1.2', '0.5', '1067422');
INSERT INTO `garrison_encounter` VALUES ('101', '83062', 'Slavemaster Ok\'mok', '1.2', '0.5', '1067418');
INSERT INTO `garrison_encounter` VALUES ('102', '83070', 'Arcanist Earthsmasher', '1.2', '0.5', '1067414');
INSERT INTO `garrison_encounter` VALUES ('103', '83072', 'Gorian Warmaster', '1.2', '0.5', '1067410');
INSERT INTO `garrison_encounter` VALUES ('104', '83072', 'Gorian Warmaster', '1.2', '0.5', '1067410');
INSERT INTO `garrison_encounter` VALUES ('105', '83233', 'Blademoon Genesaur', '0.9', '0.53', '1067400');
INSERT INTO `garrison_encounter` VALUES ('106', '83217', 'Botani Grower', '0.9', '0.55', '1067380');
INSERT INTO `garrison_encounter` VALUES ('107', '83217', 'Botani Warder', '0.9', '0.55', '1067380');
INSERT INTO `garrison_encounter` VALUES ('108', '83103', 'Shadeback Ravager', '0.9', '0.45', '1067345');
INSERT INTO `garrison_encounter` VALUES ('109', '83106', 'Shadeback Ravager', '0.9', '0.45', '1067346');
INSERT INTO `garrison_encounter` VALUES ('110', '83107', 'Shadeback Ravager', '0.9', '0.45', '1067343');
INSERT INTO `garrison_encounter` VALUES ('111', '83170', 'Blazing Infernal', '0.75', '0.55', '1067195');
INSERT INTO `garrison_encounter` VALUES ('112', '83214', 'Stonescale', '1.45', '0.4', '1067187');
INSERT INTO `garrison_encounter` VALUES ('113', '83213', 'Bloodhide Watcher', '1.45', '0.4', '1067186');
INSERT INTO `garrison_encounter` VALUES ('114', '84858', 'Snow Lurker', '1.45', '0.4', '1087727');
INSERT INTO `garrison_encounter` VALUES ('115', '83147', 'Magma Elemental', '1.2', '0.4', '1067228');
INSERT INTO `garrison_encounter` VALUES ('116', '83152', 'Ice Fury', '1.2', '0.4', '1067233');
INSERT INTO `garrison_encounter` VALUES ('118', '83301', 'Hyacinth Mandragora', '1', '0.4', '1067341');
INSERT INTO `garrison_encounter` VALUES ('119', '83129', 'Shadowmoon Voidclaw', '1.5', '0.5', '1067347');
INSERT INTO `garrison_encounter` VALUES ('120', '83292', 'Podling Squirt', '1.9', '0.45', '1067330');
INSERT INTO `garrison_encounter` VALUES ('121', '83151', 'Frost Fury', '1.2', '0.4', '1067221');
INSERT INTO `garrison_encounter` VALUES ('122', '83192', 'Ravenous Ironchewer', '1.4', '0.45', '1067288');
INSERT INTO `garrison_encounter` VALUES ('123', '83041', 'Sootstained Enforcer', '1.2', '0.5', '1067421');
INSERT INTO `garrison_encounter` VALUES ('124', '83225', 'Moonbark Ancient', '0.55', '0.55', '1067302');
INSERT INTO `garrison_encounter` VALUES ('125', '83133', 'Shadowmoon Voidaxe', '1.5', '0.5', '1067294');
INSERT INTO `garrison_encounter` VALUES ('126', '83386', 'Essence of Darkness', '1.4', '0.55', '1067428');
INSERT INTO `garrison_encounter` VALUES ('127', '83117', 'Thunderlord Talon', '1.5', '0.5', '1067306');
INSERT INTO `garrison_encounter` VALUES ('128', '83179', 'Ata\'gar Warrior', '1.55', '0.5', '1067318');
INSERT INTO `garrison_encounter` VALUES ('129', '83038', 'Icemaw Matriarch', '0.55', '0.45', '1067266');
INSERT INTO `garrison_encounter` VALUES ('130', '83228', 'Silent Watcher', '0.55', '0.45', '1067301');
INSERT INTO `garrison_encounter` VALUES ('131', '83127', 'Iron Grunt', '1.5', '0.5', '1067457');
INSERT INTO `garrison_encounter` VALUES ('132', '83234', 'Ula', '0.9', '0.53', '1067398');
INSERT INTO `garrison_encounter` VALUES ('133', '83195', 'Boneyard Gorger', '1.4', '0.45', '1067289');
INSERT INTO `garrison_encounter` VALUES ('134', '83056', 'Gorge Bonepicker', '0.75', '0.55', '1067391');
INSERT INTO `garrison_encounter` VALUES ('135', '83339', 'Iron Pulverizer', '1.05', '0.45', '1067267');
INSERT INTO `garrison_encounter` VALUES ('136', '83173', 'Tol\'ressa', '1.1', '0.5', '1067198');
INSERT INTO `garrison_encounter` VALUES ('137', '83194', 'Goren Stonechewer', '1.5', '0.45', '1067291');
INSERT INTO `garrison_encounter` VALUES ('138', '83351', 'Adherent Sun-Caller', '0.9', '0.55', '1067402');
INSERT INTO `garrison_encounter` VALUES ('139', '83178', 'Shadowborne Reverend', '1.55', '0.5', '1067320');
INSERT INTO `garrison_encounter` VALUES ('140', '83072', 'Gordunni Basher', '1.2', '0.5', '1067410');
INSERT INTO `garrison_encounter` VALUES ('141', '83063', 'Skagg', '1.2', '0.55', '1067417');
INSERT INTO `garrison_encounter` VALUES ('142', '83141', 'Shattered Hand Sadist', '1.4', '0.5', '1067245');
INSERT INTO `garrison_encounter` VALUES ('143', '83114', 'Ravaging Knuckledragger', '1', '0.45', '1067307');
INSERT INTO `garrison_encounter` VALUES ('144', '83103', 'Shadeback Ravager', '0.9', '0.45', '1067345');
INSERT INTO `garrison_encounter` VALUES ('145', '83060', 'Howling Rylak', '0.55', '0.45', '1067264');
INSERT INTO `garrison_encounter` VALUES ('146', '83374', 'Sangrikass', '1.4', '0.55', '1067218');
INSERT INTO `garrison_encounter` VALUES ('147', '83143', 'Shattered Hand Blood-Singer', '1.4', '0.5', '1067244');
INSERT INTO `garrison_encounter` VALUES ('148', '83369', 'Tumultous Water Spirit', '1.2', '0.4', '1067324');
INSERT INTO `garrison_encounter` VALUES ('149', '83065', 'Gorian Arcanist', '1.2', '0.5', '1067412');
INSERT INTO `garrison_encounter` VALUES ('150', '83140', 'Warsong Earthcaller', '1.5', '0.5', '1067358');
INSERT INTO `garrison_encounter` VALUES ('151', '83073', 'Gorian Gladiator', '1.2', '0.5', '1067411');
INSERT INTO `garrison_encounter` VALUES ('152', '83460', 'Burning Blademaster', '1.4', '0.5', '1067269');
INSERT INTO `garrison_encounter` VALUES ('153', '83059', 'Plainswalker Gronn', '0.75', '0.55', '1067312');
INSERT INTO `garrison_encounter` VALUES ('154', '83257', 'Void-Torn Spirit', '1.4', '0.5', '1067476');
INSERT INTO `garrison_encounter` VALUES ('155', '83122', 'Thunderlord Grappler', '1.5', '0.5', '1067284');
INSERT INTO `garrison_encounter` VALUES ('156', '83291', 'Podling Nibbler', '1.9', '0.45', '1067332');
INSERT INTO `garrison_encounter` VALUES ('157', '83123', 'Blackrock Grunt', '1.5', '0.5', '1067356');
INSERT INTO `garrison_encounter` VALUES ('158', '83062', 'Stonemaul Guard', '1.2', '0.5', '1067418');
INSERT INTO `garrison_encounter` VALUES ('159', '83047', 'Stonemaul Intimidator', '0.9', '0.5', '1067357');
INSERT INTO `garrison_encounter` VALUES ('160', '83356', 'Adherent Squallbringer', '0.9', '0.55', '1067404');
INSERT INTO `garrison_encounter` VALUES ('161', '83336', 'Iron Shredder', '1.05', '0.45', '1067371');
INSERT INTO `garrison_encounter` VALUES ('162', '83354', 'Adherent Bladewing', '0.9', '0.55', '1067401');
INSERT INTO `garrison_encounter` VALUES ('163', '83081', 'Bloodmane Gutripper', '0.8', '0.55', '1067431');
INSERT INTO `garrison_encounter` VALUES ('164', '83135', 'Warsong Outriders', '1.5', '0.5', '1067388');
INSERT INTO `garrison_encounter` VALUES ('165', '83135', 'Warsong Raider', '1.5', '0.5', '1067388');
INSERT INTO `garrison_encounter` VALUES ('166', '83246', 'Riverbeast', '0.95', '0.45', '1067250');
INSERT INTO `garrison_encounter` VALUES ('167', '83431', 'Ashmane Clefthoof', '1.1', '0.4', '1067247');
INSERT INTO `garrison_encounter` VALUES ('168', '83217', 'Blademoon Botani', '0.9', '0.55', '1067380');
INSERT INTO `garrison_encounter` VALUES ('169', '83101', 'Icespine Ravager', '0.9', '0.45', '1067344');
INSERT INTO `garrison_encounter` VALUES ('170', '83238', 'Mandragora Lifedrinker', '1', '0.4', '1067406');
INSERT INTO `garrison_encounter` VALUES ('171', '83433', 'Wild Clefthoof', '1.1', '0.4', '1067249');
INSERT INTO `garrison_encounter` VALUES ('172', '83343', 'Iridium Geode', '0.75', '0.6', '1067227');
INSERT INTO `garrison_encounter` VALUES ('173', '83262', 'Wild Bloom', '0.75', '0.5', '1067192');
INSERT INTO `garrison_encounter` VALUES ('174', '83227', 'Shadowbark', '0.55', '0.55', '1067300');
INSERT INTO `garrison_encounter` VALUES ('175', '83372', 'Dread Raven', '0.8', '0.5', '1067279');
INSERT INTO `garrison_encounter` VALUES ('176', '83432', 'Leatherhide Bull', '1.1', '0.4', '1067248');
INSERT INTO `garrison_encounter` VALUES ('177', '83155', 'Earth Fury', '0.75', '0.5', '1067395');
INSERT INTO `garrison_encounter` VALUES ('178', '83058', 'Gronn Treeslammer', '0.75', '0.55', '1067311');
INSERT INTO `garrison_encounter` VALUES ('179', '83172', 'Wrathguard Destroyer', '1', '0.55', '1067199');
INSERT INTO `garrison_encounter` VALUES ('180', '83167', 'Imp', '1.9', '0.5', '1067188');
INSERT INTO `garrison_encounter` VALUES ('181', '83145', 'Durkath Steelmaw', '1.5', '0.5', '1067243');
INSERT INTO `garrison_encounter` VALUES ('182', '86199', 'Dargorr the Landslide', '0.75', '0.5', '1067395');
INSERT INTO `garrison_encounter` VALUES ('183', '86200', 'Enslaved Earth Spirit', '0.75', '0.5', '1067397');
INSERT INTO `garrison_encounter` VALUES ('184', '83059', 'Plainswalker Gronn', '0.75', '0.55', '1067312');
INSERT INTO `garrison_encounter` VALUES ('185', '83129', 'Shadowmoon Prophet', '1.5', '0.5', '1067347');
INSERT INTO `garrison_encounter` VALUES ('186', '83037', 'Grondo', '0.75', '0.5', '1067313');
INSERT INTO `garrison_encounter` VALUES ('187', '83219', 'Botani Grovetender', '0.9', '0.55', '1067393');
INSERT INTO `garrison_encounter` VALUES ('188', '83146', 'Shattered Hand Grunt', '1.5', '0.5', '1067242');
INSERT INTO `garrison_encounter` VALUES ('189', '83142', 'Shattered Hand Flayer', '1.5', '0.5', '1088313');
INSERT INTO `garrison_encounter` VALUES ('190', '83144', 'Shattered Hand Cleaver', '1.5', '0.5', '1088312');
INSERT INTO `garrison_encounter` VALUES ('191', '83353', 'Adherent Wing-Guard', '0.9', '0.55', '1067381');
INSERT INTO `garrison_encounter` VALUES ('192', '84761', 'Haunted Hound', '1.3', '0.5', '1067212');
INSERT INTO `garrison_encounter` VALUES ('193', '84762', 'Ghastly Guard', '1.7', '0.5', '1067274');
INSERT INTO `garrison_encounter` VALUES ('194', '84763', 'Ghastly Nightwatch', '1.7', '0.5', '1067274');
INSERT INTO `garrison_encounter` VALUES ('196', '83211', 'Rock Flayer', '0.75', '0.55', '1067203');
INSERT INTO `garrison_encounter` VALUES ('197', '84844', 'Burning Flameseer', '1.55', '0.5', '1087721');
INSERT INTO `garrison_encounter` VALUES ('198', '83460', 'Burning Blademaster', '1.4', '0.5', '1067269');
INSERT INTO `garrison_encounter` VALUES ('199', '84847', 'General Kull\'krosh', '1.4', '0.5', '1067327');
INSERT INTO `garrison_encounter` VALUES ('200', '84849', 'Iron Arbalester', '1.55', '0.5', '1087720');
INSERT INTO `garrison_encounter` VALUES ('201', '84850', 'Iron Guard', '1.55', '0.5', '1087726');
INSERT INTO `garrison_encounter` VALUES ('202', '83265', 'Moonglow Sporebat', '0.85', '0.8', '1067427');
INSERT INTO `garrison_encounter` VALUES ('203', '83113', 'Pale Fishmonger', '1', '0.45', '1067305');
INSERT INTO `garrison_encounter` VALUES ('204', '83042', 'Grimfrost Wormpriest', '1.2', '0.5', '1067420');
INSERT INTO `garrison_encounter` VALUES ('205', '82410', 'Hypnocroak', '1', '0.45', '1067273');
INSERT INTO `garrison_encounter` VALUES ('206', '83166', 'Frostwing Screecher', '0.6', '0.9', '1067392');
INSERT INTO `garrison_encounter` VALUES ('207', '83269', 'Underbrush Stalker', '1.05', '0.55', '1067201');
INSERT INTO `garrison_encounter` VALUES ('208', '85855', 'Dessicus of the Dead Pools', '0.9', '0.45', '1067365');
INSERT INTO `garrison_encounter` VALUES ('209', '83080', 'Steamscar Warrior', '0.8', '0.55', '1067429');
INSERT INTO `garrison_encounter` VALUES ('210', '83076', 'Steamscar Primalist', '0.8', '0.55', '1067430');
INSERT INTO `garrison_encounter` VALUES ('211', '83338', 'Iron Siege Cannon', '1.05', '0.45', '1067255');
INSERT INTO `garrison_encounter` VALUES ('212', '83296', 'Mad \"King\" Sporeon', '1.9', '0.45', '1067329');
INSERT INTO `garrison_encounter` VALUES ('213', '83307', 'Breezerider Kaliri', '1.35', '0.5', '1067451');
INSERT INTO `garrison_encounter` VALUES ('214', '83189', 'Deathweb Crawler', '1.2', '0.45', '1067252');
INSERT INTO `garrison_encounter` VALUES ('215', '83354', 'Adherent Wind-Lasher', '0.9', '0.55', '1067401');
INSERT INTO `garrison_encounter` VALUES ('216', '83375', 'Apexis Guardian', '0.8', '0.45', '1067374');
INSERT INTO `garrison_encounter` VALUES ('217', '83112', 'Widow Wasp', '0.9', '0.5', '1067355');
INSERT INTO `garrison_encounter` VALUES ('218', '86109', 'Gardul Venomshiv', '1.55', '0.5', '1087722');
INSERT INTO `garrison_encounter` VALUES ('219', '83069', 'Highmaul Archaeologian', '1.2', '0.5', '1067415');
INSERT INTO `garrison_encounter` VALUES ('220', '83064', 'Highmaul Laborer', '1.2', '0.55', '1067416');
INSERT INTO `garrison_encounter` VALUES ('221', '87454', 'Lug\'dol', '1.2', '0.5', '1067443');
INSERT INTO `garrison_encounter` VALUES ('222', '86121', 'Nascent Brawler', '1.2', '0.5', '1067444');
INSERT INTO `garrison_encounter` VALUES ('223', '86197', 'Arcane Anomaly', '1.2', '0.6', '1067222');
INSERT INTO `garrison_encounter` VALUES ('224', '86217', 'Vigilant Kaathar', '1.15', '0.45', '1067440');
INSERT INTO `garrison_encounter` VALUES ('225', '86217', 'Vigilant Kaathar', '1.15', '0.45', '1067440');
INSERT INTO `garrison_encounter` VALUES ('226', '86218', 'Soulbinder Nyami', '1.1', '0.55', '1067261');
INSERT INTO `garrison_encounter` VALUES ('227', '86218', 'Soulbinder Nyami', '1.1', '0.55', '1067261');
INSERT INTO `garrison_encounter` VALUES ('228', '86219', 'Azzakel', '1.1', '0.55', '1087719');
INSERT INTO `garrison_encounter` VALUES ('229', '86219', 'Azzakel', '1.1', '0.55', '1087719');
INSERT INTO `garrison_encounter` VALUES ('230', '86220', 'Teron\'gor', '1.55', '0.5', '1067277');
INSERT INTO `garrison_encounter` VALUES ('231', '86220', 'Teron\'gor', '1.55', '0.5', '1067277');
INSERT INTO `garrison_encounter` VALUES ('232', '86221', 'Magmolatus', '1.15', '0.5', '1067241');
INSERT INTO `garrison_encounter` VALUES ('233', '86221', 'Magmolatus', '1.15', '0.5', '1067241');
INSERT INTO `garrison_encounter` VALUES ('234', '86222', 'Slave Watcher Crushto', '1.2', '0.55', '1067256');
INSERT INTO `garrison_encounter` VALUES ('235', '86222', 'Slave Watcher Crushto', '1.2', '0.55', '1067256');
INSERT INTO `garrison_encounter` VALUES ('236', '86223', 'Roltall', '1.15', '0.5', '1067283');
INSERT INTO `garrison_encounter` VALUES ('237', '86224', 'Gug\'rokk', '1.2', '0.55', '1067257');
INSERT INTO `garrison_encounter` VALUES ('238', '86224', 'Gug\'rokk', '1.2', '0.55', '1067257');
INSERT INTO `garrison_encounter` VALUES ('239', '86225', 'Railmaster Rocketspark', '1.6', '0.5', '1067278');
INSERT INTO `garrison_encounter` VALUES ('240', '86225', 'Railmaster Rocketspark', '1.6', '0.5', '1067278');
INSERT INTO `garrison_encounter` VALUES ('241', '86226', 'Borka the Brute', '0.67', '0.5', '1067455');
INSERT INTO `garrison_encounter` VALUES ('242', '86226', 'Borka the Brute', '0.67', '0.5', '1067455');
INSERT INTO `garrison_encounter` VALUES ('243', '86227', 'Nitrogg Thundertower', '1.2', '0.5', '1067323');
INSERT INTO `garrison_encounter` VALUES ('244', '86227', 'Nitrogg Thundertower', '1.2', '0.5', '1067323');
INSERT INTO `garrison_encounter` VALUES ('245', '86228', 'Skylord Tovra', '1.2', '0.5', '1067337');
INSERT INTO `garrison_encounter` VALUES ('246', '86228', 'Skylord Tovra', '1.2', '0.5', '1067337');
INSERT INTO `garrison_encounter` VALUES ('247', '87451', 'Fleshrender Nok\'gar', '1.3', '0.5', '1067362');
INSERT INTO `garrison_encounter` VALUES ('248', '87451', 'Fleshrender Nok\'gar', '1.3', '0.5', '1067362');
INSERT INTO `garrison_encounter` VALUES ('249', '87450', 'Ahri\'ok Dugru', '1.4', '0.5', '1067354');
INSERT INTO `garrison_encounter` VALUES ('250', '86231', 'Makogg Emberblade', '1.4', '0.5', '1067353');
INSERT INTO `garrison_encounter` VALUES ('251', '86232', 'Oshir', '0.8', '0.55', '1067328');
INSERT INTO `garrison_encounter` VALUES ('252', '86232', 'Oshir', '0.8', '0.55', '1067328');
INSERT INTO `garrison_encounter` VALUES ('253', '86233', 'Skulloc', '0.85', '0.5', '1067434');
INSERT INTO `garrison_encounter` VALUES ('254', '86234', 'Sadana Bloodfury', '1.3', '0.5', '1067298');
INSERT INTO `garrison_encounter` VALUES ('255', '86234', 'Sadana Bloodfury', '1.3', '0.5', '1067298');
INSERT INTO `garrison_encounter` VALUES ('256', '86235', 'Nhallish', '1.2', '0.5', '1067260');
INSERT INTO `garrison_encounter` VALUES ('257', '86235', 'Nhallish', '1.2', '0.5', '1067260');
INSERT INTO `garrison_encounter` VALUES ('258', '86236', 'Bonemaw', '1.2', '0.35', '1067458');
INSERT INTO `garrison_encounter` VALUES ('259', '86236', 'Bonemaw', '1.2', '0.35', '1067458');
INSERT INTO `garrison_encounter` VALUES ('260', '86237', 'Ner\'zhul', '1.1', '0.5', '1067453');
INSERT INTO `garrison_encounter` VALUES ('261', '86237', 'Ner\'zhul', '1.1', '0.5', '1067453');
INSERT INTO `garrison_encounter` VALUES ('262', '86238', 'Ranjit', '0.9', '0.5', '1067351');
INSERT INTO `garrison_encounter` VALUES ('263', '86238', 'Ranjit', '0.9', '0.5', '1067351');
INSERT INTO `garrison_encounter` VALUES ('264', '86239', 'Araknath', '0.8', '0.45', '1067276');
INSERT INTO `garrison_encounter` VALUES ('265', '86239', 'Araknath', '0.8', '0.45', '1067276');
INSERT INTO `garrison_encounter` VALUES ('266', '86240', 'Rukhran', '0.8', '0.5', '1067454');
INSERT INTO `garrison_encounter` VALUES ('267', '86240', 'Rukhran', '0.8', '0.5', '1067454');
INSERT INTO `garrison_encounter` VALUES ('268', '86241', 'High Sage Viryx', '0.9', '0.55', '1067352');
INSERT INTO `garrison_encounter` VALUES ('269', '86241', 'High Sage Viryx', '0.9', '0.55', '1067352');
INSERT INTO `garrison_encounter` VALUES ('270', '86242', 'Witherbark', '0.55', '0.55', '1067372');
INSERT INTO `garrison_encounter` VALUES ('271', '86242', 'Witherbark', '0.55', '0.55', '1067372');
INSERT INTO `garrison_encounter` VALUES ('272', '86243', 'Life Warden Gola', '0.9', '0.55', '1067377');
INSERT INTO `garrison_encounter` VALUES ('273', '86244', 'Earthshaper Telu', '0.9', '0.55', '1067435');
INSERT INTO `garrison_encounter` VALUES ('274', '86245', 'Dulhu', '0.9', '0.5', '1067438');
INSERT INTO `garrison_encounter` VALUES ('275', '86246', 'Archmage Sol', '1.2', '0.5', '1067383');
INSERT INTO `garrison_encounter` VALUES ('276', '86246', 'Archmage Sol', '1.2', '0.5', '1067383');
INSERT INTO `garrison_encounter` VALUES ('277', '86247', 'Xeri\'tac', '1.2', '0.45', '1067442');
INSERT INTO `garrison_encounter` VALUES ('278', '86247', 'Xeri\'tac', '1.2', '0.45', '1067442');
INSERT INTO `garrison_encounter` VALUES ('279', '86248', 'Yalnu', '0.9', '0.53', '1067384');
INSERT INTO `garrison_encounter` VALUES ('280', '86248', 'Yalnu', '0.9', '0.53', '1067384');
INSERT INTO `garrison_encounter` VALUES ('281', '86249', 'Orebender Gor\'ashan', '1.2', '0.5', '1067262');
INSERT INTO `garrison_encounter` VALUES ('282', '86249', 'Orebender Gor\'ashan', '1.2', '0.5', '1067262');
INSERT INTO `garrison_encounter` VALUES ('283', '86250', 'Kyrak', '0.8', '0.5', '1067259');
INSERT INTO `garrison_encounter` VALUES ('284', '86250', 'Kyrak', '0.8', '0.5', '1067259');
INSERT INTO `garrison_encounter` VALUES ('285', '86251', 'Commander Tharbek', '1.2', '0.5', '1067271');
INSERT INTO `garrison_encounter` VALUES ('286', '86251', 'Commander Tharbek', '1.2', '0.5', '1067271');
INSERT INTO `garrison_encounter` VALUES ('287', '86252', 'Ragewing the Untamed', '0.55', '0.45', '1067263');
INSERT INTO `garrison_encounter` VALUES ('288', '86252', 'Ragewing the Untamed', '0.55', '0.45', '1067263');
INSERT INTO `garrison_encounter` VALUES ('289', '86253', 'Warlord Zaela', '1.2', '0.5', '1067304');
INSERT INTO `garrison_encounter` VALUES ('290', '86253', 'Warlord Zaela', '1.2', '0.5', '1067304');
INSERT INTO `garrison_encounter` VALUES ('291', '86362', 'Highmaul Emissary Guard', '1.2', '0.55', '1067316');
INSERT INTO `garrison_encounter` VALUES ('292', '86363', 'Watchful Eye', '1.1', '0.5', '1067461');
INSERT INTO `garrison_encounter` VALUES ('293', '86363', 'Burning Eye', '1.1', '0.5', '1067461');
INSERT INTO `garrison_encounter` VALUES ('294', '86364', 'Sargerai Binder', '1.4', '0.55', '1067369');
INSERT INTO `garrison_encounter` VALUES ('295', '86364', 'Sargerai Binder', '1.4', '0.55', '1067369');
INSERT INTO `garrison_encounter` VALUES ('296', '86365', 'Nightshade Wraith', '1.2', '0.5', '1067460');
INSERT INTO `garrison_encounter` VALUES ('297', '83097', 'Drove the Ruiner', '0.7', '0.55', '1067408');
INSERT INTO `garrison_encounter` VALUES ('298', '83043', 'Bloodmaul Taskmaster', '1.2', '0.55', '1067419');
INSERT INTO `garrison_encounter` VALUES ('299', '83046', 'Bloodmaul Bonecrusher', '0.95', '0.45', '1067342');
INSERT INTO `garrison_encounter` VALUES ('300', '83145', 'Shattered Pitmaster', '1.5', '0.5', '1067243');
INSERT INTO `garrison_encounter` VALUES ('301', '83141', 'Shattered Flayer', '1.5', '0.5', '1067245');
INSERT INTO `garrison_encounter` VALUES ('302', '83188', 'Mother Araneae', '1.2', '0.45', '1067425');
INSERT INTO `garrison_encounter` VALUES ('303', '83112', 'Widow Wasp', '0.9', '0.5', '1067355');
INSERT INTO `garrison_encounter` VALUES ('304', '86444', 'Smokemaster Snarl', '0.8', '0.55', '1067370');
INSERT INTO `garrison_encounter` VALUES ('305', '83178', 'Shadow Militant', '1.55', '0.5', '1067320');
INSERT INTO `garrison_encounter` VALUES ('306', '83179', 'Shadow Pillager', '1.55', '0.5', '1067318');
INSERT INTO `garrison_encounter` VALUES ('307', '83181', 'Gorebound Vessel', '1.55', '0.5', '1067319');
INSERT INTO `garrison_encounter` VALUES ('308', '83169', 'Ravenous Mongrel', '1.8', '0.45', '1067196');
INSERT INTO `garrison_encounter` VALUES ('309', '86508', 'Infected Bruiser', '1.7', '0.45', '1067360');
INSERT INTO `garrison_encounter` VALUES ('310', '86509', 'Infected Bar Patron', '1.8', '0.5', '1067450');
INSERT INTO `garrison_encounter` VALUES ('311', '83431', 'Flinthide', '1.1', '0.4', '1067247');
INSERT INTO `garrison_encounter` VALUES ('312', '86519', 'Gar\'lua', '1.55', '0.5', '1067385');
INSERT INTO `garrison_encounter` VALUES ('313', '83090', 'Warsong Wolf', '1.5', '0.5', '1067253');
INSERT INTO `garrison_encounter` VALUES ('314', '87432', 'Ug\'lok the Frozen', '1.2', '0.55', '1067382');
INSERT INTO `garrison_encounter` VALUES ('315', '87484', 'Brambleking Fili', '1.9', '0.45', '1067375');
INSERT INTO `garrison_encounter` VALUES ('316', '83292', 'Shadowglen Spitter', '1.9', '0.45', '1067330');
INSERT INTO `garrison_encounter` VALUES ('317', '87440', 'Iron Warden', '1.55', '0.5', '1067292');
INSERT INTO `garrison_encounter` VALUES ('318', '87438', 'Iron Reinforcement', '1.4', '0.5', '1067449');
INSERT INTO `garrison_encounter` VALUES ('319', '87439', 'Iron Trampler', '0.95', '0.45', '1067437');
INSERT INTO `garrison_encounter` VALUES ('320', '87439', 'Iron Trampler', '0.95', '0.45', '1067437');
INSERT INTO `garrison_encounter` VALUES ('321', '87434', 'Blademaster Ro\'gor', '1.55', '0.5', '1067464');
INSERT INTO `garrison_encounter` VALUES ('322', '87487', 'Darktide Pilferer', '2', '0.5', '1067309');
INSERT INTO `garrison_encounter` VALUES ('323', '87486', 'Darktide Engineer', '2', '0.5', '1067235');
INSERT INTO `garrison_encounter` VALUES ('324', '87489', 'Darktide Rylakinator', '1.1', '0.5', '1067258');
INSERT INTO `garrison_encounter` VALUES ('325', '87495', 'Soaring Blade', '0.9', '0.55', '1067349');
INSERT INTO `garrison_encounter` VALUES ('326', '87494', 'Initiate of the Dawn', '0.9', '0.55', '1067350');
INSERT INTO `garrison_encounter` VALUES ('327', '86219', 'Azzakel', '1.1', '0.5', '1087719');
INSERT INTO `garrison_encounter` VALUES ('328', '86217', 'Vigilant Kaathar', '1.15', '0.45', '1067440');
INSERT INTO `garrison_encounter` VALUES ('329', '86218', 'Soulbinder Nyami', '1.1', '0.5', '1067261');
INSERT INTO `garrison_encounter` VALUES ('330', '86220', 'Teron\'gor', '1.3', '0.45', '1067277');
INSERT INTO `garrison_encounter` VALUES ('331', '86221', 'Magmolatus', '1.05', '0.5', '1067241');
INSERT INTO `garrison_encounter` VALUES ('332', '86222', 'Slave Watcher Crushto', '1.2', '0.55', '1067256');
INSERT INTO `garrison_encounter` VALUES ('333', '86223', 'Roltall', '1.05', '0.5', '1067283');
INSERT INTO `garrison_encounter` VALUES ('334', '86224', 'Gug\'rokk', '1.2', '0.5', '1067257');
INSERT INTO `garrison_encounter` VALUES ('335', '86225', 'Railmaster Rocketspark', '1.65', '0.45', '1067278');
INSERT INTO `garrison_encounter` VALUES ('336', '86226', 'Borka the Brute', '0.67', '0.5', '1067455');
INSERT INTO `garrison_encounter` VALUES ('337', '86227', 'Nitrogg Thundertower', '1.25', '0.5', '1067323');
INSERT INTO `garrison_encounter` VALUES ('338', '86228', 'Skylord Tovra', '1.25', '0.5', '1067337');
INSERT INTO `garrison_encounter` VALUES ('339', '87451', 'Fleshrender Nok\'gar', '1.2', '0.5', '1067362');
INSERT INTO `garrison_encounter` VALUES ('340', '87452', 'Ahri\'ok Dugru', '1.3', '0.5', '1067354');
INSERT INTO `garrison_encounter` VALUES ('341', '86232', 'Oshir', '0.8', '0.55', '1067328');
INSERT INTO `garrison_encounter` VALUES ('342', '86233', 'Skulloc', '0.75', '0.55', '1067434');
INSERT INTO `garrison_encounter` VALUES ('343', '86233', 'Skulloc', '0.75', '0.55', '1067434');
INSERT INTO `garrison_encounter` VALUES ('344', '86234', 'Sadana Bloodfury', '1.3', '0.5', '1067298');
INSERT INTO `garrison_encounter` VALUES ('345', '86235', 'Nhallish', '1.2', '0.5', '1067260');
INSERT INTO `garrison_encounter` VALUES ('346', '86236', 'Bonemaw', '0.9', '0.6', '1067458');
INSERT INTO `garrison_encounter` VALUES ('347', '86237', 'Ner\'zhul', '1.1', '0.5', '1067453');
INSERT INTO `garrison_encounter` VALUES ('348', '86238', 'Ranjit', '0.9', '0.55', '1067351');
INSERT INTO `garrison_encounter` VALUES ('349', '86239', 'Araknath', '0.8', '0.45', '1067276');
INSERT INTO `garrison_encounter` VALUES ('350', '86240', 'Rukhran', '0.8', '0.5', '1067454');
INSERT INTO `garrison_encounter` VALUES ('351', '86241', 'High Sage Viryx', '0.9', '0.55', '1067352');
INSERT INTO `garrison_encounter` VALUES ('352', '86242', 'Witherbark', '0.55', '0.55', '1067372');
INSERT INTO `garrison_encounter` VALUES ('353', '86245', 'Dulhu', '0.95', '0.5', '1067438');
INSERT INTO `garrison_encounter` VALUES ('354', '86246', 'Archmage Sol', '1.2', '0.5', '1067383');
INSERT INTO `garrison_encounter` VALUES ('355', '86247', 'Xeri\'tac', '1.2', '0.45', '1067442');
INSERT INTO `garrison_encounter` VALUES ('356', '86248', 'Yalnu', '0.9', '0.53', '1067384');
INSERT INTO `garrison_encounter` VALUES ('357', '86249', 'Orebender Gor\'ashan', '1.2', '0.5', '1067262');
INSERT INTO `garrison_encounter` VALUES ('358', '86250', 'Kyrak', '0.8', '0.5', '1067259');
INSERT INTO `garrison_encounter` VALUES ('359', '86251', 'Commander Tharbek', '1.2', '0.5', '1067271');
INSERT INTO `garrison_encounter` VALUES ('360', '86252', 'Ragewing the Untamed', '0.55', '0.45', '1067263');
INSERT INTO `garrison_encounter` VALUES ('361', '86253', 'Warlord Zaela', '1.2', '0.5', '1067304');
INSERT INTO `garrison_encounter` VALUES ('362', '87444', 'Kargath Bladefist', '1.1', '0.5', '1067299');
INSERT INTO `garrison_encounter` VALUES ('363', '87447', 'The Butcher', '1.15', '0.5', '1067275');
INSERT INTO `garrison_encounter` VALUES ('364', '87446', 'Tectus, The Living Mountain', '0.9', '0.45', '1067314');
INSERT INTO `garrison_encounter` VALUES ('365', '87441', 'Brackenspore, Walker of the Deep', '1.15', '0.5', '1067446');
INSERT INTO `garrison_encounter` VALUES ('366', '87449', 'Twin Ogron', '0.95', '0.45', '1067295');
INSERT INTO `garrison_encounter` VALUES ('367', '87445', 'Ko\'ragh', '1.15', '0.5', '1067315');
INSERT INTO `garrison_encounter` VALUES ('368', '87443', 'Imperator Mar\'gok', '1.2', '0.5', '1067293');
INSERT INTO `garrison_encounter` VALUES ('369', '87400', 'Instructor Johnny Castle', '1.5', '0.5', '1067197');
INSERT INTO `garrison_encounter` VALUES ('370', '87408', 'Griftah', '1.4', '0.55', '1067194');
INSERT INTO `garrison_encounter` VALUES ('371', '87409', 'Scythetooth Raptor', '1.75', '0.5', '1067200');
INSERT INTO `garrison_encounter` VALUES ('372', '87456', 'Rampant Gladiator', '1.2', '0.5', '1067445');
INSERT INTO `garrison_encounter` VALUES ('373', '87456', 'Rampant Gladiator', '1.2', '0.5', '1067445');
INSERT INTO `garrison_encounter` VALUES ('374', '87490', 'Brokenfang', '1', '0.4', '1067459');
INSERT INTO `garrison_encounter` VALUES ('375', '87491', 'Bloodmane Shortfang', '1', '0.4', '1067387');
INSERT INTO `garrison_encounter` VALUES ('376', '87497', 'Nefarious Madame', '1.3', '0.5', '1067389');
INSERT INTO `garrison_encounter` VALUES ('377', '87498', 'Damsel of Lust', '1.7', '0.5', '1067386');
INSERT INTO `garrison_encounter` VALUES ('378', '87478', 'Crushmaul', '1', '0.4', '1067322');
INSERT INTO `garrison_encounter` VALUES ('379', '87480', 'Raketalon', '1', '0.4', '1067325');
INSERT INTO `garrison_encounter` VALUES ('380', '87479', 'Hol\'yelaa', '1', '0.4', '1067441');
INSERT INTO `garrison_encounter` VALUES ('381', '87482', 'The Burninator', '1', '0.4', '1067326');
INSERT INTO `garrison_encounter` VALUES ('382', '87481', 'Roakk the Zealot', '0.9', '0.55', '1067378');
INSERT INTO `garrison_encounter` VALUES ('383', '87462', 'Gnome Mage', '1.7', '0.45', '1067215');
INSERT INTO `garrison_encounter` VALUES ('384', '87468', 'Night Elf Druid', '1.3', '0.5', '1067214');
INSERT INTO `garrison_encounter` VALUES ('385', '87461', 'Dwarf Hunter', '1.3', '0.5', '1067213');
INSERT INTO `garrison_encounter` VALUES ('386', '87467', 'Human Warrior', '1.35', '0.5', '1067179');
INSERT INTO `garrison_encounter` VALUES ('387', '87459', 'Draenei Paladin', '1.3', '0.5', '1067189');
INSERT INTO `garrison_encounter` VALUES ('388', '87573', 'Worgen Rogue', '1.2', '0.5', '1067229');
INSERT INTO `garrison_encounter` VALUES ('389', '87471', 'Pandaren Monk', '1.25', '0.45', '1067232');
INSERT INTO `garrison_encounter` VALUES ('390', '87470', 'Orc Shaman', '1.4', '0.5', '1067181');
INSERT INTO `garrison_encounter` VALUES ('391', '87457', 'Blood Elf Mage', '1.3', '0.5', '1067183');
INSERT INTO `garrison_encounter` VALUES ('392', '87473', 'Tauren Warrior', '1', '0.4', '1067204');
INSERT INTO `garrison_encounter` VALUES ('393', '87474', 'Troll Hunter', '1.25', '0.5', '1067231');
INSERT INTO `garrison_encounter` VALUES ('394', '87458', 'Blood Elf Paladin', '1.3', '0.5', '1067185');
INSERT INTO `garrison_encounter` VALUES ('395', '87464', 'Goblin Rogue', '1.8', '0.5', '1067475');
INSERT INTO `garrison_encounter` VALUES ('396', '87466', 'Human Priest', '1.35', '0.5', '1067219');
INSERT INTO `garrison_encounter` VALUES ('397', '87460', 'Draenei Shaman', '1.3', '0.5', '1067206');
INSERT INTO `garrison_encounter` VALUES ('398', '87463', 'Gnome Warlock', '1.7', '0.45', '1067225');
INSERT INTO `garrison_encounter` VALUES ('399', '87465', 'Human Death Knight', '1.35', '0.5', '1067211');
INSERT INTO `garrison_encounter` VALUES ('400', '87475', 'Undead Priest', '1.3', '0.5', '1067216');
INSERT INTO `garrison_encounter` VALUES ('401', '87476', 'Undead Warlock', '1.3', '0.5', '1067217');
INSERT INTO `garrison_encounter` VALUES ('402', '87472', 'Tauren Druid', '1.25', '0.5', '1067205');
INSERT INTO `garrison_encounter` VALUES ('403', '87469', 'Orc Death Knight', '1.4', '0.5', '1067210');
INSERT INTO `garrison_encounter` VALUES ('404', '87437', 'Drov the Ruiner', '1', '0.4', '1067439');
INSERT INTO `garrison_encounter` VALUES ('405', '87493', 'Rukhmar', '1', '0.4', '1067456');
INSERT INTO `garrison_encounter` VALUES ('406', '87453', 'Highmaul Arcanist', '1.2', '0.5', '1067436');
INSERT INTO `garrison_encounter` VALUES ('407', '87401', 'Alarm-o-Bot', '1', '0.4', '1067182');
INSERT INTO `garrison_encounter` VALUES ('408', '87401', 'Alarm-o-Bot', '1', '0.4', '1067182');
INSERT INTO `garrison_encounter` VALUES ('409', '87403', 'Crowd Pummeler 9-60', '1', '0.4', '1067226');
INSERT INTO `garrison_encounter` VALUES ('410', '87504', 'Mighty Zoo Lock', '1.25', '0.5', '1067468');
INSERT INTO `garrison_encounter` VALUES ('411', '87404', 'Tricksy Miracle Rogue', '1.25', '0.5', '1067209');
INSERT INTO `garrison_encounter` VALUES ('412', '87405', 'Powerful Control Warrior', '1.25', '0.5', '1067220');
INSERT INTO `garrison_encounter` VALUES ('413', '87407', 'Cookie', '1.4', '0.5', '1067180');
INSERT INTO `garrison_encounter` VALUES ('414', '87406', 'Avatar of Aggramar', '1.4', '0.55', '1067474');
INSERT INTO `garrison_encounter` VALUES ('415', '87406', 'Avatar of Golganneth', '1.4', '0.55', '1067474');
INSERT INTO `garrison_encounter` VALUES ('416', '83113', 'Pale Tormentor', '1', '0.45', '1067305');
INSERT INTO `garrison_encounter` VALUES ('417', '87577', 'Coldtusk', '1', '0.4', '1067254');
INSERT INTO `garrison_encounter` VALUES ('418', '87578', 'Cyclonic Fury', '1', '0.4', '1067234');
INSERT INTO `garrison_encounter` VALUES ('419', '87579', 'Ba\'ruun', '1', '0.4', '1067190');
INSERT INTO `garrison_encounter` VALUES ('420', '87582', 'Shinri', '1.2', '0.5', '1067376');
INSERT INTO `garrison_encounter` VALUES ('421', '83036', 'Rolkor', '1', '0.4', '1067310');
INSERT INTO `garrison_encounter` VALUES ('422', '83078', 'Gelgor of the Blue Flame', '1', '0.4', '1067423');
INSERT INTO `garrison_encounter` VALUES ('423', '87662', 'Hen-Mother Hami', '1', '0.4', '1067448');
INSERT INTO `garrison_encounter` VALUES ('424', '87665', 'Steeltusk', '1', '0.4', '1067463');
INSERT INTO `garrison_encounter` VALUES ('425', '87693', 'Festerbloom', '1', '0.4', '1067447');
INSERT INTO `garrison_encounter` VALUES ('426', '83227', 'Shadowbark', '0.55', '0.55', '1067300');
INSERT INTO `garrison_encounter` VALUES ('427', '87694', 'Warmaster Blugthol', '1.2', '0.5', '1067317');
INSERT INTO `garrison_encounter` VALUES ('428', '87696', 'Soulfang', '1', '0.4', '1067340');
INSERT INTO `garrison_encounter` VALUES ('429', '81587', 'Bladespire Warlord', '1.2', '0.5', '1067336');
INSERT INTO `garrison_encounter` VALUES ('430', '81562', 'Thunderlord Warbringer', '1.55', '0.5', '1067236');
INSERT INTO `garrison_encounter` VALUES ('431', '83433', 'Wild Witherhide Bull', '1', '0.4', '1067249');
INSERT INTO `garrison_encounter` VALUES ('432', '83296', 'Thorny Stabber', '1.9', '0.45', '1067329');
INSERT INTO `garrison_encounter` VALUES ('433', '87857', 'Iron Horde Scout', '1.55', '0.5', '1067280');
INSERT INTO `garrison_encounter` VALUES ('434', '83432', 'Leatherhide Clefthoof Mother', '1.1', '0.4', '1067248');
INSERT INTO `garrison_encounter` VALUES ('435', '87861', 'Guardian of Water', '1', '0.4', '1067452');
INSERT INTO `garrison_encounter` VALUES ('436', '83115', 'Pale Skulker', '1', '0.45', '1067363');
INSERT INTO `garrison_encounter` VALUES ('437', '87869', 'Dark Acolyte', '1.5', '0.5', '1067238');
INSERT INTO `garrison_encounter` VALUES ('438', '87870', 'Gul\'var Grunt', '1', '0.4', '1067239');
INSERT INTO `garrison_encounter` VALUES ('439', '83267', 'Veloss', '1', '0.4', '1067426');
INSERT INTO `garrison_encounter` VALUES ('440', '87874', 'Sniveling Swindler', '1.8', '0.5', '1067379');
INSERT INTO `garrison_encounter` VALUES ('441', '87875', 'Rune Ritualist', '1.55', '0.5', '1067470');
INSERT INTO `garrison_encounter` VALUES ('442', '87876', 'Terrorwing Commander', '1.55', '0.5', '1067469');
INSERT INTO `garrison_encounter` VALUES ('443', '87882', 'Talonpriest Zorka', '1.25', '0.5', '1067361');
INSERT INTO `garrison_encounter` VALUES ('444', '87885', 'Grom\'kar Peon', '1.55', '0.5', '1067338');
INSERT INTO `garrison_encounter` VALUES ('445', '87887', 'Grom\'kar Vanguard', '1.5', '0.5', '1067467');
INSERT INTO `garrison_encounter` VALUES ('446', '83136', 'Admiral Kronnor', '1.4', '0.5', '1067359');
INSERT INTO `garrison_encounter` VALUES ('447', '83461', 'Midshipman Nawrog', '1.4', '0.5', '1067364');
INSERT INTO `garrison_encounter` VALUES ('448', '83065', 'Ambassador Lonn\'mog', '1.2', '0.5', '1067412');
INSERT INTO `garrison_encounter` VALUES ('449', '83147', 'Magma Elemental', '1.2', '0.4', '1067228');
INSERT INTO `garrison_encounter` VALUES ('450', '83046', 'Bonecrusher Hoorg', '1.2', '0.4', '1067342');
INSERT INTO `garrison_encounter` VALUES ('451', '83066', 'Magma Shaper Orggruk', '1.2', '0.5', '1067415');
INSERT INTO `garrison_encounter` VALUES ('452', '83219', 'Reclaimer Muurom', '1.2', '0.4', '1067393');
INSERT INTO `garrison_encounter` VALUES ('453', '83248', 'Rampaging Riverbeast', '1.2', '0.4', '1067251');
INSERT INTO `garrison_encounter` VALUES ('454', '83294', 'The Thorned Tyrant', '1.2', '0.4', '1067424');
INSERT INTO `garrison_encounter` VALUES ('455', '83145', 'Champion Wehgar', '1.4', '0.5', '1067243');
INSERT INTO `garrison_encounter` VALUES ('456', '83240', 'A Terrifying Intermission', '1', '0.4', '1067331');
INSERT INTO `garrison_encounter` VALUES ('457', '83073', 'Champion Mukcrush', '1.2', '0.5', '1067275');
INSERT INTO `garrison_encounter` VALUES ('458', '88536', 'Horrible Haunter', '1.2', '0.5', '1067471');
INSERT INTO `garrison_encounter` VALUES ('459', '88538', 'Sinister Specter', '1', '0.4', '1067472');
INSERT INTO `garrison_encounter` VALUES ('460', '88539', 'Phearful Phantasm', '1.2', '0.5', '1067473');
INSERT INTO `garrison_encounter` VALUES ('461', '83127', 'The Iron Foreman', '1.4', '0.5', '1067457');
INSERT INTO `garrison_encounter` VALUES ('462', '88548', 'Armored Attack Wolf', '1.5', '0.5', '1067390');
INSERT INTO `garrison_encounter` VALUES ('463', '83461', 'Folga of the Dancing Blade', '1.4', '0.5', '1067364');
INSERT INTO `garrison_encounter` VALUES ('464', '83125', 'Engineer Horthuk', '1.4', '0.5', '1067280');
INSERT INTO `garrison_encounter` VALUES ('465', '83149', 'Unleashed Fury', '1', '0.4', '1067223');
INSERT INTO `garrison_encounter` VALUES ('466', '83461', 'Rail Commander Lokkar', '1.4', '0.5', '1067364');
INSERT INTO `garrison_encounter` VALUES ('467', '83174', 'Demonic Defender', '1', '0.4', '1067394');
INSERT INTO `garrison_encounter` VALUES ('468', '83242', 'Thaandu of the Sargeri', '1.2', '0.5', '1067368');
INSERT INTO `garrison_encounter` VALUES ('469', '83380', 'Uzuul the Annihilator', '1', '0.55', '1067202');
INSERT INTO `garrison_encounter` VALUES ('470', '83156', 'Captured Fury', '1', '0.4', '1067396');
INSERT INTO `garrison_encounter` VALUES ('471', '88565', 'Shore Defenses', '1', '0.4', '1067272');
INSERT INTO `garrison_encounter` VALUES ('472', '83146', 'Captain Scarjowl', '1.4', '0.5', '1067242');
INSERT INTO `garrison_encounter` VALUES ('473', '83116', 'Wrek the Emaciated', '1', '0.45', '1067308');
INSERT INTO `garrison_encounter` VALUES ('474', '83063', 'Ronk the Thug', '1.2', '0.5', '1067417');
INSERT INTO `garrison_encounter` VALUES ('475', '83143', 'Blaak the Painmaker', '1.4', '0.5', '1067244');
INSERT INTO `garrison_encounter` VALUES ('476', '83038', 'Grom\'kar Assault Rylak', '0.9', '0.4', '1067266');
INSERT INTO `garrison_encounter` VALUES ('477', '83120', 'Thunderlord Champion', '1.4', '0.5', '1067286');
INSERT INTO `garrison_encounter` VALUES ('478', '83123', 'General Darlok', '1.4', '0.5', '1067356');

-- ----------------------------
-- Table structure for garrison_follower_quality
-- ----------------------------
DROP TABLE IF EXISTS `garrison_follower_quality`;
CREATE TABLE `garrison_follower_quality` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `QualityID` int(11) NOT NULL DEFAULT '0',
  `Quality` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Export of GarrFollowerQuality.db2';

-- ----------------------------
-- Records of garrison_follower_quality
-- ----------------------------
INSERT INTO `garrison_follower_quality` VALUES ('1', '1', '2000');
INSERT INTO `garrison_follower_quality` VALUES ('2', '2', '60000');
INSERT INTO `garrison_follower_quality` VALUES ('3', '3', '120000');
INSERT INTO `garrison_follower_quality` VALUES ('4', '4', '0');
INSERT INTO `garrison_follower_quality` VALUES ('5', '5', '0');

-- ----------------------------
-- Table structure for garrison_mission
-- ----------------------------
DROP TABLE IF EXISTS `garrison_mission`;
CREATE TABLE `garrison_mission` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `Level` int(11) NOT NULL DEFAULT '0',
  `ItemLevel` int(11) NOT NULL DEFAULT '0',
  `Type` int(11) NOT NULL DEFAULT '0',
  `Followers` int(11) NOT NULL DEFAULT '0',
  `field5` int(11) NOT NULL DEFAULT '0',
  `Time` int(11) NOT NULL DEFAULT '0',
  `field7` int(11) NOT NULL DEFAULT '0',
  `field8` int(11) NOT NULL DEFAULT '0',
  `Category` int(11) NOT NULL DEFAULT '0',
  `Name` text NOT NULL,
  `Description` text NOT NULL,
  `Location` text NOT NULL,
  `MissionID` int(11) NOT NULL DEFAULT '0',
  `field14` int(11) NOT NULL DEFAULT '0',
  `Resources` int(11) NOT NULL DEFAULT '0',
  `BonusReward` int(11) NOT NULL DEFAULT '0',
  `Experience` int(11) NOT NULL DEFAULT '0',
  `Percentage` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Export of GarrMission.db2';

-- ----------------------------
-- Records of garrison_mission
-- ----------------------------
INSERT INTO `garrison_mission` VALUES ('2', '90', '0', '23', '1', '0', '1800', '9999999', '101', '3', 'Gronnlings Abound', 'Gronnlings are a menace to the region.  We should partner with the Frostwolf clan to thin the population.  One may even join our cause...', 'Frostwall Approach', '26253', '12', '0', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('3', '90', '0', '23', '1', '0', '1800', '9999999', '101', '3', 'Landfall Recovery', 'It would be a worthwhile endeavor to search the ship we escaped Tanaan Jungle in, it\'s bound to turn up something useful!', 'The Burning Glacier', '26251', '12', '5', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('5', '90', '0', '24', '1', '0', '1800', '259200', '0', '18', 'Powdered Gronnling', 'Gronnling bones have been discovered in the cavern behind the garrison. Your alchemists are interested in digging them up for research purposes.', 'Garrison Caves', '26539', '19', '10', '0', '100', '100');
INSERT INTO `garrison_mission` VALUES ('6', '90', '0', '12', '1', '0', '1800', '9999999', '101', '3', 'Grimfrost Wolfbeaters', 'The Grimfrost Ogres east of the Icewind Drifts are blatantly mistreating frostwolves.  Unacceptable!', 'Grimfrost Hill', '26320', '12', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('7', '90', '0', '15', '1', '0', '1800', '9999999', '101', '3', 'Stonefury Rescue', 'There are reports of a bloody massacre on the Stonefury Cliffs!  Scatter the Frostbeak Scavengers and rescue wounded allies!', 'Stonefury Cliffs', '28567', '12', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('36', '90', '0', '23', '1', '0', '1800', '259200', '0', '18', 'Frosty Potions', 'A scout has located a cache of Thunderlord potions frozen in the snow. Recovering them will allow your alchemists to learn about the ingredients of Draenor.', 'Icewind Drifts', '26539', '19', '10', '0', '100', '100');
INSERT INTO `garrison_mission` VALUES ('42', '90', '400', '23', '1', '0', '7', '999999999', '0', '4', 'Gimme Loots', 'Grab all the loots', 'Auction House', '26539', '26', '0', '1', '5', '50');
INSERT INTO `garrison_mission` VALUES ('43', '90', '0', '29', '3', '0', '10', '9999999', '101', '3', 'Frostfire Patrol', 'This is Matt\'s test mission *bugles and horns blair a triumphant tune*', 'Icewind Plains', '26539', '26', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('44', '90', '0', '15', '1', '0', '1800', '9999999', '101', '3', 'Skyward Threat', 'Rylaks from the Burning Glacier have been attacking our supply caravans.  The material must flow!', 'Frostwall Approach', '26252', '12', '0', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('45', '90', '0', '23', '1', '0', '1800', '259200', '0', '18', 'Ogre Remedies', 'Bladespire ogres on the Burning Glacier usually carry packs of supplies when they venture out into the dunes of Frostfire. They may contain new types of potions.', 'Burning Glacier', '26539', '19', '10', '0', '100', '100');
INSERT INTO `garrison_mission` VALUES ('54', '90', '400', '23', '1', '0', '7', '60', '0', '4', 'No Loots For You', 'Run away!!', 'Poor House', '26539', '26', '0', '0', '5', '0');
INSERT INTO `garrison_mission` VALUES ('55', '90', '0', '21', '3', '0', '8', '9999999', '101', '13', 'Orcs in the Mountains!', 'Kill the orcs in the mountains.', 'Orc Mountain', '26539', '18', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('56', '90', '0', '22', '3', '0', '8', '9999999', '103', '25', 'Ogres in the Desert!', 'Kill the ogres in the desert.', 'Ogre Desert', '26539', '18', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('57', '90', '0', '23', '3', '0', '8', '9999999', '104', '24', 'Undead In The Snow', 'Kill the undead in the snow.', 'Undead Snow', '26539', '18', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('58', '90', '0', '24', '3', '0', '8', '9999999', '106', '23', 'Arakkoa Underground', 'Kill the arakkoa underground.', 'Arakkoa Underground', '26539', '18', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('59', '90', '0', '25', '3', '0', '8', '9999999', '107', '18', 'Beasts in the Jungle', 'Kill the beasts in the jungle.', 'Beast Jungle', '26539', '18', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('60', '90', '0', '26', '3', '0', '8', '9999999', '105', '19', 'Demons in the Forest', 'Kill the demons in the forest.', 'Demon Forest', '26539', '18', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('61', '90', '0', '27', '3', '0', '8', '9999999', '102', '20', 'Breakers in Town', 'Kill the breakers in the town.', 'Breaker Town', '26539', '18', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('62', '90', '0', '28', '3', '0', '8', '9999999', '101', '21', 'Furies in the Swamp', 'Kill the furies in the swamp.', 'Fury Swamp', '26539', '18', '0', '0', '100', '100');
INSERT INTO `garrison_mission` VALUES ('63', '90', '0', '29', '3', '0', '8', '9999999', '106', '22', 'Primals in the Plains', 'Kill the primals in the plains.', 'Primal Plains', '26539', '18', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('64', '90', '0', '21', '1', '0', '600', '9999999', '0', '3', 'One Mission To Rule Them All', 'The sword of a thousand truths must be destroyed in the Blackrock Foundry.  Make it happen dawg.', 'Gorgrond', '26539', '18', '0', '0', '200', '0');
INSERT INTO `garrison_mission` VALUES ('65', '90', '0', '26', '1', '0', '1800', '9999999', '106', '13', 'The Night Watch', 'A regular patrol around the garrison seems like a good idea...', 'Garrison Border', '27657', '20', '5', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('66', '90', '0', '26', '1', '0', '1800', '9999999', '106', '3', 'Killing the Corrupted', 'Corrupted beasts continue to plague the Cursed Woods. The Starfall Sentinels request our aid in putting the poor creatures out of their misery.', 'Cursed Woods', '26601', '28', '0', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('67', '90', '0', '11', '3', '0', '3600', '0', '101', '3', 'Thunderlord Assault', 'We could send several of our best on a raid of the Thunderlord outpost of Colossal\'s Fall!  Go forth to victory.', 'Colossal\'s Fall', '28566', '12', '10', '1', '200', '65');
INSERT INTO `garrison_mission` VALUES ('73', '90', '0', '28', '1', '0', '300', '86400', '101', '6', 'Clean the Latrine', 'It\'s best not to lose a bet with the janitor.', 'Outhouse', '29014', '29', '0', '0', '0', '100');
INSERT INTO `garrison_mission` VALUES ('86', '90', '0', '26', '1', '0', '1800', '9999999', '106', '3', 'Landfall Recovery', 'It would be a worthwhile endeavor to search the ship we escaped Tanaan Jungle in, it\'s bound to turn up something useful!', 'Eventide Landing', '26958', '28', '5', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('87', '90', '0', '23', '1', '0', '1800', '9999999', '101', '13', 'The Night Watch', 'A regular patrol around the garrison seems like a good idea...', 'Garrison Border', '28319', '20', '5', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('88', '90', '0', '15', '1', '0', '1800', '9999999', '106', '3', 'Skyward Threat', 'Lunarfall Ravens from Moonflower Valley have been attacking our supply caravans.  The material must flow!', 'Moonflower Valley', '28564', '28', '0', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('89', '90', '0', '11', '3', '0', '3600', '0', '106', '3', 'Shadowmoon Assault', 'We could send several of our best on a raid of the Shadowmoon outpost of Shaz\'Gul!  For the Alliance!', 'Shaz\'Gul', '26960', '28', '10', '1', '200', '65');
INSERT INTO `garrison_mission` VALUES ('90', '90', '0', '19', '1', '0', '1800', '9999999', '106', '3', 'Fey Friends', 'Arcane Anomalies are endangering the Fey Dragons of the Shimmer Mor.  Unacceptable!', 'The Shimmer Mor', '28565', '28', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('91', '90', '0', '11', '1', '0', '1800', '9999999', '106', '3', 'Rangari Rescue', 'The Shadowmoon have taken several Rangari and Alliance forces captive.  Infiltrate and rescue!', 'Shaz\'gul', '26963', '28', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('107', '100', '0', '29', '3', '0', '36000', '129600', '104', '3', 'Savage Kindred', 'The gronn and the ogres are working together on a two-pronged attack. We must disrupt their machinations before they overwhelm us.', 'Nagrand', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('108', '100', '0', '11', '3', '0', '28800', '129600', '103', '3', 'Industrial Espionage', 'If there are design flaws in the Blackrock clan\'s war machines, we can disable them. Break in and inspect the war machines for weaknesses.', 'Gorgrond', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('109', '100', '0', '20', '3', '0', '36000', '129600', '103', '3', 'Claws, Pincers, AND Tusks', 'Draenor can count the genesaur among its most vicious creatures. We must contain them before they spread from Gorgrond to other areas.', 'Gorgrond', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('110', '100', '0', '14', '3', '0', '28800', '129600', '107', '3', 'Birds of Pray', 'We would like to leave the vicious arakkoa alone, but capturing their temple in the Spires of Arak would give us a strategic advantage.', 'Spires of Arak', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('111', '100', '0', '16', '3', '0', '28800', '129600', '105', '3', 'Talador Burning', 'Abyssals, the minions of Sargeras, are roaming the forests of Talador, setting them ablaze. Destroy them before the forest is ash.', 'Talador', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('112', '100', '0', '21', '3', '0', '28800', '129600', '107', '3', 'Into the Saberon\'s Den', 'Many of the rescued draenei prisoners have gone missing, and we think the saberon are to blame. Go investigate, but go well armed.', 'Spires of Arak', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('113', '100', '0', '16', '3', '0', '28800', '129600', '105', '3', 'Burning Legion Vanguard', 'With infernals and felguard invading Talador, the rest of the Burning Legion can\'t be far behind. Defeat them, and try to find their source.', 'Talador', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('114', '100', '0', '19', '3', '0', '21600', '129600', '106', '3', 'Twisting the Nether', 'The voidlords and voidcallers plaguing Shadowmoon Valley are being summoned by someone. Find and kill whoever is responsible.', 'Shadowmoon Valley', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('115', '100', '0', '23', '3', '0', '21600', '129600', '101', '3', 'Hardened Creatures', 'Magnaron and goren both grow rocky plates on their hides that have magical properties. Collect these plates for our use.', 'Frostfire Ridge', '27780', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('116', '100', '0', '12', '3', '0', '28800', '129600', '101', '3', 'Iron Horde Allies', 'Numerous and powerful, ogre warlords are formidable allies of the Iron Horde. Try to induce them to leave - forcefully, if necessary.', 'Frostfire Ridge', '27780', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('117', '100', '0', '11', '3', '0', '28800', '129600', '101', '3', 'Clefthoof Rustling', 'To weaken the Thunderlord clan, we must eliminate their fiercest warriors - their clefthoof-riders. Go to their camp and free their mounts.', 'Frostfire Ridge', '27780', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('118', '100', '0', '26', '3', '0', '28800', '129600', '106', '3', 'The Corruption Spreads', 'Fel magic has corrupted a few ancients of Shadowmoon Valley. They must be cut down to end their mad rampages.', 'Shadowmoon Valley', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('119', '100', '0', '11', '2', '0', '14400', '129600', '104', '3', 'Heart of the Iron Horde', 'The Warsong clan is a symbol of the Iron Horde\'s power. Under the cover of night, set its camp ablaze. This will kill their morale.', 'Nagrand', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('120', '100', '0', '29', '2', '0', '14400', '129600', '104', '3', 'Saberon Ambush', 'Patrols have reported that roving bands of saberon are waylaying merchants traveling through Nagrand. Find them and stop them.', 'Nagrand', '27780', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('125', '100', '0', '14', '2', '0', '14400', '129600', '107', '3', 'The Feathers Must Fly', 'Arakkoa talonpriests have begun sacrificing draenei to their god, Terokk. Interrupt their ritual and free the prisoners.', 'Spires of Arak', '27780', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('126', '100', '0', '26', '2', '0', '14400', '129600', '105', '3', 'Cult of Sargeras', 'In a remote corner of Talador, a small faction of draenei has embraced the worship of Sargeras. Stop their cult before it spreads.', 'Talador', '27780', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('127', '100', '0', '12', '2', '0', '14400', '129600', '103', '3', 'To Freedom!', 'Ogre slavers have captured many of the draenei displaced by the Iron Horde. Send someone to their camp and liberate them.', 'Gorgrond', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('128', '100', '0', '12', '3', '0', '28800', '129600', '104', '3', 'Go for the Heads', 'Without their leader, the high ogre attack our spies warned us about will fail. Send an assassin to kill their warlord in Nagrand.', 'Nagrand', '27780', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('129', '100', '0', '20', '3', '0', '28800', '129600', '106', '3', 'Secrets of Blademoon', 'The hardened bark of the genesaur and botani offer an opportunity to strengthen our armor.  Do what needs to be done.', 'Shadowmoon Valley', '27780', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('130', '100', '0', '15', '2', '0', '14400', '129600', '107', '3', 'Ravager Infestation', 'Pests! These ravagers are everywhere - in our supplies, in our barracks! Eradicate their nest in the Spires of Arak before they overrun us.', 'Spires of Arak', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('131', '100', '0', '16', '3', '0', '14400', '129600', '105', '3', 'The Infernals\' Fury', 'Infernals are an uncommon sight in Talador. We\'d like to keep it that way. The more that are destroyed, the better.', 'Talador', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('132', '100', '0', '15', '2', '0', '21600', '129600', '107', '3', 'The Basilisk\'s Stare', 'Basilisks have turned some arakkoa in the Spires of Arak to stone. It\'s macabre, but this stone is a precious resource. Gather it carefully.', 'Spires of Arak', '27780', '30', '0', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('133', '100', '0', '18', '2', '0', '21600', '129600', '101', '3', 'Elemental Territory', 'A group of territorial elementals has claimed an area near our supply route in Frostfire Ridge. Slay them and collect any lost supplies.', 'Frostfire Ridge', '27780', '30', '0', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('135', '90', '0', '15', '1', '0', '1800', '86400', '106', '3', 'Flora and Fauna', 'Dark magic runs amok in Shadowmoon Valley, giving rise to maddened, unnatural creatures. End their suffering.', 'Shadowmoon Valley', '28568', '34', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('136', '90', '0', '11', '1', '0', '1800', '86400', '106', '3', 'Sorcery, Meet Sword', 'Shadowmoon spellcasters are experimenting with unstable void magic near the Teluuna Observatory. Because that\'s going to end well.', 'Shadowmoon Valley', '28568', '34', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('137', '90', '0', '26', '1', '0', '1800', '86400', '106', '3', 'Weed Whacker', 'Don\'t be fooled. They\'re only adorable until they swarm you and gnaw your face off.', 'Shadowmoon Valley', '31532', '34', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('138', '90', '0', '18', '1', '0', '1800', '86400', '101', '3', 'A Bit Nippy', 'Lately the plains have been even more bitterly cold than usual. Find out what\'s causing this weather before it starts costing lives.', 'Frostfire Ridge', '28569', '34', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('139', '90', '0', '23', '1', '0', '1800', '86400', '101', '3', 'Frosty and Delicious', 'Elementals are plentiful in Frostfire Ridge. Goren love eating them, and whatever the goren eat, they inherit its powers. Not good.', 'Frostfire Ridge', '28569', '34', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('140', '90', '0', '12', '1', '0', '1800', '86400', '101', '3', 'Lord of War', 'The Bloodmaul ogres have begun raiding Horde outposts, costing us soldiers and supplies. We believe a new warlord is to blame.', 'Frostfire Ridge', '31531', '34', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('141', '91', '0', '20', '1', '0', '3600', '86400', '106', '3', 'Ancient Anguish', 'It\'s time to chop down a few ancient trees. Only the ones that are demonically corrupted, mind you. The rest, you can hug if you like.', 'Shadowmoon Valley', '28752', '34', '10', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('142', '91', '0', '11', '1', '0', '3600', '86400', '106', '3', 'No Dwarf Left Behind', 'Shadowmoon forces have abducted a Wildhammer shaman near the Cursed Woods. Rescue our foolhardy ally and bring the orcs to justice.', 'Shadowmoon Valley', '28752', '34', '10', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('143', '91', '0', '19', '1', '0', '3600', '86400', '106', '3', 'The Howling Void', 'We have reports of the Shadowmoon prowling Gloomshade Grove, shrouding the land in foul magic. Disrupt their dark rituals at any cost.', 'Shadowmoon Valley', '28752', '34', '10', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('144', '91', '0', '11', '1', '0', '3600', '86400', '101', '3', 'Silence the Thunder', 'Thunderlord hunting parties have been sighted to the north. Set an ambush in the cliffs and show them what the real Horde is made of!', 'Frostfire Ridge', '28753', '34', '10', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('145', '91', '0', '23', '1', '0', '3600', '86400', '101', '3', 'The Shadow Beneath Us', 'The Shadow Council has wormed its way into a network of caves west of the garrison. We cannot allow it to gain a foothold in the region!', 'Frostfire Ridge', '28753', '34', '10', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('146', '91', '0', '15', '1', '0', '3600', '86400', '101', '3', 'Rylak Attrition', 'Killing full-grown rylaks has not diminished their numbers in the least. Scour their nesting grounds and end this menace at the source.', 'Frostfire Ridge', '28753', '34', '10', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('147', '92', '0', '20', '1', '0', '3600', '86400', '103', '3', 'Ancient Affliction', 'A strange disease is spreading among Gorgrond\'s ancients, driving them mad. Destroy them before the infection escalates.', 'Gorgrond', '27785', '34', '10', '0', '300', '50');
INSERT INTO `garrison_mission` VALUES ('148', '92', '0', '11', '1', '0', '3600', '86400', '103', '3', 'Breaking the Blackrock', 'Scouts have recovered a map of Blackrock supply caches hidden across Gorgrond. Destroying them would strike a grave blow to our enemy.', 'Gorgrond', '27785', '34', '10', '0', '300', '50');
INSERT INTO `garrison_mission` VALUES ('149', '92', '0', '20', '1', '0', '3600', '86400', '103', '3', 'Ravenous Genesaur', 'The Gorgrond genesaur have overhunted in their territory and are raiding nearby draenei camps for food. Attack them and lure them away.', 'Gorgrond', '31533', '34', '10', '0', '300', '50');
INSERT INTO `garrison_mission` VALUES ('150', '93', '0', '22', '1', '0', '3600', '86400', '103', '3', 'Little Biters', 'A group of goren has settled in a valuable quarry nearby. We need to get them out if we plan to use the quarry in the future.', 'Gorgrond', '27786', '34', '10', '0', '400', '35');
INSERT INTO `garrison_mission` VALUES ('151', '93', '0', '21', '1', '0', '1800', '86400', '103', '3', 'Nuisance Gronnling', 'Gronnlings continue to interrupt our efforts to expand into Gorgrond. We need to thin their numbers if we wish to gain a foothold.', 'Gorgrond', '27786', '34', '10', '0', '400', '35');
INSERT INTO `garrison_mission` VALUES ('152', '93', '0', '11', '1', '0', '3600', '86400', '103', '3', 'Safer Up Close', 'Iron Horde siege weapons are deadly at a distance but vulnerable up close. Targeting their armaments with a small force may prove wise.', 'Gorgrond', '31534', '34', '10', '0', '400', '35');
INSERT INTO `garrison_mission` VALUES ('153', '94', '0', '11', '1', '0', '3600', '86400', '105', '3', 'Shivan Sightings', 'Shivan have been sighted on the outskirts of Talador. They may be the first wave of a Burning Legion incursion. Investigation is warranted.', 'Talador', '27787', '34', '10', '0', '500', '35');
INSERT INTO `garrison_mission` VALUES ('154', '94', '0', '26', '2', '0', '1800', '86400', '105', '3', 'Stalactite Appetite', 'A cavern beneath an inhabited section of Talador has become unstable due to the appetite of the goren inside. We must protect those above.', 'Talador', '27787', '34', '10', '0', '500', '35');
INSERT INTO `garrison_mission` VALUES ('155', '94', '0', '14', '1', '0', '3600', '86400', '105', '3', 'Leading the Flock', 'A charismatic high arakkoa priest is inciting his congregation to raid nearby settlements in the name of Terokk. We must defend our allies.', 'Talador', '27787', '34', '10', '0', '500', '35');
INSERT INTO `garrison_mission` VALUES ('156', '95', '0', '11', '1', '0', '3600', '86400', '105', '3', 'A Call to Action', 'Absolute power corrupts, and that\'s absolutely the reason we must neutralize this powerful warlock. Terminate with extreme prejudice.', 'Talador', '27788', '34', '10', '0', '600', '35');
INSERT INTO `garrison_mission` VALUES ('157', '95', '0', '12', '1', '0', '1800', '86400', '105', '3', 'Bully-Free Zone', 'From Azeroth to Draenor, bullies and their cronies remain the same. Please show our new \"friends\" how we deal with their kind on Azeroth.', 'Talador', '27788', '34', '10', '0', '600', '35');
INSERT INTO `garrison_mission` VALUES ('158', '95', '0', '17', '1', '0', '3600', '86400', '105', '3', 'There Can Be Only One', 'The ogron are said to be violence incarnate, but I\'ve heard your allies are called the same. Let\'s find out who lives up to their hype.', 'Talador', '31535', '34', '10', '0', '600', '35');
INSERT INTO `garrison_mission` VALUES ('159', '96', '0', '11', '1', '0', '5400', '86400', '107', '3', 'Hurts So Good', 'Shattered Hand orcs embrace pain, for they say it strengthens them. Make them very strong. Then very dead.', 'Spires of Arak', '27789', '34', '10', '0', '700', '35');
INSERT INTO `garrison_mission` VALUES ('160', '96', '0', '21', '2', '0', '2700', '86400', '107', '3', 'Crush the Pale', 'The Pale are twisted, cowardly wretches, but even scavengers can be deadly in sufficient numbers. Still, that\'s easily solved, isn\'t it?', 'Spires of Arak', '27789', '34', '10', '0', '700', '35');
INSERT INTO `garrison_mission` VALUES ('161', '96', '0', '15', '1', '0', '5400', '86400', '107', '3', 'Bug Squashing', 'Ravagers reproduce quickly and eat anything. Like crickets. They\'re also homicidal and roughly the size of horses. Not like crickets.', 'Spires of Arak', '27789', '34', '10', '0', '700', '35');
INSERT INTO `garrison_mission` VALUES ('162', '97', '0', '21', '1', '0', '5400', '86400', '107', '3', 'Ryled Up', 'Our livestock have gone missing. Tracks point to the rylaks. Deal with the threat, but don\'t take them lightly.', 'Spires of Arak', '27790', '34', '10', '0', '800', '35');
INSERT INTO `garrison_mission` VALUES ('163', '97', '0', '15', '1', '0', '2700', '86400', '107', '3', 'Blow by Blow', 'A vicious wind serpent appears to be acting on orders from an unseen master, wearing down our defenses. Seek out and end this threat.', 'Spires of Arak', '27790', '34', '10', '0', '800', '35');
INSERT INTO `garrison_mission` VALUES ('164', '97', '0', '11', '1', '0', '3600', '86400', '107', '3', 'Underhanded', 'Our informants in the Shattered Hand have turned out to be double agents. Set up a covert meeting and eliminate them.', 'Spires of Arak', '31536', '34', '10', '0', '800', '35');
INSERT INTO `garrison_mission` VALUES ('165', '98', '0', '19', '1', '0', '7200', '86400', '104', '3', 'Surge Protection', 'Energy-siphoning water spirits are giving our spellcasters fits. Time to bring in reinforcements.', 'Nagrand', '27791', '34', '10', '0', '900', '35');
INSERT INTO `garrison_mission` VALUES ('166', '98', '0', '12', '2', '0', '5400', '86400', '104', '3', 'Spell Check', 'The cause of our recent communal lethargy has been traced to Highmaul sorcerers. Find them and put an end to their sinister arcane rites.', 'Nagrand', '27791', '34', '10', '0', '900', '35');
INSERT INTO `garrison_mission` VALUES ('167', '98', '0', '11', '1', '0', '5400', '86400', '104', '3', 'Warsong Swan Song', 'Warsong shaman have enacted a spell to open an abyss beneath our feet! Kill all of them before the sands in the hourglass run out!', 'Nagrand', '31537', '34', '10', '0', '900', '35');
INSERT INTO `garrison_mission` VALUES ('168', '99', '0', '12', '1', '0', '7200', '86400', '104', '3', 'Ogre-Kill', 'High ogre warriors are in battle array, screaming that they\'re going to \"destroy the interlopers.\" Do you think they\'re talking about us?', 'Nagrand', '27792', '34', '10', '0', '1000', '35');
INSERT INTO `garrison_mission` VALUES ('169', '99', '0', '26', '2', '0', '5400', '86400', '104', '3', 'Burning Desire', 'Burning Blade orcs are attacking treasure hunters at Oshu\'gun and looting their crystals. Could they be preparing for some dark ritual?', 'Nagrand', '27792', '34', '10', '0', '1000', '35');
INSERT INTO `garrison_mission` VALUES ('170', '99', '0', '29', '1', '0', '7200', '86400', '104', '3', 'Gronn with the Wind', 'Our scouts have vanished without a trace! They were last seen entering gronn lands. Find out what happened to them.', 'Nagrand', '27792', '34', '10', '0', '1000', '35');
INSERT INTO `garrison_mission` VALUES ('171', '90', '0', '26', '1', '0', '1800', '86400', '106', '3', 'Restless Spirits', 'Shadowmoon Valley is haunted by orc spirits that lash out in confusion and rage. Until they are put to rest, no one is safe.', 'Shadowmoon Valley', '28568', '34', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('172', '90', '0', '11', '1', '0', '1800', '86400', '101', '3', 'The Observer Effect', 'The Thunderlord orcs are growing bold: their scouts follow Horde parties with little stealth or fear. Such arrogance deserves a response.', 'Frostfire Ridge', '28569', '34', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('173', '91', '0', '26', '1', '0', '3600', '86400', '106', '3', 'Podling Patrol', 'Wayward podlings have stolen a crate of our giant growth elixirs. Recover the potions before the creatures consume them and run amok.', 'Shadowmoon Valley', '28752', '34', '10', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('174', '91', '0', '11', '1', '0', '3600', '86400', '101', '3', 'Blackrock Intelligence', 'Blackrock messengers have been spotted crisscrossing the region. Any missives they are carrying could prove invaluable to our cause.', 'Frostfire Ridge', '28753', '34', '10', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('175', '92', '0', '12', '1', '0', '2700', '86400', '103', '3', 'Break Their Chains', 'Ogre slavers are carrying off our allies to fight in their arenas. Pose as a would-be gladiator and free the unwilling combatants.', 'Gorgrond', '27785', '34', '10', '0', '300', '50');
INSERT INTO `garrison_mission` VALUES ('176', '93', '0', '22', '1', '0', '2700', '86400', '103', '3', 'Ogron\'s Out of the Bag', 'We know the ogron are going to attack soon because they told us. No one ever said they were smart. Stop them before they do any real harm.', 'Gorgrond', '27786', '34', '10', '0', '400', '35');
INSERT INTO `garrison_mission` VALUES ('177', '94', '0', '14', '1', '0', '2700', '86400', '105', '3', 'All That Glitters', 'Redistributing wealth from the departed isn\'t stealing. Trouble is, there\'s a mighty arakkoan sorcerer who doesn\'t know it. Do let him know.', 'Talador', '27787', '34', '10', '0', '500', '35');
INSERT INTO `garrison_mission` VALUES ('178', '95', '0', '26', '1', '0', '2700', '86400', '105', '3', 'Shreddercide', 'The Iron Horde\'s shredders are laying waste to Talador\'s forests. Bastards. If someone\'s doing that, it should be our forces.', 'Talador', '27788', '34', '10', '0', '600', '35');
INSERT INTO `garrison_mission` VALUES ('179', '96', '0', '14', '1', '0', '3600', '86400', '107', '3', 'Worth Its Weight', 'The arakkoa have begun stealing and hoarding gold. Time to steal it back.', 'Spires of Arak', '27789', '34', '10', '0', '700', '35');
INSERT INTO `garrison_mission` VALUES ('180', '97', '0', '21', '1', '0', '3600', '86400', '107', '3', 'Curiosity Killed Them', 'Saberon have begun attacking with wild abandon. They believe they have nine lives. Prove them wrong.', 'Spires of Arak', '27790', '34', '10', '0', '800', '35');
INSERT INTO `garrison_mission` VALUES ('181', '98', '0', '29', '1', '0', '5400', '86400', '104', '3', 'Medical Necessity', 'Our soldiers in the field need medical supplies, but Warsong orcs are patrolling the road. Escort the caravan so it makes its destination.', 'Nagrand', '27791', '34', '10', '0', '900', '35');
INSERT INTO `garrison_mission` VALUES ('182', '99', '0', '29', '1', '0', '5400', '86400', '104', '3', 'Raider Danger', 'Warsong orcs have razed our camp to the north, and the lone survivor says they\'re headed this way. We must repel the invaders!', 'Nagrand', '27792', '34', '10', '0', '1000', '35');
INSERT INTO `garrison_mission` VALUES ('183', '90', '0', '15', '1', '0', '1800', '86400', '106', '3', 'Hazardous Herbivores', 'Hippos are taking over the Verdant Mire. They\'re violently territorial and extremely tough. They\'re also blocking key resources.', 'Shadowmoon Valley', '28568', '34', '0', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('184', '90', '0', '23', '1', '0', '1800', '86400', '101', '3', 'Cleave the Clefthoof', 'In this harsh environment, sturdy transportation is essential. Cripple the Iron Horde by taking out its greatest beasts of burden.', 'Frostfire Ridge', '28569', '34', '0', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('185', '91', '0', '26', '1', '0', '3600', '86400', '106', '3', 'Toxic Bloom', 'A noxious mist is pervading Blademoon Bloom. Find and neutralize the source so that our forces can pass safely through the region.', 'Shadowmoon Valley', '28752', '34', '0', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('186', '91', '0', '23', '1', '0', '3600', '86400', '101', '3', 'The Pale Dilemma', 'The pale have stolen a shipment of Iron Horde weaponry. Recover the armaments - by force if necessary - before our enemy does.', 'Frostfire Ridge', '28753', '34', '0', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('187', '92', '0', '21', '1', '0', '2700', '86400', '103', '3', 'Many-Headed Menace', 'Habitat changes in Gorgrond have forced hydras to encroach on draenei settlements. It is a pity, but we must kill these desperate beasts.', 'Gorgrond', '27785', '34', '0', '0', '300', '50');
INSERT INTO `garrison_mission` VALUES ('188', '93', '0', '15', '1', '0', '2700', '86400', '103', '3', 'Great Migration', 'Every year, great herds of clefthoof migrate through Gorgrond, causing tremors and dust storms. We must divert them from our camp.', 'Gorgrond', '27786', '34', '0', '0', '400', '35');
INSERT INTO `garrison_mission` VALUES ('189', '94', '0', '26', '1', '0', '2700', '86400', '105', '3', 'The Needs of the Many', 'Elementals are chaotic by nature and are best avoided. This elemental has something we need: resources. Make it an offer it cannot refuse.', 'Talador', '27787', '34', '0', '0', '500', '35');
INSERT INTO `garrison_mission` VALUES ('190', '95', '0', '26', '1', '0', '2700', '86400', '105', '3', 'Uproot the Problem', 'Our garrison requires resources, so it\'s harvest time. Kindly remind this savage plant and its minions why we\'re atop the food chain.', 'Talador', '27788', '34', '0', '0', '600', '35');
INSERT INTO `garrison_mission` VALUES ('191', '96', '0', '20', '1', '0', '3600', '86400', '107', '3', 'Knotty by Nature', 'An ancient has begun attacking everyone and everything in its path. On a related note, we\'re currently low on firewood.', 'Spires of Arak', '27789', '34', '0', '0', '700', '35');
INSERT INTO `garrison_mission` VALUES ('192', '97', '0', '15', '1', '0', '3600', '86400', '107', '3', 'Nevermore', 'We think the recent dread raven attacks are connected to the arakkoa. We\'ll investigate them. You hunt down and kill their minions.', 'Spires of Arak', '27790', '34', '0', '0', '800', '35');
INSERT INTO `garrison_mission` VALUES ('193', '98', '0', '15', '1', '0', '5400', '86400', '104', '3', 'Mercy Killing', 'War has ravaged the region\'s plant life, and the clefthoof don\'t have enough to eat. We should thin out their numbers before they starve.', 'Nagrand', '27791', '34', '0', '0', '900', '35');
INSERT INTO `garrison_mission` VALUES ('194', '99', '0', '18', '1', '0', '5400', '86400', '104', '3', 'Out of His Element', 'Enraged, Stonegar has begun attacking shaman near the Throne of the Elements. We must send forces to aid.', 'Nagrand', '27792', '34', '0', '0', '1000', '35');
INSERT INTO `garrison_mission` VALUES ('195', '91', '0', '20', '3', '0', '5400', '86400', '106', '3', 'The Best-Laid Plans...', 'Our former courier got spooked and dropped an important package from Karabor in a thicket of botani and ancients.  Retrieve the package!', 'Shadowmoon Valley', '28752', '36', '20', '1', '300', '65');
INSERT INTO `garrison_mission` VALUES ('196', '91', '0', '12', '3', '0', '5400', '86400', '101', '3', 'Free Labor', 'Bladespire ogres have captured a band of laborers from the garrison. Infiltrate their fortress and free the workers.', 'Frostfire Ridge', '28753', '36', '20', '1', '300', '65');
INSERT INTO `garrison_mission` VALUES ('197', '92', '0', '17', '3', '0', '5400', '86400', '103', '3', 'Dumb and Dumber', 'Several gronn and ogron came here to attack us, but the dumb beasts are just fighting each other. Let\'s kill them all and end this farce.', 'Gorgrond', '27785', '36', '20', '1', '450', '50');
INSERT INTO `garrison_mission` VALUES ('198', '93', '0', '11', '3', '0', '5400', '86400', '103', '3', 'Guinea Pigs', 'Blackrock orcs are testing out their war machines on the region\'s unsuspecting fauna. This aggression will not stand!', 'Gorgrond', '27786', '36', '20', '1', '600', '20');
INSERT INTO `garrison_mission` VALUES ('199', '94', '0', '16', '3', '0', '5400', '86400', '105', '3', 'The Plot Thickens', 'Demons have been skulking about the Court of Souls, and it\'s not because they enjoy the scenery. Find out what they\'re plotting.', 'Talador', '27787', '36', '20', '1', '750', '20');
INSERT INTO `garrison_mission` VALUES ('200', '95', '0', '16', '3', '0', '5400', '86400', '105', '3', 'Demonic Retribution', 'Agents of the Burning Legion are hunting down refugee draenei from Shattrath. Gather the refugees and take them to safety.', 'Talador', '27788', '36', '20', '1', '900', '20');
INSERT INTO `garrison_mission` VALUES ('201', '96', '0', '14', '3', '0', '6750', '86400', '107', '3', 'Bad Day at Work', 'Several laborers have gone missing, and we fear that the arakkoa plan to burn them alive in sacrifice. We must save them!', 'Spires of Arak', '27789', '36', '20', '1', '1050', '20');
INSERT INTO `garrison_mission` VALUES ('202', '97', '0', '11', '3', '0', '6750', '86400', '107', '3', 'Shattered Expectations', 'We have intel that a unit of Shattered Hand orcs is en route to assist the Iron Horde in Talador. Too bad they\'ll never make it.', 'Spires of Arak', '27790', '36', '20', '1', '1200', '20');
INSERT INTO `garrison_mission` VALUES ('203', '98', '0', '19', '3', '0', '7200', '86400', '104', '3', 'Scared of the Dark', 'To the south lies Shadowfall Canyon, where wretched creatures are subjucating spirits of Fire, Earth, and Wind. Hopefully, no worse.', 'Nagrand', '27791', '36', '20', '1', '1350', '20');
INSERT INTO `garrison_mission` VALUES ('204', '99', '0', '29', '3', '0', '5400', '86400', '104', '3', 'Spell Trouble', 'High ogres are enslaving gronn with pernicious magic, almost doubling the size of their army. We must put an end to the spellcasting.', 'Nagrand', '27792', '36', '20', '1', '1500', '20');
INSERT INTO `garrison_mission` VALUES ('205', '91', '0', '11', '3', '0', '5400', '86400', '106', '3', 'Lessons of History', 'Shadowmoon orcs are summoning void beasts through hastily opened portals that could tear Draenor apart, once again. Stop them!', 'Shadowmoon Valley', '28752', '36', '15', '1', '300', '65');
INSERT INTO `garrison_mission` VALUES ('206', '91', '0', '23', '3', '0', '5400', '86400', '101', '3', 'Drunk with Power', 'Gronn attacked an alchemist\'s wagon, and now they\'re hopped up on strength elixirs.  Let\'s get \'em before they find the invisibility potions.', 'Frostfire Ridge', '28753', '36', '15', '1', '300', '65');
INSERT INTO `garrison_mission` VALUES ('207', '92', '0', '20', '3', '0', '5400', '86400', '103', '3', 'Environmental Hazard', 'The genesaur and their botani minions are attacking caravans on the road to Talador. We\'ll make them rue the day they were sprouted!', 'Gorgrond', '27785', '36', '15', '1', '450', '20');
INSERT INTO `garrison_mission` VALUES ('208', '93', '0', '12', '3', '0', '5400', '86400', '103', '3', 'Elements of Surprise', 'Our scouts spotted a group of ogre slavers gathering in Gorgrond. We cannot allow them a chance to fortify their position. Attack!', 'Gorgrond', '27786', '36', '15', '1', '600', '20');
INSERT INTO `garrison_mission` VALUES ('209', '94', '0', '11', '3', '0', '5400', '86400', '105', '3', 'Lending a Hand', 'Shattered Hand raiders do not simply kill, they take trophies from their victims. We know their next target. Stop them.', 'Talador', '27787', '36', '15', '1', '750', '20');
INSERT INTO `garrison_mission` VALUES ('210', '95', '0', '14', '3', '0', '5400', '86400', '105', '3', 'Flock Together', 'Arakkoa flocks are looting abandoned draenei homesteads. They\'re also killing any who try to stop them. That makes them our problem now.', 'Talador', '27788', '36', '15', '1', '900', '20');
INSERT INTO `garrison_mission` VALUES ('211', '96', '0', '19', '3', '0', '6750', '86400', '107', '3', 'Peace unto You', 'The distressed spirits of fallen Azeroth soldiers have begun to rise. Quickly, let us end their torment.', 'Spires of Arak', '27789', '36', '15', '1', '1050', '20');
INSERT INTO `garrison_mission` VALUES ('212', '97', '0', '15', '3', '0', '6750', '86400', '107', '3', 'Cat Scratch Fever', 'Some sadistic saberon think the chaos on Draenor means they can butcher innocents without fear of punishment. Disabuse them of that notion.', 'Spires of Arak', '27790', '36', '15', '1', '1200', '20');
INSERT INTO `garrison_mission` VALUES ('213', '98', '0', '11', '3', '0', '7200', '86400', '104', '3', 'Fired Up', 'Some important Burning Blade orcs wish to discuss battle strategy with Iron Horde leaders. It would be a shame if they never arrived.', 'Nagrand', '27791', '36', '15', '1', '1350', '20');
INSERT INTO `garrison_mission` VALUES ('214', '99', '0', '11', '3', '0', '7200', '86400', '104', '3', 'A Way Out', 'A group of defectors wishes to escape the Iron Horde! Let\'s clear the way for them.', 'Nagrand', '27792', '36', '15', '1', '1500', '20');
INSERT INTO `garrison_mission` VALUES ('215', '90', '0', '21', '1', '0', '21600', '86400', '101', '16', 'Where The Rock Flayers Went', 'Modern anthropologists had hoped Draenor would reveal the origin of the rock flayer.  They were disappointed.', 'Rock Flayer Island', '27781', '29', '10', '1', '500', '80');
INSERT INTO `garrison_mission` VALUES ('217', '90', '0', '26', '1', '0', '1800', '86400', '106', '35', 'Umbrafen Treasures', 'Scouts have heard rumors of valuables within a mysterious cave in the Umbrafen.', 'Shadowmoon Valley', '28282', '37', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('218', '90', '0', '23', '1', '0', '1800', '86400', '101', '35', 'Precious Riches', 'Rumors grow of a Pale orc that thrives on ambushing the unsuspecting for anything that shines.  He might be found in a cave to the southwest.', 'Frostfire Ridge', '28281', '37', '10', '0', '100', '65');
INSERT INTO `garrison_mission` VALUES ('219', '91', '0', '12', '1', '0', '3600', '86400', '101', '35', 'Borrok\'s Fools', 'The Grimfrost ogres regularly rob Frostwolf orcs and offer them as sacrifices to Brorrok the Devourer.  Let\'s relieve them of their stolen goods.', 'Frostfire Ridge', '28761', '37', '10', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('220', '91', '0', '26', '1', '0', '3600', '86400', '106', '35', 'The Hypnocroak', 'Legends speak of a cave above the Nightmarsh replete with valuable trade resources - along with an amphibious abomination residing in it.', 'Shadowmoon Valley', '28756', '37', '10', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('221', '91', '0', '23', '1', '0', '2700', '9999999', '101', '13', 'Aerial Rodents of Unusual Size', 'The frostwing screechers of the Southwind Cliffs are overpopulated.  If ignored, this will eventually disrupt mining operations.', 'Frostfire Ridge', '28763', '20', '10', '0', '200', '50');
INSERT INTO `garrison_mission` VALUES ('222', '91', '0', '15', '1', '0', '2700', '9999999', '106', '13', 'Warpstalker Predators', 'We\'ll need to regularly fend off warpstalkers in the Shimmering Mor to ensure a steady supply of fey dragons for transport purposes.', 'Shadowmoon Valley', '28759', '20', '10', '0', '200', '50');
INSERT INTO `garrison_mission` VALUES ('223', '92', '0', '22', '2', '0', '2700', '86400', '103', '13', 'Dessicus Maximus', 'The elementals of Gorgrond are regularly whipped into a frenzy by Dessicus of the Dead Pools.  Let\'s see what we can do about that.', 'Gorgrond', '28308', '20', '10', '0', '300', '50');
INSERT INTO `garrison_mission` VALUES ('224', '93', '0', '22', '2', '0', '2700', '86400', '103', '13', 'Primal Savages', 'The saberon continue to be a menace to our outpost in Gorgrond.  Let\'s show them why this world isn\'t named Sabronia.', 'Gorgrond', '28330', '20', '10', '0', '400', '50');
INSERT INTO `garrison_mission` VALUES ('228', '91', '0', '23', '1', '0', '2700', '9999999', '101', '13', 'Artillery Suppression', 'Iron Horde artillery units have positioned themselves on the ridge above the Shivering Trench.  This is a clear and present danger!', 'Frostfire Ridge', '28763', '20', '10', '0', '200', '50');
INSERT INTO `garrison_mission` VALUES ('229', '91', '0', '26', '1', '0', '2700', '9999999', '106', '13', 'Podlings of Unusual Size', 'Some say the podlings of Gloomshade Grove bow to the mad king of the east.  Find him, and end his reign of podling terror.', 'Shadowmoon Valley', '28759', '20', '10', '0', '200', '50');
INSERT INTO `garrison_mission` VALUES ('230', '94', '0', '15', '2', '0', '2700', '86400', '105', '13', 'The Kaliri Nuisance', 'A reduced kaliri population would be greatly appreciated by virtually every hero moving about the skies, for all time.', 'Talador', '28335', '20', '10', '0', '500', '50');
INSERT INTO `garrison_mission` VALUES ('231', '95', '0', '15', '2', '0', '2700', '86400', '105', '13', 'Hapless Heroes', 'Scouting reports indicate some substandard adventurers are trapped in Deathweb Hollow.  I suppose we should lend a hand.', 'Talador', '28336', '20', '10', '0', '600', '50');
INSERT INTO `garrison_mission` VALUES ('232', '96', '0', '21', '2', '0', '3600', '86400', '107', '13', 'Raiders of the Lost Shard', 'The arakkoa have enslaved more outcasts and have resumed their excavation efforts.  Another setback for them is in order.', 'Spires of Arak', '28337', '20', '10', '0', '700', '50');
INSERT INTO `garrison_mission` VALUES ('240', '90', '0', '21', '5', '0', '8', '9999999', '101', '35', 'Orcs in the Mountains!', 'Kill the orcs in the mountains.', 'Orc Mountain', '26539', '18', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('241', '90', '0', '21', '5', '0', '8', '9999999', '101', '16', 'Orcs in the Mountains!', 'Kill the orcs in the mountains.', 'Orc Mountain', '26539', '18', '0', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('242', '97', '0', '15', '2', '0', '5400', '86400', '107', '35', 'Honey I Got Your Money', 'Don\'t you worry.  Just make like a bear, get in, get the honey, and get out.', 'Spires of Arak', '28428', '37', '10', '0', '800', '50');
INSERT INTO `garrison_mission` VALUES ('243', '97', '0', '21', '2', '0', '3600', '86400', '107', '13', 'The Upper Hand', 'The Shattered Hand clan is positioned on the Shattered Rise and likely to strike again.  Handle it!', 'Spires of Arak', '28426', '20', '10', '0', '800', '50');
INSERT INTO `garrison_mission` VALUES ('244', '98', '0', '12', '2', '0', '3600', '86400', '104', '13', 'Gorging Ogres', 'The Highmaul ogres have an excavation underway at Stonecrag Gorge.  Get in there and extract information about their relics!', 'Nagrand', '28429', '20', '10', '0', '900', '50');
INSERT INTO `garrison_mission` VALUES ('245', '99', '0', '12', '2', '0', '3600', '86400', '104', '13', 'Lug\'dol\'s Thugs', 'Lug\'dol and his nascent brawler thugs are using our supply escorts for combat practice.  You know we can\'t allow this to continue.', 'Nagrand', '28432', '20', '10', '0', '1000', '50');
INSERT INTO `garrison_mission` VALUES ('247', '100', '615', '17', '3', '0', '28800', '129600', '103', '3', 'Pier Pressure', 'The Iron Horde have built a harbor in Gorgrond that they call the Iron Docks. Show our enemy how fragile their vaunted \"iron\" truly is.', 'Iron Docks', '28576', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('248', '100', '615', '14', '3', '0', '36000', '129600', '107', '3', 'A Bird in the Hand', 'A little bird informed me that the arakkoa of Skyreach have been preying on our forward scouts. Make sure they regret that decision.', 'Skyreach', '28576', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('249', '100', '615', '11', '3', '0', '28800', '129600', '164', '3', 'Return to Sender', 'Orcs from an alternate universe invaded our Blackrock Mountain. How ironic. Let\'s educate them on what happened to the previous residents.', 'Upper Blackrock Spire', '28576', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('250', '100', '615', '16', '3', '0', '28800', '129600', '105', '3', 'No Rest for the Noble', 'Using a mausoleum as a staging ground is a completely new level of disrespect. Let\'s allow the departed to rest with smiles on their faces.', 'Auchindoun', '28576', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('251', '100', '615', '12', '3', '0', '28800', '129600', '101', '3', 'Miner Difficulties', 'The Bloodmaul use slave labor to fuel their efforts. If we liberate the miners, we\'ll disrupt their operations. Textbook win-win situation.', 'Bloodmaul Slag Mines', '28576', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('252', '100', '615', '22', '3', '0', '28800', '129600', '103', '3', 'Off the Rails', 'Two words: Orcish engineering.  Even odds their rail system will take care of itself, but let\'s stack the odds in our favor.', 'Grimrail Depot', '28576', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('253', '100', '615', '20', '3', '0', '28800', '129600', '104', '3', 'Ziggy Sawdust X3-9', 'The Ziggy Sawdust X3-9 is the latest in clear-cutting technology. Unleash the adorable robot on the Everbloom and enjoy the show.', 'The Everbloom', '28576', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('254', '100', '615', '11', '3', '0', '28800', '129600', '106', '3', 'Two Feet in the Grave', 'How convenient of Ner\'zhul and his lackeys to flee into their clan\'s burial grounds. It will make choosing their graves much quicker.', 'Shadowmoon Burial Grounds', '28576', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('255', '100', '615', '17', '3', '0', '28800', '129600', '103', '3', 'All Aboard the Fail Boat', 'Our enemies boast that their massive ship at the Iron Docks cannot be sunk. This crate of shiny new seaforium charges begs to differ.', 'Iron Docks', '28576', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('256', '100', '615', '14', '3', '0', '28800', '129600', '107', '3', 'Shiny!', 'The dread ravens are gathering scraps from below Skyreach. They are building more of those constructs or nests. They must be stopped.', 'Skyreach', '28576', '30', '25', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('257', '100', '615', '21', '3', '0', '28800', '129600', '164', '3', 'Egg Them On!', 'The druids fear that the proto-drake populations are plummeting. Rescue their eggs from Warlord Zaela\'s army in Upper Blackrock Spire.', 'Upper Blackrock Spire', '28576', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('258', '100', '615', '16', '3', '0', '28800', '129600', '105', '3', 'Auchindoom!', 'Scouts report that strange figures are lurking near Auchindoun, disturbing the mausoleums. We must destroy anything with even a hint of fel.', 'Auchindoun', '28576', '30', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('259', '100', '615', '17', '3', '0', '14400', '129600', '101', '3', 'Fire and Fury', 'Rescuing slaves in the Bloodmaul Slag Mines is our priority, but to do this, we must thin the number of fire furies there.', 'Bloodmaul Slag Mines', '28576', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('260', '100', '615', '22', '3', '0', '14400', '129600', '103', '3', 'Sparks and Steel', 'Goblin engineers are designing weapons for Blackrock and Thunderlord orcs. Well, we have weapons too. Go to Grimrail Depot and use them.', 'Grimrail Depot', '28576', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('261', '100', '615', '26', '3', '0', '14400', '129600', '104', '3', 'Savage Patch', 'Some magi stopped to smell the roses in the Everbloom. Oops. If we hurry, we should be able to save some of them.', 'The Everbloom', '28576', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('262', '100', '615', '24', '3', '0', '14400', '129600', '106', '3', 'Under Ner\'zhul\'s Nose', 'Some of the Shadowmoon clan\'s treasures are poorly guarded. We\'ll take what we can and destroy the rest!', 'Shadowmoon Burial Grounds', '28576', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('263', '100', '615', '11', '3', '0', '28800', '129600', '103', '3', 'Listen All Y\'all', 'They won\'t stand it. They\'ll know we planned it. The Iron Horde will think we\'re a mirage, but it\'s just sabotage.', 'Iron Docks', '28576', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('264', '100', '615', '14', '3', '0', '28800', '129600', '107', '3', 'Clawing Skyward', 'Atop Skyreach, the high arakkoa are building mechanical constructs as weapons of war. Stop them, but beware their dread raven guards.', 'Skyreach', '28576', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('265', '100', '615', '21', '3', '0', '28800', '129600', '164', '3', 'Iron Horde\'s Blackrock', 'To siege the Iron Horde held Blackrock Spire, we must rid it of dragonkin and proto-dragons. Their mysterious keeper must also perish.', 'Upper Blackrock Spire', '28576', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('266', '100', '0', '24', '2', '0', '14400', '129600', '105', '3', 'No Rest for Auchindoun', 'The Shadow Council is raising warriors interred at Auchindoun to create draenei constructs. Stop them before they defile the place further.', 'Auchindoun', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('267', '100', '0', '12', '2', '0', '14400', '129600', '101', '3', 'Waylay the Warlord', 'The Bloodmaul have discovered a powerful artifact, and their warlord is overseeing the excavation. Ambush the warlord and take the artifact.', 'Bloodmaul Slag Mines', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('268', '100', '615', '22', '3', '0', '14400', '129600', '103', '3', 'Who\'s the Boss?', 'The Grimrail Depot owes its efficiency to goblin and orc overseers. Kill them, and you\'ll kill its ability to ship arms to distant camps.', 'Grimrail Depot', '28576', '30', '0', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('269', '100', '615', '20', '3', '0', '14400', '129600', '104', '3', 'Griefing with the Enemy', 'We dispatched a mage to deal with a hostile pack of genesaur, but now he\'s on their side. He must be infected. Time to send in the cavalry.', 'The Everbloom', '28576', '30', '0', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('270', '90', '0', '0', '1', '0', '7', '99999999', '101', '4', 'Frosticleave', 'Testing mission complete and all that.', 'Frostfire Ridge', '26539', '18', '0', '0', '0', '0');
INSERT INTO `garrison_mission` VALUES ('271', '100', '0', '12', '3', '0', '21600', '86400', '101', '13', 'Rescue on the Bloody Cliffs', 'The bloodmaul ogres of the Stonefury Cliffs are infamous as brutal slavemasters.  Show no quarter and free their slaves.', 'Frostfire Ridge', '31124', '32', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('272', '100', '0', '17', '3', '0', '21600', '86400', '103', '13', 'Ruinous Portents', 'If our troops are to become battle hardened, there would be no better test than the immense magnaron, Drov the Ruiner.', 'Gorgrond', '31124', '32', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('273', '100', '0', '26', '3', '0', '28800', '86400', '106', '13', 'Rise to Power', 'The Sargerai have taken control of Socrethar\'s Rise and are establishing countless demonic portals, we can\'t let this continue.', 'Shadowmoon Valley', '31124', '32', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('274', '100', '0', '16', '3', '0', '28800', '86400', '105', '13', 'An Eye for Terror', 'We need to shut down the legion gateways in operation within Shattrath.  Strike out at their vigilant watchful eyes.', 'Talador', '31124', '32', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('275', '100', '0', '22', '3', '0', '28800', '86400', '104', '13', 'Harboring Ill Will', 'The Iron Horde is running critical supply missions from the beach of Ironfist Harbor.  Three of our best should disrupt their operations.', 'Nagrand', '31124', '32', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('276', '100', '0', '11', '3', '0', '36000', '86400', '107', '13', 'Bladefist Hold', 'The Shattered Hand clan has established the ironically named Bladefist Hold.  Let\'s make a mockery of it.', 'Spires of Arak', '31124', '32', '25', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('277', '92', '0', '25', '1', '0', '3600', '86400', '103', '35', 'Mother Araneae', 'With great risk comes great reward.  Plumb the dangers of Darksting Cave in southern Gorgrond and see what you can find.', 'Gorgrond', '28739', '37', '10', '0', '300', '50');
INSERT INTO `garrison_mission` VALUES ('278', '93', '0', '25', '1', '0', '3600', '86400', '103', '35', 'Steamscar Plunder', 'The Steamscar of Gorgrond are notorious looters.  Let\'s repossess the valuables for great justice.', 'Gorgrond', '28740', '37', '10', '0', '400', '50');
INSERT INTO `garrison_mission` VALUES ('279', '94', '0', '24', '1', '0', '3600', '86400', '105', '35', 'The Tomb of Lights', 'Shadow Council pillagers continue to ravage the Auchenai catacombs of the Tomb of Lights.', 'Talador', '28741', '37', '10', '0', '500', '50');
INSERT INTO `garrison_mission` VALUES ('280', '95', '0', '24', '2', '0', '3600', '86400', '103', '35', 'Reagents of Mass Destruction', 'As it happens, the reagents used by the Shadow Council fetch a handy sum on the black market.  We\'re certain there\'s a stockpile in Gul\'rok.', 'Talador', '28742', '37', '10', '0', '600', '50');
INSERT INTO `garrison_mission` VALUES ('281', '97', '0', '21', '2', '0', '5400', '86400', '107', '35', 'Profiting From Profiteers', 'The goblins of Pinchwhistle *are* the noxious local environment.  I see no reason we shouldn\'t relieve them of some valuables.', 'Spires of Arak', '28743', '37', '10', '0', '700', '50');
INSERT INTO `garrison_mission` VALUES ('282', '98', '0', '15', '2', '0', '7200', '86400', '104', '35', 'Flint\'s Hide', 'Get it!', 'Nagrand', '28744', '37', '10', '0', '900', '50');
INSERT INTO `garrison_mission` VALUES ('283', '99', '0', '26', '2', '0', '7200', '86400', '104', '35', 'The Wolfmother\'s Pelts', 'The wolfmother Gar\'lua raises the most ferocious of Warsong wolves.  Their pelts would dent the Warsong war effort and fetch a tidy sum.', 'Nagrand', '28745', '37', '10', '0', '900', '50');
INSERT INTO `garrison_mission` VALUES ('284', '100', '0', '23', '2', '0', '21600', '86400', '101', '35', 'Ug\'lok the Incompetent', 'Even an ogre as incompetent as Ug\'lok the Frozen can amass a fortune via slave labor, but be prepared to counter his magic should it happen to work.', 'Frostfire Ridge', '28879', '33', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('285', '100', '0', '20', '2', '0', '21600', '86400', '106', '35', 'The One True Brambleking', 'Rumor has it that Brambleking Fili\'s staff points the way to a hidden cache.  Myth?', 'Shadowmoon Valley', '28879', '33', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('286', '100', '0', '11', '2', '0', '21600', '86400', '103', '35', 'Lost in the Foundry', 'The Iron Horde excavations taking place in The Pit occasionally turn up rare diamonds.  Retrieve one and we\'ll make use of it.', 'Gorgrond', '28879', '33', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('287', '100', '0', '22', '2', '0', '21600', '86400', '103', '35', 'Blackrock Munitions', 'Not only does the ammunition of the Iron Horde carry value on the black market, but lifting some means less for the Earthbreaker cannon.', 'Gorgrond', '28879', '33', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('288', '100', '0', '14', '3', '0', '21600', '86400', '107', '35', 'The Golden Halls of Skyreach', 'You and I both know the arakkoa are loaded.  Feels like we could make a fortune just chipping away at the walls.', 'Skyreach', '28879', '33', '25', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('289', '100', '0', '26', '2', '0', '21600', '86400', '106', '35', 'Profitable Machinations', 'A joint operation between the Venture Co. and Iron Horde is taking place on Darktide Roost.  Profit must be some sort of motive.', 'Shadowmoon Valley', '28879', '33', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('290', '100', '630', '17', '3', '0', '28800', '129600', '103', '3', 'Seas the Day', 'The Iron Docks have resumed operations despite our previous incursions. Let us show them how base their iron is against our mettle.', 'Iron Docks', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('291', '100', '630', '14', '3', '0', '28800', '129600', '107', '3', 'Murder Most Fowl', 'The denizens of Skyreach have stymied the war effort in arakkoan territory. Time to implement our new strategy: Operation Open Season.', 'Skyreach', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('292', '100', '630', '11', '3', '0', '28800', '129600', '164', '3', 'Eviction', 'The Iron Horde and its lackeys have \"energetically\" refused to vacate our own Blackrock Mountain. Go collect back rent from their corpses.', 'Upper Blackrock Spire', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('293', '100', '630', '11', '3', '0', '28800', '129600', '105', '3', 'A Grave Mistake', 'The Sargerai and their allies continue their desecration of Auchindoun. I recommend executing our \"no traitor left behind alive\" policy.', 'Auchindoun', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('294', '100', '630', '12', '3', '0', '36000', '129600', '101', '3', 'Labor Dispute', 'Ogres are bringing in slaves to shore up defenses in the mines. Free the slaves, and the area will be vulnerable to attack.', 'Bloodmaul Slag Mines', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('295', '100', '630', '11', '3', '0', '28800', '129600', '103', '3', 'Logistics Are For Professionals', 'The Blackrocks need an uninterrupted supply of metal to fuel production of their war machines. Let\'s make life difficult for them.', 'Grimrail Depot', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('296', '100', '630', '20', '3', '0', '36000', '129600', '104', '3', 'Rustle in the Bushes', 'The natives are restless. Again. The Kirin Tor have asked us to help them with some \"gardening.\"', 'The Everbloom', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('297', '100', '630', '24', '3', '0', '21600', '129600', '106', '3', 'To Rest Again', 'Our shaman have prepared a ritual to return souls summoned by the Shadowmoon to rest. Go and perform this ritual at the burial grounds.', 'Shadowmoon Burial Grounds', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('298', '100', '630', '22', '3', '0', '21600', '129600', '103', '3', 'Running Aground', 'The Iron Horde\'s flagship is at the Iron Docks. It\'s intimidating, but if it\'s steered into a shoal, it\'ll founder like any other boat.', 'Iron Docks', '28886', '38', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('299', '100', '630', '21', '3', '0', '28800', '129600', '107', '3', 'Is It a Bird? A Zeppelin?', 'Apexis devices are sun powered, but what if the sun is blocked? Zeppelins will work, and the goblins might help. Go, but pack airsick bags.', 'Skyreach', '28886', '38', '40', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('300', '100', '630', '21', '3', '0', '28800', '129600', '164', '3', 'Lava, Lava Everywhere', 'A fortress in a volcano is both ingenious and foolhardy. Impenetrable, yes, but a shaman can cause an eruption that will decimate it.', 'Upper Blackrock Spire', '28886', '38', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('301', '100', '630', '16', '3', '0', '28800', '129600', '105', '3', 'Clearing the Caches', 'Soulbinder Nyami has hidden caches of reagents for the Sargerai\'s summoning rituals all across Auchindoun. Find the caches and destroy them.', 'Auchindoun', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('302', '100', '630', '12', '3', '0', '21600', '129600', '101', '3', 'Shafted Miners', 'A mineshaft has collapsed, and those Bloodmaul dogs have left the enslaved miners down there to die. We must dig them out!', 'Bloodmaul Slag Mines', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('303', '100', '630', '11', '3', '0', '21600', '129600', '103', '3', 'Got a Light?', 'A train full of artillery and explosives is about to leave the depot. Detonating one bomb should be enough to destroy the entire payload.', 'Grimrail Depot', '28886', '38', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('304', '100', '630', '25', '3', '0', '28800', '129600', '104', '3', 'Spored to Death', 'The river we\'ve been using as our water source has been tainted by those nasty spores! Find a new source before people get thirsty.', 'The Everbloom', '28886', '38', '40', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('305', '100', '630', '19', '3', '0', '28800', '129600', '106', '3', 'Ghost Wranglers', 'The spirits of the burial grounds are restless. Seek their knowledge and their power to end Ner\'zhul\'s scheming once and for all.', 'Shadowmoon Burial Grounds', '28886', '38', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('306', '100', '630', '17', '3', '0', '28800', '129600', '103', '3', 'Rollin\' like a Kraken', 'With some diving gear and a little luck, a seaborne attack could be our best chance of infiltrating the heavily fortified Iron Docks.', 'Iron Docks', '28886', '38', '30', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('307', '100', '630', '14', '3', '0', '28800', '129600', '107', '3', 'Operation Skybreach', 'The arakkoa view us \"ground dwellers\" as nothing more than beasts. Let us see what they think after we lay waste to their lofty spires.', 'Skyreach', '28886', '38', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('308', '100', '630', '21', '3', '0', '28800', '129600', '164', '3', 'We Have a Mole', 'Getting into Blackrock Spire will not be easy, but we have a secret weapon: mole machines. The Black Irons will never know what hit them.', 'Upper Blackrock Spire', '28886', '38', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('309', '100', '630', '24', '3', '0', '21600', '129600', '105', '3', 'The Lost World', 'A portal in Auchindoun links Draenor to the demon world of Argus. Cut off the Legion\'s reinforcements: destroy every trace of the portal.', 'Auchindoun', '28886', '38', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('310', '100', '630', '18', '3', '0', '21600', '129600', '101', '3', 'Sixteen Tons', 'Slaves in the Slag Mines are ripe for revolt. Aid them. Then ask if they\'ve dug up any powerful relics lately. Try to be subtle.', 'Bloodmaul Slag Mines', '28886', '38', '20', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('311', '100', '630', '11', '3', '0', '21600', '129600', '103', '3', 'Can\'t Go Home This Way', 'Trains symbolize inevitability, but you\'ve never put much stock in fate. That\'s good: you\'ve got a huge infrastructure to shatter.', 'Grimrail Depot', '28886', '38', '0', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('312', '100', '630', '25', '3', '0', '21600', '129600', '104', '3', 'Magical Mystery Tour', 'The Kirin Tor who were sent to Gorgrond have gone silent. Turned or slain? Find out what\'s going on, and get them back on task if you can.', 'The Everbloom', '28886', '38', '0', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('313', '100', '645', '28', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28909', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('314', '100', '645', '17', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28909', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('315', '100', '645', '12', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28909', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('316', '100', '645', '29', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28909', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('317', '100', '645', '28', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28908', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('318', '100', '645', '17', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28908', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('319', '100', '645', '12', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28908', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('320', '100', '645', '29', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28908', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('321', '100', '645', '28', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28907', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('322', '100', '645', '17', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28907', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('323', '100', '645', '12', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28907', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('324', '100', '645', '29', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28907', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('325', '100', '645', '28', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28906', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('326', '100', '645', '17', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28906', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('327', '100', '645', '12', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28906', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('328', '100', '645', '29', '3', '0', '28800', '1209000', '104', '3', 'Highmaul Raid', 'The time for a critical assault on Highmaul is at hand.  To victory!', 'Highmaul', '28906', '41', '100', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('329', '90', '0', '27', '1', '0', '1800', '86400', '101', '12', 'The Dance Studio', 'Rumored not to exist, yet clearly baseless.', 'Northrend Academy of Dance', '27781', '29', '5', '1', '100', '100');
INSERT INTO `garrison_mission` VALUES ('330', '90', '0', '25', '1', '0', '21600', '86400', '103', '16', 'Finding Farahlon', 'The Netherstorm must have come from somewhere.  Get on a boat, sail northeast, and bring something good back while you\'re at it.', 'Farahlon', '27781', '29', '10', '1', '500', '80');
INSERT INTO `garrison_mission` VALUES ('331', '90', '0', '26', '1', '0', '1800', '86400', '105', '13', 'Tracking Griftah', 'Some shifty troll sold our troops a bunch of worthless junk.  Rough him up a bit and get a refund!', 'Talador', '27781', '29', '10', '1', '500', '50');
INSERT INTO `garrison_mission` VALUES ('332', '98', '0', '29', '1', '0', '3600', '86400', '104', '16', 'Mysteries of Lok-rath', 'The Warsong clan stationed in Lok-rath is rumored to stockpile some strange form of currency.  It may hold some historic significance.', 'Nagrand', '29008', '29', '10', '1', '900', '30');
INSERT INTO `garrison_mission` VALUES ('333', '90', '0', '28', '1', '0', '300', '86400', '106', '6', 'Clean the Latrine', 'It\'s best not to lose a bet with the janitor.', 'Outhouse', '29013', '29', '0', '0', '50', '100');
INSERT INTO `garrison_mission` VALUES ('334', '100', '0', '12', '3', '0', '36000', '252000', '104', '3', 'Mogor\'s Dilemma', 'Mogor the Ogre is having trouble with some gladiators, but slaughtering other ogres would look bad for him.  He\'ll pay us for the dirty work.', 'Nagrand', '27780', '31', '25', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('335', '100', '0', '15', '3', '0', '36000', '252000', '107', '3', 'The Bloodmane Scrolls', 'The ruthless war prides of the Bloodmane have made off with the Reveries of Sethek, scrolls sacred to the outcasts.', 'Spires of Arak', '27780', '31', '25', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('336', '100', '0', '16', '3', '0', '36000', '252000', '105', '3', 'The Liberation of Shattrath', 'The once-great city of Shattrath continues to be subjugated by the legion.  Cry havoc, and let slip the followers of war.', 'Talador', '27780', '31', '25', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('337', '100', '0', '0', '1', '0', '57600', '252000', '104', '12', 'The Ring of Trials', 'Sometimes, a follower\'s skills just aren\'t cutting the mustard.  The ring offers a rare opportunity to retrain.', 'Nagrand', '27780', '31', '25', '1', '1500', '100');
INSERT INTO `garrison_mission` VALUES ('338', '100', '0', '29', '3', '0', '28800', '86400', '104', '3', 'Domination of Nagrand', 'Slay our Alliance adversaries for the glory of the Horde!  Go forth to victory!', 'Nagrand', '29219', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('339', '100', '0', '23', '3', '0', '28800', '86400', '101', '3', 'Domination of Frostfire', 'Slay our Alliance adversaries for the glory of the Horde!  Go forth to victory!', 'Frostfire Ridge', '29219', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('340', '100', '0', '21', '3', '0', '28800', '86400', '107', '3', 'Domination of Spires of Arak', 'Slay our Alliance adversaries for the glory of the Horde!  Go forth to victory!', 'Spires of Arak', '29219', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('341', '100', '0', '26', '3', '0', '28800', '86400', '105', '3', 'Domination of Talador', 'Slay our Alliance adversaries for the glory of the Horde!  Go forth to victory!', 'Talador', '29219', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('342', '100', '0', '22', '3', '0', '28800', '86400', '103', '3', 'Domination of Gorgrond', 'Slay our Alliance adversaries for the glory of the Horde!  Go forth to victory!', 'Gorgrond', '29219', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('343', '100', '0', '29', '3', '0', '28800', '86400', '104', '3', 'Domination of Nagrand', 'Slay our Horde adversaries for great justice and for the Alliance!', 'Nagrand', '29220', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('344', '100', '0', '23', '3', '0', '28800', '86400', '101', '3', 'Domination of Frostfire', 'Slay our Horde adversaries for great justice and for the Alliance!', 'Frostfire Ridge', '29220', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('345', '100', '0', '21', '3', '0', '28800', '86400', '107', '3', 'Domination of Spires of Arak', 'Slay our Horde adversaries for great justice and for the Alliance!', 'Spires of Arak', '29220', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('346', '100', '0', '26', '3', '0', '28800', '86400', '105', '3', 'Domination of Talador', 'Slay our Horde adversaries for great justice and for the Alliance!', 'Talador', '29220', '30', '10', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('347', '100', '0', '22', '3', '0', '28800', '86400', '103', '3', 'Domination of Gorgrond', 'Slay our Horde adversaries for great justice and for the Alliance!', 'Gorgrond', '29220', '30', '0', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('348', '100', '0', '29', '3', '0', '28800', '86400', '104', '3', 'Conquest of Nagrand', 'Slay our Horde adversaries for great justice and for the Alliance!', 'Nagrand', '29220', '42', '10', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('349', '100', '0', '23', '3', '0', '28800', '86400', '101', '3', 'Conquest of Frostfire', 'Slay our Horde adversaries for great justice and for the Alliance!', 'Frostfire Ridge', '29220', '42', '10', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('350', '100', '0', '21', '3', '0', '28800', '86400', '107', '3', 'Conquest of Spires of Arak', 'Slay our Horde adversaries for great justice and for the Alliance!', 'Spires of Arak', '29220', '42', '10', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('351', '100', '0', '26', '3', '0', '28800', '86400', '105', '3', 'Conquest of Talador', 'Slay our Horde adversaries for great justice and for the Alliance!', 'Talador', '29220', '42', '10', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('352', '100', '0', '22', '3', '0', '28800', '86400', '103', '3', 'Conquest of Gorgrond', 'Slay our Horde adversaries for great justice and for the Alliance!', 'Gorgrond', '29220', '42', '0', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('353', '100', '0', '29', '3', '0', '28800', '86400', '104', '3', 'Conquest of Nagrand', 'Slay our Alliance adversaries for the glory of the Horde!  Go forth to victory!', 'Nagrand', '29219', '42', '10', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('354', '100', '0', '23', '3', '0', '28800', '86400', '101', '3', 'Conquest of Frostfire', 'Slay our Alliance adversaries for the glory of the Horde!  Go forth to victory!', 'Frostfire Ridge', '29219', '42', '10', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('355', '100', '0', '21', '3', '0', '28800', '86400', '107', '3', 'Conquest of Spires of Arak', 'Slay our Alliance adversaries for the glory of the Horde!  Go forth to victory!', 'Spires of Arak', '29219', '42', '10', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('356', '100', '0', '26', '3', '0', '28800', '86400', '105', '3', 'Conquest of Talador', 'Slay our Alliance adversaries for the glory of the Horde!  Go forth to victory!', 'Talador', '29219', '42', '10', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('357', '100', '0', '22', '3', '0', '28800', '86400', '103', '3', 'Conquest of Gorgrond', 'Slay our Alliance adversaries for the glory of the Horde!  Go forth to victory!', 'Gorgrond', '29219', '42', '10', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('358', '100', '0', '22', '3', '0', '36000', '252000', '103', '3', 'Drov the Ruiner', 'Gorgrond plays host to one of the most fearsome creatures in all Draenor.  Tremble before his might, but come back with some plunder afterwards.', 'Gorgrond', '27780', '31', '25', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('359', '100', '0', '21', '3', '0', '36000', '252000', '107', '3', 'Rukhmar', 'A manifestation of a Sun God, Rukhmar demands fealty and obedience.  He will not be defeated easily.', 'Spires of Arak', '27780', '31', '25', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('360', '100', '0', '12', '3', '0', '36000', '252000', '104', '3', 'Errand for the SPS', 'If you really want to join the Steamwheedle Preservation Society, you\'ve got to really care about Highmaul artifacts.', 'Nagrand', '27780', '31', '25', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('361', '100', '0', '25', '3', '0', '36000', '252000', '102', '3', 'Blingtron\'s Secret Vault', 'While we were occupied with the sabotage of the Dark Portal, Blingtron managed to build a vault of riches.  Find it!', 'Tanaan Jungle', '27780', '31', '25', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('362', '90', '0', '26', '1', '0', '10800', '86400', '106', '12', 'Hearthstone Tournament', 'We could use a master tactician around here.  Plus, we might get some autographs.', 'Garrison', '29013', '29', '0', '1', '400', '100');
INSERT INTO `garrison_mission` VALUES ('363', '90', '0', '23', '1', '0', '10800', '86400', '101', '12', 'Hearthstone Tournament', 'We could use a master tactician around here.  Plus, we might get some autographs.', 'Garrison', '29014', '29', '0', '1', '400', '100');
INSERT INTO `garrison_mission` VALUES ('364', '90', '0', '26', '1', '0', '14400', '86400', '105', '10', 'The Burning Crusade', 'Rumor has it there\'s a chili cook-off soon, and frankly I\'m tired of eating the same gruel every day.  Send someone to get whatever wins.', 'Talador', '27781', '29', '10', '1', '200', '100');
INSERT INTO `garrison_mission` VALUES ('365', '90', '0', '21', '3', '0', '28800', '86400', '164', '12', 'Path of the Titans', 'Behold!  Endless customization awaits in this system within a system within a system.', 'Cataclysm Zone', '27781', '29', '25', '1', '400', '50');
INSERT INTO `garrison_mission` VALUES ('366', '90', '0', '15', '3', '0', '36000', '57600', '101', '3', 'Tusks Like Icicles', 'Coldtusk the boar continues to roam the shivering trench, and clearly we can\'t rely on the ravagers to take care of him.', 'Frostfire Ridge', '28569', '43', '20', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('367', '91', '0', '18', '3', '0', '36000', '57600', '101', '3', 'Fury of the Southwind Cliffs', 'The furies that roam the Southwind Cliffs have been known to leave their dead frozen in place.  Perhaps there are things worth retrieving.', 'Frostfire Ridge', '28753', '43', '20', '0', '400', '65');
INSERT INTO `garrison_mission` VALUES ('368', '90', '0', '20', '3', '0', '36000', '57600', '106', '3', 'The Umbral Giant', 'The marshes of the Umbrafen are the stomping ground of Ba\'ruun, a danger to all that need enter the fen.', 'Shadowmoon Valley', '28568', '43', '20', '0', '200', '65');
INSERT INTO `garrison_mission` VALUES ('369', '91', '0', '20', '3', '0', '36000', '57600', '106', '3', 'The Hills of Talbuk Past', 'Rumors speak of a ghostly talbuk that haunts the southern reaches of Shadowmoon Valley, we should investigate.', 'Shadowmoon Valley', '28752', '43', '20', '0', '400', '65');
INSERT INTO `garrison_mission` VALUES ('370', '92', '0', '17', '3', '0', '36000', '57600', '103', '3', 'Rolkor', 'Comb the northern wastelands of Gorgond for Rolkor.  Find him and defeat him for the glory of the garrison.', 'Shadowmoon Valley', '27785', '43', '20', '0', '600', '50');
INSERT INTO `garrison_mission` VALUES ('371', '93', '0', '21', '3', '0', '36000', '57600', '103', '3', 'Breaker of the Blue Flame', 'Our scouts speak of a cavern in northern Gorgrond in which a strange ogron named Gelgor of the Blue Flame lies.', 'Gorgrond', '27786', '43', '20', '0', '800', '50');
INSERT INTO `garrison_mission` VALUES ('372', '94', '0', '26', '3', '0', '36000', '57600', '105', '3', 'The Mother of All Kaliri', 'Hen-Mother Hami.  She who breeds the swooping terrors of the sky.  Find her, subdue her, and restore freedom to the skies.', 'Talador', '27787', '43', '20', '0', '1000', '50');
INSERT INTO `garrison_mission` VALUES ('373', '95', '0', '11', '3', '0', '36000', '57600', '105', '3', 'Steeltusk Lives', 'The Iron Horde have equipped a particularly violent elekk named Steeltusk.  Let\'s make sure this doesn\'t become more common.', 'Talador', '27788', '43', '20', '0', '1200', '50');
INSERT INTO `garrison_mission` VALUES ('374', '96', '0', '20', '3', '0', '36000', '57600', '107', '3', 'Murkbog Terror', 'Scouts report a many-headed beast named Festerbloom waylaying travelers crossing the Murkbog.  Clear the path for everyone\'s sake.', 'Spires of Arak', '27789', '43', '20', '0', '1400', '50');
INSERT INTO `garrison_mission` VALUES ('375', '97', '0', '26', '3', '0', '36000', '57600', '107', '3', 'Ancient of Corruption', 'Rumors swirl of an ancient gone awry in the undergrowth.  Track down this \"Shadowbark\" and take care of business.', 'Spires of Arak', '27790', '43', '20', '0', '1600', '50');
INSERT INTO `garrison_mission` VALUES ('376', '98', '0', '12', '3', '0', '36000', '57600', '104', '3', 'The Gorian Warmaster', 'We\'ve isolated the murderous Warmaster Blugthol to a tower within Mar\'gok\'s Overwatch.  It\'s time to take him out.', 'Nagrand', '27791', '43', '20', '0', '1800', '50');
INSERT INTO `garrison_mission` VALUES ('377', '99', '0', '29', '3', '0', '36000', '57600', '104', '3', 'Heart of the Sabermaw', 'Just north of Grommashar dwell the Sabermaw clan, notorious outpost raiders.  Strike out at their leader, Soulfang, and crush their morale.', 'Nagrand', '27792', '43', '20', '0', '2000', '50');
INSERT INTO `garrison_mission` VALUES ('378', '90', '0', '21', '3', '0', '3600', '86400', '103', '3', 'Boom Is the Word', 'A very large Iron Horde fuel-storage tank sits alongside a rail bridge to the north. Crack the tank open, add some dynamite.  BOOM!', 'Gorgrond', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('379', '90', '0', '21', '3', '0', '3600', '86400', '107', '3', 'Too Much Business', 'We suspect a goblin merchant of selling to both us and the Iron Horde. Tail him, and if he is double-dealing, drag him back by his ears.', 'Spires of Arak', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('380', '100', '0', '29', '2', '0', '14400', '129600', '104', '3', 'Raiding the Raiders', 'Iron Horde raiders have descended on nearby draenei villages. Find the raiders\' camp and raid it. Turnabout, they say, is fair play.', 'Nagrand', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('381', '90', '0', '24', '3', '0', '3600', '86400', '105', '3', 'What\'s Mine Is a Mine', 'Word has it that an Iron Horde mine near our outpost has hit a rich vein. We can\'t let them have nice things. Storm in and claim the mine.', 'Talador', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('382', '90', '0', '11', '3', '0', '3600', '86400', '105', '3', 'Iron Horde Chef', 'Iron Horde cuisine is revolting, but the refugees need food. Find orc camps and pilfer what rations they have. Mints might be prudent too.', 'Talador', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('383', '90', '0', '11', '3', '0', '3600', '86400', '101', '3', 'Thunderlords, Ho!', 'Rumor has it that the Thunderlord clan is using strange \"blades of omen\" that can change size. Destroy these weapons while we can.', 'Frostfire Ridge', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('384', '90', '0', '11', '3', '0', '3600', '86400', '104', '3', 'Warchief\'s Pride', 'Grommash has a legendary ego and temper. Hit the Warsong hard and he\'ll be consumed with the need for vengeance. We want him distracted.', 'Nagrand', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('385', '100', '0', '11', '2', '0', '14400', '129600', '103', '3', 'Hefty Metal', 'Blackrock scouts have discovered a vast trove of metal ore perfect for making weapons. Make sure they never report their findings.', 'Gorgrond', '27780', '30', '15', '0', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('386', '90', '0', '11', '3', '0', '3600', '86400', '106', '3', 'The Dark Night Rises', 'Curiously, our scouts say a Shadowmoon encampment is always vulnerable at night. You\'d think they\'d be more used to the dark. Oh well.', 'Shadowmoon Valley', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('387', '90', '0', '11', '3', '0', '3600', '86400', '107', '3', 'Shattered Land', 'Some Shattered Hand raiders have scattered, but though they look haggard, they may soon find new victims to splatter.', 'Spires of Arak', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('388', '90', '0', '12', '3', '0', '3600', '86400', '104', '3', 'Ogre-Stepping Boundaries', 'Those Highmaul ogres are annexing more land than we can afford to lose. We\'re green-lighting a terminal solution to this problem: you.', 'Nagrand', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('389', '90', '0', '23', '3', '0', '3600', '86400', '101', '3', 'It\'ll Be Ogre Soon', 'Bloodmaul ogres enslave others and then rely on their forced labor. Go inspire the downtrodden by kicking the fight out of their oppressors.', 'Frostfire Ridge', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('390', '90', '0', '12', '3', '0', '3600', '86400', '101', '3', 'Ogre Confident', 'The Bladespire clan has concocted a \"foolproof\" plan of defense. Make it backfire by killing their elite and leaving their \"fools\" alive.', 'Frostfire Ridge', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('391', '90', '0', '22', '3', '0', '3600', '86400', '107', '3', 'Spring Preening', 'We cannot allow the arakkoa to acquire more of their deadly Apexis technology. Raid their excavation sites. Leave nothing but dust behind!', 'Spires of Arak', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('392', '90', '0', '14', '3', '0', '3600', '86400', '105', '3', 'Pluck Them All', 'Our alchemists have found uses for the oils secreted by arakkoa feathers. Gather as many as you can - by any means necessary.', 'Talador', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('393', '90', '0', '14', '3', '0', '3600', '86400', '107', '3', 'Mean Screech', 'We have crafted a device to mimic an arakkoa\'s cries for help. Deploy it in the Spires of Arak to draw our feathery foes into an ambush.', 'Spires of Arak', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('394', '90', '0', '25', '3', '0', '3600', '86400', '103', '3', 'Mulch Ado about Nothing', 'The podlings have been raiding the graves of our allies to feed one of their genesaur. Recover the bodies. If they want mulch, mulch them!', 'Gorgrond', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('395', '90', '0', '15', '3', '0', '3600', '86400', '103', '3', 'Lunch Breakers', 'We need to get the gronn riled again. Kill the local meat-based wildlife and see if starvation turns them against the Iron Horde nearby.', 'Gorgrond', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('396', '90', '0', '20', '3', '0', '3600', '86400', '106', '3', 'Night of the Primals', 'Our scouts report that the botani are snatching workers from the quarries. Kill the primals and find out what they are using the orcs for.', 'Shadowmoon Valley', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('397', '90', '0', '26', '3', '0', '3600', '86400', '106', '3', 'Green Fel', 'The reckless warlock Gul\'dan intends to create a fel volcano in the center of Shadowmoon Valley. Stop him before he dooms us all.', 'Shadowmoon Valley', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('398', '90', '0', '11', '3', '0', '3600', '86400', '105', '3', 'Highway to Fel', 'Shadow Council agents plan to cut a path through Tanaan and pave it with their enemies\' bones. Such an atrocity must not come to pass.', 'Talador', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('399', '90', '0', '16', '3', '0', '3600', '86400', '105', '3', 'Felraiser', 'Warlocks in the northwest are draining life from the land, preparing to summon a demon lord. Well? Don\'t just stand there!', 'Talador', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('400', '90', '0', '21', '3', '0', '3600', '86400', '104', '3', 'Beyond the Pale', 'Pale orcs stole a powerful trinket and fled to the mountains. Now they vow to kill anyone who comes near \"the precious.\" Challenge accepted!', 'Nagrand', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('401', '90', '0', '18', '3', '0', '3600', '86400', '104', '3', 'Bucket Brigade', 'Massive flooding has cut off our supply lines, and the water elementals responsible show no signs of stopping. Let\'s hang them out to dry.', 'Nagrand', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('402', '90', '0', '15', '3', '0', '3600', '86400', '104', '3', 'Deathhoof', 'A doomsday weapon has been enchanted to look like an adorable baby clefthoof. Clefthoof violently defend their young. Proceed with caution.', 'Nagrand', '29928', '44', '10', '0', '100', '0');
INSERT INTO `garrison_mission` VALUES ('403', '100', '0', '27', '3', '0', '86000', '1209000', '104', '3', 'Rock the Boat', 'Disrupt tense negotiations between the Iron Horde and the Highmail Ogres to acquire an Abrogator Stone for Khadgar.', 'Highmaul Harbor', '30819', '45', '50', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('404', '100', '0', '12', '3', '0', '86000', '1209000', '101', '3', 'He Keeps it Where?', 'A powerful Bloodmaul magma-shaper is using an Abrogator Stone as a piece of navel jewelry. Don\'t tell Khadgar where we got it.', 'Bloodmaul Stronghold', '30819', '45', '50', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('405', '100', '0', '20', '3', '0', '86000', '1209000', '102', '3', 'Lost in the Weeds', 'A failed Stonemaul assault on the Overlook Ruins has left ogre corpses everywhere. Search them for an Abrogator stone!', 'Overlook Ruins', '30819', '45', '50', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('406', '100', '0', '27', '3', '0', '86000', '1209000', '104', '3', 'It\'s Rigged!', 'Ogres are wagering an Abrogator Stone on an upcoming Ring of Blood gladiatorial contest. Send in the ringers.', 'The Ring of Blood', '30819', '45', '50', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('407', '100', '0', '13', '3', '0', '86000', '1209000', '105', '3', 'Tower of Terror', 'Scouts have discovered a ruined ogre guard tower in Talador. It\'s haunted. Also, we\'re running out of scouts.', 'The Haunted Precipice', '30819', '45', '50', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('408', '100', '0', '11', '3', '0', '86000', '1209000', '103', '3', 'The Pits', 'Good news! We\'ve located some Elemental Runes that Khadgar is looking for. Bad News: They\'re in The Pits.', 'Blackrock Foundry Grounds', '30820', '46', '75', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('409', '100', '0', '22', '3', '0', '86000', '1209000', '103', '3', 'The Great Train Robbery', 'The Iron Horde is moving an Elemental Rune via train to the Blackrock Foundry. It\'s ambush time.', 'Gorgrond', '30820', '46', '75', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('410', '100', '0', '16', '3', '0', '86000', '1209000', '105', '3', 'A Rune with a View', 'In the chaos of the battle of Shattrath, the Sargeri absconded with some of the Elemental Runes that Khadgar is looking for.', 'Sha\'tari Skymesa', '30820', '46', '75', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('411', '100', '0', '29', '3', '0', '86000', '1209000', '104', '3', 'Rocks Fall. Everyone Dies.', 'The Iron Horde have subjugated a stone fury in Ironfist Harbor. It\'s got the Elemental Runes Khadgar seeks.', 'Ironfist Harbor', '30820', '46', '75', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('412', '100', '0', '24', '3', '0', '86000', '1209000', '107', '3', 'Beyond the Pale', 'The Bladefist clan are capturing Pale prisoners. We suspect they\'re looking for the same Runes as Khadgar.', 'Bladefist Hold', '30820', '46', '75', '1', '1500', '0');
INSERT INTO `garrison_mission` VALUES ('413', '100', '0', '27', '3', '0', '86000', '1209000', '101', '3', 'Pumping Iron', 'Elemental Runes have been smuggled to the Iron Siegeworks. Practically a suicide mission. Send followers you don\'t like.', 'Iron Siegeworks', '30820', '46', '75', '1', '1500', '0');

-- ----------------------------
-- Table structure for garrison_plot
-- ----------------------------
DROP TABLE IF EXISTS `garrison_plot`;
CREATE TABLE `garrison_plot` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `PlotSize` int(11) NOT NULL DEFAULT '0',
  `PlotId` int(11) NOT NULL DEFAULT '0',
  `Data0` int(11) NOT NULL DEFAULT '0',
  `PlotName` text NOT NULL,
  `Data1` int(11) NOT NULL DEFAULT '0',
  `Data2` int(11) NOT NULL DEFAULT '0',
  `Data3` int(11) NOT NULL DEFAULT '0',
  `Data4` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Export of GarrPlot.db2';

-- ----------------------------
-- Records of garrison_plot
-- ----------------------------
INSERT INTO `garrison_plot` VALUES ('12', '3', '0', '0', 'Small Plots', '2', '3', '233957', '233958');
INSERT INTO `garrison_plot` VALUES ('13', '4', '1', '0', 'Medium Plots', '1', '2', '232409', '232373');
INSERT INTO `garrison_plot` VALUES ('14', '5', '2', '0', 'Large Plots', '1', '2', '232411', '232410');
INSERT INTO `garrison_plot` VALUES ('28', '4', '4', '0', 'Mine', '0', '0', '239085', '239086');
INSERT INTO `garrison_plot` VALUES ('29', '4', '3', '0', 'Farm', '0', '0', '0', '0');
INSERT INTO `garrison_plot` VALUES ('31', '3', '5', '0', 'Fishing Hut', '0', '0', '0', '0');
INSERT INTO `garrison_plot` VALUES ('42', '3', '6', '0', 'Pet Menagerie', '0', '0', '0', '0');
