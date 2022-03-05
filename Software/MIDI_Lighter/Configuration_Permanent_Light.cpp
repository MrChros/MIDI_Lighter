#include "Configuration_Permanent_Light.h"

MIDI_Lighter::Configuration_Permanent_Light::Configuration_Permanent_Light()
{
	_Internal_Update = false;
	
	_Resources = gcnew System::Resources::ResourceManager("MIDI_Lighter.MIDI_Lighter", System::Reflection::Assembly::GetExecutingAssembly());
	
	System::Drawing::Font^ Font_Bold = gcnew System::Drawing::Font(this->Font->Name, this->Font->Size, System::Drawing::FontStyle::Bold);

	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount = 2;
	Table_Layout_Main->ColumnCount = 5;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Top;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 8)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 110)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,  45)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,   50)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,  80)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,   50)));


		System::Windows::Forms::Label^ Label_Title = gcnew System::Windows::Forms::Label();
		Label_Title->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Title->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		Label_Title->Text = "Permanent Light:";
		Label_Title->Font = Font_Bold;
	Table_Layout_Main->Controls->Add(Label_Title, 0, 0);


		System::Windows::Forms::Label^ Label_Color_Text = gcnew System::Windows::Forms::Label();
		Label_Color_Text->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Color_Text->Text = "Color:";
		Label_Color_Text->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
	Table_Layout_Main->Controls->Add(Label_Color_Text, 1, 0);

		_Label_Color				= gcnew System::Windows::Forms::Label();
		_Label_Color->Dock			= System::Windows::Forms::DockStyle::Top;
		_Label_Color->Margin		= System::Windows::Forms::Padding(4, 2, 4, 7);
		_Label_Color->Height		= 21;
		_Label_Color->BackColor		= System::Drawing::Color::Black;
		_Label_Color->BorderStyle	= System::Windows::Forms::BorderStyle::Fixed3D;
		_Label_Color->Click		   += gcnew System::EventHandler(this, &Configuration_Permanent_Light::Label_Color_Click);
	Table_Layout_Main->Controls->Add(_Label_Color, 2, 0);

		_ColorDialog = gcnew System::Windows::Forms::ColorDialog();


	/////////////////////
	// Horizontal Line //
	/////////////////////
		System::Windows::Forms::Label^ Label_Horizontal_Line_1 = gcnew System::Windows::Forms::Label();
		Label_Horizontal_Line_1->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Horizontal_Line_1->Height = 2;
		Label_Horizontal_Line_1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
	Table_Layout_Main->Controls->Add(Label_Horizontal_Line_1, 0, 1);
	Table_Layout_Main->SetColumnSpan(Label_Horizontal_Line_1, 5);

	this->Controls->Add(Table_Layout_Main);
}

System::Void MIDI_Lighter::Configuration_Permanent_Light::Label_Color_Click(System::Object^ sender, System::EventArgs^ e)
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