<?php
	
	//This file is used to get the latlon of a station when selected. It returns JSON.
	require 'configuration.php';
	$station = $_GET["stationID"];
	$conn = new mysqli($servername, $username, $password, $database);

		// Check connection
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
		}

	$sql="Select Concat('{\"lat\":', `Latitude(DecDeg)`, ' , \"lng\":',`Longitude(DecDeg)`,'}' ) as latlon from oceandata.`event` where Station='".$station ."' limit 1";
	
	$res=$conn->query($sql);
	//echo $res; die;
	$latlon=$res->fetch_object();
	echo $latlon->latlon;
	$res->close();
	$conn->close();

?>
