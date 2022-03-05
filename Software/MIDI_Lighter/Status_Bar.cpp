#include "Status_Bar.h"


MIDI_Lighter::Status_Bar::Status_Bar()
{
	_Resources = gcnew System::Resources::ResourceManager("MIDI_Lighter.MIDI_Lighter", System::Reflection::Assembly::GetExecutingAssembly());

	_Label_Connection_Status = gcnew System::Windows::Forms::ToolStripLabel;
	System::Drawing::Font^ Label_Font = gcnew System::Drawing::Font(_Label_Connection_Status->Font, System::Drawing::FontStyle::Bold);
	_Label_Connection_Status->Font = Label_Font;

	_Label_Connection_Status_Icon = gcnew System::Windows::Forms::ToolStripLabel;

	this->Items->Add(gcnew System::Windows::Forms::ToolStripLabel("USB Connection Status:"));
	this->Items->Add(_Label_Connection_Status);
	this->Items->Add(_Label_Connection_Status_Icon);

	System::Windows::Forms::ToolStripLabel^ Label_Spacer = gcnew System::Windows::Forms::ToolStripLabel();
	Label_Spacer->Text = "            ";
	this->Items->Add(Label_Spacer);

	_Label_Sync_Status = gcnew System::Windows::Forms::ToolStripLabel;
	_Label_Sync_Status->Font = Label_Font;

	_Label_Sync_Status_Icon = gcnew System::Windows::Forms::ToolStripLabel;

	this->Items->Add(gcnew System::Windows::Forms::ToolStripLabel("Sync Status:"));
	this->Items->Add(_Label_Sync_Status);
	this->Items->Add(_Label_Sync_Status_Icon);


	Update_USB_Connection_Status(USB_CONNECTION::DISCONNECTED);
	Update_Sync_Status(SYNC_STATUS::UNKNOWN);
}

void MIDI_Lighter::Status_Bar::Update_USB_Connection_Status(USB_CONNECTION status)
{
	switch(status)
	{
	case USB_CONNECTION::CONNECTED:
		_Label_Connection_Status->ForeColor = System::Drawing::Color::FromArgb(0xFF00BB00);
		_Label_Connection_Status->Text = "Connected";
		_Label_Connection_Status_Icon->Image = (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"connect")));
		break;

	case USB_CONNECTION::DISCONNECTED:
		_Label_Connection_Status->ForeColor = System::Drawing::Color::FromArgb(0xFFFF0000);
		_Label_Connection_Status->Text = "Disconnected";
		_Label_Connection_Status_Icon->Image = (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"disconnect")));
		break;

	case USB_CONNECTION::FAILED:
		_Label_Connection_Status->ForeColor = System::Drawing::Color::FromArgb(0xFFFF0000);
		_Label_Connection_Status->Text = "Connection Failed";
		_Label_Connection_Status_Icon->Image = (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"disconnect")));
		break;

	case USB_CONNECTION::LOST:
		_Label_Connection_Status->ForeColor = System::Drawing::Color::FromArgb(0xFFFF0000);
		_Label_Connection_Status->Text = "Connection Lost";
		_Label_Connection_Status_Icon->Image = (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"disconnect")));
		break;

	default:
		break;
	}
}

void MIDI_Lighter::Status_Bar::Update_Sync_Status(SYNC_STATUS status)
{
	switch (status)
	{
	case SYNC_STATUS::SYNCED:
		_Label_Sync_Status->ForeColor = System::Drawing::Color::FromArgb(0xFF00BB00);
		_Label_Sync_Status->Text = "In sync";
		_Label_Sync_Status_Icon->Image = (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"tick")));
		break;

	case SYNC_STATUS::SYNCING:
		_Label_Sync_Status->ForeColor = System::Drawing::Color::FromArgb(0xFF0000FF);
		_Label_Sync_Status->Text = "Syncing...";
		_Label_Sync_Status_Icon->Image = (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"arrow_refresh")));
		break;

	case SYNC_STATUS::FAILED:
		_Label_Sync_Status->ForeColor = System::Drawing::Color::FromArgb(0xFFFF0000);
		_Label_Sync_Status->Text = "Failed";
		_Label_Sync_Status_Icon->Image = (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"cross")));
		break;

	case SYNC_STATUS::UNKNOWN:
		_Label_Sync_Status->ForeColor = System::Drawing::Color::FromArgb(0xFF000000);
		_Label_Sync_Status->Text = "Unknown";
		_Label_Sync_Status_Icon->Image = (cli::safe_cast<System::Drawing::Image^>(_Resources->GetObject(L"help")));
		break;

	default:
		break;
	}
}