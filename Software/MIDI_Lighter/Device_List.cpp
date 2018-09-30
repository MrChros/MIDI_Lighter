#include "Device_List.h"

MIDI_Lighter::Device_List::Device_List()
{
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
		_DataGrid_Devices->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^ >(2) {
			gcnew System::Windows::Forms::DataGridViewTextBoxColumn(),
			gcnew System::Windows::Forms::DataGridViewTextBoxColumn()
		});

		_DataGrid_Devices->Columns[0]->HeaderText	= "#";
		_DataGrid_Devices->Columns[0]->Width		= 40;
		_DataGrid_Devices->Columns[0]->SortMode		= System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
		_DataGrid_Devices->Columns[1]->HeaderText	= "Device Name";
		_DataGrid_Devices->Columns[1]->AutoSizeMode	= System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
		_DataGrid_Devices->Columns[1]->SortMode		= System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
	Table_Layout_Main->Controls->Add(_DataGrid_Devices, 0, 2);


		_Label_Number_Devices = gcnew System::Windows::Forms::Label();
		_Label_Number_Devices->Dock = System::Windows::Forms::DockStyle::Top;
		_Label_Number_Devices->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
	Table_Layout_Main->Controls->Add(_Label_Number_Devices, 0, 3);



	_DataGrid_Devices->Rows->Add(gcnew cli::array< System::String^ >(2) { (_DataGrid_Devices->RowCount + 1).ToString(), "Test " + (_DataGrid_Devices->RowCount + 1).ToString()});
	_DataGrid_Devices->Rows->Add(gcnew cli::array< System::String^ >(2) { (_DataGrid_Devices->RowCount + 1).ToString(), "Test " + (_DataGrid_Devices->RowCount + 1).ToString()});
	_DataGrid_Devices->Rows->Add(gcnew cli::array< System::String^ >(2) { (_DataGrid_Devices->RowCount + 1).ToString(), "Test " + (_DataGrid_Devices->RowCount + 1).ToString()});
	_DataGrid_Devices->Rows->Add(gcnew cli::array< System::String^ >(2) { (_DataGrid_Devices->RowCount + 1).ToString(), "Test " + (_DataGrid_Devices->RowCount + 1).ToString()});
	_DataGrid_Devices->Rows->Add(gcnew cli::array< System::String^ >(2) { (_DataGrid_Devices->RowCount + 1).ToString(), "Test " + (_DataGrid_Devices->RowCount + 1).ToString()});


	this->Controls->Add(Table_Layout_Main);

	Label_Number_Devices_Update();
}

System::Void MIDI_Lighter::Device_List::Button_Refresh_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	_DataGrid_Devices->Rows->Clear();

	System::Collections::Generic::List<MIDI_Lighter_Wrapper::Device^ >^ Device_List = _MIDI_Lighter->Get_Device_List();

	for (int i = 0; i < Device_List->Count; i++)
	{
		_DataGrid_Devices->Rows->Add(gcnew cli::array< System::String^ >(2) { (Device_List[i]->Index + 1).ToString(), Device_List[i]->Name });
	}

	Label_Number_Devices_Update();
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
