#include "LogProgress.h"

LogProgress::LogProgress::LogProgress()
{
	this->Size			= System::Drawing::Size(150, 80);
	this->MaximumSize	= this->Size;
	this->MinimumSize	= this->Size;
	this->Text			= "";
	
		_Progress_Bar = gcnew LogProgressBar();
		_Progress_Bar->Location = System::Drawing::Point(5, 10);
		_Progress_Bar->Size = System::Drawing::Size(this->Width - 27, 21);
	this->Controls->Add(_Progress_Bar);

	this->Set_Progress_Maximum(1);

	this->MaximizeBox	= false;
	this->MinimizeBox	= false;
	this->ControlBox	= false;
	this->ShowInTaskbar = false;
}

void LogProgress::LogProgress::Set_Title(System::String^ title)
{
	this->Text = title;
}

void LogProgress::LogProgress::Set_Progress_Maximum(unsigned int maximum)
{
	if (maximum > 0)
	{
		_Progress_Bar->Maximum = maximum;
	}
}

void LogProgress::LogProgress::Set_Progress_Value(unsigned int value)
{
	if (value > (unsigned int)_Progress_Bar->Maximum)
	{
		value = _Progress_Bar->Maximum;
	}
	
	_Progress_Bar->Value = value;
}