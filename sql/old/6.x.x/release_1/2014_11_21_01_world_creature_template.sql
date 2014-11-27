ALTER TABLE `creature_template`
ADD COLUMN `nameAlt`  char(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `name`,
ADD COLUMN `subnameAlt`  char(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL AFTER `subname`,
ADD COLUMN `npcflag2`  int(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `npcflag`,
ADD COLUMN `FlagQuest`  int(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `RacialLeader`,
MODIFY COLUMN `family`  smallint(5) NOT NULL DEFAULT 0 AFTER `dynamicflags`;
