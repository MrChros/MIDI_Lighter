#include "Configuration_RGB_Order.h"

MIDI_Lighter::Configuration_RGB_Order::Configuration_RGB_Order()
{
	System::Drawing::Font^ Font_Bold = gcnew System::Drawing::Font(this->Font->Name, this->Font->Size, System::Drawing::FontStyle::Bold);
	
	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount = 2;
	Table_Layout_Main->ColumnCount = 4;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Top;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 8)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 110)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50)));
	
		System::Windows::Forms::Label^ Label_Title = gcnew System::Windows::Forms::Label();
		Label_Title->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Title->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		Label_Title->Text = "RGB Order:";
		Label_Title->Font = Font_Bold;
	Table_Layout_Main->Controls->Add(Label_Title, 0, 0);

		_ComboBox_RGB_Order = gcnew System::Windows::Forms::ComboBox();
		_ComboBox_RGB_Order->Dock = System::Windows::Forms::DockStyle::Fill;
		_ComboBox_RGB_Order->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
		_ComboBox_RGB_Order->Font = gcnew System::Drawing::Font("Courier New", 8.0);
		_ComboBox_RGB_Order->Margin = System::Windows::Forms::Padding(4, 1, 4, 0);
		
		_ComboBox_RGB_Order->Items->Add("Red   - Green - Blue");
		_ComboBox_RGB_Order->Items->Add("Red   - Blue  - Green");
		_ComboBox_RGB_Order->Items->Add("Green - Red   - Blue");
		_ComboBox_RGB_Order->Items->Add("Green - Blue  - Red");
		_ComboBox_RGB_Order->Items->Add("Blue  - Red   - Green");
		_ComboBox_RGB_Order->Items->Add("Blue  - Green - Red");
		_ComboBox_RGB_Order->SelectedIndex = 0;

		_ComboBox_RGB_Order->SelectedIndexChanged += gcnew System::EventHandler(this, &Configuration_RGB_Order::Value_Changed);
	Table_Layout_Main->Controls->Add(_ComboBox_RGB_Order, 1, 0);
	Table_Layout_Main->SetColumnSpan(_ComboBox_RGB_Order, 3);

	/////////////////////
	// Horizontal Line //
	/////////////////////
		System::Windows::Forms::Label^ Label_Horizontal_Line_1 = gcnew System::Windows::Forms::Label();
		Label_Horizontal_Line_1->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Horizontal_Line_1->Height = 2;
		Label_Horizontal_Line_1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
	Table_Layout_Main->Controls->Add(Label_Horizontal_Line_1, 0, 1);
	Table_Layout_Main->SetColumnSpan(Label_Horizontal_Line_1, 4);

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

	_Internal_Update = false;
}

System::Void MIDI_Lighter::Configuration_RGB_Order::Update(MIDI_Lighter_Wrapper::Configuration_RGB_Order^ configuration_rgb_order)
{
	_Internal_Update = true;

	if((int32_t)configuration_rgb_order->RGB_Order < _ComboBox_RGB_Order->Items->Count)
	{
		_ComboBox_RGB_Order->SelectedIndex = (int)configuration_rgb_order->RGB_Order;
	}

	_Internal_Update = false;
}

System::Void MIDI_Lighter::Configuration_RGB_Order::Value_Changed(System::Object^ sender, System::EventArgs^ e)
{
	if (_Internal_Update == true) { return; }

	MIDI_Lighter_Wrapper::Configuration_RGB_Order^ Configuration_RGB_Order = gcnew MIDI_Lighter_Wrapper::Configuration_RGB_Order();

	Configuration_RGB_Order->RGB_Order = (MIDI_Lighter_Wrapper::RGB_Order)_ComboBox_RGB_Order->SelectedIndex;

	Update_Values(Configuration_RGB_Order);
}