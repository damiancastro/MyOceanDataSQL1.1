<?php
	
	//This file is used to get the latlon of a station when selected. It returns JSON.
	require 'configuration.php';
	$it = $_GET["InstrumentType"];
	$conn = new mysqli($servername, $username, $password, $database);

		// Check connection
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
		}

	/*$sql="Select distinct Concat(
		'{\"position\":{\"lat\":', `Latitude(DecDeg)`, ' , \"lng\":',`Longitude(DecDeg)`,'},',
		'\"title\":\"',Station,'\"}'
		) as marker 
		from oceandata.`event` 
	where InstrumentType='".$it."'"; */
	//$sql="Call getMarkers ('".$it."');";
	$sql='Select Station as title, InstrumentType, `Longitude(DecDeg)` as lng, `Latitude(DecDeg)` as lat
	 from metadata e where InstrumentType="'.$it.'" and `DeployTime(ISO8601)` in (Select min(`DeployTime(ISO8601)`) from metadata me where me.Station = e.Station) order by Station';
	$vector = array();	
	$res=$conn->query($sql);

	//while($row = $res->fetch_object()){
	while($row = $res->fetch_array(MYSQLI_ASSOC)){
		//$vector[]= $row->marker;
		$vector[]= $row;
  
	}
	//echo '['.implode (",",$vector).']';
	header('Content-Type: application/json;charset=utf-8');
	echo json_encode( $vector);
	$res->close();
	$conn->close();

?>
