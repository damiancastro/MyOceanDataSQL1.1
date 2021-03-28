/****************************************************************
File: frmOceandata.h
Author: Damián Castro
frmOceandata contains all the decalratiomn for frmOceandata 
form. This form uses the dataHandler class that control access
to the input files and data store (mysql).
*****************************************************************/
#pragma once
#include"dataHandler.h"

namespace OceanData {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace OceanDataHandler;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Data::Odbc;
	using namespace System::IO;
	using namespace System::Xml;
	using namespace System::Text::RegularExpressions;
	private value struct Metadata {String^ Name; String^ Value; String ^Type; String^ Table;String ^ FieldName;String ^ Function;String ^ ParameterStr;String ^DataTable;};
	private value struct Data {String^ Name; String^ Value; String ^Type;String ^ Function;String ^ ParameterStr; String ^destination;};
	
	/// <summary>
	/// Summary for frmOceanData
	/// </summary>
	public ref class frmOceanData : public System::Windows::Forms::Form
	{
	public:
		frmOceanData(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmOceanData()
		{
			if (components)
			{
				delete components;
			}
		}


	protected:







	private: System::Windows::Forms::OpenFileDialog^  openSourceFile;




	private: System::Windows::Forms::BindingSource^  dsSourceFile;

	private:


	protected:

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>




	private: System::Windows::Forms::BindingSource^  dsDataStore;
	private: System::Windows::Forms::FolderBrowserDialog^  openSourceFolder;





















































private: System::Windows::Forms::PictureBox^  pictureBox1;
private: System::Windows::Forms::TabPage^  tabPage2;
private: System::Windows::Forms::TextBox^  txtUploadMsg;
private: System::Windows::Forms::ProgressBar^  progressBar1;
private: System::Windows::Forms::TabPage^  tabPage1;
private: System::Windows::Forms::Button^  btnUpd;
private: System::Windows::Forms::DataGridView^  dataGridSource;
private: System::Windows::Forms::TabPage^  tabPage6;
private: System::Windows::Forms::DataGridView^  dGridVars;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  colFileVars;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  colDbVars;
private: System::Windows::Forms::DataGridViewComboBoxColumn^  colTable;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  colNewVar;
private: System::Windows::Forms::DataGridViewComboBoxColumn^  colType;
private: System::Windows::Forms::Button^  btnAdd2Schm;
private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::Button^  btnSaveAs;
private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::TextBox^  txtProc;
private: System::Windows::Forms::TextBox^  txtSchemaNewName;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::ComboBox^  cmbSchemas;
private: System::Windows::Forms::TabPage^  tabPage4;
private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::ListBox^  lstMssSchm;
private: System::Windows::Forms::Button^  btnUploadBatch;
private: System::Windows::Forms::Label^  lblLstFldsFile;
private: System::Windows::Forms::ListBox^  lstFileFlds;
private: System::Windows::Forms::Button^  btnLoad;
private: System::Windows::Forms::CheckBox^  chkAutodetect;
private: System::Windows::Forms::TextBox^  textSourceFile;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::ComboBox^  cmbInstrument;
private: System::Windows::Forms::Button^  btnBrowse;
private: System::Windows::Forms::TabPage^  tabPage3;
private: System::Windows::Forms::GroupBox^  groupBox2;
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::ListBox^  lstMetadata;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::TextBox^  txtDriver;
private: System::Windows::Forms::Button^  btnDbConn;
private: System::Windows::Forms::Label^  pblPass;
private: System::Windows::Forms::TextBox^  txtPass;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::TextBox^  txtDB;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::TextBox^  txtSrv;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::ListBox^  lstFldDb;
private: System::Windows::Forms::TextBox^  txtUserName;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::TabControl^  tabControl1;





























			 //Visual studio generate form values



	private: System::Windows::Forms::BindingSource^  dsMetadata;

#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmOceanData::typeid));
				 this->openSourceFile = (gcnew System::Windows::Forms::OpenFileDialog());
				 this->dsSourceFile = (gcnew System::Windows::Forms::BindingSource(this->components));
				 this->dsDataStore = (gcnew System::Windows::Forms::BindingSource(this->components));
				 this->dsMetadata = (gcnew System::Windows::Forms::BindingSource(this->components));
				 this->openSourceFolder = (gcnew System::Windows::Forms::FolderBrowserDialog());
				 this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
				 this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
				 this->txtUploadMsg = (gcnew System::Windows::Forms::TextBox());
				 this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
				 this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
				 this->btnUpd = (gcnew System::Windows::Forms::Button());
				 this->dataGridSource = (gcnew System::Windows::Forms::DataGridView());
				 this->tabPage6 = (gcnew System::Windows::Forms::TabPage());
				 this->dGridVars = (gcnew System::Windows::Forms::DataGridView());
				 this->colFileVars = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->colDbVars = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->colTable = (gcnew System::Windows::Forms::DataGridViewComboBoxColumn());
				 this->colNewVar = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->colType = (gcnew System::Windows::Forms::DataGridViewComboBoxColumn());
				 this->btnAdd2Schm = (gcnew System::Windows::Forms::Button());
				 this->label15 = (gcnew System::Windows::Forms::Label());
				 this->btnSaveAs = (gcnew System::Windows::Forms::Button());
				 this->label13 = (gcnew System::Windows::Forms::Label());
				 this->txtProc = (gcnew System::Windows::Forms::TextBox());
				 this->txtSchemaNewName = (gcnew System::Windows::Forms::TextBox());
				 this->label9 = (gcnew System::Windows::Forms::Label());
				 this->cmbSchemas = (gcnew System::Windows::Forms::ComboBox());
				 this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
				 this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
				 this->label6 = (gcnew System::Windows::Forms::Label());
				 this->lstMssSchm = (gcnew System::Windows::Forms::ListBox());
				 this->btnUploadBatch = (gcnew System::Windows::Forms::Button());
				 this->lblLstFldsFile = (gcnew System::Windows::Forms::Label());
				 this->lstFileFlds = (gcnew System::Windows::Forms::ListBox());
				 this->btnLoad = (gcnew System::Windows::Forms::Button());
				 this->chkAutodetect = (gcnew System::Windows::Forms::CheckBox());
				 this->textSourceFile = (gcnew System::Windows::Forms::TextBox());
				 this->label3 = (gcnew System::Windows::Forms::Label());
				 this->label1 = (gcnew System::Windows::Forms::Label());
				 this->cmbInstrument = (gcnew System::Windows::Forms::ComboBox());
				 this->btnBrowse = (gcnew System::Windows::Forms::Button());
				 this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
				 this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
				 this->label10 = (gcnew System::Windows::Forms::Label());
				 this->lstMetadata = (gcnew System::Windows::Forms::ListBox());
				 this->label8 = (gcnew System::Windows::Forms::Label());
				 this->txtDriver = (gcnew System::Windows::Forms::TextBox());
				 this->btnDbConn = (gcnew System::Windows::Forms::Button());
				 this->pblPass = (gcnew System::Windows::Forms::Label());
				 this->txtPass = (gcnew System::Windows::Forms::TextBox());
				 this->label4 = (gcnew System::Windows::Forms::Label());
				 this->txtDB = (gcnew System::Windows::Forms::TextBox());
				 this->label2 = (gcnew System::Windows::Forms::Label());
				 this->txtSrv = (gcnew System::Windows::Forms::TextBox());
				 this->label7 = (gcnew System::Windows::Forms::Label());
				 this->lstFldDb = (gcnew System::Windows::Forms::ListBox());
				 this->txtUserName = (gcnew System::Windows::Forms::TextBox());
				 this->label5 = (gcnew System::Windows::Forms::Label());
				 this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dsSourceFile))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dsDataStore))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dsMetadata))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
				 this->tabPage2->SuspendLayout();
				 this->tabPage1->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridSource))->BeginInit();
				 this->tabPage6->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dGridVars))->BeginInit();
				 this->tabPage4->SuspendLayout();
				 this->groupBox1->SuspendLayout();
				 this->tabPage3->SuspendLayout();
				 this->groupBox2->SuspendLayout();
				 this->tabControl1->SuspendLayout();
				 this->SuspendLayout();
				 // 
				 // openSourceFile
				 // 
				 this->openSourceFile->FileName = L"openFileDialog1";
				 // 
				 // pictureBox1
				 // 
				 this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
				 this->pictureBox1->InitialImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.InitialImage")));
				 this->pictureBox1->Location = System::Drawing::Point(27, 12);
				 this->pictureBox1->Name = L"pictureBox1";
				 this->pictureBox1->Size = System::Drawing::Size(478, 92);
				 this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
				 this->pictureBox1->TabIndex = 1;
				 this->pictureBox1->TabStop = false;
				 // 
				 // tabPage2
				 // 
				 this->tabPage2->Controls->Add(this->txtUploadMsg);
				 this->tabPage2->Controls->Add(this->progressBar1);
				 this->tabPage2->Location = System::Drawing::Point(4, 22);
				 this->tabPage2->Name = L"tabPage2";
				 this->tabPage2->Padding = System::Windows::Forms::Padding(3);
				 this->tabPage2->Size = System::Drawing::Size(524, 442);
				 this->tabPage2->TabIndex = 5;
				 this->tabPage2->Text = L"Log";
				 this->tabPage2->UseVisualStyleBackColor = true;
				 // 
				 // txtUploadMsg
				 // 
				 this->txtUploadMsg->Location = System::Drawing::Point(9, 6);
				 this->txtUploadMsg->Multiline = true;
				 this->txtUploadMsg->Name = L"txtUploadMsg";
				 this->txtUploadMsg->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
				 this->txtUploadMsg->Size = System::Drawing::Size(509, 398);
				 this->txtUploadMsg->TabIndex = 6;
				 // 
				 // progressBar1
				 // 
				 this->progressBar1->Location = System::Drawing::Point(7, 410);
				 this->progressBar1->Name = L"progressBar1";
				 this->progressBar1->Size = System::Drawing::Size(511, 26);
				 this->progressBar1->Step = 1;
				 this->progressBar1->TabIndex = 5;
				 this->progressBar1->UseWaitCursor = true;
				 // 
				 // tabPage1
				 // 
				 this->tabPage1->Controls->Add(this->btnUpd);
				 this->tabPage1->Controls->Add(this->dataGridSource);
				 this->tabPage1->Location = System::Drawing::Point(4, 22);
				 this->tabPage1->Name = L"tabPage1";
				 this->tabPage1->Padding = System::Windows::Forms::Padding(3);
				 this->tabPage1->Size = System::Drawing::Size(524, 442);
				 this->tabPage1->TabIndex = 0;
				 this->tabPage1->Text = L"Source";
				 this->tabPage1->UseVisualStyleBackColor = true;
				 // 
				 // btnUpd
				 // 
				 this->btnUpd->Location = System::Drawing::Point(443, 413);
				 this->btnUpd->Name = L"btnUpd";
				 this->btnUpd->Size = System::Drawing::Size(75, 23);
				 this->btnUpd->TabIndex = 1;
				 this->btnUpd->Text = L"Update";
				 this->btnUpd->UseVisualStyleBackColor = true;
				 this->btnUpd->Click += gcnew System::EventHandler(this, &frmOceanData::btnUpd_Click);
				 // 
				 // dataGridSource
				 // 
				 this->dataGridSource->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
				 this->dataGridSource->Location = System::Drawing::Point(0, 6);
				 this->dataGridSource->Name = L"dataGridSource";
				 this->dataGridSource->Size = System::Drawing::Size(518, 401);
				 this->dataGridSource->TabIndex = 0;
				 // 
				 // tabPage6
				 // 
				 this->tabPage6->Controls->Add(this->dGridVars);
				 this->tabPage6->Controls->Add(this->btnAdd2Schm);
				 this->tabPage6->Controls->Add(this->label15);
				 this->tabPage6->Controls->Add(this->btnSaveAs);
				 this->tabPage6->Controls->Add(this->label13);
				 this->tabPage6->Controls->Add(this->txtProc);
				 this->tabPage6->Controls->Add(this->txtSchemaNewName);
				 this->tabPage6->Controls->Add(this->label9);
				 this->tabPage6->Controls->Add(this->cmbSchemas);
				 this->tabPage6->Location = System::Drawing::Point(4, 22);
				 this->tabPage6->Name = L"tabPage6";
				 this->tabPage6->Size = System::Drawing::Size(524, 442);
				 this->tabPage6->TabIndex = 6;
				 this->tabPage6->Text = L"VarMap";
				 this->tabPage6->UseVisualStyleBackColor = true;
				 // 
				 // dGridVars
				 // 
				 this->dGridVars->AllowUserToAddRows = false;
				 this->dGridVars->AllowUserToDeleteRows = false;
				 this->dGridVars->AllowUserToOrderColumns = true;
				 this->dGridVars->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
				 this->dGridVars->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {
					 this->colFileVars,
						 this->colDbVars, this->colTable, this->colNewVar, this->colType
				 });
				 this->dGridVars->EditMode = System::Windows::Forms::DataGridViewEditMode::EditOnEnter;
				 this->dGridVars->Location = System::Drawing::Point(3, 61);
				 this->dGridVars->Name = L"dGridVars";
				 this->dGridVars->RowHeadersVisible = false;
				 this->dGridVars->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
				 this->dGridVars->Size = System::Drawing::Size(516, 266);
				 this->dGridVars->TabIndex = 32;
				 this->dGridVars->CellValidating += gcnew System::Windows::Forms::DataGridViewCellValidatingEventHandler(this, &frmOceanData::dGridVars_CellValidating);
				 this->dGridVars->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &frmOceanData::dGridVars_CellValueChanged);
				 this->dGridVars->CurrentCellDirtyStateChanged += gcnew System::EventHandler(this, &frmOceanData::dGridVars_CurrentCellDirtyStateChanged);
				 // 
				 // colFileVars
				 // 
				 this->colFileVars->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
				 this->colFileVars->FillWeight = 25;
				 this->colFileVars->HeaderText = L"File Variables";
				 this->colFileVars->Name = L"colFileVars";
				 this->colFileVars->ReadOnly = true;
				 // 
				 // colDbVars
				 // 
				 this->colDbVars->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
				 this->colDbVars->FillWeight = 27;
				 this->colDbVars->HeaderText = L"Database Variables";
				 this->colDbVars->Name = L"colDbVars";
				 this->colDbVars->Resizable = System::Windows::Forms::DataGridViewTriState::True;
				 this->colDbVars->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
				 // 
				 // colTable
				 // 
				 this->colTable->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
				 this->colTable->DisplayStyle = System::Windows::Forms::DataGridViewComboBoxDisplayStyle::ComboBox;
				 this->colTable->FillWeight = 23;
				 this->colTable->HeaderText = L"Table";
				 this->colTable->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Metadata", L"Measurement" });
				 this->colTable->Name = L"colTable";
				 this->colTable->Resizable = System::Windows::Forms::DataGridViewTriState::True;
				 // 
				 // colNewVar
				 // 
				 this->colNewVar->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
				 this->colNewVar->FillWeight = 25;
				 this->colNewVar->HeaderText = L"New Variables";
				 this->colNewVar->Name = L"colNewVar";
				 this->colNewVar->ReadOnly = true;
				 // 
				 // colType
				 // 
				 this->colType->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
				 this->colType->DisplayStyle = System::Windows::Forms::DataGridViewComboBoxDisplayStyle::ComboBox;
				 this->colType->FillWeight = 18;
				 this->colType->HeaderText = L"Data Type";
				 this->colType->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"Number", L"Text", L"DateTime" });
				 this->colType->Name = L"colType";
				 // 
				 // btnAdd2Schm
				 // 
				 this->btnAdd2Schm->Location = System::Drawing::Point(402, 342);
				 this->btnAdd2Schm->Name = L"btnAdd2Schm";
				 this->btnAdd2Schm->Size = System::Drawing::Size(117, 32);
				 this->btnAdd2Schm->TabIndex = 8;
				 this->btnAdd2Schm->Text = L"Save mapping";
				 this->btnAdd2Schm->UseVisualStyleBackColor = true;
				 this->btnAdd2Schm->Click += gcnew System::EventHandler(this, &frmOceanData::btnAdd2Schm_Click);
				 // 
				 // label15
				 // 
				 this->label15->AutoSize = true;
				 this->label15->Location = System::Drawing::Point(3, 400);
				 this->label15->Name = L"label15";
				 this->label15->Size = System::Drawing::Size(210, 13);
				 this->label15->TabIndex = 31;
				 this->label15->Text = L"Procedure to run in the database (Optional)\r\n";
				 // 
				 // btnSaveAs
				 // 
				 this->btnSaveAs->Location = System::Drawing::Point(3, 342);
				 this->btnSaveAs->Name = L"btnSaveAs";
				 this->btnSaveAs->Size = System::Drawing::Size(112, 32);
				 this->btnSaveAs->TabIndex = 27;
				 this->btnSaveAs->Text = L"Clone Mapping As:";
				 this->btnSaveAs->UseVisualStyleBackColor = true;
				 this->btnSaveAs->Click += gcnew System::EventHandler(this, &frmOceanData::btnSaveAs_Click);
				 // 
				 // label13
				 // 
				 this->label13->AutoSize = true;
				 this->label13->Location = System::Drawing::Point(118, 338);
				 this->label13->Name = L"label13";
				 this->label13->Size = System::Drawing::Size(65, 13);
				 this->label13->TabIndex = 29;
				 this->label13->Text = L"Clone Name";
				 // 
				 // txtProc
				 // 
				 this->txtProc->Location = System::Drawing::Point(6, 416);
				 this->txtProc->Name = L"txtProc";
				 this->txtProc->Size = System::Drawing::Size(201, 20);
				 this->txtProc->TabIndex = 30;
				 this->txtProc->Validated += gcnew System::EventHandler(this, &frmOceanData::txtProc_Validated);
				 // 
				 // txtSchemaNewName
				 // 
				 this->txtSchemaNewName->Location = System::Drawing::Point(121, 354);
				 this->txtSchemaNewName->Name = L"txtSchemaNewName";
				 this->txtSchemaNewName->Size = System::Drawing::Size(165, 20);
				 this->txtSchemaNewName->TabIndex = 28;
				 // 
				 // label9
				 // 
				 this->label9->AutoSize = true;
				 this->label9->Location = System::Drawing::Point(3, 37);
				 this->label9->Name = L"label9";
				 this->label9->Size = System::Drawing::Size(97, 13);
				 this->label9->TabIndex = 24;
				 this->label9->Text = L"Variable Mappings:";
				 // 
				 // cmbSchemas
				 // 
				 this->cmbSchemas->CausesValidation = false;
				 this->cmbSchemas->FormattingEnabled = true;
				 this->cmbSchemas->Location = System::Drawing::Point(117, 34);
				 this->cmbSchemas->Name = L"cmbSchemas";
				 this->cmbSchemas->Size = System::Drawing::Size(339, 21);
				 this->cmbSchemas->TabIndex = 23;
				 this->cmbSchemas->SelectedIndexChanged += gcnew System::EventHandler(this, &frmOceanData::cmbSchemas_SelectedIndexChanged);
				 // 
				 // tabPage4
				 // 
				 this->tabPage4->Controls->Add(this->groupBox1);
				 this->tabPage4->Location = System::Drawing::Point(4, 22);
				 this->tabPage4->Name = L"tabPage4";
				 this->tabPage4->Size = System::Drawing::Size(524, 442);
				 this->tabPage4->TabIndex = 3;
				 this->tabPage4->Text = L"File";
				 this->tabPage4->UseVisualStyleBackColor = true;
				 // 
				 // groupBox1
				 // 
				 this->groupBox1->Controls->Add(this->label6);
				 this->groupBox1->Controls->Add(this->lstMssSchm);
				 this->groupBox1->Controls->Add(this->btnUploadBatch);
				 this->groupBox1->Controls->Add(this->lblLstFldsFile);
				 this->groupBox1->Controls->Add(this->lstFileFlds);
				 this->groupBox1->Controls->Add(this->btnLoad);
				 this->groupBox1->Controls->Add(this->chkAutodetect);
				 this->groupBox1->Controls->Add(this->textSourceFile);
				 this->groupBox1->Controls->Add(this->label3);
				 this->groupBox1->Controls->Add(this->label1);
				 this->groupBox1->Controls->Add(this->cmbInstrument);
				 this->groupBox1->Controls->Add(this->btnBrowse);
				 this->groupBox1->Location = System::Drawing::Point(5, 18);
				 this->groupBox1->Name = L"groupBox1";
				 this->groupBox1->Size = System::Drawing::Size(514, 418);
				 this->groupBox1->TabIndex = 26;
				 this->groupBox1->TabStop = false;
				 this->groupBox1->Text = L"Input File";
				 // 
				 // label6
				 // 
				 this->label6->AutoSize = true;
				 this->label6->Location = System::Drawing::Point(270, 106);
				 this->label6->Name = L"label6";
				 this->label6->Size = System::Drawing::Size(75, 13);
				 this->label6->TabIndex = 25;
				 this->label6->Text = L"New Variables";
				 // 
				 // lstMssSchm
				 // 
				 this->lstMssSchm->FormattingEnabled = true;
				 this->lstMssSchm->Location = System::Drawing::Point(273, 122);
				 this->lstMssSchm->Name = L"lstMssSchm";
				 this->lstMssSchm->Size = System::Drawing::Size(220, 238);
				 this->lstMssSchm->TabIndex = 24;
				 // 
				 // btnUploadBatch
				 // 
				 this->btnUploadBatch->Location = System::Drawing::Point(299, 384);
				 this->btnUploadBatch->Name = L"btnUploadBatch";
				 this->btnUploadBatch->Size = System::Drawing::Size(95, 22);
				 this->btnUploadBatch->TabIndex = 13;
				 this->btnUploadBatch->Text = L"Update Batch";
				 this->btnUploadBatch->UseVisualStyleBackColor = true;
				 this->btnUploadBatch->Click += gcnew System::EventHandler(this, &frmOceanData::btnUploadBatch_Click);
				 // 
				 // lblLstFldsFile
				 // 
				 this->lblLstFldsFile->AutoSize = true;
				 this->lblLstFldsFile->Location = System::Drawing::Point(19, 106);
				 this->lblLstFldsFile->Name = L"lblLstFldsFile";
				 this->lblLstFldsFile->Size = System::Drawing::Size(80, 13);
				 this->lblLstFldsFile->TabIndex = 23;
				 this->lblLstFldsFile->Text = L"Variables in File";
				 // 
				 // lstFileFlds
				 // 
				 this->lstFileFlds->FormattingEnabled = true;
				 this->lstFileFlds->Location = System::Drawing::Point(22, 122);
				 this->lstFileFlds->Name = L"lstFileFlds";
				 this->lstFileFlds->Size = System::Drawing::Size(220, 238);
				 this->lstFileFlds->TabIndex = 22;
				 // 
				 // btnLoad
				 // 
				 this->btnLoad->Enabled = false;
				 this->btnLoad->Location = System::Drawing::Point(400, 383);
				 this->btnLoad->Name = L"btnLoad";
				 this->btnLoad->Size = System::Drawing::Size(94, 23);
				 this->btnLoad->TabIndex = 3;
				 this->btnLoad->Text = L"Load";
				 this->btnLoad->UseVisualStyleBackColor = true;
				 this->btnLoad->Click += gcnew System::EventHandler(this, &frmOceanData::btnLoad_Click);
				 // 
				 // chkAutodetect
				 // 
				 this->chkAutodetect->AutoSize = true;
				 this->chkAutodetect->Location = System::Drawing::Point(310, 37);
				 this->chkAutodetect->Name = L"chkAutodetect";
				 this->chkAutodetect->Size = System::Drawing::Size(157, 17);
				 this->chkAutodetect->TabIndex = 14;
				 this->chkAutodetect->Text = L"Autodetect Instrument Type";
				 this->chkAutodetect->UseVisualStyleBackColor = true;
				 this->chkAutodetect->CheckedChanged += gcnew System::EventHandler(this, &frmOceanData::chkAutodetect_CheckedChanged);
				 // 
				 // textSourceFile
				 // 
				 this->textSourceFile->Location = System::Drawing::Point(131, 69);
				 this->textSourceFile->Name = L"textSourceFile";
				 this->textSourceFile->Size = System::Drawing::Size(263, 20);
				 this->textSourceFile->TabIndex = 0;
				 // 
				 // label3
				 // 
				 this->label3->AutoSize = true;
				 this->label3->Location = System::Drawing::Point(19, 75);
				 this->label3->Name = L"label3";
				 this->label3->Size = System::Drawing::Size(50, 13);
				 this->label3->TabIndex = 8;
				 this->label3->Text = L"Input File";
				 // 
				 // label1
				 // 
				 this->label1->AutoSize = true;
				 this->label1->Location = System::Drawing::Point(19, 38);
				 this->label1->Name = L"label1";
				 this->label1->Size = System::Drawing::Size(83, 13);
				 this->label1->TabIndex = 6;
				 this->label1->Text = L"Instrument Type";
				 // 
				 // cmbInstrument
				 // 
				 this->cmbInstrument->CausesValidation = false;
				 this->cmbInstrument->FormattingEnabled = true;
				 this->cmbInstrument->Location = System::Drawing::Point(131, 35);
				 this->cmbInstrument->Name = L"cmbInstrument";
				 this->cmbInstrument->Size = System::Drawing::Size(138, 21);
				 this->cmbInstrument->TabIndex = 2;
				 this->cmbInstrument->SelectedIndexChanged += gcnew System::EventHandler(this, &frmOceanData::cmbInstrument_SelectedIndexChanged);
				 // 
				 // btnBrowse
				 // 
				 this->btnBrowse->Location = System::Drawing::Point(400, 69);
				 this->btnBrowse->Name = L"btnBrowse";
				 this->btnBrowse->Size = System::Drawing::Size(67, 20);
				 this->btnBrowse->TabIndex = 1;
				 this->btnBrowse->Text = L"Browse ...";
				 this->btnBrowse->UseVisualStyleBackColor = true;
				 this->btnBrowse->Click += gcnew System::EventHandler(this, &frmOceanData::btnBrowse_Click);
				 // 
				 // tabPage3
				 // 
				 this->tabPage3->Controls->Add(this->groupBox2);
				 this->tabPage3->Location = System::Drawing::Point(4, 22);
				 this->tabPage3->Name = L"tabPage3";
				 this->tabPage3->Padding = System::Windows::Forms::Padding(3);
				 this->tabPage3->Size = System::Drawing::Size(524, 442);
				 this->tabPage3->TabIndex = 7;
				 this->tabPage3->Text = L"Database";
				 this->tabPage3->UseVisualStyleBackColor = true;
				 // 
				 // groupBox2
				 // 
				 this->groupBox2->Controls->Add(this->label10);
				 this->groupBox2->Controls->Add(this->lstMetadata);
				 this->groupBox2->Controls->Add(this->label8);
				 this->groupBox2->Controls->Add(this->txtDriver);
				 this->groupBox2->Controls->Add(this->btnDbConn);
				 this->groupBox2->Controls->Add(this->pblPass);
				 this->groupBox2->Controls->Add(this->txtPass);
				 this->groupBox2->Controls->Add(this->label4);
				 this->groupBox2->Controls->Add(this->txtDB);
				 this->groupBox2->Controls->Add(this->label2);
				 this->groupBox2->Controls->Add(this->txtSrv);
				 this->groupBox2->Controls->Add(this->label7);
				 this->groupBox2->Controls->Add(this->lstFldDb);
				 this->groupBox2->Controls->Add(this->txtUserName);
				 this->groupBox2->Controls->Add(this->label5);
				 this->groupBox2->Location = System::Drawing::Point(5, 16);
				 this->groupBox2->Name = L"groupBox2";
				 this->groupBox2->Size = System::Drawing::Size(513, 423);
				 this->groupBox2->TabIndex = 28;
				 this->groupBox2->TabStop = false;
				 this->groupBox2->Text = L"Destination Database";
				 // 
				 // label10
				 // 
				 this->label10->AutoSize = true;
				 this->label10->Location = System::Drawing::Point(313, 159);
				 this->label10->Name = L"label10";
				 this->label10->Size = System::Drawing::Size(158, 13);
				 this->label10->TabIndex = 32;
				 this->label10->Text = L"Metadata Variables in Database";
				 // 
				 // lstMetadata
				 // 
				 this->lstMetadata->FormattingEnabled = true;
				 this->lstMetadata->Location = System::Drawing::Point(316, 175);
				 this->lstMetadata->Name = L"lstMetadata";
				 this->lstMetadata->Size = System::Drawing::Size(177, 199);
				 this->lstMetadata->TabIndex = 31;
				 // 
				 // label8
				 // 
				 this->label8->AutoSize = true;
				 this->label8->Location = System::Drawing::Point(18, 18);
				 this->label8->Name = L"label8";
				 this->label8->Size = System::Drawing::Size(35, 13);
				 this->label8->TabIndex = 30;
				 this->label8->Text = L"Driver";
				 // 
				 // txtDriver
				 // 
				 this->txtDriver->Location = System::Drawing::Point(21, 34);
				 this->txtDriver->Name = L"txtDriver";
				 this->txtDriver->Size = System::Drawing::Size(268, 20);
				 this->txtDriver->TabIndex = 29;
				 // 
				 // btnDbConn
				 // 
				 this->btnDbConn->Location = System::Drawing::Point(398, 380);
				 this->btnDbConn->Name = L"btnDbConn";
				 this->btnDbConn->Size = System::Drawing::Size(95, 21);
				 this->btnDbConn->TabIndex = 28;
				 this->btnDbConn->Text = L"Connect";
				 this->btnDbConn->UseVisualStyleBackColor = true;
				 this->btnDbConn->Click += gcnew System::EventHandler(this, &frmOceanData::btnDbConn_Click);
				 // 
				 // pblPass
				 // 
				 this->pblPass->AutoSize = true;
				 this->pblPass->Location = System::Drawing::Point(309, 59);
				 this->pblPass->Name = L"pblPass";
				 this->pblPass->Size = System::Drawing::Size(53, 13);
				 this->pblPass->TabIndex = 27;
				 this->pblPass->Text = L"Password";
				 // 
				 // txtPass
				 // 
				 this->txtPass->Location = System::Drawing::Point(312, 75);
				 this->txtPass->Name = L"txtPass";
				 this->txtPass->PasswordChar = '*';
				 this->txtPass->Size = System::Drawing::Size(174, 20);
				 this->txtPass->TabIndex = 26;
				 // 
				 // label4
				 // 
				 this->label4->AutoSize = true;
				 this->label4->Location = System::Drawing::Point(17, 104);
				 this->label4->Name = L"label4";
				 this->label4->Size = System::Drawing::Size(53, 13);
				 this->label4->TabIndex = 25;
				 this->label4->Text = L"Database";
				 // 
				 // txtDB
				 // 
				 this->txtDB->Location = System::Drawing::Point(20, 120);
				 this->txtDB->Name = L"txtDB";
				 this->txtDB->Size = System::Drawing::Size(269, 20);
				 this->txtDB->TabIndex = 24;
				 // 
				 // label2
				 // 
				 this->label2->AutoSize = true;
				 this->label2->Location = System::Drawing::Point(17, 59);
				 this->label2->Name = L"label2";
				 this->label2->Size = System::Drawing::Size(38, 13);
				 this->label2->TabIndex = 23;
				 this->label2->Text = L"Server";
				 // 
				 // txtSrv
				 // 
				 this->txtSrv->Location = System::Drawing::Point(21, 75);
				 this->txtSrv->Name = L"txtSrv";
				 this->txtSrv->Size = System::Drawing::Size(268, 20);
				 this->txtSrv->TabIndex = 22;
				 // 
				 // label7
				 // 
				 this->label7->AutoSize = true;
				 this->label7->Location = System::Drawing::Point(28, 159);
				 this->label7->Name = L"label7";
				 this->label7->Size = System::Drawing::Size(177, 13);
				 this->label7->TabIndex = 21;
				 this->label7->Text = L"Measurement Variables in Database";
				 // 
				 // lstFldDb
				 // 
				 this->lstFldDb->FormattingEnabled = true;
				 this->lstFldDb->Location = System::Drawing::Point(31, 175);
				 this->lstFldDb->Name = L"lstFldDb";
				 this->lstFldDb->Size = System::Drawing::Size(177, 199);
				 this->lstFldDb->TabIndex = 20;
				 // 
				 // txtUserName
				 // 
				 this->txtUserName->Location = System::Drawing::Point(312, 34);
				 this->txtUserName->Multiline = true;
				 this->txtUserName->Name = L"txtUserName";
				 this->txtUserName->Size = System::Drawing::Size(174, 23);
				 this->txtUserName->TabIndex = 16;
				 // 
				 // label5
				 // 
				 this->label5->AutoSize = true;
				 this->label5->Location = System::Drawing::Point(309, 18);
				 this->label5->Name = L"label5";
				 this->label5->Size = System::Drawing::Size(60, 13);
				 this->label5->TabIndex = 15;
				 this->label5->Text = L"User Name";
				 // 
				 // tabControl1
				 // 
				 this->tabControl1->Controls->Add(this->tabPage3);
				 this->tabControl1->Controls->Add(this->tabPage4);
				 this->tabControl1->Controls->Add(this->tabPage6);
				 this->tabControl1->Controls->Add(this->tabPage1);
				 this->tabControl1->Controls->Add(this->tabPage2);
				 this->tabControl1->Location = System::Drawing::Point(3, 110);
				 this->tabControl1->Name = L"tabControl1";
				 this->tabControl1->SelectedIndex = 0;
				 this->tabControl1->Size = System::Drawing::Size(532, 468);
				 this->tabControl1->TabIndex = 0;
				 // 
				 // frmOceanData
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(538, 580);
				 this->Controls->Add(this->pictureBox1);
				 this->Controls->Add(this->tabControl1);
				 this->Name = L"frmOceanData";
				 this->Text = L"MyOceanDataSQL";
				 this->Load += gcnew System::EventHandler(this, &frmOceanData::frmOceanData_Load);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dsSourceFile))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dsDataStore))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dsMetadata))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
				 this->tabPage2->ResumeLayout(false);
				 this->tabPage2->PerformLayout();
				 this->tabPage1->ResumeLayout(false);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridSource))->EndInit();
				 this->tabPage6->ResumeLayout(false);
				 this->tabPage6->PerformLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dGridVars))->EndInit();
				 this->tabPage4->ResumeLayout(false);
				 this->groupBox1->ResumeLayout(false);
				 this->groupBox1->PerformLayout();
				 this->tabPage3->ResumeLayout(false);
				 this->groupBox2->ResumeLayout(false);
				 this->groupBox2->PerformLayout();
				 this->tabControl1->ResumeLayout(false);
				 this->ResumeLayout(false);

			 }
#pragma endregion


	private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btnLoad_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void frmOceanData_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btnUpd_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btnUpdNoLoad_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btnUploadBatch_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void txtUserName_TextChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void cmbInstrument_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void chkAutodetect_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btnAdd2Schm_Click(System::Object^  sender, System::EventArgs^  e);

			 //private: System::Void txtDest_Validated(System::Object^  sender, System::EventArgs^  e);
			 //private: System::Void txtDestFunc_Validated(System::Object^  sender, System::EventArgs^  e);
			 //private: System::Void txtPar_Validated(System::Object^  sender, System::EventArgs^  e);
			 //private: System::Void chkTextFld_Validated(System::Object^  sender, System::EventArgs^  e);
			 //private: System::Void chkTextFld_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btnSaveAs_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void cmbSchemas_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btnDbConn_Click(System::Object^  sender, System::EventArgs^  e);
			 //
			 //private: System::Void txtName_Validated(System::Object^  sender, System::EventArgs^  e);
			 //private: System::Void txtName_Validating(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e);
	private: System::Void txtProc_Validated(System::Object^  sender, System::EventArgs^  e);

	private: System::Void dGridVars_CellValidating(System::Object^  sender, System::Windows::Forms::DataGridViewCellValidatingEventArgs^  e);

	private: System::Void dGridVars_CurrentCellDirtyStateChanged(System::Object^  sender, System::EventArgs^  e);


	private: System::Void dGridVars_CellValueChanged(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e);




//CustomFunctions:


	private: System::Void fillSourceTypesCombo();
			void frmOceanData::initSchmFldLst();
			void  frmOceanData::loadConfig();
			void  frmOceanData::listDBMeasurements();
			void  frmOceanData::loadDefSchema();
			void frmOceanData::listFileFlds();
			void frmOceanData::saveSchema();
			array<String^>^ frmOceanData::getUnusedDBFields(int opt);
			void frmOceanData::listMissingFields();
			void frmOceanData::addNewVar(String ^varname, String ^ dest, String ^ table, String ^ dType);
			void frmOceanData::updVar(String^ varName, String^ dest, String ^ table, String ^ dType);
			void frmOceanData::addDBCol(String^ varName, String ^ table, String^ dType);

//Variables
	private:
		dataHandler ^ dh;
		String^ source;
		bool chtTextChanged;
		bool dirtySchema;




	};
}

//Constants
#define  METADATA  1
#define  DATA  2
#define  ALL 3