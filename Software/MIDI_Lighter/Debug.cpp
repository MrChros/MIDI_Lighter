#include "Debug.h"

MIDI_Lighter::Debug::Debug(MIDI_Lighter::Device_List^ device_list)
{
	_Device_List	= device_list;

	System::Drawing::Font^ Font_Header = gcnew System::Drawing::Font(this->Font->Name, this->Font->Size + 2, System::Drawing::FontStyle::Bold);

	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount = 5;
	Table_Layout_Main->ColumnCount = 1;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));		// 0
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));		// 1
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));		// 2
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));		// 3
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));	// 0

		System::Windows::Forms::Label^ Label_Header = gcnew System::Windows::Forms::Label();
		Label_Header->Dock			= System::Windows::Forms::DockStyle::Bottom;
		Label_Header->Font			= Font_Header;
		Label_Header->TextAlign		= System::Drawing::ContentAlignment::MiddleLeft;
		Label_Header->Text			= "Debug (only for debug)";
		Label_Header->ForeColor		= System::Drawing::Color::White;
		Label_Header->BackColor		= System::Drawing::Color::FromArgb(236, 192, 83);
		Label_Header->BorderStyle	= System::Windows::Forms::BorderStyle::FixedSingle;
	Table_Layout_Main->Controls->Add(Label_Header, 0, 0);

		System::Windows::Forms::Button^ Button_Read_EEPROM = gcnew System::Windows::Forms::Button();
		Button_Read_EEPROM->Dock = System::Windows::Forms::DockStyle::Top;
		Button_Read_EEPROM->Text = "Read EEPROM (Debug)";
		Button_Read_EEPROM->UseVisualStyleBackColor = true;
		Button_Read_EEPROM->Click += gcnew System::EventHandler(this, &Debug::Button_Read_EEPROM_Click);
	Table_Layout_Main->Controls->Add(Button_Read_EEPROM, 0, 1);

		_Button_Read_ADC = gcnew System::Windows::Forms::Button();
		_Button_Read_ADC->Dock = System::Windows::Forms::DockStyle::Top;
		_Button_Read_ADC->Text = "Toggle Periodic ADC Read (Now: Stopped)";
		_Button_Read_ADC->UseVisualStyleBackColor = true;
		_Button_Read_ADC->Click += gcnew System::EventHandler(this, &Debug::Button_Read_ADC_Click);
	Table_Layout_Main->Controls->Add(_Button_Read_ADC, 0, 2);


		_Button_Measurement = gcnew System::Windows::Forms::Button();
		_Button_Measurement->Dock = System::Windows::Forms::DockStyle::Top;
		_Button_Measurement->Text = "Start Programmed Measurement Sequence";
		_Button_Measurement->UseVisualStyleBackColor = true;
		_Button_Measurement->Click += gcnew System::EventHandler(this, &Debug::Button_Measurement_Click);
	Table_Layout_Main->Controls->Add(_Button_Measurement, 0, 3);
	
	
	this->Controls->Add(Table_Layout_Main);


	_Timer_Read_ADC = gcnew System::Windows::Forms::Timer();
	_Timer_Read_ADC->Interval = 100;
	_Timer_Read_ADC->Tick += gcnew System::EventHandler(this, &Debug::Timer_Read_ADC_Tick);

	_Timer_Measurement = gcnew System::Windows::Forms::Timer();
	_Timer_Measurement->Interval = 100;
	_Timer_Measurement->Tick += gcnew System::EventHandler(this, &Debug::Timer_Measurement_Tick);

	_Read_ADC_Counter = 0;

	_LogProgress = gcnew LogProgress::LogProgress();
	


	_Background_Worker_Read_EEPROM = gcnew System::ComponentModel::BackgroundWorker();
	_Background_Worker_Read_EEPROM->WorkerReportsProgress		= true;
	_Background_Worker_Read_EEPROM->WorkerSupportsCancellation	= true;
	_Background_Worker_Read_EEPROM->DoWork					   += gcnew System::ComponentModel::DoWorkEventHandler				(this, &Debug::Background_Worker_Read_EEPROM);
	_Background_Worker_Read_EEPROM->ProgressChanged			   += gcnew System::ComponentModel::ProgressChangedEventHandler		(this, &Debug::Background_Worker_ProgressChanged);
	_Background_Worker_Read_EEPROM->RunWorkerCompleted		   += gcnew System::ComponentModel::RunWorkerCompletedEventHandler	(this, &Debug::Background_Worker_RunWorkerCompleted);



	_Measurement_Per_Config		= 1;
	_List_Measurement_Prescaler = gcnew System::Collections::Generic::List<MIDI_Lighter_Wrapper::Timer4_Presacler>();
	_List_Measurement_Top		= gcnew System::Collections::Generic::List<System::UInt16>();

#ifdef NDEBUG
	Button_Read_EEPROM->Enabled		= false;
	_Button_Read_ADC->Enabled		= false;
	_Button_Measurement->Enabled	= false;
#endif // NDEBUG

}

System::Void MIDI_Lighter::Debug::Button_Read_EEPROM_Click(System::Object^ sender, System::EventArgs^ e)
{
	System::Windows::Forms::SaveFileDialog^ SaveFileDialog_User_Page = gcnew System::Windows::Forms::SaveFileDialog();
	SaveFileDialog_User_Page->Filter = "ASCII files (*.txt)|*.txt";
	SaveFileDialog_User_Page->ShowDialog(this);

	System::String^ Selected_File = SaveFileDialog_User_Page->FileName;
	delete SaveFileDialog_User_Page;
	if (Selected_File->Length == 0) { return; }

	_LogProgress->Set_Title("Reading EEPROM...");
	_LogProgress->Set_Progress_Maximum(0x3FF);
	_LogProgress->Set_Progress_Value(0);
	_LogProgress->Show(this);
	_LogProgress->Location = System::Drawing::Point(this->Left + this->Width / 2 - _LogProgress->Width / 2, this->Top + this->Height / 2 - _LogProgress->Height / 2);

	this->_Background_Worker_Read_EEPROM->RunWorkerAsync(gcnew array<System::String^>(1) { Selected_File });
}

System::Void MIDI_Lighter::Debug::Button_Read_ADC_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (_Timer_Read_ADC->Enabled)
	{
		_Timer_Read_ADC->Stop();
	}
	else
	{
		_Timer_Read_ADC->Start();
	}

	if (_Timer_Read_ADC->Enabled)
	{
		_Button_Read_ADC->Text = "Toggle Periodic ADC Read (Now: Running)";
	}
	else
	{
		_Button_Read_ADC->Text = "Toggle Periodic ADC Read (Now: Stopped)";
	}
}

System::Void MIDI_Lighter::Debug::Timer_Read_ADC_Tick(System::Object^ sender, System::EventArgs^ e)
{
	System::UInt32 ADC_Value = _Device_List->Read_ADC();

	if (ADC_Value >= 0)
	{
		System::Console::WriteLine("Received ADC Value: " + ADC_Value.ToString("00000000") + " = " + ((5000.0f * (float)ADC_Value) / 1024.0f).ToString("000.000") + "mA (" + _Read_ADC_Counter++.ToString("D6") + ")");
	}
	else
	{
		System::Console::WriteLine("Error when reading ADC Value");
	}
}

System::Void MIDI_Lighter::Debug::Button_Measurement_Click(System::Object^ sender, System::EventArgs^ e)
{
	if(_Timer_Measurement->Enabled == false)
	{
		_List_Measurement_Prescaler->Clear();
		_List_Measurement_Prescaler->Add(MIDI_Lighter_Wrapper::Timer4_Presacler::PRESCALER_4);
		_List_Measurement_Prescaler->Add(MIDI_Lighter_Wrapper::Timer4_Presacler::PRESCALER_8);
		_List_Measurement_Prescaler->Add(MIDI_Lighter_Wrapper::Timer4_Presacler::PRESCALER_16);
		_List_Measurement_Prescaler->Add(MIDI_Lighter_Wrapper::Timer4_Presacler::PRESCALER_32);
		_List_Measurement_Prescaler->Add(MIDI_Lighter_Wrapper::Timer4_Presacler::PRESCALER_64);

		_List_Measurement_Top->Clear();
		_List_Measurement_Top->Add(4000);	// 2,0ms
		_List_Measurement_Top->Add(2000);	// 1,0ms
		_List_Measurement_Top->Add(1000);	// 0,5ms
		_List_Measurement_Top->Add(500);	// 0,25ms

		_Measurement_Per_Config = 100;

		_Measurement_Current_Measurement	= 0;
		_Measurement_Current_Prescaler		= 0;	_Device_List->Set_Timer4_Prescaler	(_List_Measurement_Prescaler[_Measurement_Current_Prescaler]);
		_Measurement_Current_Top			= 0;	_Device_List->Set_Timer1_Top		(_List_Measurement_Top[_Measurement_Current_Top]);

	//	System::Console::WriteLine("Setting Prescaler to: " + _List_Measurement_Prescaler[_Measurement_Current_Prescaler].ToString());
	//	System::Console::WriteLine("Setting Top to:       " + _List_Measurement_Top[_Measurement_Current_Top].ToString());
	

		_Measurement_Writer	= gcnew System::IO::StreamWriter("Measurment.txt");

		_Measurement_Writer->Write(	"Timer4 Prescaler" + ";" +
									"Timer1 Top" + ";" +
									"#Measurement" + ";" +
									"ADC_Value" + "\n");
		_Measurement_Writer->Flush();

		_Read_ADC_Counter = 0;

		_Timer_Measurement->Start();
	}
	else
	{
		_Timer_Measurement->Stop();
		_Measurement_Writer->Close();
	}
}

System::Void MIDI_Lighter::Debug::Timer_Measurement_Tick(System::Object^ sender, System::EventArgs^ e)
{
	if (_Measurement_Current_Measurement < _Measurement_Per_Config)
	{
		System::UInt32 ADC_Value = _Device_List->Read_ADC();
		
	
		_Measurement_Writer->Write(	_List_Measurement_Prescaler[_Measurement_Current_Prescaler].ToString()	+ ";" + 
									_List_Measurement_Top[_Measurement_Current_Top].ToString()				+ ";" +
									_Measurement_Current_Measurement.ToString()								+ ";" +
									ADC_Value.ToString()													+ "\n");
		_Measurement_Writer->Flush();


		System::Console::WriteLine(	"Received ADC Value: " + ADC_Value.ToString("00000000") + " = " + ((5000.0f * (float)ADC_Value) / 1024.0f).ToString("000.000") + "mA (" + _Measurement_Current_Measurement.ToString("D6") + ") // " +
									"Prescaler: " + _List_Measurement_Prescaler[_Measurement_Current_Prescaler].ToString() + " | " + 
									"Top: " + _List_Measurement_Top[_Measurement_Current_Top]);
		

		_Measurement_Current_Measurement++;
	}
	else
	{
		_Measurement_Current_Measurement = 0;
		if (_Measurement_Current_Top < _List_Measurement_Top->Count-1)
		{
			_Measurement_Current_Top++;		_Device_List->Set_Timer1_Top(_List_Measurement_Top[_Measurement_Current_Top]);

			_Measurement_Current_Measurement = 0;

//			System::Console::WriteLine("Setting Top to:       " + _List_Measurement_Top[_Measurement_Current_Top]);
		}
		else if(_Measurement_Current_Prescaler < _List_Measurement_Prescaler->Count-1)
		{
			_Measurement_Current_Prescaler++;	_Device_List->Set_Timer4_Prescaler	(_List_Measurement_Prescaler[_Measurement_Current_Prescaler]);
			_Measurement_Current_Top = 0;		_Device_List->Set_Timer1_Top		(_List_Measurement_Top[_Measurement_Current_Top]);

			_Measurement_Current_Measurement = 0;

//			System::Console::WriteLine("Setting Prescaler to: " + _List_Measurement_Prescaler[_Measurement_Current_Prescaler].ToString());
//			System::Console::WriteLine("Setting Top to:       " + _List_Measurement_Top[_Measurement_Current_Top].ToString());
		}
		else
		{
			_Timer_Measurement->Stop();
			_Measurement_Writer->Close();
			System::Console::WriteLine("Measurement finished");
		}
	}
}