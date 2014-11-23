ALTER TABLE `character_pet`
ADD COLUMN `SpecGroups`  varchar(10) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '0 0' AFTER `curmana`,
ADD COLUMN `SpecCount`  tinyint(3) UNSIGNED NOT NULL DEFAULT 1 AFTER `ActiveSpecGroup`,
ADD COLUMN `ActiveSpecGroup`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `SpecGroups`;