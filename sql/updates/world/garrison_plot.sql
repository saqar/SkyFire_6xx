/*
Navicat MySQL Data Transfer

Source Server         : Skyfire6xx
Source Server Version : 50540
Source Host           : localhost:3306
Source Database       : world

Target Server Type    : MYSQL
Target Server Version : 50540
File Encoding         : 65001

Date: 2014-11-06 05:07:39
*/

SET FOREIGN_KEY_CHECKS=0;

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
