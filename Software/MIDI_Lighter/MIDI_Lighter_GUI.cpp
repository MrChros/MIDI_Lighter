#include "MIDI_Lighter_GUI.h"

MIDI_Lighter_GUI::MIDI_Lighter_GUI::MIDI_Lighter_GUI()
{
	this->Text = L"MIDI Lighter";
	this->Size = System::Drawing::Size(1000, 460);

	_Resources = gcnew System::Resources::ResourceManager("MIDI_Lighter.MIDI_Lighter", System::Reflection::Assembly::GetExecutingAssembly());


		_Device_List = gcnew MIDI_Lighter::Device_List();
		_Device_List->Dock = System::Windows::Forms::DockStyle::Fill;

	
		System::Windows::Forms::TableLayoutPanel^ Table_Layout_Edit = gcnew System::Windows::Forms::TableLayoutPanel();
		Table_Layout_Edit->RowCount		= 3;
		Table_Layout_Edit->ColumnCount	= 1;
		Table_Layout_Edit->Dock			= System::Windows::Forms::DockStyle::Fill;
		Table_Layout_Edit->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 260)));		// 0
		Table_Layout_Edit->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,  100)));		// 1
		Table_Layout_Edit->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,  30)));		// 2
		Table_Layout_Edit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));	// 0

			_Device					= gcnew MIDI_Lighter::Device();
			_Device->Dock			= System::Windows::Forms::DockStyle::Fill;
//			_Device->BorderStyle	= System::Windows::Forms::BorderStyle::Fixed3D;
		Table_Layout_Edit->Controls->Add(_Device, 0, 0);

			_Debug					= gcnew MIDI_Lighter::Debug(_Device_List);
			_Debug->Dock			= System::Windows::Forms::DockStyle::Fill;
//			_Debug->BorderStyle		= System::Windows::Forms::BorderStyle::Fixed3D;
		Table_Layout_Edit->Controls->Add(_Debug, 0, 1);

			_Status_Bar = gcnew MIDI_Lighter::Status_Bar();
		Table_Layout_Edit->Controls->Add(_Status_Bar, 0, 2);

	
	_Split_Container = gcnew System::Windows::Forms::SplitContainer();
	_Split_Container->Dock				= System::Windows::Forms::DockStyle::Fill;
	_Split_Container->BorderStyle		= System::Windows::Forms::BorderStyle::Fixed3D;
	_Split_Container->Cursor			= System::Windows::Forms::Cursors::SizeWE;
	_Split_Container->Panel1->Cursor	= System::Windows::Forms::Cursors::Default;
	_Split_Container->Panel2->Cursor	= System::Windows::Forms::Cursors::Default;
	_Split_Container->SplitterMoved    += gcnew System::Windows::Forms::SplitterEventHandler(this, &MIDI_Lighter_GUI::Split_Container_SplitterMoved);
	
	
	_Split_Container->Panel1->Controls->Add(_Device_List);
	_Split_Container->Panel2->Controls->Add(Table_Layout_Edit);


	this->Controls->Add(_Split_Container);


	_Device_List->Connection_Changed					+= gcnew MIDI_Lighter::Connection_Changed					(this,		&MIDI_Lighter_GUI		::Connection_Connection_Changed);
	_Device_List->Connection_Changed					+= gcnew MIDI_Lighter::Connection_Changed					(_Device,	&MIDI_Lighter::Device	::Connection_Connection_Changed);
	_Device_List->Sync_Status_Update					+= gcnew MIDI_Lighter::Sync_Status_Update					(this,		&MIDI_Lighter_GUI		::Sync_Status_Changed);
	
	_Device_List->Configuration_MIDI_Update				+= gcnew MIDI_Lighter::Configuration_MIDI_Update			(_Device, &MIDI_Lighter::Device::Set_Configuration_MIDI);
	_Device_List->Configuration_No_Data_Light_Update	+= gcnew MIDI_Lighter::Configuration_No_Data_Light_Update	(_Device, &MIDI_Lighter::Device::Set_Configuration_No_Data_Light);
	_Device_List->Configuration_Permanent_Light_Update	+= gcnew MIDI_Lighter::Configuration_Permanent_Light_Update	(_Device, &MIDI_Lighter::Device::Set_Configuration_Permanent_Light);
	_Device_List->Configuration_Device_Update			+= gcnew MIDI_Lighter::Configuration_Device_Update			(_Device, &MIDI_Lighter::Device::Set_Configuration_Device);
	_Device_List->Configuration_RGB_Order_Update		+= gcnew MIDI_Lighter::Configuration_RGB_Order_Update		(_Device, &MIDI_Lighter::Device::Set_Configuration_RGB_Order);
	_Device_List->Configuration_Changed					+= gcnew MIDI_Lighter::Configuration_Changed				(_Device, &MIDI_Lighter::Device::Configuration_Changed);

	_Device->Changed_Configuration_MIDI					+= gcnew MIDI_Lighter::Changed_Configuration_MIDI			(_Device_List, &MIDI_Lighter::Device_List::Update_Configuration_MIDI);
	_Device->Changed_Configuration_No_Data_Light		+= gcnew MIDI_Lighter::Changed_Configuration_No_Data_Light	(_Device_List, &MIDI_Lighter::Device_List::Update_Configuration_No_Data_Light);
	_Device->Changed_Configuration_Permanent_Light		+= gcnew MIDI_Lighter::Changed_Configuration_Permanent_Light(_Device_List, &MIDI_Lighter::Device_List::Update_Configuration_Permanent_Light);
	_Device->Changed_Device_Name						+= gcnew MIDI_Lighter::Changed_Device_Name					(_Device_List, &MIDI_Lighter::Device_List::Update_Device);
	_Device->Changed_Configuration_RGB_Order			+= gcnew MIDI_Lighter::Changed_Configuration_RGB_Order		(_Device_List, &MIDI_Lighter::Device_List::Update_Configuration_RGB_Order);
	_Device->EEPROM_Save								+= gcnew MIDI_Lighter::EEPROM_Save							(_Device_List, &MIDI_Lighter::Device_List::Update_EEPROM);
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
	_Status_Bar->Update_USB_Connection_Status(status);
}

System::Void MIDI_Lighter_GUI::MIDI_Lighter_GUI::Sync_Status_Changed(MIDI_Lighter::SYNC_STATUS status)
{
	_Status_Bar->Update_Sync_Status(status);
}

System::Void MIDI_Lighter_GUI::MIDI_Lighter_GUI::Configuration_Changed(System::Boolean pending)
{
	
}