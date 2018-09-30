#include "Debug.h"

MIDI_Lighter::Debug::Debug(LogProgress::LogProgress^ logprogress, MIDI_Lighter::Device_List^ device_list)
{
	_LogProgress	= logprogress;
	_Device_List	= device_list;

	_Background_Worker_Read_EEPROM = gcnew System::ComponentModel::BackgroundWorker();
	_Background_Worker_Read_EEPROM->WorkerReportsProgress		= true;
	_Background_Worker_Read_EEPROM->WorkerSupportsCancellation	= true;
	_Background_Worker_Read_EEPROM->DoWork					   += gcnew System::ComponentModel::DoWorkEventHandler				(this, &Debug::Background_Worker_Read_EEPROM);
	_Background_Worker_Read_EEPROM->ProgressChanged			   += gcnew System::ComponentModel::ProgressChangedEventHandler		(this, &Debug::Background_Worker_ProgressChanged);
	_Background_Worker_Read_EEPROM->RunWorkerCompleted		   += gcnew System::ComponentModel::RunWorkerCompletedEventHandler	(this, &Debug::Background_Worker_RunWorkerCompleted);
}

System::ComponentModel::BackgroundWorker^ MIDI_Lighter::Debug::Background_Worker_Read_EEPROM()
{
	return _Background_Worker_Read_EEPROM;
}

System::Void MIDI_Lighter::Debug::Background_Worker_ProgressChanged(System::Object^ sender, System::ComponentModel::ProgressChangedEventArgs^ e)
{
	_LogProgress->Set_Progress_Value(e->ProgressPercentage);
}

System::Void MIDI_Lighter::Debug::Background_Worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e)
{
	if (e->Cancelled == true)
	{
		_LogProgress->Hide();
	}
	else if (!(e->Error == nullptr))
	{
		//this.tbProgress.Text = ("Error: " + e->Error.Message);
	}
	else
	{
		System::Threading::Thread::Sleep(100);
		_LogProgress->Hide();
	}
}

System::Void MIDI_Lighter::Debug::Background_Worker_Read_EEPROM(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
{
	////////////////////
	// Read Parameter //
	////////////////////
	array<System::String^>^ Parameter = (array<System::String^>^)(e->Argument);

	System::String^ Selected_File = Parameter[0];

	const unsigned int EEPROM_START_ADDRESS = 0;
	const unsigned int EEPROM_END_ADDRESS = 0x3FF;

	unsigned int Line_Break_Bytes = 0;
	Line_Break_Bytes = 4;
	Line_Break_Bytes = 8;
	Line_Break_Bytes = 16;
	Line_Break_Bytes = 32;
	//		Line_Break_Bytes = 64;

	System::String^ Hex_Format = "X";
	array<System::Byte>^ EEPROM_Data = gcnew array<System::Byte>(64);

	System::IO::StreamWriter^ Stream_Writer = gcnew System::IO::StreamWriter(Selected_File);

	for (uint32_t i = EEPROM_START_ADDRESS; i <= EEPROM_END_ADDRESS; i++)
	{
		if (_Background_Worker_Read_EEPROM->CancellationPending == true)
		{
			break;
		}

		_Background_Worker_Read_EEPROM->ReportProgress(i);

		if ((i % Line_Break_Bytes) == 0)
		{
			Stream_Writer->Write("0x" + i.ToString(Hex_Format + "8") + ": ");

			for (uint32_t j = 0; j < Line_Break_Bytes && (i + j) <= EEPROM_END_ADDRESS; j++)
			{
				EEPROM_Data[j] = _Device_List->Read_EEPROM(i + j);
			}

			for (uint32_t j = 0; j < Line_Break_Bytes; j++)
			{
				Stream_Writer->Write((EEPROM_Data[j]).ToString(Hex_Format + "2"));

				if (j > 0 && (j + 1) % 32 == 0)
				{
					if (Line_Break_Bytes == 64) { Stream_Writer->Write("   "); }
					else { Stream_Writer->Write("  "); }
				}
				else if (j > 0 && (j + 1) % 16 == 0)
				{
					if (Line_Break_Bytes == 32) { Stream_Writer->Write("   "); }
					else { Stream_Writer->Write("  "); }
				}
				else if (j > 0 && (j + 1) % 8 == 0)
				{
					if (Line_Break_Bytes == 16) { Stream_Writer->Write("   "); }
					else { Stream_Writer->Write("  "); }
				}
				else if (j > 0 && (j + 1) % 4 == 0)
				{
					if (Line_Break_Bytes == 8) { Stream_Writer->Write("   "); }
					else { Stream_Writer->Write("  "); }
				}

			}

			Stream_Writer->Write(" // ");

			for (uint32_t j = 0; j < Line_Break_Bytes; j++)
			{
				if (EEPROM_Data[j] >= '!' && EEPROM_Data[j] <= '~')
				{
					Stream_Writer->Write(EEPROM_Data[j]);
				}
				else
				{
					Stream_Writer->Write(".");
				}
			}

			Stream_Writer->Write("\n");
		}
		System::Threading::Thread::Sleep(1);
	}

	Stream_Writer->Close();

	if (_Background_Worker_Read_EEPROM->CancellationPending == true)
	{
		e->Cancel = true;
	}
}