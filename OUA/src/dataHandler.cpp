/****************************************************************
File: dataHandelr.cpp
Author: Damián Castro
dataHandelr contains all the implementation for the dataHandler
class. This class contains  all the functionality to read
and upload data to the store (mysql). The form frmOceanData has to
instantiate an object of this class to process a file or a folder
*****************************************************************/

#include "StdAfx.h"
#include "dataHandler.h"

using namespace OceanDataHandler;

//dataHandler constructor. Connection to db is created here
dataHandler::dataHandler(System::String ^ confFile){
	
	try{

		initXMLDoc(confFile);
	}
	catch (Exception ^ e){

		MessageBox::Show(e->Message);

	}
}

//dataHandler destructor. Connection to db is closed here. Also temp files are to be deleted.
 dataHandler::~dataHandler()
 {
	 if (odbc)
		 odbc->Close();
	 if (origFile != fileName) 
		 if (File::Exists(path+"//"+ fileName)) 
			 File::Delete(path +"//"+ fileName);
 }

//initXMLDoc: load xml parameter file
System::Int32 dataHandler::initXMLDoc(System::String ^ confFile){
		
	xmlDoc = (gcnew XmlDocument());

	if (!confFile) confFile==L"config.xml";

	xmlDoc->PreserveWhitespace = true;

	xmlDoc->Load(confFile);

	return 0;
}
//getParamValue: use this fhuntion to get info from the xml file
System::String ^ dataHandler::getParamValue (System::String^ xmlpath){
	try	{
		XmlNode^ xmlNodeParam = xmlDoc->SelectSingleNode(xmlpath);

		if (xmlNodeParam) {
			switch (xmlNodeParam->NodeType)
			{
			case XmlNodeType::Element:
				return xmlNodeParam->InnerText;
			case XmlNodeType::Attribute:
				return xmlNodeParam->Value;
			default:
				return "";
			}
		}
		else
			return "";

	}
	catch (Exception^ e){
		if (batchMode)
			writeLog(e->Message);
		return "";
	}

}
//setMetadataDefArray: Set the array with the metadata info provide in the xml file
System::Int32  dataHandler::setMetadataDefArray (){
	try	{
		String ^ selectNode;
		XmlNode ^ attr;
		String^ xpath;
		String ^file="";
		int iMetadata = 0;

		md = gcnew array<Metadata>(0);

		metadataStyle = "EventTable";
		
		iMetadata += 1;
		
		Array::Resize(md, iMetadata);
		
		md[iMetadata - 1].Name = "InstrumentType";
		
		md[iMetadata - 1].Value = instType;
		
		xpath = L"/oceandata/schemas/schema[@name='" + schemaName + "']/metadata/";
		
		XmlNodeList^ xmlNodeSources = xmlDoc->SelectNodes(xpath+ "*");

		String^fldSel = "field[@name='";

		for each (XmlNode^ n in  xmlNodeSources) {

			iMetadata+=1;
			
			Array::Resize (md,iMetadata);

			md[iMetadata - 1].Name = n->Attributes->GetNamedItem("name") ?
				n->Attributes["name"]->Value : nullptr;

			md[iMetadata - 1].FieldName = n->Attributes->GetNamedItem("FieldName")?
				n->Attributes["FieldName"]->Value:nullptr;

			md[iMetadata - 1].Type = n->Attributes->GetNamedItem("type") ? 
				n->Attributes["type"]->Value : nullptr;

			md[iMetadata - 1].Table = n->Attributes->GetNamedItem("table") ? 
				n->Attributes["table"]->Value : nullptr;

			md[iMetadata - 1].Function = n->Attributes->GetNamedItem("function") ?
				n->Attributes["function"]->Value : nullptr;

			md[iMetadata - 1].ParameterStr = n->Attributes->GetNamedItem("parameterstr") ? 
				n->Attributes["parameterstr"]->Value : nullptr;

			md[iMetadata - 1].DataTable = n->Attributes->GetNamedItem("datatable") ? 
				n->Attributes["datatable"]->Value : nullptr;

			md[iMetadata - 1].Destination = n->Attributes->GetNamedItem("destination") ? 
				n->Attributes["destination"]->Value : nullptr;

			md[iMetadata - 1].destFunction = n->Attributes->GetNamedItem("destfunction") ? 
				n->Attributes["destfunction"]->Value : nullptr;

			md[iMetadata - 1].functionType = n->Attributes->GetNamedItem("functionType") ? 
				n->Attributes["functionType"]->Value : nullptr;

			md[iMetadata - 1].Value = n->Attributes->GetNamedItem("value") ? 
				n->Attributes["value"]->Value : nullptr;
		}
		return 1;
	}
	catch (Exception^ e){
		
		if (batchMode)
			writeLog(e->Message);
		else 
			MessageBox::Show(e->Message);

		return 0;
	}

}


String ^ dataHandler::ridOfBrackets (String ^name)
{
	String ^pattern =  "\\[|\\]|`";
	String ^replacement = "";
	Regex ^rgx = gcnew Regex(pattern);

	return rgx->Replace(name, replacement);
}
//this was used to prevent a data file being loaded more than once. It is not used
Int32 dataHandler::fileLoaded2DB( String ^fName){

	Common::DbDataReader ^ drFiles;
	
	drFiles = getReader("SELECT name from files;");

	while (drFiles->Read()){
		if (drFiles->GetString(0)==fName){
			odbc->Close();
			return 1;
		}
	}
	return 0;
	odbc->Close();
}
//check if filed exist
bool dataHandler::fieldExist(DataTable ^ table, String^ name){

	for each (DataColumn ^c in table->Columns)
	{
		if (name==c->ColumnName)
		{
			return 1;
		
		}
	}
	return 0;
}

System::Int32 dataHandler::numbDestExist( array<String^>^ strarray , String ^ destName ){

	for(int i=0; i<strarray->Length; i++) if (strarray[i] == destName) return true;
	return false;


}
//processField: builds the field call with function, and alias if their specified in the xml
System::String ^ dataHandler::processField(String ^ fieldNode){

	String ^ sqlFieldList;
	XmlNode ^ nf= xmlDoc->SelectSingleNode(fieldNode);
	String ^ selectNode = fieldNode + L"/@function";
	XmlNode ^ attrFunc = xmlDoc->SelectSingleNode(selectNode);
	
	selectNode = fieldNode + L"/@name";
	
	XmlNode ^ attrName = xmlDoc->SelectSingleNode(selectNode);
	String ^fieldName= attrName ? attrName->Value : nf->Name;
	
	sqlFieldList += attrFunc ? attrFunc->Value + "([" + fieldName + "])" : "[" +fieldName +"]";
	
	selectNode = fieldNode + L"/@sufix";
			
	XmlNode ^ attrSep = xmlDoc->SelectSingleNode(selectNode);

	if (attrSep) sqlFieldList += "+'"+attrSep->Value+"'";

	return sqlFieldList;
}
//getTable: ge table object from db
System::Data::DataTable^ dataHandler::getTable (String^ connStr, String^ sqlcmd){
	
	// Create a new data adapter based on the specified query.

	DataTable^ table;
	
	if (defaultConn == "ODBC") {

		dataAdapter = (gcnew System::Data::Odbc::OdbcDataAdapter(sqlcmd, connStr));

		table = gcnew DataTable();

		dataAdapter->Fill(table);
	}
	else{
		//dataAdapter = gcnew MySqlDataAdapter(sqlcmd, connStr);

		//table = gcnew DataTable();

		//dataAdapter->Fill(table);

	}



	DataTable^ table2 = table->Copy();

	return table;
}
//getTableOLE: ge oledb table object from db
System::Data::DataTable^ dataHandler::getTableOLE (String^ connStr, String^ sqlcmd){
			
	// Create a new data adapter based on the specified query.
	dataAdapterOLE = (gcnew System::Data::OleDb::OleDbDataAdapter(sqlcmd, connStr));
	
	DataTable^ table = gcnew DataTable();
     
	dataAdapterOLE->Fill(table);
	
	//DataTable^ table2 = table->Copy();

	return table;
}

//processInserts: most of the action happens here: this functuion wiull process all the rows in the input field
//and create an insert statment. It will also call the after procedure.
System::Int32  dataHandler::processInserts (DataTable ^sourceTable){

	String ^cmdTxt;

	try {

		String ^insert;

		Odbc::OdbcCommand ^cmd= gcnew Odbc::OdbcCommand;

		cmd->Connection=odbc;

		if (!destTrans)
			destTrans=odbc->BeginTransaction();

		cmd->Transaction=destTrans;

		String ^insertSQLMD=L"";

		String ^valuesSQLMD=L"";

		String ^dataTableName,^dataTableExtraFields, ^dataTableExtraValues;
		
		writeLog("Processing variables started at "+ DateTime::Now+".");

		if (metadataStyle=="EventTable")
		{
			String ^ proc= getParamValue("/oceandata/schemas/*[@name='"+schemaName+"']/afterProcedure");

			if (proc!="")
			{
				cmd->CommandText="DROP TABLE IF EXISTS tempmeasurement;";

				cmd->ExecuteNonQuery();
				
				String ^cmdTempTable = "CREATE TABLE `tempmeasurement` like measurement;";
			
				cmd->CommandText=cmdTempTable;

				cmd->ExecuteNonQuery();

				cmdTempTable = "ALTER TABLE `tempmeasurement` CHANGE COLUMN `id` `id` BIGINT(20) NULL , Drop Primary Key;";

				cmd->CommandText = cmdTempTable;

				cmd->ExecuteNonQuery();

			
				dataTableName="tempmeasurement";

			}
			else
			{
			dataTableName=L"measurement";
			dataTableExtraFields="";
			dataTableExtraValues= "";
			}
		}
		else
		{
			dataTableName=L"data";
			dataTableExtraFields=",idMetadata,Instrument";
			dataTableExtraValues= ", "+lastMetadataID + ",'"+schemaName+"'";
		}
		// This has to be done in the caller if it has interface label1->Visible=false;
		
		// This has to be done in the caller if it has interface progressBar1->Visible=true;
		p->Visible=true;

		p->Value=1;
		

		for each (Metadata m in md)	{
			if (m.DataTable)
			{
				insertSQLMD+=","+ m.DataTable;

				String ^ strValue ="";

				if ( m.Type=="string"|| m.Type == "datetime")
				{

				strValue="\'" + m.Value + "\'";
				}
				else
				{
					if (m.Value) strValue= m.Value; else strValue="null";
				}

				if (m.Function){
		
					if (m.ParameterStr) strValue+=",\'" + m.ParameterStr + "\'";

					strValue=m.Function + "(" + strValue + ")";
					
				}
	
						valuesSQLMD+="," + strValue;
			}
		}

		DateTime date1;
		DateTime date2;
		TimeSpan timespan1;
		TimeSpan timespan2;

		float rowcount;

		float  currow=0;

		rowcount=sourceTable->Rows->Count;

		String ^strBuffer=getParamValue("oceandata/destination/batchsize");

		int cmdBuffer=Convert::ToInt32(strBuffer->ToString());

		int commands=0;

		bool fst=1;

		String ^insertSQL=L"Insert into "+dataTableName+ " (";

		int index;

		for each (DataColumn ^c in sourceTable->Columns)
		{
			index = getDataIndex(c->ColumnName);
			
			if (index != -1) {
				if (fst == 1)
					fst=0; 
				else 
					{insertSQL+=L","; }

				insertSQL += "`";

				insertSQL += (d[index].destination && d[index].destination !="")? d[index].destination : c->ColumnName;

				insertSQL += "`";
			}
		}

		insert = insertSQL + insertSQLMD + dataTableExtraFields+") ";


		for each (DataRow ^r in sourceTable->Rows)
		{
	
			String ^valuesSQL=L"";
			String ^valueSQL="";
			String ^paramSQL="";

			currow+=1;

			bool fst=1;

			date1=DateTime::Now;

			for each (DataColumn ^col in sourceTable->Columns)
			{
				valueSQL = "";
				
				index = getDataIndex(col->ColumnName);

				if (index != -1)
				{

					if (fst == 1)
						fst = 0;
					else
					{
						valuesSQL += L",";
					}

					String ^ dataR = r[col]->ToString();

					Object ^xx = r[col];

					if (d[index].idEventFunction)
					{
						valueSQL += r[col];
					}
					else 
					{
						if (d[index].Type == "number")
							{
								if (r->IsNull(col))
									valueSQL = "null";
								else
								{
									if (r[col]->ToString()->Length > 2)
									{
										if (r[col]->ToString()->Substring(0, 3) == "NaN")
											valueSQL = "null";
										else
											valueSQL = r[col]->ToString();
									}
									else
										valueSQL = r[col]->ToString();
								}
							}

						else if (col->DataType == System::Type::GetType("System.String") || d[index].Type == "string" || d[index].Type == "datetime")
						{
							if (r[col]->ToString()->Length > 2)
							{
								if (r[col]->ToString()->Substring(0, 3) == "NaN")
									valueSQL = "null";
								else
									valueSQL = L"\"" + r[col] + L"\"";
							}
							else
								valueSQL = L"\"" + r[col] + L"\"";
							
						}
					
						if (d[index].Function)
						{
							if (valueSQL != "null")
							{
								if (d[index].ParameterStr)
									paramSQL = "," + "\"" + d[index].ParameterStr + "\"";
								valueSQL = d[index].Function + "(" + valueSQL + paramSQL + ")";
							}
						}
					}
				}
					valuesSQL+=valueSQL;

				}

			if (commands>0)
				cmdTxt+=L",( "+valuesSQL + valuesSQLMD +dataTableExtraValues+L")\n";
			else 
				cmdTxt+=L"( "+valuesSQL + valuesSQLMD +dataTableExtraValues + L")\n";

			commands+=1;
			date2= DateTime::Now;
			timespan1+= date2-date1;
			date1= DateTime::Now;

			if (commands==cmdBuffer)
			{
				
				cmdTxt=insert + "Values"+ cmdTxt;
				cmd->CommandText=cmdTxt;
				cmd->ExecuteNonQuery();
				commands=0;
				cmdTxt="";
			}
			date2= DateTime::Now;
			timespan2+= date2-date1;
			p->Value=(currow/rowcount)*100;
			Application::DoEvents();
		}
		if (commands>0)
		{
			date1= DateTime::Now;
			cmdTxt=insert + "Values"+ cmdTxt;
			cmd->CommandText=cmdTxt;
			cmd->ExecuteNonQuery();
			commands=0;
			cmdTxt="";
			date2= DateTime::Now;
			timespan2+= date2-date1;
		}
		p->Value=100;

		TimeSpan timespan3=timespan2+timespan1;
		
		cmdTxt="Insert into files(fileName, folder, name) Values('"+ path->Replace("\\","\\\\") + origFile + "','"+ path->Replace("\\","\\\\") + "', '" + origFile+"')";
		cmd->CommandText=cmdTxt;
		cmd->ExecuteNonQuery();
		String ^ proc= getParamValue("/oceandata/schemas/*[@name='"+schemaName+"']/afterProcedure");
		if (proc!="")
		{
			writeLog("Runing after procedure at "+DateTime::Now+ ".");
			p->Value=10;
			cmd->CommandText="call "+ proc + ";";
			cmd->ExecuteNonQuery();
			p->Value=50;
			cmd->CommandText = "INSERT INTO `measurement` Select  * from tempmeasurement;";
			writeLog("Doing final inserts at "+DateTime::Now+ ".");
			cmd->ExecuteNonQuery();
			cmd->CommandText = "DROP TABLE `tempmeasurement`;";
			cmd->ExecuteNonQuery();
			p->Value=100;

		}
		
		destTrans->Commit();

		// this is to be done by the calss destructor odbc->Close();
		String^ message="Update complete on "+DateTime::Now+". Data Rows updated = " + currow + ". Total time: " + System::Convert::ToInt32(timespan3.TotalSeconds) + " seconds. Time elapsed at mysql: " +  System::Convert::ToInt32(timespan2.TotalSeconds)+ " seconds.";
		
		if (batchMode)
			writeLog(message);
	
		return true;
	}

	catch (Exception ^e) {
		
		if (batchMode)
			writeLog(e->Message);
		else 
			MessageBox::Show(e->Message);

		if (destTrans->Connection)
			destTrans->Rollback();
			
		
		return false;
	}
}
//processMetadataInsertsFN: Kept for backward comp.
Int32 dataHandler::processMetadataInsertsFN()
{
	try{
		String ^cmdTxt;
		String ^ sqlNamesMD;
		String ^ sqlValuesMD;
		String ^ sqlNamesMDSpecial;
		String ^ sqlValuesMDSpecial;
		String^ xpath= L"";
		String ^ table = getParamValue(L"/oceandata/schemas/" + "*[@name='"+schemaName+"']/metadata/@table");
		Common::DbDataReader ^ dID;
		Odbc::OdbcCommand ^cmd= gcnew Odbc::OdbcCommand;

		cmd->Connection=odbc;

		cmd->Transaction=destTrans;
		
		int fst=1;

		int fstSpecial=1;

		for each (Metadata m in md)
		{
			if (m.Table){
				if (fstSpecial==1) fstSpecial=0; else {sqlNamesMDSpecial+=",";sqlValuesMDSpecial+=",";}

				if (m.Type=="string") m.Value="\'" + m.Value + "\'";

				if (m.Function){
		
					if (m.ParameterStr) m.Value+=",\'" + m.ParameterStr + "\'";

					m.Value=m.Function + "(" + m.Value + ")";
				}
				
				if (m.FieldName) sqlNamesMDSpecial+=m.FieldName; else sqlNamesMDSpecial+=m.Name;
				
				sqlValuesMDSpecial+=m.Value;
			}
			else
			{
				if (!m.DataTable)
				{
				
					if (fst==1) fst=0; else {sqlNamesMD+=","; sqlValuesMD+=",";}
				
					if (m.Type=="string") m.Value="\'" + m.Value + "\'";

					if (m.Function){
		
						if (m.ParameterStr) m.Value+=",\'" + m.ParameterStr + "\'";

						m.Value=m.Function + "(" + m.Value + ")";
					
					}

					if (m.FieldName) sqlNamesMD+=m.FieldName; else sqlNamesMD+=m.Name;
				
					sqlValuesMD+=m.Value;
				}
			}
			
		}
		
		if (sqlNamesMD && sqlValuesMD) {
			cmdTxt="Insert Into metadata ("+sqlNamesMD + L") values (" + sqlValuesMD + L") ";
			cmd->CommandText=cmdTxt;
			cmd->ExecuteNonQuery();
			cmd->CommandText="SELECT LAST_INSERT_ID() as lastid from metadata;";
			dID=cmd->ExecuteReader();
			if (dID->Read())
				lastMetadataID=dID->GetInt32(0);
			dID->Close();
		}
		if (sqlNamesMDSpecial && sqlValuesMDSpecial) {
			cmdTxt="Insert Into " + table + L"("+sqlNamesMDSpecial+ L", idMetadata) values ("+ sqlValuesMDSpecial + ", " + lastMetadataID+ L") ";
			cmd->CommandText=cmdTxt;
			cmd->ExecuteNonQuery();
			cmd->CommandText="SELECT LAST_INSERT_ID() as lastid from " + table + ";";
			dID=cmd->ExecuteReader();
			if (dID->Read())
				lastMetadata2ID=dID->GetInt32(0);
			dID->Close();
		}
	
		return true;

	}

	catch (Exception^ e){
		if (batchMode)
			writeLog(e->Message);
		else 
			MessageBox::Show(e->Message);
		return 0;
	}
}
//dNameExists: Check if metadata name exists
int dataHandler::dNameExists( String ^dName){

	int index=-1;

	for each (Data dt in d)	{
		index+=1;
		if (dt.Name==dName)
				return index;
	}

	return -1;
}

//writeDataFile:create a local copy of the data file. This is in case souece is read-only
void dataHandler::writeDataFile(String^ strDataStart)
{
// Create an instance of StreamReader to read from a file.
	StreamReader^ srInputFile;
	StreamWriter^ swDatacsv;
	StreamWriter^ swSchemaIni ;
	
	try
		{
		String ^tempFile="ProcessingStream"+fileName;
		String ^tempPath= AppDomain::CurrentDomain->BaseDirectory;

		if (File::Exists(tempPath+ tempFile)) File::Delete(tempPath+ tempFile);

		if (File::Exists(tempPath+ "schema.ini")) File::Delete(tempPath+ "schema.ini");

		origFile=fileName;

		origPath=path;

		if (metadataStyle=="Header")
		{

			if (strDataStart ="") 
				throw gcnew Exception("Header metadata style requires a DataStart parameter");

	
			Int32 dataline=0; 
	
			String^ line;


			// Read and display lines from the file until the end of 
			// the file is reached.
		
			while ( line = srInputFile->ReadLine() ){

				if (line->StartsWith(strDataStart) ) {
					swDatacsv->WriteLine( line );
					break;
				}
			}

			while ( line = srInputFile->ReadLine())
			{
					swDatacsv->WriteLine( line );
			}


		}

			
		else
		{

			File::Copy(Path::Combine(path,  fileName), Path::Combine( tempPath,  tempFile), true );

		}

		StreamWriter^ swSchemaIni = gcnew StreamWriter(tempPath+ "Schema.ini");

		swSchemaIni->WriteLine( "["+ tempFile+"]");
		
		swSchemaIni->WriteLine( "ColNameHeader = True" );

		swSchemaIni->WriteLine( "Format = TabDelimited");

		swSchemaIni->Close();


		fileName=tempFile;

		path=tempPath;

	}



	catch ( Exception^ e ) {
      // Let the user know what went wrong.

		if (batchMode)
			writeLog(e->Message);
		else 
			MessageBox::Show(e->Message);
	}
	finally
    {
		if ( swSchemaIni )
		{
			swSchemaIni->Close();
			delete (IDisposable^)swSchemaIni;
		}
		if ( srInputFile )
		{
			srInputFile->Close();
			delete (IDisposable^)srInputFile;

		}
		if ( swDatacsv )
		{
			swDatacsv->Close();
			delete (IDisposable^)swDatacsv;
		}
    }

}
//getMetadataFields: same as getfields, but they get the metadata info
System::String ^  dataHandler::getMetadataFields (){
	try{
		String^ sqlFieldList;

		int idata = 0;

		int numbDest=0;

		int firstField=1;

		int firstFieldList=1;

		for each (Metadata m in  md) {

			idata += 1;

			if (firstField == 1) firstField = 0; else sqlFieldList += " , ";

			if (m.Value)
				sqlFieldList += "'" + m.Value + "' as `" + m.Name + "`";
			
			else if (m.destFunction && m.functionType=="native")
				sqlFieldList+="'"+ m.destFunction+"' as `"+ m.Name+"`";
			else
				sqlFieldList+="`"+ m.Name+"`";
		}
		return sqlFieldList;
	}
	catch (Exception^ e){
		
		if (batchMode)
			writeLog(e->Message);
		else 
			MessageBox::Show(e->Message);

		return "";
	}
}
System::String ^  dataHandler::getMetadataSQL() {
	try {
		String^ sqlFieldList="Select ";

		int idata = 0;

		int numbDest = 0;

		int firstField = 1;

		int firstFieldList = 1;

		Metadata ^st = getMetadata(STATION);
		Metadata ^lt = getMetadata(LAT);
		Metadata ^lg = getMetadata(LONG);
		Metadata ^in = getMetadata(INSTRUMENT);
		Data ^ti = getData(TIME);
		Data ^dp = getData(DEPTH);

		String^ groupList = " group by `"+ st->Name +"`, `" + lt->Name + "`, `" + lg->Name + "`, `" + in->Name+"`" ;

		for each (Metadata m in  md) {

			idata += 1;

			if (m.Value)
			{
				if (firstField == 1) firstField = 0; else { sqlFieldList += " , "; }
				sqlFieldList += "'" + m.Value + "' as `" + m.Name + "`";
			}

			if (!((m.Destination == "Station" || m.Destination == "DeployTime(ISO8601)" || m.Destination == "Longitude(DecDeg)" || m.Destination == "MaxDepth(m)"
				|| m.Destination == "RecoverTime(ISO8601)" || m.Destination == "Latitude(DecDeg)" || m.Destination == "Instrument") || ((m.Name == "Station" && (m.Destination == nullptr ||m.Destination == ""))
				|| (m.Name == "DeployTime(ISO8601)" && (m.Destination == nullptr ||m.Destination == "")) || (m.Name == "Longitude(DecDeg)" && (m.Destination == nullptr ||m.Destination == ""))
				|| (m.Name == "Latitude(DecDeg)" && (m.Destination == nullptr ||m.Destination == "")) || (m.Name == "Instrument" && (m.Destination == nullptr ||m.Destination == ""))
				|| (m.Name == "MaxDepth(m)" && (m.Destination == nullptr ||m.Destination == "")) || (m.Name == "RecoverTime(ISO8601)" && (m.Destination == nullptr ||m.Destination == "")) || (m.Name == "InstrumentType"))))

			{
				if (firstField == 1) firstField = 0; else { sqlFieldList += " , "; }
				String ^ destName = (m.Destination==nullptr || m.Destination =="")?  m.Name: m.Destination;
				if (m.destFunction && m.functionType == "native")
					sqlFieldList += "'" + m.destFunction + "' as `" + destName + "`";
				else
					sqlFieldList += "max(`" + m.Name + "`) as `" + destName + "`";
			}

				
		}
		
			String ^ stStr = (st->Destination == "" || st->Destination == nullptr) ? "" : " as `" + st->Destination + "` ";
			sqlFieldList += ",max(`" + dp->Name + "`)as `MaxDepth(m)`,min(`" + ti->Name + "`) as `DeployTime(ISO8601)`, max(`" + ti->Name + "`) as `RecoverTime(ISO8601)`,`";
			sqlFieldList += st->Name + "`";
			sqlFieldList += st->Destination == "" || st->Destination == nullptr ? "" : " as `" + st->Destination + "`";
			sqlFieldList += ",`" + lt->Name + "`";
			sqlFieldList += lt->Destination == "" || lt->Destination == nullptr ? "" : " as `" + lt->Destination + "`";
			sqlFieldList += ",`" + lg->Name + "`";
			sqlFieldList += lg->Destination == "" || lg->Destination == nullptr ? "" : " as `" + lg->Destination + "`";
			sqlFieldList += ",`" + in->Name + "`";
			sqlFieldList +=	st->Destination == "" || st->Destination == nullptr ? "" : " as `" + st->Destination + "`";
			sqlFieldList += " from " + path + "\\" + fileName; 
			sqlFieldList += " where left(`" + ti->Name + "`,3) <> 'NaN' and isNull(`" + ti->Name + "`) =false ";
			//sqlFieldList += " where substring(`" + ti->Name + "`,1,3) <> 'NaN' ";
			//sqlFieldList += " where isNull(`" + ti->Name + "`) =false ";
			sqlFieldList +=  groupList;
			return sqlFieldList;
	}
	catch (Exception^ e) {

		if (batchMode)
			writeLog(e->Message);
		else
			MessageBox::Show(e->Message);

		return "";
	}
}
//mdNameExists: same as dnameexist, but in metadata
Int32 dataHandler::mdNameExists( String ^mdName){

	Int32 index=-1;

	for each (Metadata m in md)
	{
		index+=1;
		if (m.Name==mdName)
			return index;
	}
	return -1;
}
//processMetadataInsertsEvent: this function process the metadata from the input file to th vent table
Int32 dataHandler::processMetadataInsertsEvent(DataTable ^sourceTable )
{
	try{
		String ^cmdTxt;
		String ^ sqlNames;
		DateTime date1;
		DateTime date2;
		TimeSpan timespan1;
		TimeSpan timespan2;
		float  currow=0;
		float rowcount=sourceTable->Rows->Count;
		bool fst=1;
		int index;
		String^ xpath= L"";
		String ^ table = getParamValue(L"/oceandata/schemas/" + "*[@name='"+schemaName+"']/metadata/@table");

		writeLog("Processing metadata started at "+ DateTime::Now+".");

		Odbc::OdbcCommand ^cmd= gcnew Odbc::OdbcCommand;

		destTrans=odbc->BeginTransaction();

		cmd->Connection=odbc;

		cmd->Transaction=destTrans;



		//for each (Metadata m in md)
		//{
		//	
		//			if (fst==1) fst=0; else sqlNames+=",";

		//			sqlNames+="`";
	
		//			if (m.Destination) sqlNames+=m.Destination; else sqlNames+=m.Name;

		//			sqlNames+="`";

		//		}
		for each (DataColumn ^c in sourceTable->Columns)
		{
			if (isCalcField(c->ColumnName))
			{
				if (fst == 1)
					fst = 0;
				else
					sqlNames += L",";
				
				if (c->ColumnName == "DeployTime(ISO8601)")
					sqlNames += "`DeployTime(ISO8601)`";

				if (c->ColumnName == "RecoverTime(ISO8601)")
					sqlNames += "`RecoverTime(ISO8601)`";

				if (c->ColumnName == "MaxDepth(m)")
					sqlNames += "`MaxDepth(m)`";
			}
			else
			{
				index = getMetaDataIndex(c->ColumnName);

				if (index != -1) {
					if (fst == 1)
						fst = 0;
					else
						sqlNames += L",";

					sqlNames += "`";

					sqlNames += (md[index].Destination && md[index].Destination !="") ? md[index].Destination : c->ColumnName;

					sqlNames += "`";
				}
			}
		}

	String ^insertSQL=L"Replace  into " +table+"("+ sqlNames +",file) " ;
			
	for each (DataRow ^r in sourceTable->Rows)
		{
			String ^valuesSQL=L"";
			String ^valueSQL="";
			String ^paramSQL="";

			currow+=1;

			bool fst=1;

			date1=DateTime::Now;

			for each (DataColumn ^col in sourceTable->Columns)
			{
				if (isCalcField(col->ColumnName))
				{
					if (fst == 1)
						fst = 0;
					else
						valuesSQL += L",";

					if (col->ColumnName == "DeployTime(ISO8601)")
						valueSQL = L"str_to_date(\"" + r[col] + L"\", \"%Y-%m-%dT%H:%i:%s\")";

					if (col->ColumnName == "RecoverTime(ISO8601)")
						valueSQL = L"str_to_date(\"" + r[col] + L"\", \"%Y-%m-%dT%H:%i:%s\")";

					if (col->ColumnName == "MaxDepth(m)")
						valueSQL = r[col]->ToString();
				}
				else
				{
					index = getMetaDataIndex(col->ColumnName);
					valueSQL = "";

					if (index != -1) {
						if (fst == 1)
							fst = 0;
						else
							valuesSQL += L",";

						//if (md[index].Type == "number")
						//{
						//	if (r->IsNull(col))
						//		valueSQL = "null";
						//	else
						//		valueSQL = r[col]->ToString();
						//}
						////if (c->DataType == System::Type::GetType("System.String"))
						//else if (col->DataType == System::Type::GetType("System.String") || md[index].Type == "string" || md[index].Type == "datetime")
						//	valueSQL = L"\"" + r[col] + L"\"";
						//else
						//{
						//	if (r->IsNull(col))
						//		valueSQL = "null";
						//	else
						//		valueSQL = r[col]->ToString();
						//}

						if (md[index].Type == "number")
						{
							if (r->IsNull(col))
								valueSQL = "null";
							else
							{
								if (r[col]->ToString()->Length > 2)
								{
									if (r[col]->ToString()->Substring(0, 3) == "NaN")
										valueSQL = "null";
									else
										valueSQL = r[col]->ToString();
								}
								else
									valueSQL = r[col]->ToString();
							}
						}

						else if (col->DataType == System::Type::GetType("System.String") || md[index].Type == "string" || md[index].Type == "datetime")
						{
							if (r[col]->ToString()->Length > 2)
							{
								if (r[col]->ToString()->Substring(0, 3) == "NaN")
									valueSQL = "null";
								else
									valueSQL = L"\"" + r[col] + L"\"";
							}
							else
								valueSQL = L"\"" + r[col] + L"\"";

						}


						if (md[index].destFunction) 
						{
							if (md[index].ParameterStr) {
								paramSQL = "," + "\"" + md[index].ParameterStr + "\"";
								valueSQL = md[index].destFunction + "(" + valueSQL + paramSQL + ")";
							}
							if (md[index].functionType == "native")
								valueSQL = md[index].destFunction;
						}
					}
				}

				valuesSQL+=valueSQL;
			}
	
			cmdTxt+=L"( "+valuesSQL+",'"+origFile+ "')\n";	

			date2= DateTime::Now;
			
			timespan1+= date2-date1;
			
			date1= DateTime::Now;
			
			cmdTxt=insertSQL + "Values"+ cmdTxt;
			
			cmd->CommandText=cmdTxt;
		
			cmd->ExecuteNonQuery();
			
			cmdTxt="";
			
			date2= DateTime::Now;
			
			timespan2+= date2-date1;
			
			p->Value=(currow/rowcount)*100;
			
			Application::DoEvents();
		}

		TimeSpan timespan3=timespan2+timespan1;

		String^message="Update metadata complete on "+ date2 +". Rows updated = " + currow + ". Total time: " + System::Convert::ToInt32(timespan3.TotalSeconds) + " seconds. Time elapsed at mysql: " +  System::Convert::ToInt32(timespan2.TotalSeconds)+ " seconds.";
				
		if (batchMode)
			writeLog (message);

		return true;
	}

	catch (Exception^ e){
		if (batchMode)
			writeLog(e->Message);
		else 
			MessageBox::Show(e->Message);
		if (destTrans){
			destTrans->Rollback();
		}
		return false;
	}
}
//processBatch: once set the path, this procedure will go through the files and call updateFile
void dataHandler::processBatch()
{
	connectionStringOLE =  getParamValue(L"/oceandata/source/oledbparams") + path + ";";

	batchMode=true;

	array<String^> ^files=Directory::GetFiles(path, "*.txt");

	writeLog("Processing Started by "+UserName+ ". Target files: "+ files->Length +". Target folder: " + path + ".");

	Int32 fileNumber=1;

	initDBConn();

	for each (String ^ f in files )	{
		writeLog("Processing "+ fileNumber + " of " +files.Length);

		updateFile(Path::GetFileName(f));

		path=origPath;

		fileNumber+=1;
	}
}
//updateFile: this function will process each file
void dataHandler::updateFile(String^ file)
{
	
	
	writeLog("Processing file "+ file + " started at "+ DateTime::Now+".");

	try {


		fileName = file;

		if (autoDetect == true) {
			writeDataFile("");
			System::Data::DataTable ^ t = getTableOLE(connectionStringOLE, "Select InstrumentType from " + path + fileName + " ");
			DataRow ^ r = t->Rows[0];
			instType = r[t->Columns["InstrumentType"]]->ToString();
		}
		else
		{
			String ^ strDataStart = getParamValue("/oceandata/schemas/*[@name='" + schemaName + "']/dataStart");

			writeDataFile(strDataStart);
		}

		Int32 retMetadef = setMetadataDefArray();

		if (!retMetadef) return;

		setDataArray();

		String^ sqlcommand = L"Select * from " + path + "\\" + fileName;

		srcTable = getTableOLE(connectionStringOLE, sqlcommand);

		String^ sqlMetadata = getMetadataSQL();

		DataTable^ sourcetable = (DataTable^)getTableOLE(connectionStringOLE, sqlMetadata);

		System::Int32 ret = processMetadataInsertsEvent(sourcetable);

		if (!ret)
		{
			writeLog("Metadata update Failed");

			return;
		}

		String ^cmdFields = "Select * from " + path + "\\" + fileName + " where 1=0";

		String^ sqlSource = L"Select " + getFields(connectionStringOLE, cmdFields) + " from " + path + "\\" + fileName + " ";

		sourcetable = getTableOLE(connectionStringOLE, sqlSource);

		ret = processInserts(sourcetable);

		if (origFile != fileName)
			if (File::Exists(path + "//" + fileName))
				File::Delete(path + "//" + fileName);
	}
	catch (Exception^ e) {
		if (batchMode)
			writeLog(e->Message);
		else
			MessageBox::Show(e->Message);
	}

	writeLog("Processing file "+ file + " ended at "+ DateTime::Now+".");


}
void dataHandler::setInstrumentType()
{

		System::Data::DataTable ^ t = getTableOLE(connectionStringOLE, "Select InstrumentType from " + path + "\\" + fileName);

		DataRow ^ r = t->Rows[0];

		instType = r[t->Columns["InstrumentType"]]->ToString();
	
}
//writeLog: every message should be shown in the the label and in the log
void dataHandler::	(String ^line)
{
	if (!File::Exists("oceandata.log"))
	{
		File::Create("oceandata.log");
	}

	line+="\r\n";
	
	StreamWriter^ swLog = File::AppendText("oceandata.log");

	swLog->WriteLine(line);

	swLog->Flush();

	swLog->Close();

	delete (IDisposable^)swLog;

	if (l)
		l->AppendText(line);
}
void dataHandler::setDataArray()
{
	try
	{

		String^ xpath = L"/oceandata/schemas/schema[@name='" + schemaName + "']/fields/field";;
		String ^ nodeBase;
		String ^ selectNode;
		XmlNode ^ attr;
		String ^ selectNodeBase;
		XmlNodeList^ xmlNodeSources = xmlDoc->SelectNodes(xpath);
		String ^fldName;
		XmlNode ^ nAttr;

		d = gcnew array<Data>(1);

		int idata = 1;

		int numbDest = 0;

		d[0].idEventFunction = "getEventID";

		d[0].Name = "idEvent";

		for each (XmlNode^ n in  xmlNodeSources) {

			idata += 1;

			Array::Resize(d, idata);

			nAttr = n->Attributes->GetNamedItem("name");
			
			selectNodeBase = L"/oceandata/schemas/schema[@name='" + schemaName + L"']/fields/field[@name=\"" + nAttr->Value + "\"]";

			selectNode = selectNodeBase + L"/@name";

			attr = n->SelectSingleNode(selectNode);

			fldName = ridOfBrackets(attr->Value);

			d[idata - 1].Name = fldName;

			selectNode = selectNodeBase + L"/@destfunction";

			attr = n->SelectSingleNode(selectNode);

			if (attr) d[idata - 1].Function = attr->Value;

			selectNode = selectNodeBase + L"/@destparameterstr";

			attr = n->SelectSingleNode(selectNode);

			if (attr) d[idata - 1].ParameterStr = attr->Value;

			selectNode = selectNodeBase + L"/@type";

			attr = n->SelectSingleNode(selectNode);

			if (attr) d[idata - 1].Type = attr->Value; else d[idata - 1].Type = "number";

			selectNode = selectNodeBase + L"/@destination";

			attr = n->SelectSingleNode(selectNode);

			if (attr) {
				d[idata - 1].destination = attr->Value;
			}

		}

	}

	catch (Exception^ e){
		if (batchMode)
			writeLog(e->Message);
		else
			MessageBox::Show(e->Message);
	}
}
System::String ^  dataHandler::getFields(String^ connectionString, String^ strCommandText) {
	try
	{
		DataTable^ table = gcnew DataTable();
		String^ sqlFieldList;
		array<String^>^ strDest = gcnew array<String^> (d->Length);

		dataAdapterOLE = (gcnew System::Data::OleDb::OleDbDataAdapter(strCommandText, connectionString));
		
		dataAdapterOLE->Fill(table);

		srcTable = table;
		
		int numbDest = 0;

		int firstField = 1;

		int firstFieldList = 1;


		Metadata ^st = getMetadata(STATION);
		Metadata ^lt = getMetadata(LAT);
		Metadata ^lg = getMetadata(LONG);
		Metadata ^in = getMetadata(INSTRUMENT);

		d[0].idEventFunction = "'getEventID (\"'+ " + st->Name + "+'\",\"'+" + in->Name + "+'\",'+cstr([" + lt->Name + "])+','+ cstr([" + lg->Name + "])+')'";

		sqlFieldList=d[0].idEventFunction + " as " + d[0].Name + ", ";

		for each (Data ^ dt in  d) {

			if (fieldExist(table, dt->Name)){

				if (dt->destination && dt->destination!="") {

					if (numbDestExist(strDest, dt->Name) == false) {

						if (firstField == 1) firstField = 0; else sqlFieldList += ",";

						for each  (Data ^ dtd in d) {
							if (fieldExist(table, dtd->Name))
							{
								if ((dt->destination == dtd->destination) ) {

									if (firstFieldList == 1) firstFieldList = 0; else sqlFieldList += "+";

									sqlFieldList += "[" + dtd->Name + "] ";
								}
							}
						}

						strDest[numbDest] = dt->Name;

						numbDest++;

						sqlFieldList += L" as [" + dt->destination + "] ";

					}

				}
				else {

					if (firstField == 1) firstField = 0; else sqlFieldList += ",";

					sqlFieldList += "[" +dt->Name + "] ";

				}
				firstFieldList = 1;

			}

		}

		return sqlFieldList;
	}

	catch (Exception^ e){
		if (batchMode)
			writeLog(e->Message);
		else
			MessageBox::Show(e->Message);
		return "";
	}


}
System::String ^ dataHandler::processField(Data ^ d) {

	String ^ sqlFieldList;

	sqlFieldList += d->Function ? d->Function + "([" + d->Name + "])" : "[" + d->Name + "]";

	return sqlFieldList;
}
void dataHandler::initDBConn(){

	try {

		defaultConn = getParamValue("/oceandata/destination/default");

		
		if (defaultConn == "ODBC") {

			odbc = gcnew Odbc::OdbcConnection;

			OdbcConnectionStringBuilder ^odbcBldr = gcnew OdbcConnectionStringBuilder;

			odbcBldr->Add("Driver", getParamValue(L"/oceandata/destination/odbcparams/@driver"));
			odbcBldr->Add("Server", getParamValue(L"/oceandata/destination/odbcparams/@server"));
			odbcBldr->Add("Database", getParamValue(L"/oceandata/destination/odbcparams/@database"));
			odbcBldr->Add("UID", getParamValue(L"/oceandata/destination/odbcparams/@user"));
			odbcBldr->Add("Password", getParamValue(L"/oceandata/destination/odbcparams/@password"));

			odbc->ConnectionString = odbcBldr->ConnectionString;

			connectionString = odbcBldr->ConnectionString;

			odbc->Open();

			batchMode = true;

			//connectionbuilder=gcnew OleDb::OleDbConnectionStringBuilder(connectionString);
			dataID = "Server: " + odbcBldr["server"] + " - Database: " + odbcBldr["database"];
		}
		else {
			//mySQLConn = gcnew MySqlConnection;

			//MySqlConnectionStringBuilder ^mySQLBldr = gcnew MySqlConnectionStringBuilder;

			//mySQLBldr->Add("Server", getParamValue(L"/oceandata/destination/odbcparams/@server"));
			//mySQLBldr->Add("Database", getParamValue(L"/oceandata/destination/odbcparams/@database"));
			//mySQLBldr->Add("UserID", getParamValue(L"/oceandata/destination/odbcparams/@user"));
			//mySQLBldr->Add("Password", getParamValue(L"/oceandata/destination/odbcparams/@password"));

			//mySQLConn->ConnectionString = mySQLBldr->ConnectionString;

			//connectionString = mySQLBldr->ConnectionString;

			//mySQLConn->Open();

			//batchMode = true;

			////connectionbuilder=gcnew OleDb::OleDbConnectionStringBuilder(connectionString);
			//dataID = "Server: " + mySQLBldr["server"] + " - Database: " + mySQLBldr["database"];
		}
	}
	catch (Exception^ e)
	{
		// Let the user know what went wrong.
		if (batchMode)
			writeLog(e->Message);
		else
			MessageBox::Show(e->Message);
	}
	
}
void dataHandler::upsertXMLNode(String ^ xpath, String ^ xName, String ^ xValue) {
	
	XmlNode ^ xn = xmlDoc->SelectSingleNode(xpath);
	XmlNode ^xnChld = xn->SelectSingleNode(".//" + xName);

	if (xnChld) {
		xnChld->InnerText = xValue;
	}
	else {
		xnChld = xmlDoc->CreateElement(xName);

		xnChld->InnerText = xValue;

		xn->AppendChild(xnChld);
	}
}
void dataHandler::upsertXMLAttr(String ^ xpath, String ^ xName, String ^ xValue) {

	XmlNode ^ xn = xmlDoc->SelectSingleNode(xpath);
	XmlAttribute ^xnAttr = xn->Attributes[xName];

	if (xnAttr) {
		xnAttr->Value = xValue;
	}
	else {
		xnAttr = xmlDoc->CreateAttribute(xName);

		xnAttr->InnerText = xValue;

		xn->Attributes->Append(xnAttr);
	}
}
void dataHandler::upsertXMLAttr(String ^ xpath, String ^ xName, String ^ xValue, bool deleteBlank) {

	XmlNode ^ xn = xmlDoc->SelectSingleNode(xpath);
	XmlAttribute ^xnAttr = xn->Attributes[xName];

	if (xValue == "" && deleteBlank == true) 
	{
		if (xnAttr)
			xn->Attributes->Remove(xnAttr);
	}
	else
	{
		if (xnAttr)
			xnAttr->Value = xValue;
		else 
		{
			xnAttr = xmlDoc->CreateAttribute(xName);

			xnAttr->InnerText = xValue;

			xn->Attributes->Append(xnAttr);
		}
	}
}

void dataHandler::upsertXMLNode(String ^ xpathParent, String ^ xName, String ^ attrName, String ^ attrValue) {

	XmlNode ^ xn = xmlDoc->SelectSingleNode(xpathParent +"/"+ xName+"[@" +attrName +"='"+ attrValue+"']");
	
	if (!xn) {

		xn = xmlDoc->CreateElement(xName);

		XmlAttribute ^ xnAttr= xmlDoc->CreateAttribute(attrName);

		xnAttr->Value = attrValue;

		xn->Attributes->Append(xnAttr);

		XmlNode ^ xParent = xmlDoc->SelectSingleNode(xpathParent);

		xParent->AppendChild(xn);
	}
}

int dataHandler::getDataIndex(String ^name) {

	int index =0;

	if (!d)
		return -1;

	for each (Data dt in d) {
		if ((dt.Name == name && (dt.destination==nullptr|| dt.destination == ""))||dt.destination==name)
			return index;
		index += 1;
	}
	return -1;
}

int dataHandler::getMetaDataIndex(String ^name) {

	int index = 0;

	if (!md)
		return -1;

	for each (Metadata m in md) {
		if ((m.Name == name && (m.Destination == nullptr || m.Destination =="")) || m.Destination == name)
			return index;
		index += 1;
	}
	return -1;
}
Common::DbDataReader ^  dataHandler::getReader(String^ cmdStr) {

	Common::DbCommand ^cmd;
	
	cmd->CommandText = cmdStr;

	if (defaultConn == "ODBC") {

		cmd->Connection = odbc;
	}
	else {
		//cmd->Connection = mySQLConn;
	}

	return cmd->ExecuteReader();
}
dataHandler::Metadata ^ dataHandler::getMetadata(int type) {

	switch (type)
	{
	
	case STATION:
		for each (dataHandler::Metadata m in md)
		{
			if (fieldExist(srcTable, m.Name)) {
				if (m.Name == "Station" && (m.Destination == nullptr || m.Destination == ""))
					return m;
				if (m.Destination == "Station")
					return m;
			}
		}
		break;
	case LAT:
		for each (dataHandler::Metadata m in md)
		{
			if (fieldExist(srcTable, m.Name)) 
			{
				if (m.Name == "Latitude(DecDeg)" && (m.Destination == nullptr || m.Destination == ""))
					return m;
				if (m.Destination == "Latitude(DecDeg)")
					return m;
			}
		}
		break;
	case LONG:
		for each (dataHandler::Metadata m in md)
		{
			if (fieldExist(srcTable, m.Name)) 
			{
				if (m.Name == "Longitude(DecDeg)" && (m.Destination == nullptr || m.Destination == ""))
					return m;
				if (m.Destination == "Longitude(DecDeg)")
					return m;
			}
		}
		break;
	case INSTRUMENT:
		for each (dataHandler::Metadata m in md)
		{
			if (fieldExist(srcTable, m.Name)) 
			{
				if (m.Name == "Instrument" && (m.Destination == nullptr || m.Destination == ""))
					return m;
				if (m.Destination == "Instrument")
					return m;
			}
		}
		break;
	}
}
dataHandler::Data ^ dataHandler::getData(int type) {

	switch (type)
	{
	case TIME:
		for each (dataHandler::Data dt in d)
		{
			
			if (fieldExist(srcTable, dt.Name)) 
			{
				if (dt.Name == "Time(ISO8601)" && (dt.destination == nullptr || dt.destination == ""))
					return dt;
				if (dt.destination == "Time(ISO8601)" )
					return dt;
			}
		}
		break;
	case DEPTH:
		for each (dataHandler::Data dt in d)
		{
			if (fieldExist(srcTable, dt.Name)) 
			{
				if (dt.Name == "Depth(m)" && (dt.destination == nullptr || dt.destination == ""))
					return dt;
				if (dt.destination == "Depth(m)")
					return dt;
			}
		}
		break;
	}
}
bool dataHandler::isCalcField(String ^ name) {

	if (name == "DeployTime(ISO8601)" || name == "RecoverTime(ISO8601)" || name == "MaxDepth(m)")
		return true;
	else
		return false;
}
bool dataHandler::fldExistFile(String ^ name) {
	for each (DataColumn ^ c in srcTable->Columns)
		if (c->ColumnName == name)
			return true;
	
	return false;
}
bool dataHandler::check4DepthUnits() {
	return true;
}