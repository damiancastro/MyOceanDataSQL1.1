/****************************************************************
File: dataHandelr.h
Author: Damián Castro
dataHandelr contains all the declaration for the dataHandler
class. This class contains all the functionality to read
and upload data to the store (mysql). The form frmOceanData has
instantiate an object of thi class to process a file or a folder
*****************************************************************/
#pragma once
namespace OceanDataHandler 
{
	using namespace System::Data::Odbc;
	using namespace System::IO;
	using namespace System::Xml;
	using namespace System::Windows::Forms;
	using namespace System::Text::RegularExpressions;
	using namespace System;
	using namespace System::Data;
	using namespace System::Windows::Forms;
	//using namespace MySql::Data::MySqlClient;

	ref class dataHandler
	{
	private:
		//functions
		System::Int32 initXMLDoc(System::String ^ confFile);
		System::String ^ ridOfBrackets (String ^name);
		System::Int32 numbDestExist( array<String^>^ strarray , String ^ destName );
		System::String ^ dataHandler::processField(String ^ fieldNode);
		System::Int32 processMetadataInsertsFN();
		void dataHandler::writeLog(String ^line);
		~dataHandler();
		bool isCalcField(String ^ name);
		bool dataHandler::fldExistFile(String ^ name);
		//vars
		System::Data::Odbc::OdbcDataAdapter ^ dataAdapterODBC;
		//MySqlDataAdapter ^ dataAdapterMySQL;
		System::Data::Common::DbDataAdapter ^ dataAdapter;
		OleDb::OleDbConnectionStringBuilder ^ connectionbuilder;
		System::Data::OleDb::OleDbDataAdapter ^ dataAdapterOLE;
		OleDb::OleDbConnection ^ oledb;
		OdbcTransaction ^ destTrans;
		int lastMetadataID;
		int lastMetadata2ID;
		Boolean batchMode;
		System::String ^ origFile;
		String ^origPath;
		String ^defaultConn;
		

	public:
		//structs
		value struct Metadata {String^ Name; String^ Value; String ^Type; String^ Table;String ^ FieldName;String ^ Function;String ^ ParameterStr;String ^DataTable;String ^destFunction;String ^Destination; String ^ functionType;};
		value struct Data {String^ Name; String^ Value; String ^Type;String ^ Function;String ^ ParameterStr; String ^destination;String ^idEventFunction;};
		//vars
		XmlDocument ^ xmlDoc;
		System::String ^ schemaName;
		array<Metadata> ^md;
		array<Data> ^d;
		String ^ fileName;
		String ^ path;
		String ^ pathMetadata;
		String ^ fileNameNoPath;
		String ^ metadataStyle;
		TextBox ^l;
		ProgressBar ^p;
		String^ dataID;
		String ^ UserName;
		Odbc::OdbcConnection ^ odbc;
		//MySqlConnection ^mySQLConn;
		String ^ fields;
		String^ connectionString;
		String^ connectionStringOLE;
		String ^instType;
		bool autoDetect=false;
		DataTable ^ srcTable;

		#define TIME 1
		#define DEPTH 2
		#define STATION 3
		#define LAT 4
		#define LONG 5
		#define INSTRUMENT 6
		#define DEPLOY 7
		#define RECOVER	8
		#define	MDEPTH 9

	
		//functions
									dataHandler(System::String ^ confFIle);
		System::String ^			dataHandler::getFields (String^ connectionString, String^ strCommandText);
		System::Data::DataTable^	dataHandler::getTable (String^ connStr, String^ sqlcmd);
		System::Data::DataTable^	dataHandler::getTableOLE (String^ connStr, String^ sqlcmd);
		System::String ^			getParamValue (System::String^ xmlpath);
		System::Int32				dataHandler::processInserts (DataTable ^sourceTable);
		System::Int32				processMetadataInsertsEvent (DataTable ^sourceTable);
		System::Int32				mdNameExists( String ^mdName);
		Int32						fileLoaded2DB( String ^fName);
		System::Int32				setMetadataDefArray();
		void						writeDataFile(String ^dataStart);
		System::String ^			dataHandler::getMetadataFields ();
		void						dataHandler::processBatch();
		void						dataHandler::updateFile(String^ file);
		void						dataHandler::setInstrumentType();
		void						dataHandler::setDataArray();
		void						initDBConn();
		void						upsertXMLNode(String ^ xpath, String ^ xName, String ^ xValue);
		void						dataHandler::upsertXMLAttr(String ^ xpath, String ^ xName, String ^ xValue, bool deleteBlank);
		void						dataHandler::upsertXMLAttr(String ^ xpath, String ^ xName, String ^ xValue);
		void						dataHandler::upsertXMLNode(String ^ xpathParent, String ^ xName, String ^ attrName, String ^ attrValue);
		int							getDataIndex(String ^name);
		int							getMetaDataIndex(String ^name);
		Common::DbDataReader ^		getReader(String^ cmd);
		Int32						dNameExists(String ^dName);
		bool						fieldExist(DataTable ^ table, String^ name);
		Metadata ^					getMetadata(int type);
		Data ^						getData(int type);
		String ^					getMetadataSQL();
		bool						check4DepthUnits();
	private: 						System::String ^ dataHandler::processField(dataHandler::Data ^ d);
									

									};
}

