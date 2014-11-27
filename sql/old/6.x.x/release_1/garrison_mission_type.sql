/*
Navicat MySQL Data Transfer

Source Server         : Skyfire6xx
Source Server Version : 50540
Source Host           : localhost:3306
Source Database       : world

Target Server Type    : MYSQL
Target Server Version : 50540
File Encoding         : 65001

Date: 2014-11-06 05:14:01
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for garrison_mission_type
-- ----------------------------
DROP TABLE IF EXISTS `garrison_mission_type`;
CREATE TABLE `garrison_mission_type` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `Name` text NOT NULL,
  `MissionID` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Export of GarrMissionType.db2';

-- ----------------------------
-- Records of garrison_mission_type
-- ----------------------------
INSERT INTO `garrison_mission_type` VALUES ('3', 'Combat', '3475');
INSERT INTO `garrison_mission_type` VALUES ('4', 'Generic', '3475');
INSERT INTO `garrison_mission_type` VALUES ('5', 'Salvage', '3567');
INSERT INTO `garrison_mission_type` VALUES ('6', 'Logistics', '3580');
INSERT INTO `garrison_mission_type` VALUES ('7', 'Wildlife', '3572');
INSERT INTO `garrison_mission_type` VALUES ('8', 'Trading', '3575');
INSERT INTO `garrison_mission_type` VALUES ('9', 'Construction', '3578');
INSERT INTO `garrison_mission_type` VALUES ('10', 'Provision', '3568');
INSERT INTO `garrison_mission_type` VALUES ('11', 'Recruitment', '3583');
INSERT INTO `garrison_mission_type` VALUES ('12', 'Training', '3574');
INSERT INTO `garrison_mission_type` VALUES ('13', 'Patrol', '3582');
INSERT INTO `garrison_mission_type` VALUES ('14', 'Research', '3574');
INSERT INTO `garrison_mission_type` VALUES ('15', 'Defense', '3577');
INSERT INTO `garrison_mission_type` VALUES ('16', 'Exploration', '3476');
INSERT INTO `garrison_mission_type` VALUES ('17', 'Siege', '3570');
INSERT INTO `garrison_mission_type` VALUES ('18', 'Recipe: Alchemy', '3571');
INSERT INTO `garrison_mission_type` VALUES ('19', 'Recipe: Blacksmith', '3474');
INSERT INTO `garrison_mission_type` VALUES ('20', 'Recipe: Enchanting', '3566');
INSERT INTO `garrison_mission_type` VALUES ('21', 'Recipe: Engineering', '3581');
INSERT INTO `garrison_mission_type` VALUES ('22', 'Recipe: Inscription', '3579');
INSERT INTO `garrison_mission_type` VALUES ('23', 'Recipe: Jewelcrafting', '3576');
INSERT INTO `garrison_mission_type` VALUES ('24', 'Recipe: Leatherworking', '3584');
INSERT INTO `garrison_mission_type` VALUES ('25', 'Recipe: Tailoring', '3573');
INSERT INTO `garrison_mission_type` VALUES ('35', 'Treasure', '3575');
INSERT INTO `garrison_mission_type` VALUES ('36', 'Pet Battle', '3572');
