#include "Device_List.h"

MIDI_Lighter::Device_List::Device_List()
{
//	this->AutoScaleDimensions = System::Drawing::SizeF(96.0f, 96.0f);
//	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	
	_MIDI_Lighter	= gcnew MIDI_Lighter_Wrapper();
	_Resources		= gcnew System::Resources::ResourceManager("MIDI_Lighter.MIDI_Lighter", System::Reflection::Assembly::GetExecutingAssembly());
	

	System::Drawing::Font^ Font_Label		= gcnew System::Drawing::Font(this->Font->Name, this->Font->Size + 2, System::Drawing::FontStyle::Bold);
	System::Drawing::Font^ Font_DataGrid	= gcnew System::Drawing::Font(this->Font->Name, this->Font->Size + 0, System::Drawing::FontStyle::Bold);

	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount = 4;
	Table_Layout_Main->ColumnCount = 1;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 25)));		// 0
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));		// 1
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));		// 2
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 25)));		// 3
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));	// 0

		System::Windows::Forms::Label^ Label_Title = gcnew System::Windows::Forms::Label();
		Label_Title->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Title->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		Label_Title->Text = "Available Devices";
		Label_Title->Font = Font_Label;
	Table_Layout_Main->Controls->Add(Label_Title, 0, 0);

		_Button_Refresh = gcnew System::Windows::Forms::Button();
		_Button_Refresh->Dock = System::Windows::Forms::DockStyle::Top;
		_Button_Refresh->Text = "Refresh Devices";
		_Button_Refresh->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageBeforeText;
		_Button_Refresh->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		_Button_Refresh->Image = (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"arrow_refresh")));
		_Button_Refresh->UseVisualStyleBackColor = true;
		_Button_Refresh->Click += gcnew System::EventHandler(this, &Device_List::Button_Refresh_Click);
	Table_Layout_Main->Controls->Add(_Button_Refresh, 0, 1);

		_DataGrid_Devices = gcnew System::Windows::Forms::DataGridView();
		_DataGrid_Devices->Dock = System::Windows::Forms::DockStyle::Fill;
		_DataGrid_Devices->BackgroundColor = System::Drawing::SystemColors::ButtonHighlight;
		_DataGrid_Devices->ColumnHeadersDefaultCellStyle->Font = Font_DataGrid;
		_DataGrid_Devices->RowHeadersVisible		= false;
		_DataGrid_Devices->AllowUserToAddRows		= false;
		_DataGrid_Devices->AllowUserToDeleteRows	= false;
		_DataGrid_Devices->AllowUserToOrderColumns	= false;
		_DataGrid_Devices->AllowUserToResizeRows	= false;
		_DataGrid_Devices->ReadOnly					= true;
		_DataGrid_Devices->MultiSelect				= false;
		_DataGrid_Devices->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
		_DataGrid_Devices->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
		_DataGrid_Devices->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^ >(3) {
			gcnew System::Windows::Forms::DataGridViewTextBoxColumn(),
			gcnew System::Windows::Forms::DataGridViewTextBoxColumn(),
			gcnew System::Windows::Forms::DataGridViewTextBoxColumn()
		});
		_DataGrid_Devices->SelectionChanged += gcnew System::EventHandler(this, &MIDI_Lighter::Device_List::DataGrid_Devices_OnSelectionChanged);

		_DataGrid_Devices->Columns[0]->HeaderText	= "#";
		_DataGrid_Devices->Columns[0]->Width		= 40;
		_DataGrid_Devices->Columns[0]->SortMode		= System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
		_DataGrid_Devices->Columns[1]->HeaderText	= "Device Name";
		_DataGrid_Devices->Columns[1]->AutoSizeMode	= System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
		_DataGrid_Devices->Columns[1]->SortMode		= System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
		_DataGrid_Devices->Columns[2]->HeaderText	= "State";
		_DataGrid_Devices->Columns[2]->SortMode		= System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
	Table_Layout_Main->Controls->Add(_DataGrid_Devices, 0, 2);

		_Label_Number_Devices = gcnew System::Windows::Forms::Label();
		_Label_Number_Devices->Dock = System::Windows::Forms::DockStyle::Top;
		_Label_Number_Devices->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
	Table_Layout_Main->Controls->Add(_Label_Number_Devices, 0, 3);

	this->Controls->Add(Table_Layout_Main);


	_Timer_Poll_Connection = gcnew System::Windows::Forms::Timer;
	_Timer_Poll_Connection->Interval = 100;
	_Timer_Poll_Connection->Tick += gcnew System::EventHandler(this, &Device_List::Timer_Poll_Connection_Tick);


	_CellStyle_OK							= gcnew System::Windows::Forms::DataGridViewCellStyle();
	_CellStyle_OK->Font						= gcnew System::Drawing::Font(this->Font, System::Drawing::FontStyle::Bold);
	_CellStyle_OK->ForeColor				= System::Drawing::Color::FromArgb(  0, 187,   0);
	_CellStyle_OK->SelectionForeColor		= _CellStyle_OK->ForeColor;
	_CellStyle_OK->BackColor				= System::Drawing::Color::FromArgb(255, 255, 255);

	_CellStyle_Pending						= gcnew System::Windows::Forms::DataGridViewCellStyle();
	_CellStyle_Pending->Font				= gcnew System::Drawing::Font(this->Font, System::Drawing::FontStyle::Bold);
	_CellStyle_Pending->ForeColor			= System::Drawing::Color::FromArgb(  0,   0,   0);
	_CellStyle_Pending->BackColor			= System::Drawing::Color::FromArgb(202,  81,   0);
	_CellStyle_Pending->SelectionBackColor	= _CellStyle_Pending->BackColor;


	_List_Pending = gcnew System::Collections::Generic::List<System::Boolean>;


	_Current_USB_Connection_Status = USB_CONNECTION::DISCONNECTED;

	GUI_USB_Disconnect_Update();
	Button_Refresh_Click(nullptr, System::EventArgs::Empty);
	Label_Number_Devices_Update();
}

System::Void MIDI_Lighter::Device_List::Button_Refresh_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	_DataGrid_Devices->Rows->Clear();
	_List_Pending->Clear();

	System::Collections::Generic::List<MIDI_Lighter_Wrapper::Device^ >^ Device_List = _MIDI_Lighter->Get_Device_List();

	for (int i = 0; i < Device_List->Count; i++)
	{
		_List_Pending->Add(false);

		_DataGrid_Devices->Rows->Add(gcnew cli::array< System::String^ >(3) { (Device_List[i]->Index + 1).ToString(), Device_List[i]->Name, "OK" });
		_DataGrid_Devices->Rows[i]->Cells[2]->Style = _CellStyle_OK;
	}
	
	Label_Number_Devices_Update();
}

System::Void MIDI_Lighter::Device_List::DataGrid_Devices_OnSelectionChanged(System::Object^ sender, System::EventArgs^ e)
{
	int Selected_Index = -1;

	System::Windows::Forms::DataGridViewSelectedRowCollection^ Rows = _DataGrid_Devices->SelectedRows;
	for each(System::Windows::Forms::DataGridViewRow^ Row in Rows)
	{
		Selected_Index = Row->Index;
	}

	if (_MIDI_Lighter->IsConnected())
	{
		_MIDI_Lighter->Disconnect();
		GUI_USB_Disconnect_Update();

		_Current_USB_Connection_Status = USB_CONNECTION::DISCONNECTED;
	}

	if (Selected_Index >= 0)
	{
		if (_MIDI_Lighter->Connect(Selected_Index))
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

			Configuration_Changed(_List_Pending[Selected_Index]);
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
	Connection_Changed(_Current_USB_Connection_Status);
}

System::Void MIDI_Lighter::Device_List::Label_Number_Devices_Update()
{
	if (_DataGrid_Devices->RowCount == 1)
	{
		_Label_Number_Devices->Text = _DataGrid_Devices->RowCount.ToString() + " Device Found";
	}
	else
	{
		_Label_Number_Devices->Text = _DataGrid_Devices->RowCount.ToString() + " Devices Found";
	}
}

System::Void MIDI_Lighter::Device_List::Timer_Poll_Connection_Tick(System::Object^ sender, System::EventArgs^ e)
{
	if (_Current_USB_Connection_Status == USB_CONNECTION::CONNECTED)
	{
		if (_MIDI_Lighter->IsConnected() == false)
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

System::Void MIDI_Lighter::Device_List::GUI_USB_Connect_Update()
{

}

System::Void MIDI_Lighter::Device_List::GUI_USB_Disconnect_Update()
{
	Sync_Status_Update(SYNC_STATUS::UNKNOWN);
}

System::Void MIDI_Lighter::Device_List::EEPROM_Write_Set_Pending(System::Boolean pending)
{
	if (_MIDI_Lighter->IsConnected())
	{
		System::Windows::Forms::DataGridViewRow^ Selected_Row = nullptr;

		System::Windows::Forms::DataGridViewSelectedRowCollection^ Rows = _DataGrid_Devices->SelectedRows;
		for each(System::Windows::Forms::DataGridViewRow^ Row in Rows)
		{
			Selected_Row = Row;
		}
		
		if(Selected_Row != nullptr)
		{
			_List_Pending[Selected_Row->Index] = pending;
			Configuration_Changed(pending);
		}
	}

	Status_Cell_Update();
}

System::Void MIDI_Lighter::Device_List::Status_Cell_Update()
{
	for (int i = 0; i < _DataGrid_Devices->RowCount;i++)
	{
		if (_List_Pending[i] == true)
		{
			_DataGrid_Devices->Rows[i]->Cells[2]->Style = _CellStyle_Pending;
			_DataGrid_Devices->Rows[i]->Cells[2]->Value = "Pending";
		}
		else
		{
			_DataGrid_Devices->Rows[i]->Cells[2]->Style = _CellStyle_OK;
			_DataGrid_Devices->Rows[i]->Cells[2]->Value = "OK";
		}
	}
	_DataGrid_Devices->Update();
}

/////////////////////////////
// Public Update Functions //
/////////////////////////////
System::Void MIDI_Lighter::Device_List::Update_Configuration_MIDI(MIDI_Lighter_Wrapper::Configuration_MIDI^ configuration_midi)
{
	if (_MIDI_Lighter->IsConnected())
	{
		Sync_Status_Update(SYNC_STATUS::SYNCING);
		if (!_MIDI_Lighter->Set_Configuration_MIDI(configuration_midi))
		{
			Sync_Status_Update(SYNC_STATUS::FAILED);
		}
		Sync_Status_Update(SYNC_STATUS::SYNCED);

		EEPROM_Write_Set_Pending(true);
	}
}

System::Void MIDI_Lighter::Device_List::Update_Configuration_No_Data_Light(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ configuration_no_data_light)
{
	if (_MIDI_Lighter->IsConnected())
	{
		Sync_Status_Update(SYNC_STATUS::SYNCING);
		if (!_MIDI_Lighter->Set_Configuration_No_Data_Light(configuration_no_data_light))
		{
			Sync_Status_Update(SYNC_STATUS::FAILED);
		}
		Sync_Status_Update(SYNC_STATUS::SYNCED);

		EEPROM_Write_Set_Pending(true);
	}
}

System::Void MIDI_Lighter::Device_List::Update_Configuration_Permanent_Light(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ configuration_permanent_light)
{
	if (_MIDI_Lighter->IsConnected())
	{
		Sync_Status_Update(SYNC_STATUS::SYNCING);
		if (!_MIDI_Lighter->Set_Configuration_Permanent_Light(configuration_permanent_light))
		{
			Sync_Status_Update(SYNC_STATUS::FAILED);
		}
		Sync_Status_Update(SYNC_STATUS::SYNCED);

		EEPROM_Write_Set_Pending(true);
	}
}

System::Void MIDI_Lighter::Device_List::Update_Device(MIDI_Lighter_Wrapper::Device^ device)
{
	if (_MIDI_Lighter->IsConnected())
	{
		Sync_Status_Update(SYNC_STATUS::SYNCING);
		if (!_MIDI_Lighter->Set_Device_Name(device->Name))
		{
			Sync_Status_Update(SYNC_STATUS::FAILED);
		}

		System::Windows::Forms::DataGridViewRow^ Selected_Row = nullptr;

		System::Windows::Forms::DataGridViewSelectedRowCollection^ Rows = _DataGrid_Devices->SelectedRows;
		for each(System::Windows::Forms::DataGridViewRow^ Row in Rows)
		{
			Selected_Row = Row;
		}

		if (Selected_Row != nullptr)
		{
			Selected_Row->SetValues(gcnew cli::array< System::String^ >(2) { (Selected_Row->Index + 1).ToString(), device->Name });
		}

		Sync_Status_Update(SYNC_STATUS::SYNCED);

		Configuration_Device_Update(device);

		EEPROM_Write_Set_Pending(true);
	}
}

System::Void MIDI_Lighter::Device_List::Update_Configuration_RGB_Order(MIDI_Lighter_Wrapper::Configuration_RGB_Order^ configuration_rgb_order)
{
	if (_MIDI_Lighter->IsConnected())
	{
		Sync_Status_Update(SYNC_STATUS::SYNCING);
		if (!_MIDI_Lighter->Set_Configuration_RGB_Order(configuration_rgb_order))
		{
			Sync_Status_Update(SYNC_STATUS::FAILED);
		}
		Sync_Status_Update(SYNC_STATUS::SYNCED);

		EEPROM_Write_Set_Pending(true);
	}
}

System::Void MIDI_Lighter::Device_List::Update_EEPROM()
{
	if (_MIDI_Lighter->IsConnected())
	{
		_MIDI_Lighter->Write_EEPROM();

		EEPROM_Write_Set_Pending(false);
	}
}

System::Void MIDI_Lighter::Device_List::Set_Timer1_Top(System::UInt16 top)
{
	_MIDI_Lighter->Set_Timer1_Top(top);
}

System::Void MIDI_Lighter::Device_List::Set_Timer4_Prescaler(MIDI_Lighter_Wrapper::Timer4_Presacler prescaler)
{
	_MIDI_Lighter->Set_Timer4_Prescaler(prescaler);
}

System::Byte MIDI_Lighter::Device_List::Read_EEPROM(uint32_t address)
{
	if (_MIDI_Lighter->IsConnected())
	{
		System::Boolean^ Success = gcnew System::Boolean;
		return _MIDI_Lighter->Read_EEPROM(address, Success);
	}

	return 0;
}

System::UInt32 MIDI_Lighter::Device_List::Read_ADC()
{
	if (_MIDI_Lighter->IsConnected())
	{
		System::Boolean^ Success = gcnew System::Boolean;
		return _MIDI_Lighter->Get_ADC(Success);
	}

	return 0;
}

