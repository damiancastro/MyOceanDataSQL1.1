CREATE DATABASE  IF NOT EXISTS `oceandata` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `oceandata`;
-- MySQL dump 10.13  Distrib 5.7.12, for Win64 (x86_64)
--
-- Host: 142.130.125.40    Database: oceandata
-- ------------------------------------------------------
-- Server version	5.7.11

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `files`
--

DROP TABLE IF EXISTS `files`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `files` (
  `idfiles` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(256) NOT NULL,
  `folder` varchar(256) NOT NULL,
  `name` varchar(256) NOT NULL,
  PRIMARY KEY (`idfiles`)
) ENGINE=InnoDB AUTO_INCREMENT=196 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `measurement`
--

DROP TABLE IF EXISTS `measurement`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `measurement` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `idEvent` bigint(20) NOT NULL,
  `Time(ISO8601)` datetime DEFAULT NULL,
  `Depth(m)` float DEFAULT NULL,
    PRIMARY KEY (`id`),
  KEY `EventFK` (`idEvent`),
  KEY `DepthIdx` (`Depth(m)`),
  KEY `TimeISOIdx` (`Time(ISO8601)`)
) ENGINE=InnoDB AUTO_INCREMENT=609994 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `metadata`
--

DROP TABLE IF EXISTS `metadata`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `metadata` (
  `idEvent` int(11) NOT NULL AUTO_INCREMENT,
  `Station` char(30) NOT NULL,
  `Instrument` varchar(50) NOT NULL,
  `InstrumentType` char(10) NOT NULL,
  `Position` point NOT NULL,
  `Longitude(DecDeg)` float NOT NULL,
  `Latitude(DecDeg)` float NOT NULL,
  `DeployTime(ISO8601)` datetime,
  `RecoverTime(ISO8601)` datetime,
  `MaxDepth(m)` float,
  `file` varchar(256) NOT NULL,
  `UploadTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,

  PRIMARY KEY (`idEvent`),
  UNIQUE KEY `uniqueFieldSet` (`Station`,`Instrument`,`Longitude(DecDeg)`,`Latitude(DecDeg)`, `DeployTime(ISO8601)`),
  SPATIAL KEY `PointIdx` (`Position`),
  KEY `StatiionIdx` (`Station`),
  KEY `InsTypeIdx` (`InstrumentType`),
  KEY `LatLon` (`Longitude(DecDeg)`,`Latitude(DecDeg)`),
  KEY `lon` (`Longitude(DecDeg)`),
  KEY `lat` (`Latitude(DecDeg)`)
) ENGINE=InnoDB AUTO_INCREMENT=469092 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `event_before_insert` BEFORE INSERT ON `metadata` FOR EACH ROW BEGIN
Set New.Position=Point(New.`Latitude(DecDeg)`,New.`Longitude(DecDeg)`);
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Dumping routines for database 'oceandata'
--
/*!50003 DROP FUNCTION IF EXISTS `digits` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `digits`( str CHAR(32) ) RETURNS decimal(10,0)
BEGIN

  DECLARE i, len SMALLINT DEFAULT 1;

  DECLARE ret CHAR(32);

  DECLARE c CHAR(1);



  IF str IS NULL

  THEN 

    RETURN CAST(NULL AS DECIMAL) ;

  END IF;



  SET len = CHAR_LENGTH( str );

  REPEAT

    BEGIN

      SET c = MID( str, i, 1 );

      IF c BETWEEN '0' AND '9' THEN 

        SET ret=CONCAT(ret,c);

      END IF;

      SET i = i + 1;

    END;

  UNTIL i > len END REPEAT;

  RETURN CAST(ret AS DECIMAL);

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `getEventID` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `getEventID`(`station` CHAR(30), `instrument` CHAR(50), `latitude` FLOAT, `longitude` FLOAT) RETURNS int(11)
    READS SQL DATA
BEGIN

	Declare idEvent INT;

	Declare deployDT DATETIME;

	Select 

		MAX(e.idEvent) INTO idEvent from `metadata` e  

		where 

			e.Station=Station and

			e.instrument=instrument and

			e.`Longitude(DecDeg)` =longitude and

			e.`Latitude(DecDeg)`=latitude

		order by 

			e.idEvent desc

	LIMIT 1;

	Return idEvent;

END ;;
DELIMITER ;

/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `getpolygon` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`castrod`@`%` FUNCTION `getpolygon`(`lat` DOUBLE, `lon` DOUBLE, `radius` DOUBLE, `corner` TINYINT) RETURNS geometry
    NO SQL
    DETERMINISTIC
BEGIN

DECLARE i TINYINT DEFAULT 1;  

DECLARE a, b, c DOUBLE;  

DECLARE res TEXT;

  

IF corner < 3 || radius > 500 THEN  

    RETURN NULL;  

END IF;  

  

SET res = CONCAT(lat + radius / 111.12, ' ', lon, ',');  

  

WHILE i < corner  do

    SET c = RADIANS(360 / corner * i);  

    SET a = lat + COS(c) * radius / 111.12;  

    SET b = lon + SIN(c) * radius / (COS(RADIANS(lat + COS(c) * radius / 111.12 / 111.12)) * 111.12);  

    SET res = CONCAT(res, a, ' ', b, ',');  

    SET i = i + 1;  

END WHILE;  

  

RETURN GEOMFROMTEXT(CONCAT('POLYGON((', res, lat + radius / 111.12, ' ', lon, '))'));  

END ;;
DELIMITER ;

/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `updateADCPDepthTemp` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateADCPDepthTemp`()
BEGIN

	Update tempmeasurement m 

		set `InstrumentDepth(m)`= `Depth(m)`;

	

	Update tempmeasurement m 

		set `Depth(m)`= `CellDepth(m)`

		where  `CellDepth(m)` is not null;

	

	Insert Into tempmeasurement (`time(ISO8601)`, idEvent, `Depth(m)`, `InstrumentDepth(m)`, `Temperature(oC)`, `Tilt(o)`, `Pressure(dbar)`) 

		Select distinct `time(ISO8601)`, idEvent, `InstrumentDepth(m)`, `InstrumentDepth(m)`, `Temperature(oC)`, `Tilt(o)`, `Pressure(dbar)` 

		from  tempmeasurement m;



		Update tempmeasurement m 

			set `Temperature(oC)`= null, `Pressure(dBar)`=null, `Tilt(o)`= null 

			where  `CellDepth(m)` is not null;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `updateInstrumentDepthCTD` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateInstrumentDepthCTD`()
    MODIFIES SQL DATA
    DETERMINISTIC
BEGIN

	Update tempmeasurement m 

		set `InstrumentDepth(m)`= `Depth(m)` ;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `getMarkers`(in InstType char(10))
BEGIN
    Set @divhdr='<div id="content"> 
		<h1 id="firstHeading" class="firstHeading">';
	Set @coor='</h1><p> Lat-Long:';
    Set @tablehdr='</p><div id="bodyContent">
		<p><table class="table table-striped">
    <thead>
	<tr>
		<td >Deployment</td>
		<td >Depth(m)</td></tr>
        
    </thead>
    <tbody>';
    Set @tail='</tbody></p>
    </div>
 </div>';
    
    Select CONCAT( @divhdr, station, '[',InstrumentType, ']', @coor,Convert(Avg(`Latitude(DecDeg)`), CHAR), ' - ', 
    Convert(Avg(`Longitude(DecDeg)`), CHAR),
	@tablehdr, getInfoTable(station), @tail ) as infoW, Concat(
		'{\"position\":{\"lat\":', `Latitude(DecDeg)`, ' , \"lng\":',`Longitude(DecDeg)`,'},',
		'\"title\":\"',Station,'\"}') as marker from metadata group by Station ;

END;
DELIMITER ;


-- Dump completed on 2017-03-24 11:43:59
