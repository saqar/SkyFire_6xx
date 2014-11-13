CREATE TABLE IF NOT EXISTS `battle_pay_entry` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `order` int(10) unsigned NOT NULL DEFAULT '0',
  `groupId` int(10) unsigned NOT NULL DEFAULT '0',
  `productId` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `banner` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `battle_pay_entry` (`id`, `order`, `groupId`, `productId`, `flags`, `banner`) VALUES
    (6, 3, 13, 50, 0, 0),
    (7, 4, 13, 51, 0, 0),
    (8, 5, 13, 52, 0, 0),
    (9, 6, 13, 53, 0, 0),
    (10, 7, 13, 54, 0, 0),
    (11, 7, 13, 55, 0, 0),
    (12, 8, 13, 56, 0, 0),
    (13, 9, 13, 57, 0, 0),
    (14, 1, 15, 58, 0, 0),
    (15, 3, 15, 59, 0, 0),
    (16, 15, 15, 60, 0, 0),
    (17, 5, 15, 61, 0, 0),
    (18, 6, 15, 62, 0, 0),
    (25, 0, 15, 73, 0, 0),
    (28, 4, 15, 80, 0, 0),
    (29, 1, 13, 81, 0, 0),
    (47, 0, 17, 104, 0, 2);
