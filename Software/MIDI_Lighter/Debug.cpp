#include "Debug.h"

MIDI_Lighter::Debug::Debug(MIDI_Lighter::Device_List^ device_list)
{
	_Device_List	= device_list;

	
	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount = 3;
	Table_Layout_Main->ColumnCount = 1;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));		// 0
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));		// 1
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));		// 2
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));	// 0


		System::Windows::Forms::Button^ Button_Read_EEPROM = gcnew System::Windows::Forms::Button();
		Button_Read_EEPROM->Dock = System::Windows::Forms::DockStyle::Top;
		Button_Read_EEPROM->Text = "Read EEPROM (Debug)";
		Button_Read_EEPROM->UseVisualStyleBackColor = true;
		Button_Read_EEPROM->Click += gcnew System::EventHandler(this, &Debug::Button_Read_EEPROM_Click);
	Table_Layout_Main->Controls->Add(Button_Read_EEPROM, 0, 0);

		_Button_Read_ADC = gcnew System::Windows::Forms::Button();
		_Button_Read_ADC->Dock = System::Windows::Forms::DockStyle::Top;
		_Button_Read_ADC->Text = "Toggle Periodic ADC Read (Now: Stopped)";
		_Button_Read_ADC->UseVisualStyleBackColor = true;
		_Button_Read_ADC->Click += gcnew System::EventHandler(this, &Debug::Button_Read_ADC_Click);
	Table_Layout_Main->Controls->Add(_Button_Read_ADC, 0, 1);

	this->Controls->Add(Table_Layout_Main);


	_Timer_Read_ADC = gcnew System::Windows::Forms::Timer();
	_Timer_Read_ADC->Interval = 500;
	_Timer_Read_ADC->Tick += gcnew System::EventHandler(this, &Debug::Timer_Read_ADC_Tick);

	_Read_ADC_Counter = 0;

	_LogProgress = gcnew LogProgress::LogProgress();
	


	_Background_Worker_Read_EEPROM = gcnew System::ComponentModel::BackgroundWorker();
	_Background_Worker_Read_EEPROM->WorkerReportsProgress		= true;
	_Background_Worker_Read_EEPROM->WorkerSupportsCancellation	= true;
	_Background_Worker_Read_EEPROM->DoWork					   += gcnew System::ComponentModel::DoWorkEventHandler				(this, &Debug::Background_Worker_Read_EEPROM);
	_Background_Worker_Read_EEPROM->ProgressChanged			   += gcnew System::ComponentModel::ProgressChangedEventHandler		(this, &Debug::Background_Worker_ProgressChanged);
	_Background_Worker_Read_EEPROM->RunWorkerCompleted		   += gcnew System::ComponentModel::RunWorkerCompletedEventHandler	(this, &Debug::Background_Worker_RunWorkerCompleted);
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
	System::Int16 ADC_Value = _Device_List->Read_ADC();

	if (ADC_Value >= 0)
	{
		System::Console::WriteLine("Received ADC Value: " + ADC_Value.ToString() + " (" + _Read_ADC_Counter++ + ")");
	}
	else
	{
		System::Console::WriteLine("Error when reading ADC Value");
	}
}