#include "Device_Edit.h"

MIDI_Lighter::Device_Edit::Device_Edit()
{
	_Resources = gcnew System::Resources::ResourceManager("MIDI_Lighter.MIDI_Lighter", System::Reflection::Assembly::GetExecutingAssembly());

	System::Windows::Forms::TableLayoutPanel^ Table_Layout_Main = gcnew System::Windows::Forms::TableLayoutPanel();
	Table_Layout_Main->RowCount = 1;
	Table_Layout_Main->ColumnCount = 2;
	Table_Layout_Main->Dock = System::Windows::Forms::DockStyle::Fill;
	Table_Layout_Main->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));		// 0
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));	// 0
	Table_Layout_Main->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 15)));	// 1

		_VScrollBar = gcnew System::Windows::Forms::VScrollBar();
		_VScrollBar->Dock = System::Windows::Forms::DockStyle::Right;
	Table_Layout_Main->Controls->Add(_VScrollBar, 1, 0);

		
		_Table_Layout_Devices = gcnew System::Windows::Forms::TableLayoutPanel();
		_Table_Layout_Devices->RowCount			= 3;
		_Table_Layout_Devices->ColumnCount		= 1;
		_Table_Layout_Devices->Dock				= System::Windows::Forms::DockStyle::Fill;
		_Table_Layout_Devices->CellBorderStyle	= System::Windows::Forms::TableLayoutPanelCellBorderStyle::OutsetDouble;

		_Table_Layout_Devices->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 400)));		// 0
		_Table_Layout_Devices->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 100)));		// 0
		_Table_Layout_Devices->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));	// 0
	Table_Layout_Main->Controls->Add(_Table_Layout_Devices, 0, 0);

	MIDI_Lighter::Device^ Device_Test = gcnew MIDI_Lighter::Device();
	Device_Test->Dock = System::Windows::Forms::DockStyle::Fill;

	_Table_Layout_Devices->Controls->Add(Device_Test, 0, 0);

	this->Controls->Add(Table_Layout_Main);
}
