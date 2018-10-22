#include "Configuration_MIDI.h"

MIDI_Lighter::Configuration_MIDI::Configuration_MIDI()
{
	_Internal_Update = true;
	
	System::Drawing::Font^ Font_Bold	= gcnew System::Drawing::Font(this->Font, System::Drawing::FontStyle::Bold);
	System::Drawing::Font^ Font_Reguar	= gcnew System::Drawing::Font(this->Font, System::Drawing::FontStyle::Regular);

	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount		= 8;
	Table_Layout_Main->ColumnCount	= 3;
	Table_Layout_Main->Dock			= System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,  8)));
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 8)));
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 100)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,  100)));
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,  40)));


	//////////////////
	// MIDI Channel //
	//////////////////
		System::Windows::Forms::Label^ Label_Channel = gcnew System::Windows::Forms::Label();
		Label_Channel->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Channel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		Label_Channel->Text = "MIDI Channel:";
		Label_Channel->Font = Font_Bold;
	Table_Layout_Main->Controls->Add(Label_Channel, 0, 0);

		_ComboBox_Channel = gcnew System::Windows::Forms::ComboBox();
		_ComboBox_Channel->Dock = System::Windows::Forms::DockStyle::Fill;
		_ComboBox_Channel->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
		_ComboBox_Channel->Font = gcnew System::Drawing::Font("Courier New", 8.0);
		_ComboBox_Channel->Margin = System::Windows::Forms::Padding(4, 1, 4, 0);
		for (int i = 0; i < 16; i++) { _ComboBox_Channel->Items->Add(System::String::Format("Ch{0,2:D2}", i+1)); }
		_ComboBox_Channel->SelectedIndex = 0;
		_ComboBox_Channel->SelectedIndexChanged += gcnew System::EventHandler(this, &Configuration_MIDI::Value_Changed);
	Table_Layout_Main->Controls->Add(_ComboBox_Channel, 1, 0);
	Table_Layout_Main->SetColumnSpan(_ComboBox_Channel, 2);

	/////////////////////
	// Horizontal Line //
	/////////////////////
		System::Windows::Forms::Label^ Label_Horizontal_Line_1 = gcnew System::Windows::Forms::Label();
		Label_Horizontal_Line_1->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Horizontal_Line_1->Height = 2;
		Label_Horizontal_Line_1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
	Table_Layout_Main->Controls->Add(Label_Horizontal_Line_1, 0, 1);
	Table_Layout_Main->SetColumnSpan(Label_Horizontal_Line_1, 3);

	////////////////
	// MIDI Notes //
	////////////////

	array<System::String^>^ Label_Notes_Text = gcnew array<System::String^>(3);
	Label_Notes_Text[0] = "Note Red:";
	Label_Notes_Text[1] = "Note Green:";
	Label_Notes_Text[2] = "Note Blue:";

	_ComboBox_Notes = gcnew array<System::Windows::Forms::ComboBox^>(3);
	for(int i = 0; i < 3; i++)
	{
			System::Windows::Forms::Label^ Label = gcnew System::Windows::Forms::Label();
			Label->Dock				= System::Windows::Forms::DockStyle::Top;
			Label->TextAlign		= System::Drawing::ContentAlignment::MiddleLeft;
			Label->Text				= Label_Notes_Text[i];
			if(i==0)	{ Label->ForeColor = System::Drawing::Color::Red;	} else
			if(i==1)	{ Label->ForeColor = System::Drawing::Color::Green;	} else
						{ Label->ForeColor = System::Drawing::Color::Blue;	}
			Label->Font				= Font_Bold;
		Table_Layout_Main->Controls->Add(Label, 0, 2+i);

			System::Windows::Forms::ComboBox^ ComboBox = gcnew System::Windows::Forms::ComboBox();
			ComboBox->Dock			= System::Windows::Forms::DockStyle::Fill;
			ComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			ComboBox->Font			= gcnew System::Drawing::Font("Courier New", 8.0);
			ComboBox->Margin		= System::Windows::Forms::Padding(4, 1, 4, 0);
			ComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Configuration_MIDI::Value_Changed);
		Table_Layout_Main->Controls->Add(ComboBox, 1, 2+i);
		Table_Layout_Main->SetColumnSpan(ComboBox, 2);

		_ComboBox_Notes[i] = ComboBox;
	}

	/////////////////////
	// Horizontal Line //
	/////////////////////
		System::Windows::Forms::Label^ Label_Horizontal_Line_2 = gcnew System::Windows::Forms::Label();
		Label_Horizontal_Line_2->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Horizontal_Line_2->Height = 2;
		Label_Horizontal_Line_2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
	Table_Layout_Main->Controls->Add(Label_Horizontal_Line_2, 0, 5);
	Table_Layout_Main->SetColumnSpan(Label_Horizontal_Line_2, 3);
	
		System::Windows::Forms::Label^ Label_Shift = gcnew System::Windows::Forms::Label();
		Label_Shift->Dock = System::Windows::Forms::DockStyle::Top;
		Label_Shift->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		Label_Shift->Text = "Value Factor:";
		Label_Shift->Font = Font_Bold;
	Table_Layout_Main->Controls->Add(Label_Shift, 0, 6);

		_TrackBar_Factor = gcnew System::Windows::Forms::TrackBar();
		_TrackBar_Factor->Dock = System::Windows::Forms::DockStyle::Fill;
		_TrackBar_Factor->RightToLeft = System::Windows::Forms::RightToLeft::No;
		_TrackBar_Factor->Orientation = System::Windows::Forms::Orientation::Horizontal;
		_TrackBar_Factor->Maximum = 200;
		_TrackBar_Factor->Minimum = 1;
		_TrackBar_Factor->TickFrequency = 10;
		_TrackBar_Factor->ValueChanged += gcnew System::EventHandler(this, &Configuration_MIDI::Value_Changed);
	Table_Layout_Main->Controls->Add(_TrackBar_Factor, 1, 6);

		_Label_Factor = gcnew System::Windows::Forms::Label();
		_Label_Factor->Dock = System::Windows::Forms::DockStyle::Fill;
		_Label_Factor->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		_Label_Factor->Font = Font_Reguar;
	Table_Layout_Main->Controls->Add(_Label_Factor, 2, 6);


		System::Windows::Forms::Label^ Label_Skip_Note_Off	= gcnew System::Windows::Forms::Label();
		Label_Skip_Note_Off->Dock							= System::Windows::Forms::DockStyle::Fill;
		Label_Skip_Note_Off->TextAlign						= System::Drawing::ContentAlignment::MiddleLeft;
		Label_Skip_Note_Off->Text							= "Skip Note Off:";
		Label_Skip_Note_Off->Font							= Font_Bold;
	Table_Layout_Main->Controls->Add(Label_Skip_Note_Off, 0, 7);

		_CheckBox_Skip_Note_Off						= gcnew System::Windows::Forms::CheckBox();	
		_CheckBox_Skip_Note_Off->Dock				= System::Windows::Forms::DockStyle::Fill;
		_CheckBox_Skip_Note_Off->RightToLeft		= System::Windows::Forms::RightToLeft::No;
		_CheckBox_Skip_Note_Off->Padding			= System::Windows::Forms::Padding(0, 3, 0, 0);
		_CheckBox_Skip_Note_Off->CheckedChanged	   += gcnew System::EventHandler(this, &Configuration_MIDI::Value_Changed);
	Table_Layout_Main->Controls->Add(_CheckBox_Skip_Note_Off, 1, 7);


	this->Controls->Add(Table_Layout_Main);

	array<System::String^>^ Midi_Note_List = gcnew array<System::String^>(120);

	int i = 0;

	Midi_Note_List[i++] = L"C-1 ";
	Midi_Note_List[i++] = L"C#-1";
	Midi_Note_List[i++] = L"D-1 ";
	Midi_Note_List[i++] = L"Eb-1";
	Midi_Note_List[i++] = L"E-1 ";
	Midi_Note_List[i++] = L"F-1 ";
	Midi_Note_List[i++] = L"F#-1";
	Midi_Note_List[i++] = L"G-1 ";
	Midi_Note_List[i++] = L"G#-1";
	Midi_Note_List[i++] = L"A-1 ";
	Midi_Note_List[i++] = L"Bb-1";
	Midi_Note_List[i++] = L"B-1 ";
	Midi_Note_List[i++] = L"C0  ";
	Midi_Note_List[i++] = L"C#0 ";
	Midi_Note_List[i++] = L"D0  ";
	Midi_Note_List[i++] = L"Eb0 ";
	Midi_Note_List[i++] = L"E0  ";
	Midi_Note_List[i++] = L"F0  ";
	Midi_Note_List[i++] = L"F#0 ";
	Midi_Note_List[i++] = L"G0  ";
	Midi_Note_List[i++] = L"G#0 ";
	Midi_Note_List[i++] = L"A0  ";
	Midi_Note_List[i++] = L"Bb0 ";
	Midi_Note_List[i++] = L"B0  ";
	Midi_Note_List[i++] = L"C1  ";
	Midi_Note_List[i++] = L"C#1 ";
	Midi_Note_List[i++] = L"D1  ";
	Midi_Note_List[i++] = L"Eb1 ";
	Midi_Note_List[i++] = L"E1  ";
	Midi_Note_List[i++] = L"F1  ";
	Midi_Note_List[i++] = L"F#1 ";
	Midi_Note_List[i++] = L"G1  ";
	Midi_Note_List[i++] = L"G#1 ";
	Midi_Note_List[i++] = L"A1  ";
	Midi_Note_List[i++] = L"Bb1 ";
	Midi_Note_List[i++] = L"B1  ";
	Midi_Note_List[i++] = L"C2  ";
	Midi_Note_List[i++] = L"C#2 ";
	Midi_Note_List[i++] = L"D2  ";
	Midi_Note_List[i++] = L"Eb2 ";
	Midi_Note_List[i++] = L"E2  ";
	Midi_Note_List[i++] = L"F2  ";
	Midi_Note_List[i++] = L"F#2 ";
	Midi_Note_List[i++] = L"G2  ";
	Midi_Note_List[i++] = L"G#2 ";
	Midi_Note_List[i++] = L"A2  ";
	Midi_Note_List[i++] = L"Bb2 ";
	Midi_Note_List[i++] = L"B2  ";
	Midi_Note_List[i++] = L"C3  ";
	Midi_Note_List[i++] = L"C#3 ";
	Midi_Note_List[i++] = L"D3  ";
	Midi_Note_List[i++] = L"Eb3 ";
	Midi_Note_List[i++] = L"E3  ";
	Midi_Note_List[i++] = L"F3  ";
	Midi_Note_List[i++] = L"F#3 ";
	Midi_Note_List[i++] = L"G3  ";
	Midi_Note_List[i++] = L"G#3 ";
	Midi_Note_List[i++] = L"A3  ";
	Midi_Note_List[i++] = L"Bb3 ";
	Midi_Note_List[i++] = L"B3  ";
	Midi_Note_List[i++] = L"C4  ";
	Midi_Note_List[i++] = L"C#4 ";
	Midi_Note_List[i++] = L"D4  ";
	Midi_Note_List[i++] = L"Eb4 ";
	Midi_Note_List[i++] = L"E4  ";
	Midi_Note_List[i++] = L"F4  ";
	Midi_Note_List[i++] = L"F#4 ";
	Midi_Note_List[i++] = L"G4  ";
	Midi_Note_List[i++] = L"G#4 ";
	Midi_Note_List[i++] = L"A4  ";
	Midi_Note_List[i++] = L"Bb4 ";
	Midi_Note_List[i++] = L"B4  ";
	Midi_Note_List[i++] = L"C5  ";
	Midi_Note_List[i++] = L"C#5 ";
	Midi_Note_List[i++] = L"D5  ";
	Midi_Note_List[i++] = L"Eb5 ";
	Midi_Note_List[i++] = L"E5  ";
	Midi_Note_List[i++] = L"F5  ";
	Midi_Note_List[i++] = L"F#5 ";
	Midi_Note_List[i++] = L"G5  ";
	Midi_Note_List[i++] = L"G#5 ";
	Midi_Note_List[i++] = L"A5  ";
	Midi_Note_List[i++] = L"Bb5 ";
	Midi_Note_List[i++] = L"B5  ";
	Midi_Note_List[i++] = L"C6  ";
	Midi_Note_List[i++] = L"C#6 ";
	Midi_Note_List[i++] = L"D6  ";
	Midi_Note_List[i++] = L"Eb6 ";
	Midi_Note_List[i++] = L"E6  ";
	Midi_Note_List[i++] = L"F6  ";
	Midi_Note_List[i++] = L"F#6 ";
	Midi_Note_List[i++] = L"G6  ";
	Midi_Note_List[i++] = L"G#6 ";
	Midi_Note_List[i++] = L"A6  ";
	Midi_Note_List[i++] = L"Bb6 ";
	Midi_Note_List[i++] = L"B6  ";
	Midi_Note_List[i++] = L"C7  ";
	Midi_Note_List[i++] = L"C#7 ";
	Midi_Note_List[i++] = L"D7  ";
	Midi_Note_List[i++] = L"Eb7 ";
	Midi_Note_List[i++] = L"E7  ";
	Midi_Note_List[i++] = L"F7  ";
	Midi_Note_List[i++] = L"F#7 ";
	Midi_Note_List[i++] = L"G7  ";
	Midi_Note_List[i++] = L"G#7 ";
	Midi_Note_List[i++] = L"A7  ";
	Midi_Note_List[i++] = L"Bb7 ";
	Midi_Note_List[i++] = L"B7  ";
	Midi_Note_List[i++] = L"C8  ";
	Midi_Note_List[i++] = L"C#8 ";
	Midi_Note_List[i++] = L"D8  ";
	Midi_Note_List[i++] = L"Eb8 ";
	Midi_Note_List[i++] = L"E8  ";
	Midi_Note_List[i++] = L"F8  ";
	Midi_Note_List[i++] = L"F#8 ";
	Midi_Note_List[i++] = L"G8  ";
	Midi_Note_List[i++] = L"G#8 ";
	Midi_Note_List[i++] = L"A8  ";
	Midi_Note_List[i++] = L"Bb8 ";
	Midi_Note_List[i++] = L"B8  ";


	for (i = 0; i < Midi_Note_List->Length; i++)
	{
		System::String^ Note = System::String::Format("{0,-5} {1,3}", Midi_Note_List[i], i);
		
		_ComboBox_Notes[0]->Items->Add(Note);
		_ComboBox_Notes[1]->Items->Add(Note);
		_ComboBox_Notes[2]->Items->Add(Note);
	}

	_ComboBox_Notes[0]->SelectedIndex = 0;
	_ComboBox_Notes[1]->SelectedIndex = 0;
	_ComboBox_Notes[2]->SelectedIndex = 0;

	_Internal_Update = false;
}

System::Void MIDI_Lighter::Configuration_MIDI::Update(MIDI_Lighter_Wrapper::Configuration_MIDI^ configuration_midi)
{
	_Internal_Update = true;
	
	_ComboBox_Channel->SelectedIndex = (int)configuration_midi->Channel;

	_ComboBox_Notes[(int)MIDI_Lighter_Wrapper::Color::Red]	->SelectedIndex = (int)configuration_midi->Note[(int)MIDI_Lighter_Wrapper::Color::Red	];
	_ComboBox_Notes[(int)MIDI_Lighter_Wrapper::Color::Green]->SelectedIndex = (int)configuration_midi->Note[(int)MIDI_Lighter_Wrapper::Color::Green	];
	_ComboBox_Notes[(int)MIDI_Lighter_Wrapper::Color::Blue]	->SelectedIndex = (int)configuration_midi->Note[(int)MIDI_Lighter_Wrapper::Color::Blue	];

	_TrackBar_Factor->Value				= configuration_midi->Factor;
	_CheckBox_Skip_Note_Off->Checked	= configuration_midi->Skip_Note_Off;

	Label_Factor_Update(_TrackBar_Factor->Value);

	_Internal_Update = false;
}

System::Void MIDI_Lighter::Configuration_MIDI::Value_Changed(System::Object^ sender, System::EventArgs^ e)
{
	if(_Internal_Update == true) { return; }
	
	MIDI_Lighter_Wrapper::Configuration_MIDI^ Configuration_MIDI = gcnew MIDI_Lighter_Wrapper::Configuration_MIDI();

	Configuration_MIDI->Channel = (MIDI_Lighter_Wrapper::MIDI_Channel)_ComboBox_Channel->SelectedIndex;
	Configuration_MIDI->Note[(int)MIDI_Lighter_Wrapper::Color::Red	] = (MIDI_Lighter_Wrapper::MIDI_Note)_ComboBox_Notes[(int)MIDI_Lighter_Wrapper::Color::Red	]->SelectedIndex;
	Configuration_MIDI->Note[(int)MIDI_Lighter_Wrapper::Color::Green] = (MIDI_Lighter_Wrapper::MIDI_Note)_ComboBox_Notes[(int)MIDI_Lighter_Wrapper::Color::Green]->SelectedIndex;
	Configuration_MIDI->Note[(int)MIDI_Lighter_Wrapper::Color::Blue	] = (MIDI_Lighter_Wrapper::MIDI_Note)_ComboBox_Notes[(int)MIDI_Lighter_Wrapper::Color::Blue	]->SelectedIndex;
	Configuration_MIDI->Factor			= _TrackBar_Factor->Value;
	Configuration_MIDI->Skip_Note_Off	= _CheckBox_Skip_Note_Off->Checked;

	Label_Factor_Update(Configuration_MIDI->Factor);

	Update_Values(Configuration_MIDI);
}

void MIDI_Lighter::Configuration_MIDI::Label_Factor_Update(unsigned int value)
{
	_Label_Factor->Text = ((value * 100) / 100).ToString() + "%";
}