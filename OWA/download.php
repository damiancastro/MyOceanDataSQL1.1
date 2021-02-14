<?php
		
	//This file contains the download proc. It receives a SQL sentence as parameter. It returns a file.
	require 'configuration.php';
	mysqli_report(MYSQLI_REPORT_STRICT	);
	$sql = $_POST['sqltxt'];
	$res='<html lang="en">
		<head>
			<meta http-equiv="content-type" content="text/html; charset=UTF-8">
			<meta charset="utf-8">
			<title>Aquaculture Query Application</title>
			<meta name="generator" content="Bootply" />
			<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
			<link href="css/bootstrap.min.css" rel="stylesheet">
			<link href="css/bootstrap-multiselect.css" rel="stylesheet">
			<link href="css/bootstrap-datetimepicker.css" rel="stylesheet">
			<!--[if lt IE 9]>
				<script src="//html5shim.googlecode.com/svn/trunk/html5.js"></script>
			<![endif]-->
			<link href="css/styles.css" rel="stylesheet">
			<!-- script references -->
			<script src="//ajax.googleapis.com/ajax/libs/jquery/2.0.2/jquery.min.js"></script>
			<link href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.10.2/themes/ui-lightness/jquery-ui.css" rel="stylesheet">
				<!-- script references -->
			<script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.10.2/jquery-ui.min.js"></script>
			<script src="js/bootstrap.min.js"></script>
			<script type="text/javascript" src="http://maps.googleapis.com/maps/api/js?libraries=drawing"></script>


		</head>';
	try{
		// Create connection
			
		$conn = new mysqli($servername, $username, $password, $database);
		$fst=0;
		
//		$file="/srv/www/htdocs/oceandata/files/data".TIME().".csv";			// e.g. using Linux server
		$file="C:/wamp64/tmp/data".TIME().".csv"; 							// e.g. using WAMP server
		$sql .=" into outfile '".$file."'	FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n'";
		//echo $sql; die;
		$result=$conn->query($sql);
		if ($result){
			$conn->close();
			register_shutdown_function('displayFatalError');
			header("Content-Description: File Transfer"); 
			header("Content-Type: text/csv"); 
			header("Content-Disposition: attachment; filename=\"".$file."\""); 
			header("Content-Disposition", "attachment;filename=myfilename.csv");
			header('Connection: close');
			$handle=fopen($file, 'rb');
			ob_start();
			while (!feof($handle)) {
				//Sends the file in chunks, so no memory issues
				$buffer = fread($handle, 20971520);
				$buffer=str_replace("\N","NaN",$buffer);
				echo $buffer;
				ob_flush();
				flush();
			}
			fclose($handle);
			$ret=unlink($file);
			exit();
		}
			
	}
		

		
	catch (Exception $e) {
		displayError($e->getMessage());
	}

function displayError($error)
{
	global $sql, $res;
	echo $res;
?>

	<body>

		<div class="navbar navbar-custom navbar-fixed-top">
		 <div class="navbar-header"><a class="navbar-brand" href="#">Error in the query processing</a>
			  <a class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
				<span class="icon-bar"></span>
				<span class="icon-bar"></span>
				<span class="icon-bar"></span>
			  </a>
			</div>
			<div class="navbar-collapse collapse">
			  <ul class="nav navbar-nav">
				<li class="active" id="try_again"><a href="#">Try again</a></li>
				<li id="back"><a href="#">Go back</a></li> 	
				<li>&nbsp;</li>
			  </ul>
			  
			</div>
		</div>
		<hr>
		<div class="col-lg-12" id="error">
			<div class="alert alert-danger" role="alert">
				<h3>Error in the processing of the query, sorry.</h3>
				<h4>The error is :</h4>
				<p>
				<?php echo $error;?>
				</p>
			</div>
			<hr>
			<div class="alert alert-info" role="alert">
				<h3>The T-SQL (database language) statement you just executed is:</h3>
				<p><?php echo $sql;?><p>
				<h3>Please send this statement along with the error message to your database administrator.</h3>
			</div>
		</div>
		<hr>


	</body>
		<script>
			$(document).ready(function(){
				$( "#try_again" ).click(function() {
					location.reload();
				});
				$( "#back" ).click(function() {
					window.history.back();
				});
			});

		</script>
</html>
<?php 
	}
function displayFatalError()
{	
global $res;	
$error=error_get_last();
if (!$error) exit();
echo $res;

?>


	<body>

		<div class="navbar navbar-custom navbar-fixed-top">
		 <div class="navbar-header"><a class="navbar-brand" href="#">Error in the query processing</a>
			  <a class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
				<span class="icon-bar"></span>
				<span class="icon-bar"></span>
				<span class="icon-bar"></span>
			  </a>
			</div>
			<div class="navbar-collapse collapse">
			  <ul class="nav navbar-nav">
				<li id="back"><a href="#">Go back</a></li> 	
				<li>&nbsp;</li>
			  </ul>
			  
			</div>
		</div>
		<hr>
		<div class="col-lg-12" id="error">
			<div class="alert alert-danger" role="alert">
				<h3>Fatal error in the query processing, sorry.</h3>
			</div>
		</div>
		<hr>


	</body>
		<script>
			$(document).ready(function(){
				$( "#try_again" ).click(function() {
					location.reload();
				});
				$( "#back" ).click(function() {
					window.history.back();
				});
			});

		</script>
</html>
<?php
}
?>