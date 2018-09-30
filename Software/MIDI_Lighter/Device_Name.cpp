#include "Device_Name.h"

MIDI_Lighter::Device_Name::Device_Name()
{
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
		Label_Title->Text = "Device Name:";
	Table_Layout_Main->Controls->Add(Label_Title, 0, 0);

		_TextBox_Name				= gcnew System::Windows::Forms::TextBox();
		_TextBox_Name->Dock			= System::Windows::Forms::DockStyle::Top;
		_TextBox_Name->Margin		= System::Windows::Forms::Padding(4, 4, 4, 0);
		_TextBox_Name->Height		= 21;
		_TextBox_Name->MaxLength	= 31;
		_TextBox_Name->KeyPress    += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Device_Name::TextBox_Name_KeyPress);
	Table_Layout_Main->Controls->Add(_TextBox_Name, 1, 0);
	Table_Layout_Main->SetColumnSpan(_TextBox_Name, 2);

		System::Windows::Forms::Button^ Button_Set_Name		= gcnew System::Windows::Forms::Button();
		Button_Set_Name->Dock								= System::Windows::Forms::DockStyle::Top;
		Button_Set_Name->Text								= "Set Name";
		Button_Set_Name->UseVisualStyleBackColor			= true;
		Button_Set_Name->Click += gcnew System::EventHandler(this, &Device_Name::Button_Set_Name_Click);
	Table_Layout_Main->Controls->Add(Button_Set_Name, 2, 0);

//	_GroupBox_Main = gcnew System::Windows::Forms::GroupBox();
//	_GroupBox_Main->Text = "Device";
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

System::Void MIDI_Lighter::Device_Name::TextBox_Name_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
	if (e->KeyChar == (char)System::Windows::Forms::Keys::Enter)
	{
		Button_Set_Name_Click(nullptr, System::EventArgs::Empty);
	}
}

System::Void MIDI_Lighter::Device_Name::Button_Set_Name_Click(System::Object^ sender, System::EventArgs^ e)
{
	if(_TextBox_Name->Text->Trim()->Length > 0)
	{
		MIDI_Lighter_Wrapper::Device^ Device = gcnew MIDI_Lighter_Wrapper::Device();
		Device->Name = _TextBox_Name->Text->Trim();

		Update_Values(Device);
	}
	else
	{
		System::Windows::Forms::MessageBox::Show(
			"Invalid device data",
			"Error", System::Windows::Forms::MessageBoxButtons::OK,
			System::Windows::Forms::MessageBoxIcon::Error);
	}
}

System::Void MIDI_Lighter::Device_Name::Update(MIDI_Lighter_Wrapper::Device^ device)
{
	_TextBox_Name->Text = device->Name;
}