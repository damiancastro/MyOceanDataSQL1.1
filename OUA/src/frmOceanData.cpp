/****************************************************************
File: frmOceandata.cpp
Author: Damián Castro

frmOceandata.cpp contains all the code for frmOceandata 
form. All the control event procedure and utility procedures.
This form uses the dataHandler class that control access
to the input files and data store (mysql). The dataHandler class
is intatianted using the varible dh.
*****************************************************************/
#include "StdAfx.h"
#include "frmOceanData.h"

using namespace OceanData;
using namespace OceanDataHandler;


//btnBrowse_Click: Event used for single file proicessing
System::Void frmOceanData::btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
	
	if(openSourceFile->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		textSourceFile->Text= openSourceFile->FileName;
		
		dh->fileName=openSourceFile->SafeFileName;
		
		dh->path=Path::GetDirectoryName(textSourceFile->Text)+"\\";

		dh->fileNameNoPath=Path::GetFileName(textSourceFile->Text);

		btnLoad->Enabled=true;
	}

}



//btnLoad_Click: Single file processing can load the file before processing for inspection
System::Void frmOceanData::btnLoad_Click(System::Object^  sender, System::EventArgs^  e) {

	try
	{
		String ^ xmlPath;
		XmlNode^ xmlNodeSource;

		dh->connectionStringOLE =  dh->getParamValue(L"/oceandata/source/oledbparams") + dh->path + ";";

		dh->initDBConn();

		listDBMeasurements();

		dh->schemaName = cmbSchemas->Text;

		xmlPath = L"/oceandata/schemas/schema[@name='" + dh->schemaName + "']";

		xmlNodeSource = dh->xmlDoc->SelectSingleNode(xmlPath);

		source=xmlNodeSource->Name;

		if (chkAutodetect->Checked){
			dh->writeDataFile("");
			 System::Data::DataTable ^ t = dh->getTableOLE(dh->connectionStringOLE, "Select InstrumentType from " + dh->path + dh->fileName + " ");
			 DataRow ^ r = t->Rows[0];
			 dh->instType = r[t->Columns["InstrumentType"]]->ToString();
		}
		else {
			dh->schemaName = cmbSchemas->Text;
			String ^ strDataStart = dh->getParamValue(xmlPath+"/dataStart");
			dh->writeDataFile(strDataStart);
			dh->instType=cmbInstrument->Text;
		}

		if (dh->instType == "") {
			MessageBox::Show("Instrument Type cannot be blank.");
			return;
		}
		int idxInstType = cmbInstrument->FindStringExact(cmbInstrument->Text);
		
		if (idxInstType == -1) {
			dh->instType = cmbInstrument->Text;
			cmbInstrument->Items->Add(cmbInstrument->Text);
			dh->upsertXMLNode("/oceandata/schemas/schema[@name='" + dh->schemaName + "']/InstrumentTypes", "InstrumentType", "name", dh->instType);
			dh->xmlDoc->Save("config.xml");
		}
				
		Int32 retMetadef = dh->setMetadataDefArray();

		if (!retMetadef) return;

		//Now we only use the event table.All other metadata collection procedures will be deprecated
		//if (dh->metadataStyle=="EventTable")
		//{
		//	//dsMetadata->DataSource = dh->getTableOLE(dh->connectionStringOLE, "Select distinct " + dh->getMetadataFields()  +" from " + dh->path + dh->fileName + " ");
		//	dsMetadata->DataSource = dh->getTableOLE(dh->connectionStringOLE, dh->getMetadataSQL());
		//}

		//Change to bring all rows, not only those from the schema
		//String^ sqlcommand = L"Select " + dh->getFields(connectionStringOLE, cmdFields, true)  +" from " + dh->path +"\\"+ dh->fileName;
		String ^cmdFields = "Select * from " + dh->path + "\\" + dh->fileName + " where 1=0";
		
		dh->setDataArray();

		dh->fields = dh->getFields(dh->connectionStringOLE, cmdFields);

		String^ sqlcommand = L"Select * from " + dh->path + "\\" + dh->fileName;

		dh->srcTable = dh->getTableOLE(dh->connectionStringOLE, sqlcommand);

		dsSourceFile->DataSource = dh->srcTable;

		initSchmFldLst();

		listFileFlds();

		listMissingFields();

		if (!tabControl1->TabPages->Contains(tabPage6))
			tabControl1->TabPages->Add(tabPage6);

		if (!tabControl1->TabPages->Contains(tabPage1))
			tabControl1->TabPages->Add(tabPage1);
		tabPage1->Text = dh->fileNameNoPath;
	}
	catch(Exception^ e)
	{
		MessageBox::Show(e->Message);
	}
}

//frmOceanData_Load: Form load, Initialization (Datahandler instance,etc.) should happen here.
System::Void frmOceanData::frmOceanData_Load(System::Object^  sender, System::EventArgs^  e) {

	loadConfig();
	//listDBMeasurements();
	//initSchmFldLst();
	
	}

//btnUpd_Click: After loading a file, update is doen by this event
System::Void frmOceanData::btnUpd_Click(System::Object^  sender, System::EventArgs^  e){

	try {

		this->Cursor = Cursors::WaitCursor;

		if (!tabControl1->TabPages->Contains(tabPage2))
			this->tabControl1->TabPages->Add(tabPage2);

		tabControl1->SelectedTab = tabPage2;

		//String^ sqlMetadata = "Select distinct " + dh->getMetadataFields() + " from " + dh->path + "\\" + dh->fileName + " ";

		String^ sqlMetadata = dh->getMetadataSQL();

		DataTable^ sourcetable = (DataTable^)dh->getTableOLE(dh->connectionStringOLE, sqlMetadata);

		dh->p = progressBar1;

		dh->l = txtUploadMsg;

		System::Int32 ret = dh->processMetadataInsertsEvent(sourcetable);

		String^ sqlSource = L"Select " + dh->fields + " from " + dh->path + "\\" + dh->fileName;

		//String^ sqlSource = L"Select * from " + dh->path + "\\" + dh->fileName;

		sourcetable = dh->getTableOLE(dh->connectionStringOLE, sqlSource);

		ret = dh->processInserts(sourcetable);

		this->Cursor = Cursors::Default;
	}
	catch (Exception ^e) {

		MessageBox::Show(e->Message);

	}
}

//btnUpdNoLoad_Click: this event update a sibngle file without loading it first for inspection
System::Void frmOceanData::btnUpdNoLoad_Click(System::Object^  sender, System::EventArgs^  e){
	
	frmOceanData::btnLoad_Click(sender,  e);

	tabControl1->SelectedTab = tabPage2;

	DataTable^ table = (DataTable^)dsMetadata->DataSource;

	dh->p = progressBar1;

	dh->l = txtUploadMsg;

	System::Int32 ret = dh->processMetadataInsertsEvent(table);

	String^ sqlSource = L"Select " + dh->fields + " from " + dh->path + "\\" + dh->fileName;

	//String^ sqlSource = L"Select * from " + dh->path + "\\" + dh->fileName;

	DataTable ^sourcetable = dh->getTableOLE(dh->connectionStringOLE, sqlSource);

	ret = dh->processInserts(sourcetable);
	
}

//btnUploadBatch_Click: upload an entire folder
System::Void frmOceanData::btnUploadBatch_Click(System::Object^  sender, System::EventArgs^  e){

	if (!tabControl1->TabPages->Contains(tabPage2))
		tabControl1->TabPages->Add(tabPage2);
	
	int idxInstType = cmbInstrument->FindStringExact(cmbInstrument->Text);

	if (idxInstType == -1) {
		dh->instType = cmbInstrument->Text;
		cmbInstrument->Items->Add(cmbInstrument->Text);
		dh->upsertXMLNode("/oceandata/schemas/schema[@name='" + dh->schemaName + "']/InstrumentTypes", "InstrumentType", "name", dh->instType);
		dh->xmlDoc->Save("config.xml");
	}


	if (dh->instType == "") {
		MessageBox::Show("Instrument Type cannot be blank.");
		return;
	}
	
	if (openSourceFolder->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		dh->path=openSourceFolder->SelectedPath;

		tabControl1->SelectedTab=tabPage2;

		dh->p=progressBar1;

		dh->l=txtUploadMsg;

		dh->processBatch();
	}

}

//fillSourceTypesCombo: fills the combo box with all the Instrument types from the xml file
System::Void frmOceanData::fillSourceTypesCombo(){

	try
	{
		String ^xpath;
		
		xpath = "/oceandata/schemas/schema[@name='"+dh->schemaName+"']/InstrumentTypes/InstrumentType/@name";
		
		XmlNodeList^ xmlNodeSources = dh->xmlDoc->SelectNodes(xpath);
		
		cmbInstrument->Items->Clear();
		
		for each (XmlNode^ n in  xmlNodeSources)
		{
			cmbInstrument->Items->Add(n->InnerText);
		}

		if (cmbInstrument->Items->Count>0)
			cmbInstrument->Text = cmbInstrument->Items[0]->ToString();
	}
	catch (Exception^ e)
	{
		MessageBox::Show(e->Message);
	}
}

//cmbInstrument_SelectedIndexChanged: when instrument changes, metadata should change, too
System::Void frmOceanData::cmbInstrument_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 
	dh->instType=cmbInstrument->SelectedItem->ToString();
	
	if (dh->md) 
		dh->md[0].Value = dh->instType;
	 
}


//txtUserName_TextChanged: change the user in the dataHandler object
System::Void frmOceanData::txtUserName_TextChanged(System::Object^  sender, System::EventArgs^  e) {	
	dh->UserName=txtUserName->Text;
}
//funtion to read fields from schmea in config.xml
void frmOceanData::initSchmFldLst() {
	
	DataGridViewComboBoxCell  ^cbcell;
	String ^dest;
	array<String^> ^unUsedDFld = getUnusedDBFields(DATA);
	array<String^> ^unUsedMDFld = getUnusedDBFields(METADATA);
	array<String^> ^unUsedFld = getUnusedDBFields(ALL);

	dGridVars->Rows->Clear();

	String^ sqlSource = L"Select * from " + dh->path + "\\" + dh->fileName;

	DataTable ^sourcetable = dh->getTableOLE(dh->connectionStringOLE, sqlSource);
	
	for each (OceanDataHandler::dataHandler::Metadata md in dh->md) {

		if (md.Name!="Position" && md.Name != "InstrumentType" )
		{
			dest = md.Destination ? md.Destination : md.Name;
			
			if (dh->fieldExist(sourcetable, md.Name))
			{
				
				String ^ dType = "Number";

				if (md.Type == "string")
					dType = "Text";
				if (md.destFunction == "str_to_date")
					dType = "DateTime";
				
				dGridVars->Rows->Add(md.Name, dest, "Metadata", "",dType);

				cbcell = gcnew DataGridViewComboBoxCell;

				cbcell->DisplayStyle = DataGridViewComboBoxDisplayStyle::ComboBox;

				cbcell->Items->Add(dest);

				cbcell->Value = dest;

				cbcell->Items->Add("New Variable...");

				cbcell->Items->Add("Ignore");

				if (dh->odbc && unUsedMDFld)
					for each (String^ s in unUsedMDFld)
						cbcell->Items->Add(s);

				dGridVars->Rows[dGridVars->Rows->Count - 1]->Cells[1] = cbcell;
			}
			
		}
	}

	for each (OceanDataHandler::dataHandler::Data d in dh->d) {
		
		if (d.Name != "idEvent" && d.Name != "id") {

			dest = d.destination ? d.destination : d.Name;

			if (dh->fieldExist(sourcetable, d.Name))
			{
				String ^ dType = "Number";

				if (d.Type == "string")
					dType = "Text";
				if (d.Function == "str_to_date")
					dType = "DateTime";

				dGridVars->Rows->Add(d.Name, dest, "Measurement", "", dType);

				cbcell = gcnew DataGridViewComboBoxCell;

				cbcell->DisplayStyle = DataGridViewComboBoxDisplayStyle::ComboBox;
				
				cbcell->Items->Add(dest);

				cbcell->Value = dest;

				cbcell->Items->Add("New Variable...");

				cbcell->Items->Add("Ignore");

				if (dh->odbc && unUsedDFld)
					for each (String^ s in unUsedDFld)
						cbcell->Items->Add(s);
				 
				dGridVars->Rows[dGridVars->Rows->Count - 1]->Cells[1] = cbcell;
			}

		}
	}

	txtProc->Text = dh->getParamValue("/oceandata/schemas/*[@name='" + dh->schemaName + "']/afterProcedure");
}

//function to diabled Insatrument type
void frmOceanData::chkAutodetect_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (chkAutodetect->Checked) {
		cmbInstrument->Enabled = false;
		dh->autoDetect = true;
	}

	else{
		cmbInstrument->Enabled = true;
		dh->autoDetect = false;
	}

}

System::Void frmOceanData::btnAdd2Schm_Click(System::Object^  sender, System::EventArgs^  e) {
	
	this->Cursor = Cursors::WaitCursor;

	saveSchema();

	this->Cursor = Cursors::Default;
}
void frmOceanData::loadConfig() {

	dh = gcnew dataHandler(L"config.xml");

	txtDriver->Text = dh->getParamValue(L"/oceandata/destination/odbcparams/@driver");

	txtSrv->Text= dh->getParamValue(L"/oceandata/destination/odbcparams/@server");

	txtUserName->Text= dh->getParamValue(L"/oceandata/destination/odbcparams/@user");

	txtDB->Text= dh->getParamValue(L"/oceandata/destination/odbcparams/@database");

	txtPass->Text = dh->getParamValue(L"/oceandata/destination/odbcparams/@password");

	//this->Text = "Oceandata+ Connected to " + dh->dataID;

	dh->UserName = txtUserName->Text;

	this->dataGridSource->DataSource = dsSourceFile;

	loadDefSchema();

	fillSourceTypesCombo();

	this->tabControl1->TabPages->Remove(tabPage1);

	this->tabControl1->TabPages->Remove(tabPage2);

	this->tabControl1->TabPages->Remove(tabPage6);

	dh->autoDetect = chkAutodetect->Checked;

	//int ret = dh->setMetadataDefArray();

	//dh->setDataArray();
}

void frmOceanData::listDBMeasurements(){
	
	lstFldDb->Items->Clear();

	String ^ SqlCommand = "SHOW COLUMNS	FROM measurement where Field not in('idEvent','id');";

	DataTable ^tblDbFlds = dh->getTable(dh->connectionString, SqlCommand);

	for each (System::Data::DataRow ^r in tblDbFlds->Rows) {
		lstFldDb->Items->Add(r["Field"]);
	}

	SqlCommand = "SHOW COLUMNS	FROM metadata where Field not in('Position','InstrumentType','file','UploadTime', 'idEvent', 'DeployTime(ISO8601)', 'RecoverTime(ISO8601)');";

	tblDbFlds = dh->getTable(dh->connectionString, SqlCommand);

	lstMetadata->Items->Clear();

	for each (System::Data::DataRow ^r in tblDbFlds->Rows) {
		lstMetadata->Items->Add(r["Field"]);
	}
}
void frmOceanData::loadDefSchema() {
	
	cmbSchemas->Items->Clear();

	try
	{
		String ^xpath;

		xpath = "/oceandata/schemas/schema/@name";

		XmlNodeList^ xmlNodeSources = dh->xmlDoc->SelectNodes(xpath);

		cmbSchemas->Items->Clear();

		for each (XmlNode^ n in  xmlNodeSources)
		{
			cmbSchemas->Items->Add(n->Value);
		}

		if (cmbSchemas->Items->Count>0)
			cmbSchemas->Text = cmbSchemas->Items[0]->ToString();
	}
	catch (Exception^ e)
	{
		MessageBox::Show(e->Message);
	}
}

void frmOceanData::listFileFlds() {

	lstFileFlds->Items->Clear();

	for each (DataGridViewColumn ^c in dataGridSource->Columns) {
		lstFileFlds->Items->Add(c->Name);
	}

	lstMssSchm->Items->Clear();

	for each (DataGridViewColumn ^c in dataGridSource->Columns) {
		bool  found = false;

		for each (DataGridViewRow ^ i in dGridVars->Rows) {
			String ^ var = i->Cells[0]->Value->ToString();
			if (c->Name == var)
				found = true;
		}

		if (found == false && c->Name != "InstrumentType") 
			lstMssSchm->Items->Add(c->Name);

	}

}

System::Void frmOceanData::btnSaveAs_Click(System::Object^  sender, System::EventArgs^  e) {

	String ^newName = txtSchemaNewName->Text;

	if (newName != "") {
		if (dh->xmlDoc->SelectSingleNode("/oceandata/schemas/schema[@name='" + newName + "']"))
			MessageBox::Show("Schema already exists!");
		else {
			XmlNode ^xCurrSchema = dh->xmlDoc->SelectSingleNode("/oceandata/schemas/schema[@name='" + dh->schemaName + "']");

			XmlNode ^ xCopy = xCurrSchema->Clone();

			xCopy->Attributes["name"]->Value = newName;

			XmlNode ^xparent = dh->xmlDoc->SelectSingleNode("/oceandata/schemas");

			xparent->AppendChild(xCopy);

			dh->xmlDoc->Save("config.xml");
		}
	}
	else
		MessageBox::Show("Cloned schema should have a name!");

	loadDefSchema();

	cmbSchemas->SelectedIndex=cmbSchemas->Items->Count-1;
}

System::Void frmOceanData::cmbSchemas_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

	dh->schemaName = cmbSchemas->Text;

	if (dh->fileName)
	{
		if (dirtySchema = true) {
			System::Windows::Forms::DialogResult res = MessageBox::Show("Do you want to save the chnages?", "Unsaved Data!", System::Windows::Forms::MessageBoxButtons::YesNo);
			if (res == System::Windows::Forms::DialogResult::Yes)
				saveSchema();
		}
		txtProc->Text = dh->getParamValue("/oceandata/schemas/*[@name='" + dh->schemaName + "']/afterProcedure");
		dh->setDataArray();
		Int32 retMetadef = dh->setMetadataDefArray();
		initSchmFldLst();

		listFileFlds();

		listMissingFields();
	}


}

System::Void frmOceanData::btnDbConn_Click(System::Object^  sender, System::EventArgs^  e) {

	dh->upsertXMLAttr("/oceandata/destination/odbcparams", "driver", txtDriver->Text);

	dh->upsertXMLAttr("/oceandata/destination/odbcparams", "server", txtSrv->Text);

	dh->upsertXMLAttr("/oceandata/destination/odbcparams", "database", txtDB->Text);

	dh->upsertXMLAttr("/oceandata/destination/odbcparams", "user", txtUserName->Text);

	//please comment the following line before release
	dh->upsertXMLAttr("/oceandata/destination/odbcparams", "password", txtPass->Text);

	dh->xmlDoc->Save("config.xml");

	dh->initDBConn();

	if (dh->odbc){
		listDBMeasurements();

		this->Text = "Oceandata+ Connected to " + dh->dataID;
	}
	else {
		MessageBox::Show("Oceandata+ could not connect to the database");
	}
}

System::Void frmOceanData::txtProc_Validated(System::Object^  sender, System::EventArgs^  e) {
	
	if (txtProc->Modified) {
			String ^xpath = "/oceandata/schemas/schema[@name='" + dh->schemaName + "']";

			dh->upsertXMLNode(xpath, "afterProcedure", txtProc->Text);

			dh->xmlDoc->Save("config.xml");
	}
}

System::Void frmOceanData::dGridVars_CellValidating(System::Object^  sender, System::Windows::Forms::DataGridViewCellValidatingEventArgs^  e) {

	if (e->ColumnIndex == 1) 
	{

		if (e->ToString() == "New Variable...") {

			DataGridView ^dgv;

			dgv = (DataGridView^)sender;

			dgv->CurrentCell = dgv->Rows[e->RowIndex]->Cells[3];
		}
	}

	if (e->ColumnIndex == 2) 
	{
		DataGridView ^dgv;

		dgv = (DataGridView^)sender;

		for each (DataGridViewRow ^ r in dgv->Rows) {

			if (r->Cells[1]->Value == e->ToString()) {
				MessageBox::Show("Measurement Variable in database must be unique.");
				e->Cancel = true;
			}
		}

	}
}

System::Void frmOceanData::dGridVars_CurrentCellDirtyStateChanged(System::Object^  sender, System::EventArgs^  e) {
	
	if (dGridVars->CurrentCell)
	{
		if (dGridVars->CurrentCell->ColumnIndex == 1) {
			String^val = dGridVars->CurrentCell->EditedFormattedValue->ToString();
			if (val == "New Variable...") {
				dGridVars->Rows[dGridVars->CurrentCell->RowIndex]->Cells[3]->ReadOnly = false;
				dGridVars->Rows[dGridVars->CurrentCell->RowIndex]->Cells[3]->Value = dGridVars->Rows[dGridVars->CurrentCell->RowIndex]->Cells[0]->Value;
			}
			else {
				dGridVars->Rows[dGridVars->CurrentCell->RowIndex]->Cells[3]->ReadOnly = true;
				dGridVars->Rows[dGridVars->CurrentCell->RowIndex]->Cells[3]->Value = "";
			}
		}
		dirtySchema = true;
	}

}

array<String^>^ frmOceanData::getUnusedDBFields(int opt) {

	array<String^>^ local;
	int i = 0;
	bool found;

	if (opt==2||opt==3) 
	{
		for each (String^ dbf in lstFldDb->Items)
		{
			found = false;

			for each (OceanDataHandler::dataHandler::Data d in dh->d) 
			{
				if (d.destination == dbf || (!d.destination &&d.Name == dbf))
				{
					for each (DataGridViewRow^ r in dGridVars->Rows)
					{
						if (r->Cells[2]->Value == "Measurement" && d.Name == r->Cells[0]->Value)
							found = true;
					}
				}
			}

			if (!found) 
			{
				Array::Resize(local, i + 1);

				local[i] = dbf;

				i++;
			}
		}
	}
	
	if (opt == 1 || opt == 3) {
		for each (String^ dbf in lstMetadata->Items)
		{
			found = false;
			
			for each (OceanDataHandler::dataHandler::Metadata md in dh->md)
			{
				if (md.Destination == dbf || (!md.Destination &&md.Name == dbf))
				{
					for each (DataGridViewRow^ r in dGridVars->Rows)
					{
						if (r->Cells[2]->Value == "Measurement" && md.Name == r->Cells[0]->Value)
							found = true;
					}
				}
			}

			if (!found) {
				Array::Resize(local, i + 1);

				local[i] = dbf;

				i++;
				}
			}
	}

	return local;
	
	
}

void frmOceanData::saveSchema() {
	try
	{
		String ^ SqlCommand;
		DataTable ^tblDbFld;
		Odbc::OdbcCommand ^cmd = gcnew Odbc::OdbcCommand;
		cmd->Connection = dh->odbc;
		String ^ newVarStr ;

		for each(DataGridViewRow ^i in dGridVars->Rows) {
			if (i->Cells[1]->Value != "Ignore")
			{
				String^ fldName = i->Cells[0]->Value->ToString();
				DataGridViewComboBoxCell ^ cbVar = (DataGridViewComboBoxCell^)i->Cells[1];
				String ^ newVarStr = (String^)cbVar->Value;
			
				bool newVar = newVarStr == "New Variable..." ? true : false;

				String ^dest;

				if (newVar)
					dest = i->Cells[3]->Value->ToString();
				else
					dest =(String^) i->Cells[1]->Value;
				
				DataGridViewComboBoxCell ^ cbTable = (DataGridViewComboBoxCell^)i->Cells[2];
				String^ table = cbTable->Value->ToString();
				XmlNode^ eX;
				String ^val = i->Cells[3]->Value->ToString();
				DataGridViewComboBoxCell ^ cbType = (DataGridViewComboBoxCell^)i->Cells[4];
				String ^ dType = cbType->Value->ToString();

				dest = dest == fldName ? "" : dest;

				dType = dType == "Text" ? "string" : dType;

				if (newVar)
					addNewVar(fldName, dest, table->ToLower(), dType);
				else
					updVar(fldName, dest, table->ToLower(), dType);
			}

		}
		if (txtProc->Modified)
		{
			String ^xpath = "/oceandata/schemas/schema[@name='" + dh->schemaName + "']";

			dh->upsertXMLNode(xpath, "afterProcedure", txtProc->Text);
		}

		dh->xmlDoc->PreserveWhitespace = true;

		dh->xmlDoc->Save("config.xml");

		String ^cmdFields = "Select * from " + dh->path + "\\" + dh->fileName + " where 1=0";

		dh->setMetadataDefArray();
		
		dh->setDataArray();

		dh->fields = dh->getFields(dh->connectionStringOLE, cmdFields);

		listDBMeasurements();

		initSchmFldLst();

		listFileFlds();

		dirtySchema = false;
	}
	catch (Exception^ e)
	{
		MessageBox::Show(e->Message);
	}
}

System::Void frmOceanData::dGridVars_CellValueChanged(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
	
	dirtySchema = true;

	if (e->ColumnIndex == 2) {
		if (dh)
		{
			String^ dest;

			int dindex = dh->getDataIndex(dGridVars->Rows[e->RowIndex]->Cells[0]->Value->ToString());
			int mdindex = dh->getMetaDataIndex(dGridVars->Rows[e->RowIndex]->Cells[0]->Value->ToString());

			if (mdindex != -1)
			{
				OceanData::dataHandler::Metadata md = dh->md[mdindex];
				dest = md.Destination ? md.Destination : md.Name;
			}
			else
			{
				if (dindex != -1)
				{
					OceanData::dataHandler::Data d = dh->d[dindex];
					dest = d.destination ? d.destination : d.Name;
				}
			}

			
			if (dGridVars->CurrentCell->Value->ToString() == "Metadata")
			{
				array<String^> ^unUsedMDFld = getUnusedDBFields(METADATA);

				DataGridViewComboBoxCell ^cbcell = (DataGridViewComboBoxCell^)dGridVars->Rows[e->RowIndex]->Cells[1];

				String ^ oldDest = cbcell->Items[0]->ToString();
				
				cbcell->Items->Clear();

				//cbcell->Items->Add(oldDest);

				cbcell->Items->Add("New Variable...");
				cbcell->Items->Add("Ignore");

				if (dest)
				{
					cbcell->Items->Add(dest);
					cbcell->Value = dest;
				}
				else
					cbcell->Value = "New Variable...";

				if (dh->odbc && unUsedMDFld)
					for each (String^ s in unUsedMDFld)
					cbcell->Items->Add(s);

			}
			else
			{
				array<String^> ^unUsedDFld = getUnusedDBFields(DATA);

				DataGridViewComboBoxCell ^cbcell = (DataGridViewComboBoxCell^)dGridVars->Rows[e->RowIndex]->Cells[1];

				String ^ oldDest = cbcell->Items[0]->ToString();

				cbcell->Items->Clear();

				cbcell->Items->Add(oldDest);

				cbcell->Items->Add("New Variable...");
				cbcell->Items->Add("Ignore");

				if (dest)
				{
					cbcell->Items->Add(dest);
					cbcell->Value = dest;
				}
				else
					cbcell->Value = "New Variable...";

				if (dh->odbc && unUsedDFld)
					for each (String^ s in unUsedDFld)
						cbcell->Items->Add(s);
			}
		}
	}
	if (e->ColumnIndex == 1)
		if (e->RowIndex != -1)
		{
			DataGridViewComboBoxCell ^ varcell = (DataGridViewComboBoxCell^)dGridVars->Rows[e->RowIndex]->Cells[1];

			if (varcell->Value->ToString() == "New Variable...")
			{
				dGridVars->Rows[e->RowIndex]->Cells[4]->Value = "Number";
				dGridVars->Rows[e->RowIndex]->Cells[3]->Value = "";
			}
		}
}

void frmOceanData::listMissingFields() {
	DataGridViewComboBoxCell  ^cbcell;
	String ^dest;
	array<String^> ^unUsedDFld = getUnusedDBFields(DATA);

	for each (String ^ mssFld in lstMssSchm->Items) {
		
		dest = mssFld;
		
		dGridVars->Rows->Add(mssFld, mssFld, "Measurement");

		cbcell = gcnew DataGridViewComboBoxCell;

		cbcell->DisplayStyle = DataGridViewComboBoxDisplayStyle::ComboBox;

		//cbcell->Items->Add(dest);

		cbcell->Items->Add("New Variable...");

		cbcell->Items->Add("Ignore");

		cbcell->Value = "Ignore";

		dGridVars->Rows[dGridVars->Rows->Count - 1]->Cells[4]->Value="Number";

		if (dh->odbc && unUsedDFld)
			for each (String^ s in unUsedDFld)
				cbcell->Items->Add(s);

		dGridVars->Rows[dGridVars->Rows->Count - 1]->Cells[1] = cbcell; 

		dGridVars->Rows[dGridVars->Rows->Count - 1]->Cells[3]->Value = "";
	}
}
void frmOceanData::addNewVar(String ^varName, String ^ dest, String ^ table, String ^dType) {

	XmlNode^ eX;
	int mdindex = dh->getMetaDataIndex(varName);
	int dindex = dh->getDataIndex(varName);
	String ^ xpathGen;

	if (table == "metadata") {
		if (mdindex == -1)
			eX = dh->xmlDoc->CreateElement("field");
		else
			eX = dh->xmlDoc->SelectSingleNode("oceandata/schemas/schema[@name='" + dh->schemaName + "']/metadata/field[@name='" + varName + "']");
		if (dindex > -1) {
			XmlNode ^rN = dh->xmlDoc->SelectSingleNode("oceandata/schemas/schema[@name='" + dh->schemaName + "']/fields/field[@name='" + varName + "']");
			rN->ParentNode->RemoveChild(rN);
		}
		
		xpathGen = "oceandata/schemas/schema[@name='" + dh->schemaName + "']/metadata";

		}
	else {
		if (dindex == -1)
			eX = dh->xmlDoc->CreateElement("field");
		else
			eX = dh->xmlDoc->SelectSingleNode("oceandata/schemas/schema[@name='" + dh->schemaName + "']/fields/field[@name='" + varName + "']");
		if (mdindex > -1) {
			XmlNode ^rN = dh->xmlDoc->SelectSingleNode("oceandata/schemas/schema[@name='" + dh->schemaName + "']/metadata/field[@name='" + varName + "']");
			rN->ParentNode->RemoveChild(rN);
		}
		xpathGen = "oceandata/schemas/schema[@name='" + dh->schemaName + "']/fields";

	}


	XmlAttribute ^attr = dh->xmlDoc->CreateAttribute("name");
	attr->Value = varName;
	eX->Attributes->Append(attr);

	attr = dh->xmlDoc->CreateAttribute("type");
	attr->Value = dType->ToLower();
	eX->Attributes->Append(attr);

	if (dType == "DateTime") 
	{
		attr = dh->xmlDoc->CreateAttribute("destfunction");
		attr->Value = "str_to_date";
		eX->Attributes->Append(attr);

		attr = dh->xmlDoc->CreateAttribute("destparameterstr");
		attr->Value = "%Y-%m-%dT%H:%i:%s";
		eX->Attributes->Append(attr);

	}


	XmlNode ^xGenSchm = dh->xmlDoc->SelectSingleNode(xpathGen);

	xGenSchm->AppendChild(eX);

	dh->upsertXMLAttr(xpathGen + "/field[@name='" + varName + "']", "destination", dest, true);

	addDBCol(dest!=""?dest:varName, table, dType);

	
}
void frmOceanData::updVar(String^ varName, String^ dest, String ^ table, String^ dType) {
	int dindex = dh->getDataIndex(varName);
	int mdindex = dh->getMetaDataIndex(varName);
	String ^xpath;
	
	if (table == "metadata")
	{
		xpath = "/oceandata/schemas/schema[@name='" + dh->schemaName + "']/metadata";
		
		if (dindex != -1) {
			XmlNode ^rN = dh->xmlDoc->SelectSingleNode("oceandata/schemas/schema[@name='" + dh->schemaName + "']/fields/field[@name='" + varName + "']");
			dh->xmlDoc->RemoveChild(rN);
		}

		if (mdindex == -1)
			dh->upsertXMLNode(xpath, "field", "name", varName);
		
		dh->upsertXMLAttr(xpath + "/field[@name='" + varName + "']", "destination", dest, true);
	}
	else
	{
		xpath = "/oceandata/schemas/schema[@name='" + dh->schemaName + "']/fields";

		if (mdindex != -1) {
			XmlNode ^rN = dh->xmlDoc->SelectSingleNode("oceandata/schemas/schema[@name='" + dh->schemaName + "']/metadata/field[@name='" + varName + "']");
			dh->xmlDoc->RemoveChild(rN);
		}

		if (dindex == -1)
			dh->upsertXMLNode(xpath, "field", "name", varName);


		addDBCol((dest != "" && dest!=nullptr) ? dest : varName, table, dType);
	}

	dh->upsertXMLAttr(xpath + "/field[@name='" + varName + "']", "destination", dest, true);

	dh->upsertXMLAttr(xpath + "/field[@name='" + varName + "']", "type", dType->ToLower(), true);

	if (dType == "DateTime")
	{
		dh->upsertXMLAttr(xpath + "/field[@name='" + varName + "']", "destfunction", "str_to_date", true);

		dh->upsertXMLAttr(xpath + "/field[@name='" + varName + "']", "destparameterstr", "%Y-%m-%dT%H:%i:%s", true);
	}
	else
	{
		dh->upsertXMLAttr(xpath + "/field[@name='" + varName + "']", "destfunction", "", true);

		dh->upsertXMLAttr(xpath + "/field[@name='" + varName + "']", "destparameterstr", "", true);
	}
}
void frmOceanData::addDBCol(String^ varName, String ^ table, String^ dType) {
	
	String ^ SqlCommand;
	DataTable ^tblDbFld;
	Odbc::OdbcCommand ^cmd = gcnew Odbc::OdbcCommand;

	dType = dType->ToLower() == "number" ? "` float;" : dType->ToLower() == "string"?"` varchar(50);": "` datetime; ";
	cmd->Connection = dh->odbc;
	
	if (!lstMetadata->Items->Contains(varName) && !lstFldDb->Items->Contains(varName))
	{

		SqlCommand = "Alter table " + table->ToLower() + " add `" + varName + dType;

		cmd->CommandText = SqlCommand;

		cmd->ExecuteNonQuery();
	}
}