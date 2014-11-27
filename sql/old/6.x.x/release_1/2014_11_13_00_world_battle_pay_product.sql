CREATE TABLE IF NOT EXISTS `battle_pay_product` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `title` varchar(50) NOT NULL,
  `description` varchar(500) NOT NULL,
  `normalPrice` bigint(20) unsigned NOT NULL DEFAULT '0',
  `currentPrice` bigint(20) unsigned NOT NULL DEFAULT '0',
  `itemId` int(10) unsigned NOT NULL DEFAULT '0',
  `quantity` int(10) unsigned NOT NULL DEFAULT '0',
  `displayId` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `choiceType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `battle_pay_product` (`id`, `title`, `description`, `normalPrice`, `currentPrice`, `itemId`, `quantity`, `displayId`, `type`, `choiceType`, `flags`) VALUES
    (50, 'Lil\' K.T.', 'Chill your enemies and chortle with glee with Lil\' K.T. by your side.', 100000, 100000, 49693, 1, 30507, 0, 2, 47),
    (51, 'Soul of the Aspects', 'Untamed but friendly, this golden dragon will fly the skies with you.', 100000, 100000, 78916, 1, 40019, 0, 2, 47),
    (52, 'Lil\' Ragnaros', 'Your foes will be purged by fire with Lil\' Ragnaros on your team.', 100000, 100000, 68385, 1, 37541, 0, 2, 47),
    (53, 'Lil\' XT', 'Everybody\'s ready to play when Lil\' X.T. is around.', 100000, 100000, 54847, 1, 32031, 0, 2, 47),
    (54, 'Cinder Kitten', 'This Cinder Kitten will char your face off with cuteness.', 100000, 100000, 92707, 1, 46720, 0, 2, 47),
    (55, 'Cenarion Hatchling', 'Restore balance to nature with the Cenarion Hatchling by your side.', 100000, 100000, 70099, 1, 16943, 0, 2, 47),
    (56, 'Blossoming Ancient', 'Grow your collection with the life-giving Blossoming Ancient.', 100000, 100000, 98550, 1, 48933, 0, 2, 47),
    (57, 'Pandaren Monk', 'Befriend the best of Pandaria with the powerful Pandaren Monk.', 100000, 100000, 49665, 1, 30414, 0, 2, 47),
    (58, 'Armored Bloodwing', 'Monstrous ruler of the night skies, this flying mount is prepared for serious mayhem.\r\n', 250000, 250000, 95341, 1, 48020, 0, 3, 47),
    (59, 'Winged Guardian', 'Light up the night on this blazing, eagle-winged lion.', 250000, 250000, 69846, 1, 38260, 0, 3, 47),
    (60, 'Swift Windsteed', 'Cavort among the clouds on this wind-running wonder.', 250000, 250000, 92724, 1, 46729, 0, 3, 47),
    (61, 'Heart of the Aspects', 'Glowing with inner light, this luminous flying dragon defies the darkness.', 250000, 250000, 78924, 1, 40029, 0, 3, 47),
    (62, 'Celestial Steed', 'Saddle up the stars on this supernatural flying mount.', 250000, 250000, 54811, 1, 31958, 0, 3, 47),
    (73, 'Iron Skyreaver', 'One of the most fearsome flying mounts the Iron Horde fields.', 250000, 250000, 107951, 1, 53038, 0, 3, 47),
    (80, 'Enchanted Fey Dragon', 'Stand out no matter the situation on this color-shifting dragon.', 250000, 250000, 97989, 1, 48714, 0, 3, 47),
    (81, 'Alterac Brew-Pup', 'Scale the highest mountain with the trusty Alterac Brew-Pup by your side.', 100000, 100000, 106240, 1, 51988, 0, 2, 47),
    (104, 'Warforged Nightmare', 'Through the magic of a fantastical hitching post, you and your allies can ride this magnificent metal steed to your next conquest.  So bring the finest horse ever crafted to your stables and unite the war band!  Grandmaster smiths are standing by for your order.', 300000, 300000, 112326, 1, 55896, 0, 3, 47);
