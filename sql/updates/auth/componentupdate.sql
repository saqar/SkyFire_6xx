/*
SQLyog Community v12.01 (64 bit)
MySQL - 5.1.73-community : Database - auth
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`auth` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `auth`;

/*Table structure for table `battlenet_components` */

DROP TABLE IF EXISTS `battlenet_components`;

CREATE TABLE `battlenet_components` (
  `Program` varchar(4) NOT NULL,
  `Platform` varchar(4) NOT NULL,
  `Build` int(11) unsigned NOT NULL,
  PRIMARY KEY (`Program`,`Platform`,`Build`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `battlenet_components` */

insert  into `battlenet_components`(`Program`,`Platform`,`Build`) values ('Bnet','Win',37165),('Bnet','Wn64',37165),('enUS','Win',19103),('WoW','base',19057),('WoW','enGB',19103),('WoW','itIT',19103),('WoW','Win',19103),('WoWB','enGB',19103),('WoWB','enUS',19085),('WoWB','itIT',19103),('WoWB','ruRU',19103),('WoWB','Win',19085),('WoWB','Wn64',19103);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
