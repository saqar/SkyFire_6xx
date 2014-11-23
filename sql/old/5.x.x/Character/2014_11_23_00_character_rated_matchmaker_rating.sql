
SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for character_rated_matchmaker_rating
-- ----------------------------
DROP TABLE IF EXISTS `character_rated_matchmaker_rating`;
CREATE TABLE `character_rated_matchmaker_rating` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `matchmakerRating` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
