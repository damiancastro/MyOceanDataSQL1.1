<?php
	
	//This file is used to get the latlon of a station when selected. It returns JSON.
	require 'configuration.php';
	$s = $_GET["Station"];
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
	//$sql="Call getInfoW ('".$it."');";
	//$sql='Select min(`Time(ISO8601)`) as deploy, max(`Time(ISO8601)`) as recover from measurement m join metadata me on m.idEvent=me.idEvent where InstrumentType="'.$it.'" and Station="'.$s.'" ';
	$sql='Select Distinct DATE_FORMAT(`DeployTime(ISO8601)`, "%Y-%b-%d") as deploy, DATE_FORMAT(`RecoverTime(ISO8601)`, "%Y-%b-%d") as recover,Truncate(`MaxDepth(m)`, 0) as depth from metadata where InstrumentType="'.$it.'" and Station="'.$s.'" ';

	$vector = array();	
	$res=$conn->query($sql);

	while($row = $res->fetch_array(MYSQLI_ASSOC)){
		$vector[]= $row;
  
	}
	//echo '['.implode (",",$vector).']';
	//$rows=$res->fetch_array(MYSQLI_ASSOC);
	//header('Content-Type: application/json;charset=utf-8');
	echo json_encode( $vector);
	$res->close();
	$conn->close();

?>
