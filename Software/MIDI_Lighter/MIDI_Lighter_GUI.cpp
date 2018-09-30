#include "MIDI_Lighter_GUI.h"

MIDI_Lighter_GUI::MIDI_Lighter_GUI::MIDI_Lighter_GUI()
{
	this->Text = L"MIDI Lighter";
	this->Size = System::Drawing::Size(1200, 480);

	_Resources = gcnew System::Resources::ResourceManager("MIDI_Lighter.MIDI_Lighter", System::Reflection::Assembly::GetExecutingAssembly());


	MIDI_Lighter::Device_List^ Device_List = gcnew MIDI_Lighter::Device_List();
	Device_List->Dock = System::Windows::Forms::DockStyle::Fill;

	MIDI_Lighter::Device_Edit^ Device_Edit = gcnew MIDI_Lighter::Device_Edit();
	Device_Edit->Dock = System::Windows::Forms::DockStyle::Fill;


	_Split_Container = gcnew System::Windows::Forms::SplitContainer();
	_Split_Container->Dock				= System::Windows::Forms::DockStyle::Fill;
	_Split_Container->BorderStyle		= System::Windows::Forms::BorderStyle::Fixed3D;
	_Split_Container->Cursor			= System::Windows::Forms::Cursors::SizeWE;
	_Split_Container->Panel1->Cursor	= System::Windows::Forms::Cursors::Default;
	_Split_Container->Panel2->Cursor	= System::Windows::Forms::Cursors::Default;
	_Split_Container->SplitterMoved    += gcnew System::Windows::Forms::SplitterEventHandler(this, &MIDI_Lighter_GUI::Split_Container_SplitterMoved);
	
	
	_Split_Container->Panel1->Controls->Add(Device_List);
	_Split_Container->Panel2->Controls->Add(Device_Edit);


	this->Controls->Add(_Split_Container);
/*
	this->Size			= System::Drawing::Size(600, 408);
	this->MinimumSize	= this->Size;

	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount		= 5;
	Table_Layout_Main->ColumnCount	= 2;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 55)));		// 0
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,  50)));		// 1
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,  50)));		// 2
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,  50)));		// 3
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));		// 4
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));	// 0
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));	// 1

		_Configuration_MIDI = gcnew MIDI_Lighter::Configuration_MIDI();
		_Configuration_MIDI->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->Controls->Add(_Configuration_MIDI, 1, 0);
	Table_Layout_Main->SetRowSpan(_Configuration_MIDI, 3);

		_Configuration_No_Data_Light = gcnew MIDI_Lighter::Configuration_No_Data_Light();
		_Configuration_No_Data_Light->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->Controls->Add(_Configuration_No_Data_Light, 1, 1);

		_Configuration_Permanent_Light = gcnew MIDI_Lighter::Configuration_Permanent_Light();
		_Configuration_Permanent_Light->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->Controls->Add(_Configuration_Permanent_Light, 1, 2);

		_Device = gcnew MIDI_Lighter::Device();
		_Device->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->Controls->Add(_Device, 1, 3);

	
		MIDI_Lighter::Connection^ Connection = gcnew MIDI_Lighter::Connection();
		Connection->Dock = System::Windows::Forms::DockStyle::Fill;
		Connection->Connection_Changed						+= gcnew MIDI_Lighter::Connection_Changed					(this,								&MIDI_Lighter_GUI::Connection_Connection_Changed);
		Connection->Sync_Status_Update						+= gcnew MIDI_Lighter::Sync_Status_Update					(this,								&MIDI_Lighter_GUI::Sync_Status_Changed);
		Connection->Configuration_MIDI_Update				+= gcnew MIDI_Lighter::Configuration_MIDI_Update			(_Configuration_MIDI,				&MIDI_Lighter::Configuration_MIDI::Update);
		Connection->Configuration_No_Data_Light_Update		+= gcnew MIDI_Lighter::Configuration_No_Data_Light_Update	(_Configuration_No_Data_Light,		&MIDI_Lighter::Configuration_No_Data_Light::Update);
		Connection->Configuration_Permanent_Light_Update	+= gcnew MIDI_Lighter::Configuration_Permanent_Light_Update	(_Configuration_Permanent_Light,	&MIDI_Lighter::Configuration_Permanent_Light::Update);
		Connection->Configuration_Device_Update				+= gcnew MIDI_Lighter::Configuration_Device_Update			(_Device,							&MIDI_Lighter::Device::Update);
		Connection->Configuration_Changed					+= gcnew MIDI_Lighter::Configuration_Changed				(this,								&MIDI_Lighter_GUI::Configuration_Changed);
	Table_Layout_Main->Controls->Add(Connection, 0, 0);
	Table_Layout_Main->SetColumnSpan(Connection, 2);


	_Configuration_MIDI->Update_Values				+= gcnew MIDI_Lighter::Update_Values_Configuration_MIDI				(Connection, &MIDI_Lighter::Connection::Update_Configuration_MIDI);
	_Configuration_No_Data_Light->Update_Values		+= gcnew MIDI_Lighter::Update_Values_Configuration_No_Data_Light	(Connection, &MIDI_Lighter::Connection::Update_Configuration_No_Data_Light);
	_Configuration_Permanent_Light->Update_Values	+= gcnew MIDI_Lighter::Update_Values_Configuration_Permanent_Light	(Connection, &MIDI_Lighter::Connection::Update_Configuration_Permanent_Light);
	_Device->Update_Values							+= gcnew MIDI_Lighter::Update_Values_Device							(Connection, &MIDI_Lighter::Connection::Update_Device);


		System::Windows::Forms::Button^ Button_Read_EEPROM	= gcnew System::Windows::Forms::Button();
		Button_Read_EEPROM->Dock							= System::Windows::Forms::DockStyle::Top;
		Button_Read_EEPROM->Text							= "Read EEPROM (Debug)";
		Button_Read_EEPROM->UseVisualStyleBackColor			= true;
		Button_Read_EEPROM->Click						   += gcnew System::EventHandler(this, &MIDI_Lighter_GUI::Button_Read_EEPROM_Click);
	Table_Layout_Main->Controls->Add(Button_Read_EEPROM, 0, 4);
	
		_Button_Write_EEPROM							= gcnew System::Windows::Forms::Button();
		_Button_Write_EEPROM->Dock						= System::Windows::Forms::DockStyle::Top;
		_Button_Write_EEPROM->Text						= "Save Configuration to EEPROM";
		_Button_Write_EEPROM->UseVisualStyleBackColor	= true;
		_Button_Write_EEPROM->Enabled					= false;
		_Button_Write_EEPROM->Click					   += gcnew System::EventHandler(Connection, &MIDI_Lighter::Connection::Update_EEPROM);
	Table_Layout_Main->Controls->Add(_Button_Write_EEPROM, 1, 4);

	this->Controls->Add(Table_Layout_Main);

	_Status_Bar = gcnew MIDI_Lighter::Status_Bar();
	this->Controls->Add(_Status_Bar);

	_Connection				= Connection;
	_EEPROM_Write_Pending	= false;

	_LogProgress	= gcnew LogProgress::LogProgress();
	_Debug			= gcnew MIDI_Lighter::Debug(_LogProgress, Connection);

	Connection_Connection_Changed(MIDI_Lighter::USB_CONNECTION::DISCONNECTED);
*/
}

MIDI_Lighter_GUI::MIDI_Lighter_GUI::~MIDI_Lighter_GUI() { }


void MIDI_Lighter_GUI::MIDI_Lighter_GUI::Split_Container_SplitterMoved(System::Object^ sender, System::Windows::Forms::SplitterEventArgs^ e)
{
	if (_Split_Container->SplitterDistance < 160)
	{
		_Split_Container->SplitterDistance = 160;
	}
}

System::Void MIDI_Lighter_GUI::MIDI_Lighter_GUI::Connection_Connection_Changed(MIDI_Lighter::USB_CONNECTION status)
{
	_Configuration_MIDI->Enabled			= status == MIDI_Lighter::USB_CONNECTION::CONNECTED;
	_Configuration_No_Data_Light->Enabled	= status == MIDI_Lighter::USB_CONNECTION::CONNECTED;
	_Configuration_Permanent_Light->Enabled = status == MIDI_Lighter::USB_CONNECTION::CONNECTED;
	_Device->Enabled						= status == MIDI_Lighter::USB_CONNECTION::CONNECTED;

	_Status_Bar->Update_USB_Connection_Status(status);
}

System::Void MIDI_Lighter_GUI::MIDI_Lighter_GUI::Sync_Status_Changed(MIDI_Lighter::SYNC_STATUS status)
{
	_Status_Bar->Update_Sync_Status(status);
}

System::Void MIDI_Lighter_GUI::MIDI_Lighter_GUI::Configuration_Changed(System::Boolean pending)
{
	_EEPROM_Write_Pending			= pending;
	_Button_Write_EEPROM->Enabled	= pending;

	if (pending)
	{
		_Button_Write_EEPROM->BackColor = System::Drawing::Color::FromArgb(202, 81, 0);
	}
	else
	{
		_Button_Write_EEPROM->BackColor = System::Drawing::Color::FromArgb(225, 225, 225);
	}
}

System::Void MIDI_Lighter_GUI::MIDI_Lighter_GUI::Button_Read_EEPROM_Click(System::Object^ sender, System::EventArgs^ e)
{
	System::Windows::Forms::SaveFileDialog^ SaveFileDialog_User_Page = gcnew System::Windows::Forms::SaveFileDialog();
	SaveFileDialog_User_Page->Filter = "ASCII files (*.txt)|*.txt";
	SaveFileDialog_User_Page->ShowDialog(this);

	System::String^ Selected_File = SaveFileDialog_User_Page->FileName;
	delete SaveFileDialog_User_Page;
	if (Selected_File->Length == 0) { return; }

	_LogProgress->Set_Title("Reading EEPROM...");
	_LogProgress->Set_Progress_Maximum(0x3FF);
	_LogProgress->Set_Progress_Value(0);
	_LogProgress->Show(this);
	_LogProgress->Location = System::Drawing::Point(this->Left + this->Width / 2 - _LogProgress->Width / 2, this->Top + this->Height / 2 - _LogProgress->Height / 2);

	_Debug->Background_Worker_Read_EEPROM()->RunWorkerAsync(gcnew array<System::String^>(1) { Selected_File });
}
