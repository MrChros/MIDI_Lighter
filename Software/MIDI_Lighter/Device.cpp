#include "Device.h"

MIDI_Lighter::Device::Device()
{
//	this->AutoScaleDimensions = System::Drawing::SizeF(96.0f, 96.0f);
//	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	
	_Resources = gcnew System::Resources::ResourceManager("MIDI_Lighter.MIDI_Lighter", System::Reflection::Assembly::GetExecutingAssembly());

	System::Drawing::Font^ Font_Header = gcnew System::Drawing::Font(this->Font->Name, this->Font->Size + 2, System::Drawing::FontStyle::Bold);

	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount		= 5;
	Table_Layout_Main->ColumnCount	= 2;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));		// 0
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 38)));		// 1
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 60)));		// 2
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 38)));		// 3
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 60)));		// 4
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 40)));	// 0
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 60)));	// 1

		_Label_Header				= gcnew System::Windows::Forms::Label();
		_Label_Header->Dock			= System::Windows::Forms::DockStyle::Top;
		_Label_Header->TextAlign	= System::Drawing::ContentAlignment::MiddleLeft;
		_Label_Header->Font			= Font_Header;
		_Label_Header->ForeColor	= System::Drawing::Color::White;
		_Label_Header->BackColor	= System::Drawing::Color::FromArgb(23, 114, 202);
		_Label_Header->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
	Table_Layout_Main->Controls->Add(_Label_Header, 0, 0);
	Table_Layout_Main->SetColumnSpan(_Label_Header, 2);

		_Configuration_MIDI = gcnew MIDI_Lighter::Configuration_MIDI();
		_Configuration_MIDI->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->Controls->Add(_Configuration_MIDI, 0, 1);
	Table_Layout_Main->SetRowSpan(_Configuration_MIDI, 4);

		_Configuration_Permanent_Light = gcnew MIDI_Lighter::Configuration_Permanent_Light();
		_Configuration_Permanent_Light->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->Controls->Add(_Configuration_Permanent_Light, 1, 1);

		_Configuration_No_Data_Light = gcnew MIDI_Lighter::Configuration_No_Data_Light();
		_Configuration_No_Data_Light->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->Controls->Add(_Configuration_No_Data_Light, 1, 2);

		_Configuration_RGB_Order = gcnew MIDI_Lighter::Configuration_RGB_Order();
		_Configuration_RGB_Order->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->Controls->Add(_Configuration_RGB_Order, 1, 3);

		_Device_Name = gcnew MIDI_Lighter::Device_Name();
		_Device_Name->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->Controls->Add(_Device_Name, 1, 4);


	_Configuration_MIDI->Update_Values					+= gcnew MIDI_Lighter::Update_Values_Configuration_MIDI				(this, &MIDI_Lighter::Device::Update_Configuration_MIDI);
	_Configuration_No_Data_Light->Update_Values			+= gcnew MIDI_Lighter::Update_Values_Configuration_No_Data_Light	(this, &MIDI_Lighter::Device::Update_Configuration_No_Data_Light);
	_Configuration_Permanent_Light->Update_Values		+= gcnew MIDI_Lighter::Update_Values_Configuration_Permanent_Light	(this, &MIDI_Lighter::Device::Update_Configuration_Permanent_Light);
	_Device_Name->Update_Values							+= gcnew MIDI_Lighter::Update_Values_Device							(this, &MIDI_Lighter::Device::Update_Device);
	_Configuration_RGB_Order->Update_Values				+= gcnew MIDI_Lighter::Update_Values_Configuration_RGB_Order		(this, &MIDI_Lighter::Device::Update_Configuration_RGB_Order);


		_Button_Write_EEPROM							= gcnew System::Windows::Forms::Button();
		_Button_Write_EEPROM->Dock						= System::Windows::Forms::DockStyle::Top;
		_Button_Write_EEPROM->Text						= "Save Configuration to EEPROM";
		_Button_Write_EEPROM->UseVisualStyleBackColor	= true;
		_Button_Write_EEPROM->Enabled					= false;
		_Button_Write_EEPROM->Click					   += gcnew System::EventHandler(this, &MIDI_Lighter::Device::Button_Save_EEPROM_Click);
	Table_Layout_Main->Controls->Add(_Button_Write_EEPROM, 0, 5);
	Table_Layout_Main->SetColumnSpan(_Button_Write_EEPROM, 2);

	this->Controls->Add(Table_Layout_Main);

	_EEPROM_Write_Pending = false;

	Update_Header("not connected");
}

///////////////////////////////////////////////////
// Setting Values received from MIDI Lighter PCB //
///////////////////////////////////////////////////
System::Void MIDI_Lighter::Device::Connection_Connection_Changed(MIDI_Lighter::USB_CONNECTION status)
{
	if (status != MIDI_Lighter::USB_CONNECTION::CONNECTED)
	{
		Update_Header("disconnected");
	}
}

System::Void MIDI_Lighter::Device::Set_Configuration_MIDI(MIDI_Lighter_Wrapper::Configuration_MIDI^	configuration_midi)
{
	_Configuration_MIDI->Update(configuration_midi);
}

System::Void MIDI_Lighter::Device::Set_Configuration_No_Data_Light(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ configuration_no_data_light)
{
	_Configuration_No_Data_Light->Update(configuration_no_data_light);
}

System::Void MIDI_Lighter::Device::Set_Configuration_Permanent_Light(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ configuration_permanent_light)
{
	_Configuration_Permanent_Light->Update(configuration_permanent_light);
}

System::Void MIDI_Lighter::Device::Set_Configuration_Device(MIDI_Lighter_Wrapper::Device^ device)
{
	_Device_Name->Update(device);
	Update_Header(device->Name);
}

System::Void MIDI_Lighter::Device::Set_Configuration_RGB_Order(MIDI_Lighter_Wrapper::Configuration_RGB_Order^ configuration_rgb_order)
{
	_Configuration_RGB_Order->Update(configuration_rgb_order);
}

System::Void MIDI_Lighter::Device::Configuration_Changed(System::Boolean pending)
{
	_EEPROM_Write_Pending = pending;
	_Button_Write_EEPROM->Enabled = pending;

	if (pending)
	{
		_Button_Write_EEPROM->BackColor = System::Drawing::Color::FromArgb(202, 81, 0);
	}
	else
	{
		_Button_Write_EEPROM->BackColor = System::Drawing::Color::FromArgb(225, 225, 225);
	}
}

///////////////////////////////////////////////////////////////
// Sending Signals, that values have been changed in the GUI //
///////////////////////////////////////////////////////////////
System::Void MIDI_Lighter::Device::Update_Configuration_MIDI(MIDI_Lighter_Wrapper::Configuration_MIDI^ configuration_midi)
{
	Changed_Configuration_MIDI(configuration_midi);
}

System::Void MIDI_Lighter::Device::Update_Configuration_No_Data_Light(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ configuration_no_data_light)
{
	Changed_Configuration_No_Data_Light(configuration_no_data_light);
}

System::Void MIDI_Lighter::Device::Update_Configuration_Permanent_Light(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ configuration_permanent_light)
{
	Changed_Configuration_Permanent_Light(configuration_permanent_light);
}

System::Void MIDI_Lighter::Device::Update_Device(MIDI_Lighter_Wrapper::Device^ device)
{
	Changed_Device_Name(device);
}

System::Void MIDI_Lighter::Device::Update_Configuration_RGB_Order(MIDI_Lighter_Wrapper::Configuration_RGB_Order^ configuration_rgb_order)
{
	Changed_Configuration_RGB_Order(configuration_rgb_order);
}


//////////////////////////////////////////////////
// Internal function to save settings to EEPROM //
//////////////////////////////////////////////////
System::Void MIDI_Lighter::Device::Button_Save_EEPROM_Click(System::Object^ sender, System::EventArgs^ e)
{
	EEPROM_Save();
}

System::Void MIDI_Lighter::Device::Update_Header(System::String^ device_name)
{
	_Label_Header->Text = "Connected Device: " + device_name;
}