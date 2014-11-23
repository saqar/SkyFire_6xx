DROP TABLE IF EXISTS `battlenet_components`;
CREATE TABLE `battlenet_components` (
  `Program` varchar(4) NOT NULL,
  `Platform` varchar(4) NOT NULL,
  `Build` int(11) unsigned NOT NULL,
  PRIMARY KEY (`Program`,`Platform`,`Build`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
insert  into `battlenet_components`(`Program`,`Platform`,`Build`) values ('Bnet','Win',37165),('Bnet','Wn64',37165),('enUS','Win',19103),('WoW','base',19057),('WoW','enGB',19103),('WoW','itIT',19103),('WoW','Win',19103),('WoWB','enGB',19103),('WoWB','enUS',19085),('WoWB','itIT',19103),('WoWB','ruRU',19103),('WoWB','Win',19085),('WoWB','Wn64',19103);
