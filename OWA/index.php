<!-- 
Index.php.
This is front page of ocenadata app
It creates the page and all the controls using bootstrap.
Poligon.js has all the functionality.
-->
<!DOCTYPE html> 
<html lang="en">
	<head>
		<meta http-equiv="content-type" content="text/html; charset=UTF-8">
		<meta charset="utf-8">
		<title>Aquaculture Query Application</title>
		<meta name="generator" content="Bootply" />
		<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		<link href="css/bootstrap.min.css" rel="stylesheet">
		<link href="css/bootstrap-multiselect.css" rel="stylesheet">
		<link href="css/bootstrap-datetimepicker.css" rel="stylesheet">
			<!--[if lt IE 9]><script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
			<script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script><![endif]-->
		<link href="css/styles.css" rel="stylesheet">
		<link href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.10.2/themes/ui-lightness/jquery-ui.css" rel="stylesheet">
			<!-- script references -->
		<script src="//ajax.googleapis.com/ajax/libs/jquery/2.0.2/jquery.min.js"></script>
		<script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.10.2/jquery-ui.min.js"></script>
		<script src="js/bootstrap.min.js"></script>
		<script type="text/javascript" src="http://maps.googleapis.com/maps/api/js?libraries=drawing&key=AIzaSyBeMUOwoiXL1VKqRnuhUe0dsdEROzb205E"></script>
		<script src="js/help.js"></script>
		<script src="js/polygon.js"></script>
		<script src="js/bootstrap-multiselect.js"></script>
		<script src="js/moment.min.js"></script>
		<script src="js/bootstrap-datetimepicker.js"></script>

	</head>
	<?php
		require 'configuration.php';

		// Create connection
		$conn = new mysqli($servername, $username, $password);

		// Check connection
		if ($conn->connect_error) {
			die("Connection failed: " . $conn->connect_error);
			} 
	 ?>
	 <script>
	 var stationsLayerCTD;
	 <?php
		
		
		$sql = "Select date_format(min(`Time(ISO8601)`),'%d-%b-%Y' ) as minimun, date_format(max(`Time(ISO8601)`),'%d-%b-%Y' ) as maximun from oceandata.measurement";

		$result = $conn->query($sql);
		
		$row = $result->fetch_assoc();
		
		echo "var maxTime='".$row['maximun']."';";
		
		echo "var minTime='".$row['minimun']."';";
		
		$sql = "Select max(`depth(m)`) as maximun , min(`depth(m)`) as minimun from oceandata.measurement";

		$result = $conn->query($sql);
		
		$row = $result->fetch_assoc();
		
		echo "var maxDepth='".$row['maximun']."';";
		
		echo "var minDepth='".$row['minimun']."';";
	 ?>
	 $(document).ready(function() {
	 $('#timefromtxt').prop("value",minTime);
	 $('#timetotxt').prop("value",maxTime); 
	 $('#depthfromtxt').prop("value",minDepth);
	 $('#depthtotxt').prop("value",maxDepth);
	 });
	 </script>
	<body>
<!-- begin template -->

<div class="navbar navbar-custom navbar-fixed-top">
 <div class="navbar-header"><a class="navbar-brand" href="#">Actions:</a>
      <a class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </a>
    </div>
    <div class="navbar-collapse collapse">
	<div style="width:auto;float: left;">
      <ul class="nav navbar-nav" style="overflow:auto;width:auto;">
        <li class="active" id="download_menu"><a href="#">Download Measurements</a></li>
        <li id="clear_map"><a href="#">Delete Shape</a></li> 	
		<li id="show_sql"><a href="#" data-toggle="modal" data-target="#modal">Show SQL</a></li>
		<li id="help"><a href="#" data-toggle="modal" data-target="#modal">Help</a></li>
		<li>&nbsp;</li>
	  </ul>
	  </div>
	  <div style="width:auto;text-align:right;float:right;padding:5px 5px 2px 0;"> <img src="oceandata_web_trans.png" alt="Smiley face" height="50" width="250">
	  </div>
       
    </div>
</div>




<div class="csontainer-fluid" id="main">
  <div class="clearfix">
	 <div class="col-xs-8" id="map-canvas"><!--map-canvas will be postioned here--></div>
  	<div class="col-xs-4" id="left">
	<form id="download" method="post" role="form" action="download.php" >
		<input type="hidden" name="sqltxt" id="sqltxt"> 
	</form>
    <div id="variablebox" class="controlbox">     
		<div class="panel panel-default">
			<div class="panel-heading">Measurements</div>
		</div>
      <!-- item list -->
	  <div class="panel panel-default indented" id="firstpanel">
	  <select multiple name="instruments" id="instruments" class="form-control">
	  <?php
		$sql = 'SELECT distinct InstrumentType from '.$database.'.`metadata`';

		$result = $conn->query($sql);
   
		if ($result->num_rows > 0) {
			// output data of each row
			while($row = $result->fetch_assoc()) {
				echo '<option>'.$row['InstrumentType'].'</option>';
			}
		}
		
		  

	  ?>
	  </select>


	  <select multiple id="Measurements" class="form-control">
		<?php 
			$sql="SELECT `COLUMN_NAME` 
			FROM `INFORMATION_SCHEMA`.`COLUMNS` 
			WHERE `TABLE_SCHEMA`='".$database."' AND `TABLE_NAME`='measurement' AND COLUMN_NAME Not in('Depth(m)', 'id', 'idEvent', 'Time(ISO8601)', 'Time(DecDay)', 'InstrumentDepth(m)', 'ExtendedVariables')";
			$result = $conn->query($sql);
   
			if ($result->num_rows > 0) {
				// output data of each row
				while($row = $result->fetch_assoc()) {
					echo '<option>'.$row['COLUMN_NAME'].'</option>';
				}
			}
		?>
	  </select>
	  </div>
    </div>  
      <hr>
    <div id="locationbox" class="controlbox">  
		  <div class="panel panel-default">
			<div class="panel-heading">Location</div>
		  </div>
		  
		<div id="LocationContent" class="indented" >
		<ul id="tabs" class="nav nav-tabs" data-tabs="tabs">
			<li id="Station-IDTng" class="active"><a href="#Station-ID" data-toggle="tab">Station-ID</a></li>
			<li id="PolygonTng" ><a href="#Polygon" data-toggle="tab">Polygon</a></li>
			<li id="Lat-Lon-RadiusTng"><a href="#Lat-Lon-Radius" data-toggle="tab">Lat-Lon</a></li>
		</ul>

		<div id="my-tab-content" class="tab-content">
			<div class="tab-pane active" id="Station-ID">
				<p>Write the Stations separated by commas:</p>
				<div class="input-group" id="divStationID">
					<input type="text" class="form-control" id="stationID" placeholder="None type-in selection yet  (use commas!)" aria-describedby="basic-addon2">
				</div>
				<br>
				<p>Or select/search one or more stations form the list:</p>
				<div class="panel panel-default">
				<div class="panel panel-default">
				  <select multiple id="stationIDs" class="form-control">
					<?php 
						$sql="Select distinct station from ".$database.".`metadata`";
						$result = $conn->query($sql);
			   
						if ($result->num_rows > 0) {
							// output data of each row
							while($row = $result->fetch_assoc()) {
								echo '<option>'.$row['station'].'</option>';
							}
						}
					?>
				  </select>
				</div>

				<p>Or click stations on the map (they will be checked on the list).</p>
			</div>
			</div>
			<div class="tab-pane" id="Polygon">
				<p>Draw a Polygon on the map. Coordinates will be shown below</p>
				<ul class="list-group" id="PolygonUL">
					<li class="list-group-item">
						<span class="badge">1</span>
						xx.xxx xx.xxx
					</li>
					<li class="list-group-item">
						<span class="badge">2</span>
						xx.xxx xx.xxx
					</li>
					<li class="list-group-item">
						<span class="badge">3</span>
						xx.xxx xx.xxx
					</li>
				</ul>
			</div>

			<div class="tab-pane" id="Lat-Lon-Radius">
				<p>Select a Latitude (degrees north), a Longitude (degrees east) and a Radius (kilometers) in the boxes below</p>
			<div class="input-group">
				<span class="input-group-addon" id="CircleLong" style="width: 35%;">Latitude</span>
				<input id="Latitude" type="number" class="form-control" placeholder="Decimal degrees only" aria-describedby="basic-addon1">
			</div>
			<div class="input-group">
				<span class="input-group-addon" id="basic-addon2" style="width: 35%;">Longitude</span>
				<input id="Longitude" type="number" class="form-control" placeholder="Decimal degrees only" aria-describedby="basic-addon1">
			</div>
			<div class="input-group">
				<span class="input-group-addon" id="basic-addon3" style="width: 35%;">Radius</span>
				<input id="radius" type="number" class="form-control" placeholder="0 for a single point search" aria-describedby="basic-addon1">
			</div>
			</div>
		</div>
	</div>
</div>	 

    <hr>
    <div id="timebox" class="controlbox">   
		<div class="panel panel-default">
			<div class="panel-heading">Time [dd-mmm-yyyy (ex. 01-Jan-1901)] </div>
		</div>
		<div class="form-check indented">
			  <label class="form-check-label">
				<input class="form-check-input" type="checkbox" id="AllHistData" value="All" checked>
				All available
			  </label>
		</div>
			
		<div class="indented boxcontent" id="timecontent" >

			<div class='input-group date' id='timefrom'>
				<span class="input-group-addon" style="width: 10%;">From</span>
				<input type='text' class="form-control" id="timefromtxt"/>
				
			</div>
			<div class="input_separator">
			</div>
			<div class='input-group date' id='timeto'>
				<span class="input-group-addon" style="width: 30%;">To</span>
				<input type='text' class="form-control" id="timetotxt" />

			</div>
		</div>
	</div>
	<hr>
	<div id="depthbox" class="controlbox">     
		<div class="panel panel-default">
			<div class="panel-heading">Depth [Meters below sea level]</div>
		</div>
		<div class="form-check indented">
			  <label class="form-check-label">
				<input class="form-check-input" type="checkbox" id="AllDepthData" value="All" checked>
				All available
			  </label>
		</div>
		<div class="indented boxcontent">
			<div class='input-group date' id='depthfrom'>
				<span class="input-group-addon" style="width: 30%;">From</span>
				<input type='number' class="form-control" id="depthfromtxt" />

			</div>
			<div class="input_separator">
			</div>
			<div class='input-group date' id='depthto'>
				<span class="input-group-addon" style="width: 30%;">To</span>
				<input type='number' class="form-control" id="depthtotxt" />
			</div>
		</div>
	</div>
      
      <hr>
	 <div id="actionbox" >
		<button type="button" class="btn btn-danger" id="btnReset" data-toggle="modal" data-target="#modal">Reset Filter</button>
		<button type="button" class="btn btn-primary" id="btnDownload">Download Measurements</button>

	 </div>
    

    </div>
   
    
  </div>
</div>
<div class="modal fade" tabindex="-1" role="dialog" id="modal">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title" id="modalTitle">SQL Statement</h4>
      </div>
      <div class="modal-body">
        <p id="modalMsg">nada</p>
      </div>
      <div class="modal-footer">
		<button type="button" class="btn btn-default" data-dismiss="modal">Cancel</button>
        <button type="button" class="btn btn-primary btn-default" id="modalBtn">OK</button>
      </div>
    </div><!-- /.modal-content -->
  </div><!-- /.modal-dialog -->
</div><!-- /.modal -->
<!-- end template -->
<?php $conn->close(); ?>

	</body>
</html>