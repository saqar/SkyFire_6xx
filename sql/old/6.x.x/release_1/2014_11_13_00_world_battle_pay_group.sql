CREATE TABLE IF NOT EXISTS `battle_pay_group` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `order` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(16) NOT NULL,
  `icon` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `order` (`order`,`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `battle_pay_group` (`id`, `order`, `name`, `icon`, `type`) VALUES
    (13, 3, 'Pets', 939380, 0),
    (15, 4, 'Mounts', 939379, 0),
    (17, 1, 'Featured', 939376, 1),
    (22, 20, 'Services', 939382, 0);
