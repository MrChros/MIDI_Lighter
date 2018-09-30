#include "Connection.h"

MIDI_Lighter::Connection::Connection()
{
	_MIDI_Lighter = gcnew MIDI_Lighter_Wrapper();

	_Resources = gcnew System::Resources::ResourceManager("MIDI_Lighter.MIDI_Lighter", System::Reflection::Assembly::GetExecutingAssembly());

	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount		= 1;
	Table_Layout_Main->ColumnCount	= 3;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,  100)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 100)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 150)));

		_ComboBox_Devices = gcnew System::Windows::Forms::ComboBox();
		_ComboBox_Devices->Dock				= System::Windows::Forms::DockStyle::Fill;
		_ComboBox_Devices->DropDownStyle	= System::Windows::Forms::ComboBoxStyle::DropDownList;
		_ComboBox_Devices->Enabled			= false;
		_ComboBox_Devices->Margin			= System::Windows::Forms::Padding(4, 4, 4, 0);
	Table_Layout_Main->Controls->Add(_ComboBox_Devices, 0, 0);

		_Button_Refresh = gcnew System::Windows::Forms::Button();
		_Button_Refresh->Dock						= System::Windows::Forms::DockStyle::Top;
		_Button_Refresh->Text						= "Refresh";
		_Button_Refresh->TextImageRelation			= System::Windows::Forms::TextImageRelation::ImageBeforeText;
		_Button_Refresh->TextAlign					= System::Drawing::ContentAlignment::MiddleRight;
		_Button_Refresh->Image = (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"arrow_refresh")));
		_Button_Refresh->UseVisualStyleBackColor	= true;
		_Button_Refresh->Click += gcnew System::EventHandler(this, &Connection::Button_Refresh_Click);
	Table_Layout_Main->Controls->Add(_Button_Refresh, 1, 0);

		_Button_Connect = gcnew System::Windows::Forms::Button();
		_Button_Connect->Dock						= System::Windows::Forms::DockStyle::Top;
		_Button_Connect->TextImageRelation			= System::Windows::Forms::TextImageRelation::ImageBeforeText;
		_Button_Connect->TextAlign					= System::Drawing::ContentAlignment::MiddleRight;
		_Button_Connect->UseVisualStyleBackColor	= true;
		_Button_Connect->Click += gcnew System::EventHandler(this, &Connection::Button_Connect_Click);
	Table_Layout_Main->Controls->Add(_Button_Connect, 2, 0);

	_GroupBox_Main = gcnew System::Windows::Forms::GroupBox();
	_GroupBox_Main->Text = "Connection";
	_GroupBox_Main->Font = gcnew System::Drawing::Font(this->Font, System::Drawing::FontStyle::Bold);
	_GroupBox_Main->Dock = System::Windows::Forms::DockStyle::Fill;
	_GroupBox_Main->Controls->Add(Table_Layout_Main);

	for each (System::Windows::Forms::Control^ Control in this->_GroupBox_Main->Controls)
	{
		Control->Font = gcnew System::Drawing::Font(this->Font, System::Drawing::FontStyle::Regular);
	}

	this->Controls->Add(_GroupBox_Main);

	_Timer_Poll_Connection = gcnew System::Windows::Forms::Timer;
	_Timer_Poll_Connection->Interval = 100;
	_Timer_Poll_Connection->Tick += gcnew System::EventHandler(this, &Connection::Timer_Poll_Connection_Tick);

	_Current_USB_Connection_Status = USB_CONNECTION::DISCONNECTED;

	GUI_USB_Disconnect_Update();
	Button_Refresh_Click(nullptr, System::EventArgs::Empty);
}

System::Void MIDI_Lighter::Connection::Button_Refresh_Click(System::Object^ sender, System::EventArgs^ e)
{
	System::Collections::Generic::List<MIDI_Lighter_Wrapper::Device^ >^ Device_List = _MIDI_Lighter->Get_Device_List();

	_ComboBox_Devices->Items->Clear();
	
	if (Device_List->Count == 0)
	{
		_ComboBox_Devices->Enabled = false;
		return;
	}
		
	_ComboBox_Devices->BeginUpdate();
		for (int i = 0; i < Device_List->Count; i++)
		{
			_ComboBox_Devices->Items->Add(Device_List[i]->Index.ToString() + ": " + Device_List[i]->Name);
		}
	_ComboBox_Devices->EndUpdate();

	_ComboBox_Devices->Enabled = true;

	if(_ComboBox_Devices->SelectedIndex < 0)
	{
		_ComboBox_Devices->SelectedIndex = 0;
	}
}

System::Void MIDI_Lighter::Connection::Button_Connect_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (_MIDI_Lighter->IsConnected())
	{
		_MIDI_Lighter->Disconnect();
		GUI_USB_Disconnect_Update();

		_Current_USB_Connection_Status = USB_CONNECTION::DISCONNECTED;
	}
	else
	{
		if(_ComboBox_Devices->SelectedIndex >= 0)
		{
			if(_MIDI_Lighter->Connect())
			{
				GUI_USB_Connect_Update();
				_Current_USB_Connection_Status = USB_CONNECTION::CONNECTED;
				_Timer_Poll_Connection->Start();

				Sync_Status_Update(SYNC_STATUS::SYNCING);

				System::Boolean^ Success;

				MIDI_Lighter_Wrapper::Configuration_MIDI^ Configuration_MIDI = _MIDI_Lighter->Get_Configuration_MIDI(Success);
				if (!Success) { Sync_Status_Update(SYNC_STATUS::FAILED); }
				Configuration_MIDI_Update(Configuration_MIDI);

				MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ Configuration_No_Data_Light = _MIDI_Lighter->Get_Configuration_No_Data_Light(Success);
				if (!Success) { Sync_Status_Update(SYNC_STATUS::FAILED); }
				Configuration_No_Data_Light_Update(Configuration_No_Data_Light);

				MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ Configuration_Permanent_Light = _MIDI_Lighter->Get_Configuration_Permanent_Light(Success);
				if (!Success) { Sync_Status_Update(SYNC_STATUS::FAILED); }
				Configuration_Permanent_Light_Update(Configuration_Permanent_Light);

				MIDI_Lighter_Wrapper::Device^ Device = gcnew MIDI_Lighter_Wrapper::Device();
				Device->Name = _MIDI_Lighter->Get_Device_Name(Success);
				if (!Success) { Sync_Status_Update(SYNC_STATUS::FAILED); }
				Configuration_Device_Update(Device);

				MIDI_Lighter_Wrapper::Configuration_RGB_Order^ Configuration_RGB_Order = _MIDI_Lighter->Get_Configuration_RGB_Order(Success);
				if (!Success) { Sync_Status_Update(SYNC_STATUS::FAILED); }
				Configuration_RGB_Order_Update(Configuration_RGB_Order);

				Sync_Status_Update(SYNC_STATUS::SYNCED);
			}
			else
			{
				System::Windows::Forms::MessageBox::Show(
					"Connection failed",
					"Error", System::Windows::Forms::MessageBoxButtons::OK,
					System::Windows::Forms::MessageBoxIcon::Error);

				_Current_USB_Connection_Status = USB_CONNECTION::FAILED;
			}
		}
	}
	Connection_Changed(_Current_USB_Connection_Status);
}

System::Void MIDI_Lighter::Connection::Timer_Poll_Connection_Tick(System::Object^ sender, System::EventArgs^ e)
{
	if (_Current_USB_Connection_Status == USB_CONNECTION::CONNECTED)
	{
		if(_MIDI_Lighter->IsConnected() == false)
		{
			_Current_USB_Connection_Status = USB_CONNECTION::LOST;
			Connection_Changed(_Current_USB_Connection_Status);

			GUI_USB_Disconnect_Update();
			Button_Refresh_Click(nullptr, System::EventArgs::Empty);
		}
	}
	else
	{
		_Timer_Poll_Connection->Stop();
	}
}

System::Void MIDI_Lighter::Connection::GUI_USB_Connect_Update()
{
	_Button_Connect->Text		= "Disconnect";
	_Button_Connect->Image		= (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"disconnect")));
	_Button_Refresh->Enabled	= false;
	_ComboBox_Devices->Enabled	= false;
}

System::Void MIDI_Lighter::Connection::GUI_USB_Disconnect_Update()
{
	_Button_Connect->Text		= "Connect";
	_Button_Connect->Image		= (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"connect")));
	_Button_Refresh->Enabled	= true;
	_ComboBox_Devices->Enabled	= true;

	Sync_Status_Update(SYNC_STATUS::UNKNOWN);
}

System::Void MIDI_Lighter::Connection::Update_Configuration_MIDI(MIDI_Lighter_Wrapper::Configuration_MIDI^ configuration_midi)
{
	if (_MIDI_Lighter->IsConnected())
	{
		Sync_Status_Update(SYNC_STATUS::SYNCING);
		if (!_MIDI_Lighter->Set_Configuration_MIDI(configuration_midi))
		{
			Sync_Status_Update(SYNC_STATUS::FAILED);
		}
		Sync_Status_Update(SYNC_STATUS::SYNCED);

		Configuration_Changed(true);
	}
}

System::Void MIDI_Lighter::Connection::Update_Configuration_No_Data_Light(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ configuration_no_data_light)
{
	if (_MIDI_Lighter->IsConnected())
	{
		Sync_Status_Update(SYNC_STATUS::SYNCING);
		if (!_MIDI_Lighter->Set_Configuration_No_Data_Light(configuration_no_data_light))
		{
			Sync_Status_Update(SYNC_STATUS::FAILED);
		}
		Sync_Status_Update(SYNC_STATUS::SYNCED);

		Configuration_Changed(true);
	}
}

System::Void MIDI_Lighter::Connection::Update_Configuration_Permanent_Light(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ configuration_permanent_light)
{
	if (_MIDI_Lighter->IsConnected())
	{
		Sync_Status_Update(SYNC_STATUS::SYNCING);
		if (!_MIDI_Lighter->Set_Configuration_Permanent_Light(configuration_permanent_light))
		{
			Sync_Status_Update(SYNC_STATUS::FAILED);
		}
		Sync_Status_Update(SYNC_STATUS::SYNCED);

		Configuration_Changed(true);
	}
}

System::Void MIDI_Lighter::Connection::Update_Device(MIDI_Lighter_Wrapper::Device^ device)
{
	if (_MIDI_Lighter->IsConnected())
	{
		Sync_Status_Update(SYNC_STATUS::SYNCING);
		if (!_MIDI_Lighter->Set_Device_Name(device->Name))
		{
			Sync_Status_Update(SYNC_STATUS::FAILED);
		}
		
		int Index				= _ComboBox_Devices->SelectedIndex;
		_ComboBox_Devices->Items->Remove(_ComboBox_Devices->SelectedItem);
		_ComboBox_Devices->Items->Insert(Index, Index.ToString() + ": " + device->Name);
		_ComboBox_Devices->SelectedIndex = Index;

		Sync_Status_Update(SYNC_STATUS::SYNCED);

		Configuration_Changed(true);
	}
}

System::Void MIDI_Lighter::Connection::Update_Configuration_RGB_Order(MIDI_Lighter_Wrapper::Configuration_RGB_Order^ configuration_rgb_order)
{
	if (_MIDI_Lighter->IsConnected())
	{
		Sync_Status_Update(SYNC_STATUS::SYNCING);
		if (!_MIDI_Lighter->Set_Configuration_RGB_Order(configuration_rgb_order))
		{
			Sync_Status_Update(SYNC_STATUS::FAILED);
		}
		Sync_Status_Update(SYNC_STATUS::SYNCED);

		Configuration_Changed(true);
	}
}

System::Byte MIDI_Lighter::Connection::Read_EEPROM(uint32_t address)
{
	if (_MIDI_Lighter->IsConnected())
	{
		System::Boolean^ Success = gcnew System::Boolean;
		return _MIDI_Lighter->Read_EEPROM(address, Success);
	}
	return 0;
}

System::Void MIDI_Lighter::Connection::Update_EEPROM(System::Object^ sender, System::EventArgs^ e)
{
	if (_MIDI_Lighter->IsConnected())
	{
		_MIDI_Lighter->Write_EEPROM();

		Configuration_Changed(false);
	}
}