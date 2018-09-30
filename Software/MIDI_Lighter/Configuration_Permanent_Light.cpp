#include "Configuration_Permanent_Light.h"

MIDI_Lighter::Configuration_Permanent_Light::Configuration_Permanent_Light()
{
	_Resources = gcnew System::Resources::ResourceManager("MIDI_Lighter.MIDI_Lighter", System::Reflection::Assembly::GetExecutingAssembly());
	
	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount = 1;
	Table_Layout_Main->ColumnCount = 4;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Top;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 100)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));

		System::Windows::Forms::Label^ Label_Title = gcnew System::Windows::Forms::Label();
		Label_Title->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Title->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		Label_Title->Text = "Permanent Light:";
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
		Button_Color->Click += gcnew System::EventHandler(this, &Configuration_Permanent_Light::Button_Color_Click);
	Table_Layout_Main->Controls->Add(Button_Color, 2, 0);

//	_GroupBox_Main = gcnew System::Windows::Forms::GroupBox();
//	_GroupBox_Main->Text = "Permanent Light";
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

System::Void MIDI_Lighter::Configuration_Permanent_Light::Button_Color_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (_ColorDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		_Label_Color->BackColor = _ColorDialog->Color;

		MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ Configuration_Permanent_Light = gcnew MIDI_Lighter_Wrapper::Configuration_Permanent_Light();

		Configuration_Permanent_Light->Color = _ColorDialog->Color;
		Update_Values(Configuration_Permanent_Light);
	}
}

System::Void MIDI_Lighter::Configuration_Permanent_Light::Update(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ configuration_permanent_light)
{
	_ColorDialog->Color		= configuration_permanent_light->Color;
	_Label_Color->BackColor = _ColorDialog->Color;
}