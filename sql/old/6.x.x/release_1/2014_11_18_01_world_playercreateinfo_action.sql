DELETE FROM `playercreateinfo_action` WHERE (`race`=2 AND `class`=3) OR (`race`=2 AND `class`=1) OR (`race`=2 AND `class`=7) OR (`race`=2 AND `class`=4) OR (`race`=2 AND `class`=8) OR (`race`=2 AND `class`=10) OR (`race`=2 AND `class`=9) OR (`race`=9 AND `class`=9) OR (`race`=9 AND `class`=8) OR (`race`=9 AND `class`=1) OR (`race`=9 AND `class`=3) OR (`race`=9 AND `class`=5) OR (`race`=9 AND `class`=4) OR (`race`=9 AND `class`=7) OR (`race`=11 AND `class`=10) OR (`race`=11 AND `class`=8) OR (`race`=11 AND `class`=3) OR (`race`=11 AND `class`=7) OR (`race`=11 AND `class`=5) OR (`race`=10 AND `class`=10) OR (`race`=10 AND `class`=8) OR (`race`=10 AND `class`=9) OR (`race`=10 AND `class`=2) OR (`race`=10 AND `class`=3) OR (`race`=10 AND `class`=1) OR (`race`=10 AND `class`=4) OR (`race`=10 AND `class`=5) OR (`race`=7 AND `class`=5) OR (`race`=7 AND `class`=4) OR (`race`=7 AND `class`=1) OR (`race`=4 AND `class`=10) OR (`race`=4 AND `class`=11) OR (`race`=7 AND `class`=9) OR (`race`=7 AND `class`=8) OR (`race`=3 AND `class`=2) OR (`race`=3 AND `class`=1) OR (`race`=1 AND `class`=1) OR (`race`=1 AND `class`=3) OR (`race`=1 AND `class`=2) OR (`race`=1 AND `class`=5) OR (`race`=1 AND `class`=4) OR (`race`=1 AND `class`=9) OR (`race`=1 AND `class`=8) OR (`race`=1 AND `class`=10) OR (`race`=11 AND `class`=2) OR (`race`=7 AND `class`=10) OR (`race`=11 AND `class`=1) OR (`race`=4 AND `class`=3) OR (`race`=4 AND `class`=8) OR (`race`=3 AND `class`=3) OR (`race`=3 AND `class`=7) OR (`race`=3 AND `class`=5) OR (`race`=3 AND `class`=4) OR (`race`=3 AND `class`=10) OR (`race`=3 AND `class`=9) OR (`race`=3 AND `class`=8) OR (`race`=4 AND `class`=4) OR (`race`=4 AND `class`=5) OR (`race`=4 AND `class`=1) OR (`race`=22 AND `class`=4) OR (`race`=22 AND `class`=5) OR (`race`=22 AND `class`=3) OR (`race`=22 AND `class`=1) OR (`race`=22 AND `class`=11) OR (`race`=22 AND `class`=8) OR (`race`=22 AND `class`=9) OR (`race`=5 AND `class`=5) OR (`race`=5 AND `class`=4) OR (`race`=5 AND `class`=1) OR (`race`=5 AND `class`=3) OR (`race`=5 AND `class`=9) OR (`race`=5 AND `class`=8) OR (`race`=5 AND `class`=10) OR (`race`=8 AND `class`=8) OR (`race`=8 AND `class`=9) OR (`race`=8 AND `class`=10) OR (`race`=8 AND `class`=11) OR (`race`=8 AND `class`=1) OR (`race`=8 AND `class`=3) OR (`race`=8 AND `class`=4) OR (`race`=8 AND `class`=5) OR (`race`=8 AND `class`=7) OR (`race`=6 AND `class`=7) OR (`race`=6 AND `class`=5) OR (`race`=6 AND `class`=2) OR (`race`=6 AND `class`=3) OR (`race`=6 AND `class`=1) OR (`race`=6 AND `class`=10) OR (`race`=6 AND `class`=11) OR (`race`=24 AND `class`=8) OR (`race`=24 AND `class`=10) OR (`race`=24 AND `class`=1) OR (`race`=24 AND `class`=3) OR (`race`=24 AND `class`=4) OR (`race`=24 AND `class`=5) OR (`race`=24 AND `class`=7) OR (`race`=6 AND `class`=6) OR (`race`=2 AND `class`=6) OR (`race`=22 AND `class`=6) OR (`race`=5 AND `class`=6) OR (`race`=10 AND `class`=6) OR (`race`=8 AND `class`=6) OR (`race`=9 AND `class`=6) OR (`race`=11 AND `class`=6) OR  (`race`=4 AND `class`=6) OR (`race`=7 AND `class`=6) OR (`race`=3 AND `class`=6) OR (`race`=1 AND `class`=6);
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
-- Orc - Hunter
(2, 3, 0, 3044, 0), -- Arcane Shot
(2, 3, 9, 20572, 0), -- Blood Fury
(2, 3, 10, 9, 48),
(2, 3, 11, 982, 0), -- Revive Pet
-- Orc - Warrior
(2, 1, 0, 88163, 0), -- Attack
(2, 1, 1, 78, 0), -- Heroic Strike
(2, 1, 9, 20572, 0), -- Blood Fury
(2, 1, 72, 88163, 0), -- Attack
(2, 1, 73, 78, 0), -- Heroic Strike
(2, 1, 81, 20572, 0), -- Blood Fury
-- Orc - Shaman
(2, 7, 0, 403, 0), -- Lightning Bolt
(2, 7, 9, 33697, 0), -- Blood Fury
(2, 7, 72, 403, 0), -- Lightning Bolt
-- Orc - Rogue
(2, 4, 0, 1752, 0), -- Sinister Strike
(2, 4, 9, 20572, 0), -- Blood Fury
-- Orc - Mage
(2, 8, 0, 44614, 0), -- Frostfire Bolt
(2, 8, 9, 33702, 0), -- Blood Fury
-- Orc - Monk
(2, 10, 0, 100780, 0), -- Jab
(2, 10, 72, 100780, 0), -- Jab
(2, 10, 81, 33697, 0), -- Blood Fury
-- Orc - Warlock
(2, 9, 0, 686, 0), -- Shadow Bolt
(2, 9, 9, 33702, 0), -- Blood Fury
(2, 9, 10, 10, 48),
-- Goblin - Warlock
(9, 9, 0, 686, 0), -- Shadow Bolt
(9, 9, 9, 69070, 0), -- Rocket Jump
(9, 9, 10, 69041, 0), -- Rocket Barrage
(9, 9, 11, 10, 48),
-- Goblin - Mage
(9, 8, 0, 44614, 0), -- Frostfire Bolt
(9, 8, 9, 69070, 0), -- Rocket Jump
(9, 8, 10, 69041, 0), -- Rocket Barrage
-- Goblin - Warrior
(9, 1, 0, 88163, 0), -- Attack
(9, 1, 1, 78, 0), -- Heroic Strike
(9, 1, 9, 69070, 0), -- Rocket Jump
(9, 1, 10, 69041, 0), -- Rocket Barrage
(9, 1, 72, 88163, 0), -- Attack
(9, 1, 73, 78, 0), -- Heroic Strike
(9, 1, 81, 69070, 0), -- Rocket Jump
(9, 1, 82, 69041, 0), -- Rocket Barrage
-- Goblin - Hunter
(9, 3, 0, 3044, 0), -- Arcane Shot
(9, 3, 8, 982, 0), -- Revive Pet
(9, 3, 9, 69070, 0), -- Rocket Jump
(9, 3, 10, 69041, 0), -- Rocket Barrage
(9, 3, 11, 9, 48),
-- Goblin - Priest
(9, 5, 0, 585, 0), -- Smite
(9, 5, 9, 69070, 0), -- Rocket Jump
(9, 5, 10, 69041, 0), -- Rocket Barrage
-- Goblin - Rogue
(9, 4, 0, 1752, 0), -- Sinister Strike
(9, 4, 9, 69070, 0), -- Rocket Jump
(9, 4, 10, 69041, 0), -- Rocket Barrage
-- Goblin - Shaman
(9, 7, 0, 403, 0), -- Lightning Bolt
(9, 7, 9, 69070, 0), -- Rocket Jump
(9, 7, 10, 69041, 0), -- Rocket Barrage
(9, 7, 72, 403, 0), -- Lightning Bolt
-- Draenei - Monk
(11, 10, 0, 100780, 0), -- Jab
(11, 10, 72, 100780, 0), -- Jab
(11, 10, 81, 121093, 0), -- Gift of the Naaru
-- Draenei - Mage
(11, 8, 0, 44614, 0), -- Frostfire Bolt
(11, 8, 9, 59548, 0), -- Gift of the Naaru
(11, 8, 83, 4540, 0), -- Holy Resist
-- Draenei - Hunter
(11, 3, 0, 3044, 0), -- Arcane Shot
(11, 3, 9, 59543, 0), -- Gift of the Naaru
(11, 3, 10, 9, 48),
(11, 3, 11, 982, 0), -- Revive Pet
(11, 3, 72, 6603, 0), -- Auto Attack
(11, 3, 74, 75, 0), -- Auto Shot
(11, 3, 82, 159, 48),
(11, 3, 83, 4540, 0), -- Holy Resist
-- Draenei - Shaman
(11, 7, 0, 403, 0), -- Lightning Bolt
(11, 7, 9, 59547, 0), -- Gift of the Naaru
-- Draenei - Priest
(11, 5, 0, 585, 0), -- Smite
(11, 5, 9, 59544, 0), -- Gift of the Naaru
(11, 5, 83, 4540, 0), -- Holy Resist
-- BloodElf - Monk
(10, 10, 0, 100780, 0), -- Jab
(10, 10, 9, 129597, 0), -- Arcane Torrent
(10, 10, 72, 100780, 0), -- Jab
(10, 10, 81, 129597, 0), -- Arcane Torrent
-- BloodElf - Mage
(10, 8, 0, 44614, 0), -- Frostfire Bolt
(10, 8, 9, 28730, 0), -- Arcane Torrent
-- BloodElf - Warlock
(10, 9, 0, 686, 0), -- Shadow Bolt
(10, 9, 9, 28730, 0), -- Arcane Torrent
(10, 9, 10, 10, 48),
-- BloodElf - Paladin
(10, 2, 0, 35395, 0), -- Crusader Strike
(10, 2, 9, 155145, 0), -- Arcane Torrent
-- BloodElf - Hunter
(10, 3, 0, 3044, 0), -- Arcane Shot
(10, 3, 9, 80483, 0), -- Arcane Torrent
(10, 3, 10, 9, 48),
(10, 3, 11, 982, 0), -- Revive Pet
-- BloodElf - Warrior
(10, 1, 0, 88163, 0), -- Attack
(10, 1, 1, 78, 0), -- Heroic Strike
(10, 1, 9, 69179, 0), -- Arcane Torrent
(10, 1, 72, 88163, 0), -- Attack
(10, 1, 73, 78, 0), -- Heroic Strike
(10, 1, 81, 69179, 0), -- Arcane Torrent
-- BloodElf - Rogue
(10, 4, 0, 1752, 0), -- Sinister Strike
(10, 4, 9, 25046, 0), -- Arcane Torrent
-- BloodElf - Priest
(10, 5, 0, 585, 0), -- Smite
(10, 5, 9, 28730, 0), -- Arcane Torrent
-- Gnome - Priest
(7, 5, 0, 585, 0), -- Smite
-- Gnome - Rogue
(7, 4, 0, 1752, 0), -- Sinister Strike
-- Gnome - Warrior
(7, 1, 0, 88163, 0), -- Attack
(7, 1, 1, 78, 0), -- Heroic Strike
(7, 1, 72, 88163, 0), -- Attack
(7, 1, 73, 78, 0), -- Heroic Strike
(7, 1, 81, 20589, 0), -- Escape Artist
-- NightElf - Monk
(4, 10, 0, 100780, 0), -- Jab
(4, 10, 72, 100780, 0), -- Jab
(4, 10, 81, 58984, 0), -- Shadowmeld
-- NightElf - Druid
(4, 11, 0, 5176, 0), -- Wrath
(4, 11, 9, 58984, 0), -- Shadowmeld
(4, 11, 72, 6603, 0), -- Auto Attack
(4, 11, 74, 58984, 0), -- Shadowmeld
(4, 11, 84, 6603, 0), -- Auto Attack
(4, 11, 96, 6603, 0), -- Auto Attack
(4, 11, 108, 6603, 0), -- Auto Attack
-- Gnome - Warlock
(7, 9, 0, 686, 0), -- Shadow Bolt
(7, 9, 10, 10, 48),
-- Gnome - Mage
(7, 8, 0, 44614, 0), -- Frostfire Bolt
-- Dwarf - Paladin
(3, 2, 0, 35395, 0), -- Crusader Strike
(3, 2, 9, 20594, 0), -- Stoneform
-- Dwarf - Warrior
(3, 1, 0, 88163, 0), -- Attack
(3, 1, 1, 78, 0), -- Heroic Strike
(3, 1, 9, 20594, 0), -- Stoneform
(3, 1, 72, 88163, 0), -- Attack
(3, 1, 73, 78, 0), -- Heroic Strike
(3, 1, 81, 20594, 0), -- Stoneform
-- Human - Warrior
(1, 1, 0, 88163, 0), -- Attack
(1, 1, 1, 78, 0), -- Heroic Strike
(1, 1, 9, 59752, 0), -- Every Man for Himself
(1, 1, 72, 88163, 0), -- Attack
(1, 1, 73, 78, 0), -- Heroic Strike
(1, 1, 81, 59752, 0), -- Every Man for Himself
-- Human - Hunter
(1, 3, 0, 3044, 0), -- Arcane Shot
(1, 3, 9, 59752, 0), -- Every Man for Himself
(1, 3, 10, 9, 48),
(1, 3, 11, 982, 0), -- Revive Pet
-- Human - Paladin
(1, 2, 0, 35395, 0), -- Crusader Strike
(1, 2, 9, 59752, 0), -- Every Man for Himself
-- Human - Priest
(1, 5, 0, 585, 0), -- Smite
(1, 5, 9, 59752, 0), -- Every Man for Himself
-- Human - Rogue
(1, 4, 0, 1752, 0), -- Sinister Strike
(1, 4, 9, 59752, 0), -- Every Man for Himself
-- Human - Warlock
(1, 9, 0, 686, 0), -- Shadow Bolt
(1, 9, 9, 59752, 0), -- Every Man for Himself
(1, 9, 10, 10, 48),
-- Human - Mage
(1, 8, 0, 44614, 0), -- Frostfire Bolt
(1, 8, 9, 59752, 0), -- Every Man for Himself
-- Human - Monk
(1, 10, 0, 100780, 0), -- Jab
(1, 10, 72, 100780, 0), -- Jab
(1, 10, 81, 59752, 0), -- Every Man for Himself
-- Draenei - Paladin
(11, 2, 0, 35395, 0), -- Crusader Strike
(11, 2, 9, 59542, 0), -- Gift of the Naaru
(11, 2, 83, 4540, 0), -- Holy Resist
-- Gnome - Monk
(7, 10, 0, 100780, 0), -- Jab
(7, 10, 72, 100780, 0), -- Jab
(7, 10, 81, 20589, 0), -- Escape Artist
-- Draenei - Warrior
(11, 1, 0, 88163, 0), -- Attack
(11, 1, 1, 78, 0), -- Heroic Strike
(11, 1, 9, 28880, 0), -- Gift of the Naaru
(11, 1, 72, 88163, 0), -- Attack
(11, 1, 73, 78, 0), -- Heroic Strike
(11, 1, 81, 28880, 0), -- Gift of the Naaru
-- NightElf - Hunter
(4, 3, 0, 3044, 0), -- Arcane Shot
(4, 3, 9, 58984, 0), -- Shadowmeld
(4, 3, 10, 9, 48),
(4, 3, 11, 982, 0), -- Revive Pet
(4, 3, 81, 58984, 0), -- Shadowmeld
-- NightElf - Mage
(4, 8, 0, 44614, 0), -- Frostfire Bolt
(4, 8, 9, 58984, 0), -- Shadowmeld
-- Dwarf - Hunter
(3, 3, 0, 3044, 0), -- Arcane Shot
(3, 3, 9, 20594, 0), -- Stoneform
(3, 3, 10, 9, 48),
(3, 3, 11, 982, 0), -- Revive Pet
(3, 3, 75, 20594, 0), -- Stoneform
-- Dwarf - Shaman
(3, 7, 0, 403, 0), -- Lightning Bolt
(3, 7, 9, 20594, 0), -- Stoneform
(3, 7, 72, 403, 0), -- Lightning Bolt
-- Dwarf - Priest
(3, 5, 0, 585, 0), -- Smite
(3, 5, 9, 20594, 0), -- Stoneform
-- Dwarf - Rogue
(3, 4, 0, 1752, 0), -- Sinister Strike
(3, 4, 9, 20594, 0), -- Stoneform
-- Dwarf - Monk
(3, 10, 0, 100780, 0), -- Jab
(3, 10, 72, 100780, 0), -- Jab
(3, 10, 81, 20594, 0), -- Stoneform
-- Dwarf - Warlock
(3, 9, 0, 686, 0), -- Shadow Bolt
(3, 9, 9, 20594, 0), -- Stoneform
(3, 9, 10, 10, 48),
-- Dwarf - Mage
(3, 8, 0, 44614, 0), -- Frostfire Bolt
(3, 8, 9, 20594, 0), -- Stoneform
-- NightElf - Rogue
(4, 4, 0, 1752, 0), -- Sinister Strike
(4, 4, 4, 58984, 0), -- Shadowmeld
(4, 4, 81, 58984, 0), -- Shadowmeld
-- NightElf - Priest
(4, 5, 0, 585, 0), -- Smite
(4, 5, 9, 58984, 0), -- Shadowmeld
(4, 5, 81, 58984, 0), -- Shadowmeld
-- NightElf - Warrior
(4, 1, 0, 88163, 0), -- Attack
(4, 1, 1, 78, 0), -- Heroic Strike
(4, 1, 9, 58984, 0), -- Shadowmeld
(4, 1, 72, 88163, 0), -- Attack
(4, 1, 73, 78, 0), -- Heroic Strike
(4, 1, 81, 58984, 0), -- Shadowmeld
-- Worgen - Rogue
(22, 4, 0, 1752, 0), -- Sinister Strike
-- Worgen - Priest
(22, 5, 0, 585, 0), -- Smite
-- Worgen - Hunter
(22, 3, 0, 3044, 0), -- Arcane Shot
(22, 3, 10, 9, 48),
(22, 3, 11, 982, 0), -- Revive Pet
-- Worgen - Warrior
(22, 1, 0, 88163, 0), -- Attack
(22, 1, 1, 78, 0), -- Heroic Strike
(22, 1, 72, 88163, 0), -- Attack
(22, 1, 73, 78, 0), -- Heroic Strike
-- Worgen - Druid
(22, 11, 0, 5176, 0), -- Wrath
(22, 11, 72, 6603, 0), -- Auto Attack
(22, 11, 84, 6603, 0), -- Auto Attack
(22, 11, 96, 6603, 0), -- Auto Attack
-- Worgen - Mage
(22, 8, 0, 44614, 0), -- Frostfire Bolt
-- Worgen - Warlock
(22, 9, 0, 686, 0), -- Shadow Bolt
(22, 9, 10, 10, 48),
-- Undead - Priest
(5, 5, 0, 585, 0), -- Smite
(5, 5, 9, 20577, 0), -- Cannibalize
-- Undead - Rogue
(5, 4, 0, 1752, 0), -- Sinister Strike
(5, 4, 9, 20577, 0), -- Cannibalize
-- Undead - Warrior
(5, 1, 0, 88163, 0), -- Attack
(5, 1, 1, 78, 0), -- Heroic Strike
(5, 1, 9, 20577, 0), -- Cannibalize
(5, 1, 72, 88163, 0), -- Attack
(5, 1, 73, 78, 0), -- Heroic Strike
(5, 1, 81, 20577, 0), -- Cannibalize
-- Undead - Hunter
(5, 3, 0, 3044, 0), -- Arcane Shot
(5, 3, 9, 20577, 0), -- Cannibalize
(5, 3, 10, 9, 48),
(5, 3, 11, 982, 0), -- Revive Pet
-- Undead - Warlock
(5, 9, 0, 686, 0), -- Shadow Bolt
(5, 9, 9, 20577, 0), -- Cannibalize
(5, 9, 10, 10, 48),
-- Undead - Mage
(5, 8, 0, 44614, 0), -- Frostfire Bolt
(5, 8, 9, 20577, 0), -- Cannibalize
-- Undead - Monk
(5, 10, 0, 100780, 0), -- Jab
(5, 10, 72, 100780, 0), -- Jab
(5, 10, 81, 20577, 0), -- Cannibalize
-- Troll - Mage
(8, 8, 0, 44614, 0), -- Frostfire Bolt
(8, 8, 9, 26297, 0), -- Berserking
-- Troll - Warlock
(8, 9, 0, 686, 0), -- Shadow Bolt
(8, 9, 9, 26297, 0), -- Berserking
(8, 9, 10, 10, 48),
-- Troll - Monk
(8, 10, 0, 100780, 0), -- Jab
(8, 10, 72, 100780, 0), -- Jab
(8, 10, 81, 26297, 0), -- Berserking
-- Troll - Druid
(8, 11, 0, 5176, 0), -- Wrath
(8, 11, 9, 26297, 0), -- Berserking
(8, 11, 72, 6603, 0), -- Auto Attack
(8, 11, 84, 6603, 0), -- Auto Attack
(8, 11, 96, 6603, 0), -- Auto Attack
-- Troll - Warrior
(8, 1, 0, 88163, 0), -- Attack
(8, 1, 1, 78, 0), -- Heroic Strike
(8, 1, 9, 26297, 0), -- Berserking
(8, 1, 72, 88163, 0), -- Attack
(8, 1, 73, 78, 0), -- Heroic Strike
(8, 1, 81, 26297, 0), -- Berserking
-- Troll - Hunter
(8, 3, 0, 3044, 0), -- Arcane Shot
(8, 3, 9, 26297, 0), -- Berserking
(8, 3, 10, 9, 48),
(8, 3, 11, 982, 0), -- Revive Pet
-- Troll - Rogue
(8, 4, 0, 1752, 0), -- Sinister Strike
(8, 4, 9, 26297, 0), -- Berserking
(8, 4, 76, 26297, 0), -- Berserking
-- Troll - Priest
(8, 5, 0, 585, 0), -- Smite
(8, 5, 9, 26297, 0), -- Berserking
-- Troll - Shaman
(8, 7, 0, 403, 0), -- Lightning Bolt
(8, 7, 9, 26297, 0), -- Berserking
(8, 7, 72, 403, 0), -- Lightning Bolt
-- Tauren - Shaman
(6, 7, 0, 403, 0), -- Lightning Bolt
(6, 7, 9, 20549, 0), -- War Stomp
(6, 7, 75, 20549, 0), -- War Stomp
-- Tauren - Priest
(6, 5, 0, 585, 0), -- Smite
(6, 5, 9, 20549, 0), -- War Stomp
-- Tauren - Paladin
(6, 2, 0, 35395, 0), -- Crusader Strike
(6, 2, 9, 20549, 0), -- War Stomp
-- Tauren - Hunter
(6, 3, 0, 3044, 0), -- Arcane Shot
(6, 3, 9, 20549, 0), -- War Stomp
(6, 3, 10, 9, 48),
(6, 3, 11, 982, 0), -- Revive Pet
(6, 3, 75, 20549, 0), -- War Stomp
-- Tauren - Warrior
(6, 1, 0, 88163, 0), -- Attack
(6, 1, 1, 78, 0), -- Heroic Strike
(6, 1, 9, 20549, 0), -- War Stomp
(6, 1, 72, 88163, 0), -- Attack
(6, 1, 73, 78, 0), -- Heroic Strike
(6, 1, 81, 20549, 0), -- War Stomp
-- Tauren - Monk
(6, 10, 0, 100780, 0), -- Jab
(6, 10, 72, 100780, 0), -- Jab
(6, 10, 81, 20549, 0), -- War Stomp
-- Tauren - Druid
(6, 11, 0, 5176, 0), -- Wrath
(6, 11, 9, 20549, 0), -- War Stomp
(6, 11, 72, 6603, 0), -- Auto Attack
(6, 11, 75, 20549, 0), -- War Stomp
(6, 11, 84, 6603, 0), -- Auto Attack
(6, 11, 96, 6603, 0), -- Auto Attack
(6, 11, 108, 6603, 0), -- Auto Attack
-- PandarenNeutral - Mage
(24, 8, 0, 44614, 0), -- Frostfire Bolt
(24, 8, 9, 107079, 0), -- Quaking Palm
(24, 8, 11, 77264, 128), -- Small Bamboo Shoot
-- PandarenNeutral - Monk
(24, 10, 0, 100780, 0), -- Jab
(24, 10, 72, 100780, 0), -- Jab
(24, 10, 81, 107079, 0), -- Quaking Palm
(24, 10, 83, 77272, 128), -- Small Sugarcane Stalk
-- PandarenNeutral - Warrior
(24, 1, 0, 88163, 0), -- Attack
(24, 1, 1, 78, 0), -- Heroic Strike
(24, 1, 9, 107079, 0), -- Quaking Palm
(24, 1, 11, 77273, 128), -- Small Rice Cake
(24, 1, 72, 88163, 0), -- Attack
(24, 1, 73, 78, 0), -- Heroic Strike
(24, 1, 81, 107079, 0), -- Quaking Palm
(24, 1, 83, 77273, 128), -- Small Rice Cake
-- PandarenNeutral - Hunter
(24, 3, 0, 3044, 0), -- Arcane Shot
(24, 3, 7, 77272, 128), -- Small Sugarcane Stalk
(24, 3, 9, 107079, 0), -- Quaking Palm
(24, 3, 10, 9, 48),
(24, 3, 11, 982, 0), -- Revive Pet
-- PandarenNeutral - Rogue
(24, 4, 0, 1752, 0), -- Sinister Strike
(24, 4, 9, 107079, 0), -- Quaking Palm
(24, 4, 11, 77272, 128), -- Small Sugarcane Stalk
-- PandarenNeutral - Priest
(24, 5, 0, 585, 0), -- Smite
(24, 5, 9, 107079, 0), -- Quaking Palm
(24, 5, 11, 77264, 128), -- Small Bamboo Shoot
-- PandarenNeutral - Shaman
(24, 7, 0, 403, 0), -- Lightning Bolt
(24, 7, 9, 107079, 0), -- Quaking Palm
(24, 7, 11, 77272, 128), -- Small Sugarcane Stalk
(24, 7, 72, 403, 0), -- Lightning Bolt
-- Tauren - DeathKnight
(6, 6, 0, 6603, 0), -- Auto Attack
(6, 6, 1, 49576, 0), -- Death Grip
(6, 6, 2, 45477, 0), -- Icy Touch
(6, 6, 3, 45462, 0), -- Plague Strike
(6, 6, 4, 50842, 0), -- Pestilence
(6, 6, 5, 47541, 0), -- Death Coil
(6, 6, 10, 20549, 0), -- War Stomp
(6, 6, 75, 20549, 0), -- War Stomp
-- Orc - DeathKnight
(2, 6, 0, 6603, 0), -- Auto Attack
(2, 6, 1, 49576, 0), -- Death Grip
(2, 6, 2, 45477, 0), -- Icy Touch
(2, 6, 3, 45462, 0), -- Plague Strike
(2, 6, 4, 50842, 0), -- Pestilence
(2, 6, 5, 47541, 0), -- Death Coil
(2, 6, 10, 20572, 0), -- Blood Fury
-- Worgen - DeathKnight
(22, 6, 0, 6603, 0), -- Auto Attack
(22, 6, 1, 49576, 0), -- Death Grip
(22, 6, 2, 45477, 0), -- Icy Touch
(22, 6, 3, 45462, 0), -- Plague Strike
(22, 6, 4, 50842, 0), -- Pestilence
(22, 6, 5, 47541, 0), -- Death Coil
(22, 6, 9, 68992, 0), -- Darkflight
(22, 6, 10, 68996, 0), -- Two Forms
(22, 6, 11, 87840, 0), -- Running Wild
-- Undead - DeathKnight
(5, 6, 0, 6603, 0), -- Auto Attack
(5, 6, 1, 49576, 0), -- Death Grip
(5, 6, 2, 45477, 0), -- Icy Touch
(5, 6, 3, 45462, 0), -- Plague Strike
(5, 6, 4, 50842, 0), -- Pestilence
(5, 6, 5, 47541, 0), -- Death Coil
(5, 6, 10, 20577, 0), -- Cannibalize
-- BloodElf - DeathKnight
(10, 6, 0, 6603, 0), -- Auto Attack
(10, 6, 1, 49576, 0), -- Death Grip
(10, 6, 2, 45477, 0), -- Icy Touch
(10, 6, 3, 45462, 0), -- Plague Strike
(10, 6, 4, 50842, 0), -- Pestilence
(10, 6, 5, 47541, 0), -- Death Coil
(10, 6, 6, 50613, 0), -- Arcane Torrent
-- Troll - DeathKnight
(8, 6, 0, 6603, 0), -- Auto Attack
(8, 6, 1, 49576, 0), -- Death Grip
(8, 6, 2, 45477, 0), -- Icy Touch
(8, 6, 3, 45462, 0), -- Plague Strike
(8, 6, 4, 50842, 0), -- Pestilence
(8, 6, 5, 47541, 0), -- Death Coil
(8, 6, 10, 26297, 0), -- Berserking
-- Goblin - DeathKnight
(9, 6, 0, 6603, 0), -- Auto Attack
(9, 6, 1, 49576, 0), -- Death Grip
(9, 6, 2, 45477, 0), -- Icy Touch
(9, 6, 3, 45462, 0), -- Plague Strike
(9, 6, 4, 50842, 0), -- Pestilence
(9, 6, 5, 47541, 0), -- Death Coil
(9, 6, 9, 69070, 0), -- Rocket Jump
(9, 6, 10, 69041, 0), -- Rocket Barrage
(9, 6, 11, 69046, 0), -- Pack Hobgoblin
-- Draenei - DeathKnight
(11, 6, 0, 6603, 0), -- Auto Attack
(11, 6, 1, 49576, 0), -- Death Grip
(11, 6, 2, 45477, 0), -- Icy Touch
(11, 6, 3, 45462, 0), -- Plague Strike
(11, 6, 4, 50842, 0), -- Pestilence
(11, 6, 5, 47541, 0), -- Death Coil
(11, 6, 10, 59545, 0), -- Gift of the Naaru
-- NightElf - DeathKnight
(4, 6, 0, 6603, 0), -- Auto Attack
(4, 6, 1, 49576, 0), -- Death Grip
(4, 6, 2, 45477, 0), -- Icy Touch
(4, 6, 3, 45462, 0), -- Plague Strike
(4, 6, 4, 50842, 0), -- Pestilence
(4, 6, 5, 47541, 0), -- Death Coil
(4, 6, 10, 58984, 0), -- Shadowmeld
(4, 6, 83, 58984, 0), -- Shadowmeld
-- Gnome - DeathKnight
(7, 6, 0, 6603, 0), -- Auto Attack
(7, 6, 1, 49576, 0), -- Death Grip
(7, 6, 2, 45477, 0), -- Icy Touch
(7, 6, 3, 45462, 0), -- Plague Strike
(7, 6, 4, 50842, 0), -- Pestilence
(7, 6, 5, 47541, 0), -- Death Coil
(7, 6, 10, 20589, 0), -- Escape Artist
(7, 6, 72, 6603, 0), -- Auto Attack
(7, 6, 83, 117, 128), -- Tough Jerky
(7, 6, 84, 6603, 0), -- Auto Attack
(7, 6, 96, 6603, 0), -- Auto Attack
(7, 6, 108, 6603, 0), -- Auto Attack
-- Dwarf - DeathKnight
(3, 6, 0, 6603, 0), -- Auto Attack
(3, 6, 1, 49576, 0), -- Death Grip
(3, 6, 2, 45477, 0), -- Icy Touch
(3, 6, 3, 45462, 0), -- Plague Strike
(3, 6, 4, 50842, 0), -- Pestilence
(3, 6, 5, 47541, 0), -- Death Coil
-- Human - DeathKnight
(1, 6, 0, 6603, 0), -- Auto Attack
(1, 6, 1, 49576, 0), -- Death Grip
(1, 6, 2, 45477, 0), -- Icy Touch
(1, 6, 3, 45462, 0), -- Plague Strike
(1, 6, 4, 50842, 0), -- Pestilence
(1, 6, 5, 47541, 0), -- Death Coil
(1, 6, 11, 59752, 0); -- Every Man for Himself
