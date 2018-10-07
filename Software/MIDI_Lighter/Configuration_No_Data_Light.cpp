#include "Configuration_No_Data_Light.h"

MIDI_Lighter::Configuration_No_Data_Light::Configuration_No_Data_Light()
{
	_Internal_Update = false;
	
	_Resources = gcnew System::Resources::ResourceManager("MIDI_Lighter.MIDI_Lighter", System::Reflection::Assembly::GetExecutingAssembly());
	
	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount = 2;
	Table_Layout_Main->ColumnCount = 4;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Top;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 100)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,  50)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,  50)));


		System::Windows::Forms::Label^ Label_Title = gcnew System::Windows::Forms::Label();
		Label_Title->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Title->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		Label_Title->Text = "No Data Light:";
	Table_Layout_Main->Controls->Add(Label_Title, 0, 0);


		_Label_Color				= gcnew System::Windows::Forms::Label();
		_Label_Color->Dock			= System::Windows::Forms::DockStyle::Top;
		_Label_Color->Margin		= System::Windows::Forms::Padding(4, 2, 4, 7);
		_Label_Color->Height		= 21;
		_Label_Color->BackColor		= System::Drawing::Color::Black;
		_Label_Color->BorderStyle	= System::Windows::Forms::BorderStyle::Fixed3D;
	Table_Layout_Main->Controls->Add(_Label_Color, 1, 0);

		_ColorDialog = gcnew System::Windows::Forms::ColorDialog();

		System::Windows::Forms::Button^ Button_Color	= gcnew System::Windows::Forms::Button();
		Button_Color->Dock								= System::Windows::Forms::DockStyle::Top;
		Button_Color->Text								= "Select Color";
		Button_Color->TextImageRelation					= System::Windows::Forms::TextImageRelation::ImageBeforeText;
		Button_Color->TextAlign							= System::Drawing::ContentAlignment::MiddleRight;
		Button_Color->Image								= (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"palette")));
		Button_Color->UseVisualStyleBackColor			= true;
		Button_Color->Margin							= System::Windows::Forms::Padding(4, 1, 4, 6);
		Button_Color->Click += gcnew System::EventHandler(this, &Configuration_No_Data_Light::Button_Color_Click);
	Table_Layout_Main->Controls->Add(Button_Color, 2, 0);

		_Numeric_Timeout				= gcnew System::Windows::Forms::NumericUpDown();
		_Numeric_Timeout->Dock			= System::Windows::Forms::DockStyle::Top;
		_Numeric_Timeout->DecimalPlaces = 1;
		_Numeric_Timeout->Increment		= System::Decimal(0.2);
		_Numeric_Timeout->Minimum		= 0;
		_Numeric_Timeout->Maximum		= 20;
		_Numeric_Timeout->Height		= 21;
		_Numeric_Timeout->Text			= "Test";
		_Numeric_Timeout->Margin		= System::Windows::Forms::Padding(4, 2, 4, 5);
		_Numeric_Timeout->ValueChanged += gcnew System::EventHandler(this, &Configuration_No_Data_Light::Value_Changed);
	Table_Layout_Main->Controls->Add(_Numeric_Timeout, 3, 0);
	
		_ComboBox_Deactivate				= gcnew System::Windows::Forms::ComboBox();
		_ComboBox_Deactivate->Dock			= System::Windows::Forms::DockStyle::Fill;
		_ComboBox_Deactivate->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
		_ComboBox_Deactivate->Font			= gcnew System::Drawing::Font("Courier New", 8.0);
		_ComboBox_Deactivate->Margin		= System::Windows::Forms::Padding(4, 1, 4, 0);
		
		_ComboBox_Deactivate->Items->Add(System::String::Format("Deactivate on any MIDI traffic"));
		_ComboBox_Deactivate->Items->Add(System::String::Format("Deactivate on MIDI Channel match only"));
		_ComboBox_Deactivate->Items->Add(System::String::Format("Deactivate on MIDI Event match only"));
		_ComboBox_Deactivate->Items->Add(System::String::Format("Deactivate on MIDI Channel and Event match"));
		_ComboBox_Deactivate->SelectedIndex = 0;

		_ComboBox_Deactivate->SelectedIndexChanged += gcnew System::EventHandler(this, &Configuration_No_Data_Light::Value_Changed);
	Table_Layout_Main->Controls->Add(_ComboBox_Deactivate, 1, 1);
	Table_Layout_Main->SetColumnSpan(_ComboBox_Deactivate, 3);
		
//	_GroupBox_Main = gcnew System::Windows::Forms::GroupBox();
//	_GroupBox_Main->Text = "No Data Light";
//	_GroupBox_Main->Dock = System::Windows::Forms::DockStyle::Fill;
//	_GroupBox_Main->Font = gcnew System::Drawing::Font(this->Font, System::Drawing::FontStyle::Bold);
//	_GroupBox_Main->Controls->Add(Table_Layout_Main);
//
//	for each (System::Windows::Forms::Control^ Control in this->_GroupBox_Main->Controls)
//	{
//		Control->Font = gcnew System::Drawing::Font(this->Font, System::Drawing::FontStyle::Regular);
//	}
//
//	this->Controls->Add(_GroupBox_Main);

	this->Controls->Add(Table_Layout_Main);
}

System::Void MIDI_Lighter::Configuration_No_Data_Light::Button_Color_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (_ColorDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		_Label_Color->BackColor = _ColorDialog->Color;

		Value_Changed(nullptr, System::EventArgs::Empty);
	}
}

System::Void MIDI_Lighter::Configuration_No_Data_Light::Update(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ configuration_no_data_light)
{
	_Internal_Update = true;

	_ColorDialog->Color		= configuration_no_data_light->Color;
	_Label_Color->BackColor = _ColorDialog->Color;

	_Numeric_Timeout->Value = _Numeric_Timeout->Increment * configuration_no_data_light->Timeout;
	
	if ((int32_t)configuration_no_data_light->Deactivate < _ComboBox_Deactivate->Items->Count)
	{
		_ComboBox_Deactivate->SelectedIndex = (int32_t)configuration_no_data_light->Deactivate;
	}

	_Internal_Update = false;
}

System::Void MIDI_Lighter::Configuration_No_Data_Light::Value_Changed(System::Object^ sender, System::EventArgs^ e)
{
	if (_Internal_Update == true) { return; }

	MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ Configuration_No_Data_Light = gcnew MIDI_Lighter_Wrapper::Configuration_No_Data_Light();

	Configuration_No_Data_Light->Color		= _ColorDialog->Color;
	Configuration_No_Data_Light->Timeout	= (System::Byte) System::Decimal::ToUInt32(_Numeric_Timeout->Value / System::Decimal(0.2));
	Configuration_No_Data_Light->Deactivate = (MIDI_Lighter_Wrapper::No_Data_Light_Deactivate)_ComboBox_Deactivate->SelectedIndex;

	Update_Values(Configuration_No_Data_Light);
}