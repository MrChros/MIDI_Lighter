#pragma once

#include "Status_Bar.h"
#include "MIDI_Lighter_CLI_Wrapper.h"

namespace MIDI_Lighter
{
	public delegate void Connection_Changed						(USB_CONNECTION status);
	public delegate void Sync_Status_Update						(SYNC_STATUS status);

	public delegate void Configuration_MIDI_Update				(MIDI_Lighter_Wrapper::Configuration_MIDI^				configuration_midi);
	public delegate void Configuration_No_Data_Light_Update		(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^		configuration_no_data_light);
	public delegate void Configuration_Permanent_Light_Update	(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^	configuration_permanent_light);
	public delegate void Configuration_Device_Update			(MIDI_Lighter_Wrapper::Device^							device);
	public delegate void Configuration_RGB_Order_Update			(MIDI_Lighter_Wrapper::Configuration_RGB_Order^			configuration_rgb_order);
	public delegate void Configuration_Changed					(System::Boolean										pending);

	
	public ref class Device_List : public System::Windows::Forms::UserControl
	{
	public:
		Device_List();

		event Connection_Changed^							Connection_Changed;
		event Sync_Status_Update^							Sync_Status_Update;

		event Configuration_MIDI_Update^					Configuration_MIDI_Update;
		event Configuration_No_Data_Light_Update^			Configuration_No_Data_Light_Update;
		event Configuration_Permanent_Light_Update^			Configuration_Permanent_Light_Update;
		event Configuration_Device_Update^					Configuration_Device_Update;
		event Configuration_RGB_Order_Update^				Configuration_RGB_Order_Update;
		event Configuration_Changed^						Configuration_Changed;

		System::Void Update_Configuration_MIDI				(MIDI_Lighter_Wrapper::Configuration_MIDI^				configuration_midi);
		System::Void Update_Configuration_No_Data_Light		(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^		configuration_no_data_light);
		System::Void Update_Configuration_Permanent_Light	(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^	configuration_permanent_light);
		System::Void Update_Device							(MIDI_Lighter_Wrapper::Device^							device);
		System::Void Update_Configuration_RGB_Order			(MIDI_Lighter_Wrapper::Configuration_RGB_Order^			configuration_rgb_order);

		System::Void Update_EEPROM();

		/////////////////////
		// Debug Functions //
		/////////////////////
		System::Byte	Read_EEPROM(uint32_t address);
		System::Int16	Read_ADC();

	protected:
		~Device_List() { };

	private:
		MIDI_Lighter_Wrapper^								_MIDI_Lighter;
		System::Resources::ResourceManager^					_Resources;
		

		System::Windows::Forms::Button^						_Button_Refresh;
		System::Windows::Forms::DataGridView^				_DataGrid_Devices;
		System::Windows::Forms::Label^						_Label_Number_Devices;
		
		USB_CONNECTION										_Current_USB_Connection_Status;
		System::Windows::Forms::Timer^						_Timer_Poll_Connection;

		System::Windows::Forms::DataGridViewCellStyle^		_CellStyle_OK;
		System::Windows::Forms::DataGridViewCellStyle^		_CellStyle_Pending;

		System::Collections::Generic::List<System::Boolean>^	_List_Pending;

		System::Void Button_Refresh_Click					(System::Object^ sender, System::EventArgs^ e);
		System::Void DataGrid_Devices_OnSelectionChanged	(System::Object^ sender, System::EventArgs^ e);
		System::Void Label_Number_Devices_Update			();
		System::Void Timer_Poll_Connection_Tick				(System::Object^ sender, System::EventArgs^ e);

		System::Void GUI_USB_Connect_Update					();
		System::Void GUI_USB_Disconnect_Update				();

		System::Void EEPROM_Write_Set_Pending				(System::Boolean pending);
		System::Void Status_Cell_Update						();
	};
}

