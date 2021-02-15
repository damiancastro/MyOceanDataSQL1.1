# MyOceanDataSQL1.1
Introductory note: a manual for this application is provided at the root of this repository. We encourage any interested user, particularly those with little previous experience with MySQL and web-server applications to read it before attempting an install.

MyOceandataSQL was developed in 3 components: a GUI “Oceandata Upload Application” (OUA) to upload and consolidate data in a relational database, a MySQL database, and a web-based “Oceandata Web Application” (OWA) which allows users to extract and download data using a map-base tool and different filtering parameters. 

-Oceandata Upload Application (OUA). 
OUA is the primary interface between a user and the relational database to upload data. This GUI was developed in C++. At first, it shows up as a window with tabs. The first tab provides connection with the database while the subsequent tabs (displaying incrementally as the user progress) guide the user in the uploading process.

-MySQL database (oceandata.sql). 
The database was developed in the open source relational database system MySQL. Database systems are characterized by implementing entity-relationship (ER) models (Chen, 1976) and extended entity-relationship (EER) models (Elmasry and Navathe, 2007). In OUA, the instrument measuring event (i.e. a unique set of station, instrument, latitude and longitude) and each measurement are entities that can be represented in the database as having a relationship such as a measurement “belongs” to a single event. All the measurements are in a “measurement” table and the events in a “metadata” table.

-Oceandata Web Application (OWA). 
The OWA offers several filtering options to extract desired data using the mandatory fields “Instrument” (i.e. “InstrumentType”), “Location” (i.e. “Longitude(DecDeg)” and “Latitude(DecDeg)”), “Time” (i.e. “Time(ISO8601)”) and “Depth” (“Depth(unit)”). The interface also allows the selection of the measurement variable(s) to bring retrieve from the database. By default, variables time and depth are always included. OWA also offers three spatial options to filter data location: 1/ select stations either by clicking them on the map or by entering the station name from the list, 2/ enter a latitude (DecDeg), longitude (DecDeg) and a radius (km) or directly draw a circle on the map or 3/ draw a polygon, circle or square on the map. With the last two options, OWA will retrieve the stations that are inside the circle or polygon. Polygons are processed by the database server using MySQL spatial indexes resulting in efficient and fast execution regardless of how many rows are in the database. Data extracted are provided as a csv file which contains, as a minimum, the mandatory variables: “Station”, “Instrument”, “InstrumentType”, “Longitude(DecDeg)”, “Latitude(DecDeg)”, “Time(ISO8601)” and "Depth(unit)" plus all other variables selected by the user. Figure 4 shows OWA workflow and describes the available filtering options.


Dependencies for a local MS Windows install (suggestion/optional). 
These are the executables and dependencies to install a local MySQL server and Web server as well as utilities on a MS Windows machine (some of them could be already on machine):

Visual C++ Redist

Wamp server (PHP, Mysql and Web support for Windows)

Communication with MySQL database can be done using HeidiSQL.

For more information on the WAMP server (SQL&PHP server) see:

https://wampserver.aviatechno.net/?lang=en	(ckick on Installation requirements for more details).

Fore more information on HeidiSQL see:

https://www.heidisql.com/


Quick install guide:

1) unzip ll_vc_redist_x86_x64.zip (available on WAMP server page at: https://wampserver.aviatechno.net/files/vcpackages/all_vc_redist_x86_x64.zip)

2) install all the VC packages, 1 by 1
NOTE a functioning system may be installed without vcredist_2019_*

3) check your system using check_vcredist.exe

4) if all good, install WAMP (wampserver3.2.3_x86.exe if you want 32 bits, wampserver3.2.3_x64.exe otherwise for 64 bits)
Packages available on WAMP server page at:
https://wampserver.aviatechno.net/files/install/wampserver3.2.3_x86.exe and 
https://wampserver.aviatechno.net/files/install/wampserver3.2.3_x64.exe

One need to install all the dependencies first (all_vc_redist_x86_x64.zip) before installing the server package (wampserver3.2.3_x86.exe or wampserver3.2.3_x64.exe).

Good install of the dependencies can checked using the tool check_vcredist.exe (just launch it).
check_vcredist.exe is available on WAMP server page at: https://wampserver.aviatechno.net/files/tools/check_vcredist.exe

Communication with MySQL database can be done using HeidiSQL (HeidiSQL_11.2.0.6213_Setup.exe).
HeidiSQL_11.2.0.6213_Setup.exe is available on HeidiSQL main page at: https://www.heidisql.com/download.php


REFERENCES

Chen, P. P.-S. (1976). The Entity-Relationship Model—Toward a Unified View of Data. ACM Transactions on Database Systems, 1(1), 9-36. doi:10.1145/320434.320440

Elmasri, R., & Navathe, S. B. (2006). Fundamentals of database systems (5th ed.). Boston, MA: Addison-Wesley Longman Publishing Co.
