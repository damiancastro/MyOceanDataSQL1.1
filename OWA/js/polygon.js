var stationsLayer = [];
$(document).on('pagebeforeshow', '#index', function(){       
    alert('Pagebeforeshow');
});
$(document).ready(function(){
/* Initialization of UI components ----------------------------------*/
google.maps.event.addDomListener(window, 'load', initialize);});
    
	$(document).ready(function() {
	
		$('#stations').multiselect({
			buttonWidth: '100%'
		});  
		$('#instruments').multiselect({
			buttonWidth: '145px',
			numberDisplayed: 10,
			maxHeight:300,
			includeSelectAllOption:true,
			nonSelectedText:"Add instruments"
		}); 

		$('#Measurements').multiselect({
			buttonWidth: '225px',
			numberDisplayed: 10,
			maxHeight:300, 	
			includeSelectAllOption:true,
			nonSelectedText:"Add variables"
		}); 
		$('#stationIDs').multiselect({
			//includeSelectAllOption:true,
			buttonWidth: '100%',
			numberDisplayed: 10,
			maxHeight:300,
			enableFiltering:true,
			enableCaseInsensitiveFiltering: true,
			filterPlaceholder:'Search for stations containing...',
			nonSelectedText:"None selected yet",
			onChange: toggleStationSelection
			//onSelectAll: function() {alert('onSelectAll triggered.');}
		});

		$('#timefromtxt').datetimepicker({
			useCurrent: false, //Important! See issue #1075
			viewMode: 'days',
			format: 'DD-MMM-YYYY'
		});
        $('#timetotxt').datetimepicker({
            useCurrent: false, //Important! See issue  	#1075
			viewMode: 'days',
			format: 'DD-MMM-YYYY'
        });
        $("#timefromtxt").on("dp.change", function (e) {
            $('#timetotxt').data("DateTimePicker").minDate(e.date);
			var fromDate = $("#timefromtxt").prop("value");
			var toDate = $("#timetotxt").prop("value");
			if (toDate!=""  || fromDate!="") $('#AllHistData').attr('checked', false); else $('#AllHistData').prop('checked', true);
        });
        $("#timetotxt").on("dp.change", function (e) {
            $('#timefromtxt').data("DateTimePicker").maxDate(e.date);
			var fromDate = $("#timefromtxt").prop("value");
			var toDate = $("#timetotxt").prop("value");
			if (fromDate!=""  || toDate!="") $('#AllHistData').attr('checked', false); else $('#AllHistData').prop('checked', true);
        });
		$( "#AllHistData" ).on("change", function(e) {
			if ($( "#AllHistData" ).prop("checked")) {
				$("#timetotxt").prop("value", maxTime ); 
				$("#timefromtxt").prop("value", minTime ); 
			}
		});
		$("#depthfromtxt").on("change", function (e) {
            var fromDepth = $("#depthfromtxt").prop("value");
			var toDepth = $("#depthtotxt").prop("value");
			if (fromDepth!=""  || toDepth!="") $('#AllDepthData').attr('checked', false); else $('#AllDepthData').prop('checked', true);
        });
        $("#depthtotxt").on("change", function (e) {
            var fromDepth = $("#depthfromtxt").prop("value");
			var toDepth = $("#depthtotxt").prop("value");
			if (fromDepth!=""  || toDepth!="") $('#AllDepthData').attr('checked', false); else $('#AllDepthData').prop('checked', true);
        });
		$( "#AllDepthData" ).on("change", function(e) {
			if ($( "#AllDepthData" ).prop("checked")) {
				$("#depthtotxt").prop("value", maxDepth ); 
				$("#depthfromtxt").prop("value", minDepth ); 
			}
		});
		$( "#instruments" ).on("change", function(e) {
			$( "#instruments > option" ).each(function() {
			if (this.selected) 
				drawMarkers(this.value);
			else
				unDrawMarkers(this.value);
				//stationsLayer[this.value].setMap(map); 
				//stationsLayer[this.value].setMap(null) ;
		});
		});
		$('#tabs').tab();
		
		$( "#download_menu" ).click(function() {
			$("#sqltxt").val(getSql());
			submitSQL();
		});
		$( "#btnDownload" ).click(function() {
			$("#sqltxt").val(getSql());
			submitSQL();
		});
		$( "#clear_map" ).click(function() {
			deleteSelectedShape();
		});
		$( "#show_sql" ).click(function() {
			$("#modalTitle").text("Sql Statement");
			$("#modalMsg").text(getSql());
			$("#modalBtn").click(function(){
				$("#modal").modal('hide');
			});
		});
		$( "#btnReset" ).click(function() {
			$("#modalTitle").text("Filter Reset");
			$("#modalMsg").text("Danger! All the information you have entered, including polygon, lat-lon and Station data will be lost.");
			$("#modalBtn").click(function(){
				resetFilters();
				$("#modal").modal('hide');
			});
		});
		$( "#help" ).click(function() {
			$("#modalTitle").text("Help");
			$("#modalMsg").text(help);
			$("#modalBtn").click(function(){
				$("#modal").modal('hide');
			});
		});
		
	setInstrumentIcons();
	});

	
	//Shape manipulation variables
	var ne, sw, vertices, r, c, shapes=[];
	var drawingManager;
    var selectedShape;
	
	var selectedMarkers=[];
	var instrumentMarkers=[];
	var instClass=[];
	var markerInfoWindows=[];

	
	//Filter Validation
	var polygonValid=false, circleValid=false;
	var map, infow;

	//Function used to change shape selection
      function clearSelection() {
        if (selectedShape) {
		if (selectedShape.type != google.maps.drawing.OverlayType.MARKER)
			selectedShape.setEditable(false);
          selectedShape = null;
        }
      }
	//Function to set the selection
      function setSelection(shape) {
        clearSelection();
        selectedShape = shape;
		if (shape.type != google.maps.drawing.OverlayType.MARKER) 
			shape.setEditable(true);
        
      }
	//Function to delete shape AND related controls so filter is processed accordingly
      function deleteSelectedShape() {
        if (selectedShape) {
			if (selectedShape.type=="polygon" || selectedShape.type=="rectangle")
			{
				polygonValid=false;
				$("#PolygonUL").html('<li class="list-group-item"><span class="badge">1</span>xx.xxx xx.xxx</li><li class="list-group-item"><span class="badge">2</span>xx.xxx xx.xxx</li><li class="list-group-item"><span class="badge">3</span>xx.xxx xx.xxx</li>');
			}
			else
			{
				circleValid=false;
				$("#Latitude").val("");
				$("#Longitude").val("");
				$("#radius").val("");
			}
			$.each(shapes, function (i, s){
				if (s.type==selectedShape.type)
					shapes.splice (i,1);
				selectedShape.setMap(null);
				selectedShape=null;
				});
      }
	  }
	//Intitialize will start map, kmz, shapes and all the events that this objects support
      function initialize() {
        map = new google.maps.Map(document.getElementById('map-canvas'), {
          zoom: 9,
          center: new google.maps.LatLng(47.676406, -55.38102955),
          mapTypeId: google.maps.MapTypeId.HYBRID,
          disableDefaultUI: false,
          zoomControl: true
        });
		
        var polyOptions = {
          strokeWeight: 0,
          fillOpacity: 0.45,
          editable: true
        };
        // Creates a drawing manager attached to the map that allows the user to draw
        // markers, lines, and shapes.
		  drawingManager = new google.maps.drawing.DrawingManager({
		  //drawingMode: google.maps.drawing.OverlayType.POLYGON,
          markerOptions: {
            draggable: true
          },
          polylineOptions: {
            editable: true
          },
          polygonOptions: polyOptions,
          map: map,
		  showFigures:0,
		  drawingControlOptions:{
			  position: google.maps.ControlPosition.TOP_RIGHT,
			  drawingModes:[google.maps.drawing.OverlayType.MARKER, google.maps.drawing.OverlayType.POLYGON, google.maps.drawing.OverlayType.RECTANGLE, google.maps.drawing.OverlayType.CIRCLE]}
        });

        google.maps.event.addListener(drawingManager, 'overlaycomplete', function(e) {
            
            // Switch back to non-drawing mode after drawing a shape.
            drawingManager.setDrawingMode(null);

           
			if ((e.type=="rectangle"|| e.type=="polygon")&& polygonValid==true)
			{
				alert("Only one shape at a time! Delete existing shap first.");
				e.overlay.setMap(null);
				e.overlay=null;
				return;
			}
			
			if ((e.type=="circle"|| e.type=="marker")&& circleValid==true)
			{
				alert("Only one shape at a time! Delete existing shape first.");
				e.overlay.setMap(null);
				e.overlay=null;
				return;
			}
			var newShape;
			newShape = e.overlay;
            newShape.type = e.type;
            google.maps.event.addListener(newShape, 'click', function() {
            setSelection(newShape);
            });
            setSelection(newShape);
			showNewShape();
			shapes.push(newShape);
			if (newShape.type=='polygon')
			{
				polygonValid=true;
				newShape.getPath().addListener('set_at', showNewShape);
				newShape.getPath().addListener('insert_at', showNewShape);
			}
			if (newShape.type=='rectangle')
			{
				polygonValid=true;
				newShape.addListener('bounds_changed', showNewShape);
			}
			if (newShape.type=='circle')
			{
				circleValid=true;
				newShape.addListener('radius_changed', showNewShape);
				newShape.addListener('center_changed', showNewShape);
			}
			if (newShape.type=='marker')
			{
				circleValid=true;
			}
        });

        // Clear the current selection when the drawing mode is changed, or when the
        // map is clicked.
		google.maps.event.addListener(map, 'click', clearSelection);

   }
   //Clear all filters
   function resetFilters(){
	   	$("#PolygonUL").empty();
		$("#Latitude").val("");
		$("#Longitude").val("");
		$("#radius").val("");
		$("#stationID").val("");
		$('#stationIDs').multiselect('deselectAll', false)
		$('#stationIDs').multiselect('updateButtonText')
		polygonValid=false;
		circleValid=false;
		$.each(shapes, function (i, s){
			s.setMap(null);
			s=null;
			});
		shapes=[];
			
   }
   //Submit SQL to download calling download.php
   function submitSQL(){
		if (circleValid || polygonValid || $("#stationIDs option:selected").length > 0 || $("#stationID").val()!="" || $("#timetotxt").val()!="" || $("#timefromtxt").val()!="" || $("#depthtotxt").val()!="" || $("#depthfromtxt").val()!="")
			$( "#download" ).submit();
		else{
			$("#modalTitle").text("Operation not Allowed");
			$("#modalMsg").text("It is not allowed to download data with no filter. This is to prevent downloading millions of rows and filling you hard disk");
			$("#modalBtn").click(function(){
				$("#modal").modal('hide');
			});
			$("#modal").modal()
		}
			
   }

//this function select/unselect station. To select brings lat-lon from db
 function toggleStationSelection(option, checked, select) {
	if (instrumentMarkers[$(option).val()]){
		if (checked){
			selectMarker($(option).val());
			}
		else{
			deselectMarker($(option).val());
			}
	}
 }
 
 function selectStationID(e){
	$('#stationIDs').multiselect('select', e);

 }
 
 function deselectStationID(name){
	$('#stationIDs').multiselect('deselect', name);
	 ridofMarker(name);
 }  

 function ridofMarker(name){
	 	$.each(selectedMarkers,function(i, m ){
		if (m)
		{
			if (m.name==name)
			{
				m.marker.setMap(null);
				m.marker=null;
				selectedMarkers.splice(i,1);
				return;
			}
		}
	});
 }
 //Activate tab
 function setLocTabActive(tabName)
 {	
 	$("#Lat-Lon-Radius").removeClass("active");
	$("#Station-ID").removeClass("active");
	$("#Polygon").removeClass("active");
	$("#Lat-Lon-RadiusTng").removeClass("active");
	$("#Station-IDTng").removeClass("active");
	$("#PolygonTng").removeClass("active");
	$(tabName).addClass("active");
	$(tabName+"Tng").addClass("active");
}
//This function produces the sql to send to server
function getSql()
{
	
	var fieldlist, fieldlistQ, spatialfilter, timefilter, instrumentfilter, wherelist, depthfilter, fst=true;
	
	$('#Measurements :selected').each(function(i, selected){
		if (fieldlist){
			fieldlistQ+= ", '"+$(selected).text()+"'";
			fieldlist+= ", m.`"+$(selected).text()+"`";
		}
		else
		{
			fieldlistQ= "'"+$(selected).text()+"'";
			fieldlist= "m.`"+$(selected).text()+"`";
		}	
	});
	
	$('#instruments :selected').each(function(i, selected){
		if (instrumentfilter)
			instrumentfilter += ",'"+$(selected).text()+"'";
		else
			instrumentfilter="InstrumentType in('" +$(selected).text()+"'";
	});
	if (instrumentfilter)
		instrumentfilter+=")";
	else 
		instrumentfilter="InstrumentType in('')" 

	switch ($("#tabs li.active").attr("id"))
	{
		case "PolygonTng":
			if (polygonValid)
			{
				spatialfilter="st_Within(e.position, geomfromtext('POLYGON((" ;
				var fstVertice="";
				
				$('ul#PolygonUL li').each(function(i, li){
					if (i==0) 
						fstVertice=li.childNodes[1].data;
					
					spatialfilter+=li.childNodes[1].data + ",";
					
				});
				spatialfilter+=fstVertice + "))')) ";
			}
			break;
		case "Lat-Lon-RadiusTng":
			if (circleValid)
			{
				spatialfilter="st_Within(e.position, getpolygon("+ $("#Latitude").val() + ","+$("#Longitude").val()+","+$("#radius").val() +","+ "10))";

			}
			break;
		case "Station-IDTng":
			var spatialfiltertxt;
			if ($("#stationID").val()!="")
			{
				$.each($("#stationID").val().split(","),function(i,stationID){
					if (i!=0)
						spatialfiltertxt+=",";
					else
						spatialfiltertxt="";
					spatialfiltertxt+="'"+stationID.trim()+"'";
				});
			}
			var spatialfilterList;
			$('#stationIDs :selected').each(function (i,selected){
				if (i!=0)
					spatialfilterList+=",";
				else
					spatialfilterList="";
				spatialfilterList+="'"+$(selected).text()+"'";
			});
			if (spatialfiltertxt)
			{
				spatialfilter=spatialfiltertxt;
				if (spatialfilterList)
					spatialfilter+=","+spatialfilterList;
				
			}
			else
				if (spatialfilterList)
					spatialfilter=spatialfilterList;
			if (spatialfilterList||spatialfiltertxt)
				spatialfilter="Station in ("+spatialfilter+")";
			break;
	}
	
	if ($("#timefromtxt").val())
	{	
		timefilter="`Time(ISO8601)` >= str_to_date('" + $("#timefromtxt").val() +"', '%d-%b-%Y')"
	}
	
	if ($("#timetotxt").val())
	{	
		if (timefilter)
			timefilter+=" and `Time(ISO8601)` <= str_to_date('" + $("#timetotxt").val() +"', '%d-%b-%Y')"
		else
			timefilter="`Time(ISO8601)` <= str_to_date('" + $("#timetotxt").val() +"', '%d-%b-%Y')"
	}
	
	if ($("#depthfromtxt").val())
	{	
		depthfilter="`Depth(m)` >=" + $("#depthfromtxt").val();
	}
	
	if ($("#depthtotxt").val())
	{	
		if (depthfilter)
			depthfilter+=" and `Depth(m)` <=" + $("#depthtotxt").val();
		else
			depthfilter="`Depth(m)` <=" + $("#depthtotxt").val();
	}
	
	if (instrumentfilter)
		wherelist=instrumentfilter;
	
	if (spatialfilter){
		if (wherelist)
			wherelist+=" and " +spatialfilter;
		else
			wherelist= spatialfilter;
	}
	if (timefilter){
		if (wherelist)
			wherelist+=" and "+timefilter;
		else
			wherelist= timefilter;
	}
	
	if (depthfilter){
		if (wherelist)
			wherelist+=" and "+depthfilter;
		else
			wherelist= depthfilter;
	}
	
	if (wherelist)
		wherelist=" where "+wherelist;
	else
		wherelist="";
	
	var eventFields= "`Station`, `Longitude(DecDeg)`, `Latitude(DecDeg)`, `MaxDepth(m)`,  `InstrumentType`, `Instrument`,   `Time(ISO8601)`,`Depth(m)`";
	var eventFieldsQ= "'Station', 'Longitude(DecDeg)', 'Latitude(DecDeg)','MaxDepth(m)' , 'InstrumentType','Instrument',  'Time(ISO8601)', 'Depth(m)'";
	
	if (fieldlist) 
		fieldlist=eventFields+","+fieldlist; 
	else 
		fieldlist=eventFields;
	
	if (fieldlistQ) 
		fieldlistQ=eventFieldsQ+","+fieldlistQ; 
	else 
		fieldlistQ=eventFieldsQ;

	var unionall= "Select "+ fieldlistQ + " union all ";
	return unionall+"Select "+fieldlist+" from measurement m join metadata e on e.IdEvent=m.idEvent "+wherelist;
	
}
//Shows the selected shape
function showNewShape()
{
	if (selectedShape.type=='polygon')
		{
			  // Since this polygon has only one path, we can call getPath() to return the
			  // MVCArray of LatLngs.
			vertices = selectedShape.getPath();
			$("#PolygonUL").empty();

			// Iterate over the vertices.
			for (var i =0; i < vertices.getLength(); i++) {
	   
				var xy = vertices.getAt(i);
				$("#PolygonUL").append('<li class="list-group-item"><span class="badge">'+(i+1)+'</span>'+ xy.lat().toFixed(6) + ' ' + xy.lng().toFixed(6) + "</li>");
			}
			setLocTabActive("#Polygon");
			polygonValid=true;
		}
	if (selectedShape.type=='rectangle')
		{
			$("#PolygonUL").empty();
			var ne = selectedShape.getBounds().getNorthEast();
			var sw = selectedShape.getBounds().getSouthWest();
			//display rect bounds
			$("#PolygonUL").append('<li class="list-group-item"><span class="badge">North-East</span>'+ ne.lat().toFixed(6) + ' ' + ne.lng().toFixed(6)+ '</li>');
			$("#PolygonUL").append('<li class="list-group-item"><span class="badge">North-West</span>'+ ne.lat().toFixed(6) + ' ' + sw.lng().toFixed(6)+ '</li>');
			$("#PolygonUL").append('<li class="list-group-item"><span class="badge">South-West</span>'+ sw.lat().toFixed(6) + ' ' + sw.lng().toFixed(6)+ "</li>");
			$("#PolygonUL").append('<li class="list-group-item"><span class="badge">South-East</span>'+ sw.lat().toFixed(6) + ' ' + ne.lng().toFixed(6)+ '</li>');
			setLocTabActive("#Polygon");

		}
	if (selectedShape.type=='circle')
		{
			r = selectedShape.getRadius(),
			c = selectedShape.getCenter();
			$("#Latitude").val(c.lat().toFixed(6));
			$("#Longitude").val(c.lng().toFixed(6));
			var rkm=r/1000;
			$("#radius").val(rkm.toFixed(3));
			setLocTabActive("#Lat-Lon-Radius");
		}
	if (selectedShape.type=='marker')
		{
			m = selectedShape.getPosition(),
			$("#Latitude").val(m.lat().toFixed(6));
			$("#Longitude").val(m.lng().toFixed(6));
			$("#radius").val(1);
			setLocTabActive("#Lat-Lon-Radius");
		}
	
}
//draw the marker from the database using ajax and json
function drawMarkers(it)
{
	$.each(instClass,function(i, c ){
		if (c.name==it){
			if (c.loaded==false){
				$.ajaxSetup({async: false});
				$.getJSON( "getMarkerVector.php?InstrumentType="+it, function( data )
				{
					oldLength=instrumentMarkers.length;
					var tempdata=data
					//instrumentMarkers=instrumentMarkers.concat(data);
					for (var i = 0; i < tempdata.length; i++) {
						instrumentMarkers[tempdata[i].title]=tempdata[i];
						instrumentMarkers[tempdata[i].title].map=map;
						instrumentMarkers[tempdata[i].title].icon=c.icon;
						instrumentMarkers[tempdata[i].title].label=c.label;
						instrumentMarkers[tempdata[i].title].position={lat:Number(instrumentMarkers[tempdata[i].title].lat), lng:Number(instrumentMarkers[tempdata[i].title].lng)};
						instrumentMarkers[tempdata[i].title].marker= new google.maps.Marker(instrumentMarkers[tempdata[i].title]);
						if ($("#stationIDs > option:contains('"+tempdata[i].title+"')").prop('selected')){
							instrumentMarkers[tempdata[i].title].marker.icon.strokeWeight=3;
							instrumentMarkers[tempdata[i].title].marker.setMap(map);
							}
						instrumentMarkers[tempdata[i].title].marker.addListener('mouseover', function() {
							var mrk=this.title;
							if (!instrumentMarkers[mrk].infoWindow)
							{
								$.getJSON( "getInfoW.php?InstrumentType="+instrumentMarkers[mrk].label.text+"&Station="+mrk, function( data )	{
									var iws=data;
									var content;
									var divhdr="<div id='content' >"+ 
										"<strong>";
									var coor="</strong>";
									var tablehdr="<div id='bodyContent'>"+
											"<p><table class='table table-striped table-sm table-condensed table-bordered'>"+
											"<theadclass='thead-default'>	<tr><td >Deployment</td><td >Recovery</td><td>Depth</td></tr></thead><tbody>";
									var tail="</tbody></p></div></div>";
									var tabledata="";
									for (var i in iws) {
										tabledata=tabledata+'<tr><td>'+iws[i].deploy+'</td><td>'+iws[i].recover+'</td><td align="center">'+iws[i].depth+'</td><</tr>'
									}
									var lat = Number(instrumentMarkers[mrk].lat);
									var lng = Number(instrumentMarkers[mrk].lng);
									contentStr=divhdr + instrumentMarkers[mrk].title + '['+ instrumentMarkers[mrk].label.text + '] @ '+ coor+ lat.toFixed(3)+' '+lng.toFixed(3)+tablehdr+ tabledata+ tail;
									instrumentMarkers[mrk].infoWindow= new google.maps.InfoWindow({content: contentStr});
									}
								)
							}
								instrumentMarkers[mrk].infoWindow.open(map, this);
							});
							
						instrumentMarkers[tempdata[i].title].marker.addListener('mouseout', function() {
							instrumentMarkers[this.title].infoWindow.close();
						});
						
						instrumentMarkers[tempdata[i].title].marker.addListener('click', function() {
							var mrk=this.title;
							if (instrumentMarkers[this.title].marker.icon.strokeWeight==4){
								deselectMarker(this.title);
								$("#stationIDs option").each(function(i, o ){ 
						
								if (o.value==mrk){
									deselectStationID(o.value);
									return;
								}
							});
							}
							else {
								selectMarker(this.title);
								$("#stationIDs option").each(function(i, o ){ 
						
								if (o.value==mrk)
								{
									selectStationID(o.value);
									return;
								}
							} );
								
							}
						
						});
					}
				});
				c.loaded=true;
			}
			else{
				for (var i in instrumentMarkers) {
					if (instrumentMarkers[i].label.text==it)
					{
					if ($("#stationIDs option").checked)
						selectMarker(this.title);
 					}
				}
			}
		}
	});
}
//hide markers (still in memory)
function unDrawMarkers(it)
{
	for (var i in instrumentMarkers) {
		if (instrumentMarkers[i].label.text==it)
		{
			instrumentMarkers[i].marker.setMap(null);
		}
	}
		$.each(instClass,function(i, c ){if (c.name==it) c.loaded=false;});
}	

//set color/shape/stroke for icons and labels		
function setInstrumentIcons(){
		
		var iconColors=["red", "Aquamarine", "Blue", "Chocolate", "Cyan", "DarkMagenta","DarkSeaGreen" , "DarkSlateGray", "DeepPink", "Gold", "Indigo","LightCoral" ];
		var labelColors=["White", "Black", "White", "Black", "Black", "White","White" , "White", "Black", "Black", "White","Black" ];
		
		$("#instruments > option").each(function(i){
			if (i<13){
				instClass.push({
					name:$(this).text(),
					icon: {path:google.maps.SymbolPath.CIRCLE /*"M-1,-0.95a1,1 0 1,0 2,0a1,1 0 1,0 -2,0"*/,
						scale : 10,
						strokeColor:labelColors[i],
						//strokeColor:'White',
						strokeWeight:1,
						fillColor:iconColors[i],
						fillOpacity:0.6},
					label:{text:$(this).text() ,
						fontSize:'7px',
						fontWeight:'bold',
						color:labelColors[i]},
					loaded:false});

			}
		 });
}

function selectMarker(marker){
	instrumentMarkers[marker].marker.icon.strokeWeight=4;
	instrumentMarkers[marker].marker.icon.strokeColor="Yellow";
	instrumentMarkers[marker].marker.icon.scale = 12;
	instrumentMarkers[marker].marker.setMap(map);
}

function deselectMarker(marker){
	instrumentMarkers[marker].marker.icon.strokeWeight=1;
	instrumentMarkers[marker].marker.icon.strokeColor=instrumentMarkers[marker].marker.label.color;
	instrumentMarkers[marker].marker.icon.scale = 10;
	var clr;
	$(instClass).each(function(i,cls){
		if (cls.name==instrumentMarkers[marker].label.text)
			clr=cls.label.color;
	});
	instrumentMarkers[marker].marker.icon.strokeColor=clr;
	instrumentMarkers[marker].marker.setMap(map);
}
		
		
		
		
		
		