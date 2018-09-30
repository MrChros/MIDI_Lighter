#pragma once

namespace MIDI_Lighter
{
	public enum class USB_CONNECTION
	{
		DISCONNECTED,
		CONNECTED,
		LOST,
		FAILED
	};

	public enum class SYNC_STATUS
	{
		SYNCED,
		SYNCING,
		FAILED,
		UNKNOWN
	};
	
	public ref class Status_Bar : public System::Windows::Forms::StatusStrip
	{
	public:
		Status_Bar();
		
		void Update_USB_Connection_Status(USB_CONNECTION status);
		void Update_Sync_Status(SYNC_STATUS status);

	private:
		System::Resources::ResourceManager^		_Resources;

		System::Windows::Forms::ToolStripLabel^ _Label_Connection_Status;
		System::Windows::Forms::ToolStripLabel^ _Label_Connection_Status_Icon;

		System::Windows::Forms::ToolStripLabel^ _Label_Sync_Status;
		System::Windows::Forms::ToolStripLabel^ _Label_Sync_Status_Icon;
	};
}
