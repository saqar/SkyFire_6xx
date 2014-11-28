UPDATE `creature_template` SET `npcflag`=`npcflag`-0x30 WHERE npcflag&0x20 AND npcflag&0x10;
UPDATE `creature_template` SET `faction_A`=35 WHERE `faction_A`=0;
UPDATE `creature_template` SET `faction_H`=35 WHERE `faction_H`=0;
UPDATE `creature_template` SET `unit_class`=1 WHERE `unit_class`=0;
