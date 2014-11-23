DROP TABLE IF EXISTS `account_muted`;
/*!40101 SET @saved_cs_client = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_muted` (
`guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
`mutedate` int(10) unsigned NOT NULL DEFAULT '0',
`mutetime` int(10) unsigned NOT NULL DEFAULT '0',
`mutedby` varchar(50) NOT NULL,
`mutereason` varchar(255) NOT NULL,
PRIMARY KEY (`guid`,`mutedate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='mute List';
/*!40101 SET character_set_client = @saved_cs_client */;
-- 
-- Dumping data for table `account_muted`
-- 
LOCK TABLES `account_muted` WRITE;
/*!40000 ALTER TABLE `account_muted` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_muted` ENABLE KEYS */;
UNLOCK TABLES;



ALTER TABLE `battlenet_accounts` ADD COLUMN `LastCharacterUndelete` INT(10) UNSIGNED DEFAULT 0  NOT NULL AFTER `os`;