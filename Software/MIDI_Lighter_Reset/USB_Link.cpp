
#include "USB_Link.h"

////////////
// Public //
////////////
HAL::USB_Link::USB_Link()
{
	#ifdef __linux__
		_Thread_Handle = 0;
	#elif _WIN32
		_Thread_Handle = NULL;
	#endif

	_TX_FIFO = NULL;
	_RX_FIFO = NULL;
	_Setup_FIFO = NULL;

	_Number_Of_Endpoints = 0;
	_Endpoints = NULL;
	_Endpoints_Active = NULL;
	_Thread_Running = false;
	_Thread_Device_Valid = false;

	_Read_Poll_Rate = 1;

	_Keep_Alive_Endpoint = { USB_Device::Pipe_Type::Control, USB_Device::Transfer_Direction::Both, 0, false };
	_Keep_Alive_Rate = 50;
	_Keep_Alive_Bytes = NULL;
	_Keep_Alive_Bytes_Size = 0;

	_Receive_Event_Endpoint = { USB_Device::Pipe_Type::Control, USB_Device::Transfer_Direction::Both, 0, false };
	_Receive_Event_Byte = 0;
	_Receive_Event_Filter = false;
	_Receive_Event_Count = 0;
	_Receive_Event_Count_Clear = false;
	_Receive_Event_Count_Cleared = false;
}

HAL::USB_Link::~USB_Link()
{
	Close_Device();
	Clean_Up();
}

unsigned int HAL::USB_Link::Number_of_Devices(std::string guid)
{
	unsigned int Device_Count = 0;

	#ifdef __linux__
		_LibUSB_Handler = new LibUSB_Handler(guid);
		
		while (_LibUSB_Handler->Open_Device(Device_Count) == true)
		{
			_LibUSB_Handler->Close_Device();
			Device_Count++;
		}

		delete _LibUSB_Handler;
		_LibUSB_Handler = NULL;

	#elif _WIN32
		_WinUSB_Handler = new WinUSB_Handler(guid);
		 
		while (_WinUSB_Handler->Open_Device(Device_Count) == true)
		{
			_WinUSB_Handler->Close_Device();
			Device_Count++;
		}

		delete _WinUSB_Handler;
		_WinUSB_Handler = NULL;

	#endif

	return Device_Count;
}

bool HAL::USB_Link::Open_Device(std::string guid, unsigned int buffer_size, unsigned int device_index)
{
	#ifdef __linux__
		if (_Thread_Handle != 0)
		{
			// already initialized / thread running
			if (_Thread_Running == true)
			{
				return true;
			}
		}
	#elif _WIN32
		if (_Thread_Handle != NULL)
		{
			// already initialized / thread running
			if (_Thread_Running == true)
			{
				return true;
			}
		}
	#endif
	
	Clean_Up();

	_GUID = guid;
	_Device_Index = device_index;

	// Get Endpoints
	#ifdef __linux__
		_LibUSB_Handler = new LibUSB_Handler(_GUID);
		if (_LibUSB_Handler->Open_Device(_Device_Index) == true)
		{
			_Number_Of_Endpoints = _LibUSB_Handler->Number_Of_Endpoints();
			_Endpoints = new USB_Device::Endpoint[_Number_Of_Endpoints];
			_Endpoints_Active = new bool[_Number_Of_Endpoints];
			for (unsigned int i = 0; i < _Number_Of_Endpoints; i++)
			{
				_Endpoints_Active[i] = true;
			}
			_LibUSB_Handler->Get_Endpoints(_Endpoints);
			_LibUSB_Handler->Close_Device();
	
			_Number_Of_Endpoints_In = this->Calc_In_Endpoints();
			_Number_Of_Endpoints_Out = this->Calc_Out_Endpoints();
	
			delete _LibUSB_Handler;
			_LibUSB_Handler = NULL;
		}
		else
		{
			delete _LibUSB_Handler;
			_LibUSB_Handler = NULL;
	
			return false;
		}
	#elif _WIN32
		_WinUSB_Handler = new WinUSB_Handler(_GUID);
		if (_WinUSB_Handler->Open_Device(_Device_Index) == true)
		{
			_Number_Of_Endpoints = _WinUSB_Handler->Number_Of_Endpoints();
			_Endpoints = new USB_Device::Endpoint[_Number_Of_Endpoints];
			_Endpoints_Active = new bool[_Number_Of_Endpoints];
			for (unsigned int i = 0; i < _Number_Of_Endpoints; i++)
			{
				_Endpoints_Active[i] = true;
			}
			_WinUSB_Handler->Get_Endpoints(_Endpoints);
			_WinUSB_Handler->Close_Device();
	
			_Number_Of_Endpoints_In = this->Calc_In_Endpoints();
			_Number_Of_Endpoints_Out = this->Calc_Out_Endpoints();
	
			delete _WinUSB_Handler;
			_WinUSB_Handler = NULL;
		}
		else
		{
			delete _WinUSB_Handler;
			_WinUSB_Handler = NULL;
	
			return false;
		}
	#endif
	
	
	// Allocate FIFOs
	_RX_FIFO = new FIFO<uint8_t>*[_Number_Of_Endpoints_In];
	for (unsigned int i = 0; i < _Number_Of_Endpoints_In; i++)
	{
		_RX_FIFO[i] = new FIFO<uint8_t>(buffer_size);
	}

	_TX_FIFO = new FIFO<uint8_t>*[_Number_Of_Endpoints_Out];
	for (unsigned int i = 0; i < _Number_Of_Endpoints_Out; i++)
	{
		_TX_FIFO[i] = new FIFO<uint8_t>(buffer_size);
	}
	_Setup_FIFO = new FIFO<USB_Device::Setup_Packet>(buffer_size);


	_Stop_Thread = false;

	_Thread_Running = false;
	#ifdef __linux__
		int Thread_Creation_Error_Code = pthread_create(&_Thread_Handle, NULL, Thread, this);
		if (Thread_Creation_Error_Code > 0)
		{
			_Thread_Handle = 0;
			return false;
		}
	#elif _WIN32
		_Thread_Handle = CreateThread(NULL, 0, Thread, this, 0, NULL);
		if (_Thread_Handle == NULL)
		{
			return false;
		}
	#endif
	

	while (_Thread_Running == false)
	{
		// Wait for Worker Thread to be started
		HAL::Common::u_Sleep(1);
	}

	// Wait for Worker Thread to get valid Device Handle
	while (_Thread_Device_Valid == false)
	{
		HAL::Common::u_Sleep(1);
		
		// Worker Thread exited
		if (_Thread_Running == false)
		{
			#ifdef __linux__
				_Thread_Handle = 0;
			#elif _WIN32
				_Thread_Handle = NULL;
			#endif

			return false;
		}
	}
	
	return true;
}

void HAL::USB_Link::Close_Device()
{
	if (_Thread_Running == true)
	{
		_Stop_Thread = true;

		while (_Thread_Running == true)
		{
			// Wait for Worker Thread to exit
			HAL::Common::u_Sleep(1);
		}

		#ifdef __linux__
			pthread_join(_Thread_Handle, NULL);
			_Thread_Handle = 0;
		#elif _WIN32
			CloseHandle(_Thread_Handle);
			_Thread_Handle = NULL;
		#endif
		

		Clean_Up();
	}
}

bool HAL::USB_Link::Open()
{
	return _Thread_Running;
}

bool HAL::USB_Link::Closed()
{
	return !_Thread_Running;
}

unsigned int HAL::USB_Link::Number_Of_Endpoints()
{
	if (_Thread_Running == false)
	{
		return _Number_Of_Endpoints;
	}
	else
	{
		return 0;
	}
}

unsigned int HAL::USB_Link::Get_Endpoints(USB_Device::Endpoint* endpoints_buffer)
{
	if (_Thread_Running == false)
	{
		if (endpoints_buffer != NULL)
		{
			for (unsigned int i = 0; i < _Number_Of_Endpoints; i++)
			{
				endpoints_buffer[i] = _Endpoints[i];
			}
		}
		return _Number_Of_Endpoints;
	}
	else
	{
		return 0;
	}
}

HAL::USB_Device::Endpoint HAL::USB_Link::Find_Endpoint(USB_Device::Transfer_Direction direction, USB_Device::Pipe_Type type)
{
	HAL::USB_Device::Endpoint Required_Endpoint;
	Required_Endpoint.Valid = false;

	if (_Thread_Running == true)
	{
		for (unsigned int i = 0; i < _Number_Of_Endpoints; i++)
		{
			if ((_Endpoints[i].Direction == direction) && (_Endpoints[i].Type == type))
			{
				Required_Endpoint = _Endpoints[i];
				break;
			}
		}
	}

	return Required_Endpoint;
}

void HAL::USB_Link::Activate_Endpoint(USB_Device::Endpoint endpoint)
{
	int Index = Endpoint_Index(endpoint);

	if (Index >= 1)
	{
		_Endpoints_Active[Index] = true;
	}
}
void HAL::USB_Link::Deactivate_Endpoint(USB_Device::Endpoint endpoint)
{
	int Index = Endpoint_Index(endpoint);

	if (Index >= 1)
	{
		_Endpoints_Active[Index] = false;
	}
}

bool HAL::USB_Link::Endpoint_Active(USB_Device::Endpoint endpoint)
{
	int Index = Endpoint_Index(endpoint);

	if (Index >= 0)
	{
		return _Endpoints_Active[Index];
	}
	else
	{
		return false;
	}
}

void HAL::USB_Link::Set_Read_Poll_Rate(unsigned int rate)
{
	_Read_Poll_Rate = rate;
}


void HAL::USB_Link::Setup_Keep_Alive(HAL::USB_Device::Endpoint endpoint, unsigned int rate, uint8_t byte)
{
	this->Setup_Keep_Alive(endpoint, rate, &byte, 1);
}

void HAL::USB_Link::Setup_Keep_Alive(HAL::USB_Device::Endpoint endpoint, unsigned int rate, uint8_t* bytes, unsigned int size)
{
	int Index = Endpoint_Index(endpoint);

	if (Index >= 0)
	{
		_Keep_Alive_Endpoint = endpoint;
	}

	_Keep_Alive_Rate = rate;

	if (_Keep_Alive_Bytes != NULL)
	{
		delete [] _Keep_Alive_Bytes;
	}

	_Keep_Alive_Bytes = new uint8_t[size];

	for (unsigned int i = 0; i < size; i++)
	{
		_Keep_Alive_Bytes[i] = bytes[i];
	}

	_Keep_Alive_Bytes_Size = size;
}

void HAL::USB_Link::Setup_Receive_Event(HAL::USB_Device::Endpoint endpoint, uint8_t byte, bool filter)
{
	int Index = Endpoint_Index(endpoint);

	if (Index >= 0)
	{
		_Receive_Event_Endpoint = endpoint;
	}

	_Receive_Event_Byte = byte;
	_Receive_Event_Filter = filter;
}

unsigned int HAL::USB_Link::Receive_Event_Occured()
{
	unsigned int Count = _Receive_Event_Count;

	_Receive_Event_Count_Clear = true;
	while (_Receive_Event_Count_Cleared == false)
	{
		if (!_Thread_Running)
		{
			_Receive_Event_Count = 0;
			break;
		}
		HAL::Common::u_Sleep(1);
	}
	_Receive_Event_Count_Clear = false;

	return Count;
}


bool HAL::USB_Link::Control_Transfer(USB_Device::Setup_Packet setup_packet)
{
	if (_Number_Of_Endpoints > 0)
	{
		HAL::Timer Timer;

		while (_Setup_FIFO->Full() == true)
		{
			if (!_Thread_Running)
			{
				return false;
			}

			if (Timer.Elapsed_ms() >= _Timeout_ms)
			{
				return false;
			}
			HAL::Common::u_Sleep(1);
		}

		_Setup_FIFO->Write(setup_packet);

		return true;
	}
	else
	{
		return false;
	}
}

bool HAL::USB_Link::Control_Transfer(USB_Device::Setup_Packet setup_packet, uint8_t* data)
{
	if (_Number_Of_Endpoints > 0)
	{
		if (setup_packet.Direction == HAL::USB_Device::Transfer_Direction::Host_to_Device)
		{
			bool Result;
			Result = Buffer_Write(_Endpoints[0], data, setup_packet.Length);
			if (Result == true)
			{
				return Control_Transfer(setup_packet);
			}
			else
			{
				return false;
			}
		}
		else if (setup_packet.Direction == HAL::USB_Device::Transfer_Direction::Device_to_Host)
		{
			bool Result;
			Result = Control_Transfer(setup_packet);
			if (Result == true)
			{
				return Buffer_Read(_Endpoints[0], data, setup_packet.Length);
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

unsigned int HAL::USB_Link::Buffer_Fill_Count(USB_Device::Endpoint endpoint)
{
	int Index = Endpoint_FIFO_Index(endpoint);

	if (Index != -1)
	{
		if (endpoint.Direction == HAL::USB_Device::Transfer_Direction::Host_to_Device)
		{
			return _TX_FIFO[Index]->Fill_Count();
		}
		else if ((endpoint.Direction == HAL::USB_Device::Transfer_Direction::Device_to_Host) || (endpoint.Direction == HAL::USB_Device::Transfer_Direction::Both))
		{
			return _RX_FIFO[Index]->Fill_Count();
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

void HAL::USB_Link::Buffer_Clear(USB_Device::Endpoint endpoint)
{
	int Index = Endpoint_FIFO_Index(endpoint);

	if (Index != -1)
	{
		if (endpoint.Direction == HAL::USB_Device::Transfer_Direction::Host_to_Device)
		{
			_TX_FIFO[Index]->Clear_Write();
		}
		else if ((endpoint.Direction == HAL::USB_Device::Transfer_Direction::Device_to_Host) || (endpoint.Direction == HAL::USB_Device::Transfer_Direction::Both))
		{
			_RX_FIFO[Index]->Clear_Read();
		}
	}
}

bool HAL::USB_Link::Buffer_Write(USB_Device::Endpoint endpoint, uint8_t byte)
{
	return Buffer_Write(endpoint, &byte, 1);
}

bool HAL::USB_Link::Buffer_Write(USB_Device::Endpoint endpoint, uint8_t* bytes, unsigned int size)
{
	int Index = Endpoint_FIFO_Index(endpoint);
	
	if ((Index != -1) && ((endpoint.Direction == HAL::USB_Device::Transfer_Direction::Host_to_Device) || (endpoint.Direction == HAL::USB_Device::Transfer_Direction::Both)))
	{
		HAL::Timer Timer;

		for (unsigned int i = 0; i < size; i++)
		{
			while (_TX_FIFO[Index]->Full() == true)
			{
				if (!_Thread_Running)
				{
					return false;
				}

				if (Timer.Elapsed_ms() >= _Timeout_ms)
				{
					return false;
				}
				HAL::Common::u_Sleep(1);
			}
			_TX_FIFO[Index]->Write(bytes[i]);
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool HAL::USB_Link::Buffer_Read(USB_Device::Endpoint endpoint, uint8_t* byte)
{
	return Buffer_Read(endpoint, byte, 1);
}

bool HAL::USB_Link::Buffer_Read(USB_Device::Endpoint endpoint, uint8_t* bytes, unsigned int size)
{
	int Index = Endpoint_FIFO_Index(endpoint);

	if ((Index != -1) && ((endpoint.Direction == HAL::USB_Device::Transfer_Direction::Device_to_Host) || (endpoint.Direction == HAL::USB_Device::Transfer_Direction::Both)))
	{
		HAL::Timer Timer;

		while (_RX_FIFO[Index]->Fill_Count() < size)
		{
			if (!_Thread_Running)
			{
				return false;
			}

			if (Timer.Elapsed_ms() >= _Timeout_ms)
			{
				return false;
			}
			HAL::Common::u_Sleep(1);
		}

		for (unsigned int i = 0; i < size; i++)
		{
			bytes[i] = _RX_FIFO[Index]->Read();
		}
		return true;
	}
	else
	{
		return false;
	}
}


/////////////
// Private //
/////////////
#ifdef __linux__
	void* HAL::USB_Link::Thread(void* arguments)
#elif _WIN32
	DWORD WINAPI HAL::USB_Link::Thread(LPVOID lpParam)
#endif
{
	const unsigned int Thread_Packet_Size = 4096;
	const unsigned int Thread_Sleep_Time = 5;

	HAL::USB_Link* USB_Link_Instance;

	#ifdef __linux__
		USB_Link_Instance = (HAL::USB_Link*)arguments;
		USB_Link_Instance->_Thread_Running = true;

		USB_Link_Instance->_LibUSB_Handler = new LibUSB_Handler(USB_Link_Instance->_GUID);

		if (USB_Link_Instance->_LibUSB_Handler->Open_Device(USB_Link_Instance->_Device_Index) == false)
		{
			delete USB_Link_Instance->_LibUSB_Handler;
			USB_Link_Instance->_LibUSB_Handler = NULL;
			USB_Link_Instance->_Thread_Running = false;
	
			return 0;
		}
	#elif _WIN32
		USB_Link_Instance = (HAL::USB_Link*)lpParam;
		USB_Link_Instance->_Thread_Running = true;

		USB_Link_Instance->_WinUSB_Handler = new WinUSB_Handler(USB_Link_Instance->_GUID);
	
		if (USB_Link_Instance->_WinUSB_Handler->Open_Device(USB_Link_Instance->_Device_Index) == false)
		{
			delete USB_Link_Instance->_WinUSB_Handler;
			USB_Link_Instance->_WinUSB_Handler = NULL;
			USB_Link_Instance->_Thread_Running = false;
	
			return false;
		}
	#endif
	

	// Initialize
	for (unsigned int i = 0; i < USB_Link_Instance->_Number_Of_Endpoints; i++)
	{
		#ifdef __linux__
			USB_Link_Instance->_LibUSB_Handler->Clear(USB_Link_Instance->_Endpoints[i]);
		#elif _WIN32
			USB_Link_Instance->_WinUSB_Handler->Clear(USB_Link_Instance->_Endpoints[i]);
		#endif
	}
	unsigned int Read_Poll_Timer = 0;
	unsigned int Keep_Alive_Timer = 0;

	uint8_t Write_Buffer[Thread_Packet_Size];
	uint8_t Read_Buffer[Thread_Packet_Size];


	// Start
	USB_Link_Instance->_Thread_Device_Valid = true;
	bool Error = false;

	while (1)
	{
		for (unsigned int i = 0; i < USB_Link_Instance->_Number_Of_Endpoints; i++)
		{
			// Endpoint 0
			if (i == 0)
			{
				if (USB_Link_Instance->_Setup_FIFO->Fill_Count() > 0)
				{
					HAL::USB_Device::Setup_Packet Setup_Packet = USB_Link_Instance->_Setup_FIFO->Preview();

					if (Setup_Packet.Direction == HAL::USB_Device::Transfer_Direction::Host_to_Device)
					{
						if (USB_Link_Instance->_TX_FIFO[0]->Fill_Count() >= Setup_Packet.Length)
						{
							USB_Link_Instance->_Setup_FIFO->Read();
							USB_Link_Instance->Buffer_Read_Thread(USB_Link_Instance->_Endpoints[0], Write_Buffer, Setup_Packet.Length);
							#ifdef __linux__
								bool Result = USB_Link_Instance->_LibUSB_Handler->Control_Transfer(Setup_Packet, Write_Buffer);
							#elif _WIN32
								bool Result = USB_Link_Instance->_WinUSB_Handler->Control_Transfer(Setup_Packet, Write_Buffer);
							#endif

							if (!Result)
							{
								// Something went wrong
								Error = true;
								break;
							}
						}
					}
					else if (Setup_Packet.Direction == HAL::USB_Device::Transfer_Direction::Device_to_Host)
					{
						USB_Link_Instance->_Setup_FIFO->Read();
						#ifdef __linux__
							bool Result = USB_Link_Instance->_LibUSB_Handler->Control_Transfer(Setup_Packet, Read_Buffer);
						#elif _WIN32
							bool Result = USB_Link_Instance->_WinUSB_Handler->Control_Transfer(Setup_Packet, Read_Buffer);
						#endif
						

						if (Result)
						{
							// Put Bytes into FIFO
							if (USB_Link_Instance->Buffer_Write_Thread(USB_Link_Instance->_Endpoints[0], Read_Buffer, Setup_Packet.Length) == false)
							{
								// Timeout
								Error = true;
								break;
							}
						}
					}
				}
			}
			// Non-Endpoint 0 Transfer
			else if ((USB_Link_Instance->_Endpoints[i].Direction == USB_Device::Transfer_Direction::Host_to_Device) && (USB_Link_Instance->_Endpoints_Active[i] == true))
			{
				///////////
				// Write //
				///////////
				int Index = USB_Link_Instance->Endpoint_FIFO_Index(USB_Link_Instance->_Endpoints[i]);
				unsigned int TX_Fill_Count = USB_Link_Instance->_TX_FIFO[Index]->Fill_Count();
				if (TX_Fill_Count > 0)
				{
					bool Successful_Sent = false;

					if (TX_Fill_Count > Thread_Packet_Size)
					{
						// Pull Data from FIFO
						USB_Link_Instance->Buffer_Read_Thread(USB_Link_Instance->_Endpoints[i], Write_Buffer, Thread_Packet_Size);
						// Send Out
						#ifdef __linux__
							Successful_Sent = USB_Link_Instance->_LibUSB_Handler->Send(USB_Link_Instance->_Endpoints[i], Write_Buffer, Thread_Packet_Size);
						#elif _WIN32
							Successful_Sent = USB_Link_Instance->_WinUSB_Handler->Send(USB_Link_Instance->_Endpoints[i], Write_Buffer, Thread_Packet_Size);
						#endif
					}
					else
					{
						// Pull Data from FIFO
						USB_Link_Instance->Buffer_Read_Thread(USB_Link_Instance->_Endpoints[i], Write_Buffer, TX_Fill_Count);
						// Send Out
						#ifdef __linux__
							Successful_Sent = USB_Link_Instance->_LibUSB_Handler->Send(USB_Link_Instance->_Endpoints[i], Write_Buffer, TX_Fill_Count);
						#elif _WIN32
							Successful_Sent = USB_Link_Instance->_WinUSB_Handler->Send(USB_Link_Instance->_Endpoints[i], Write_Buffer, TX_Fill_Count);
						#endif
					}

					if (!Successful_Sent)
					{
						// Something went wrong
						Error = true;
						break;
					}

					if (	(USB_Link_Instance->_Endpoints[i].Direction	== USB_Link_Instance->_Keep_Alive_Endpoint.Direction)	&&
							(USB_Link_Instance->_Endpoints[i].Pipe_ID	== USB_Link_Instance->_Keep_Alive_Endpoint.Pipe_ID)		&&
							(USB_Link_Instance->_Endpoints[i].Type		== USB_Link_Instance->_Keep_Alive_Endpoint.Type)		&&
							(USB_Link_Instance->_Endpoints[i].Valid		== USB_Link_Instance->_Keep_Alive_Endpoint.Valid)		)
					{
						Keep_Alive_Timer = 0;
					}
				}
			}
			else if ((USB_Link_Instance->_Endpoints[i].Direction == USB_Device::Transfer_Direction::Device_to_Host) && (USB_Link_Instance->_Endpoints_Active[i] == true))
			{
				//////////
				// Read //
				//////////
				unsigned int Bytes_Read = 0;
				bool Successful_Read = true;
				if (USB_Link_Instance->_Read_Poll_Rate > 0)
				{
					if (Read_Poll_Timer >= USB_Link_Instance->_Read_Poll_Rate - 1)
					{
						#ifdef __linux__
							Successful_Read = USB_Link_Instance->_LibUSB_Handler->Receive(USB_Link_Instance->_Endpoints[i], Read_Buffer, Thread_Packet_Size, &Bytes_Read);
						#elif _WIN32
							Successful_Read = USB_Link_Instance->_WinUSB_Handler->Receive(USB_Link_Instance->_Endpoints[i], Read_Buffer, Thread_Packet_Size, &Bytes_Read);
						#endif
					}
				}


				if (!Successful_Read)
				{
					// Something went wrong
					Error = true;
					break;
				}

				if (Bytes_Read > 0)
				{
					// Put Bytes into FIFO
					if (USB_Link_Instance->Buffer_Write_Thread(USB_Link_Instance->_Endpoints[i], Read_Buffer, Bytes_Read) == false)
					{
						//timeout
						Error = true;
						break;
					}
				}
			}
		}

		if (Read_Poll_Timer >= USB_Link_Instance->_Read_Poll_Rate - 1)
		{
			Read_Poll_Timer = 0;
		}
		else
		{
			Read_Poll_Timer++;
		}

		////////////////
		// Keep Alive //
		////////////////
		if ((USB_Link_Instance->_Keep_Alive_Rate > 0) && (USB_Link_Instance->_Keep_Alive_Bytes != NULL) && (USB_Link_Instance->_Endpoints_Active[USB_Link_Instance->Endpoint_Index(USB_Link_Instance->_Keep_Alive_Endpoint)] == true) && (USB_Link_Instance->_Keep_Alive_Endpoint.Valid == true))
		{
			if (Keep_Alive_Timer == USB_Link_Instance->_Keep_Alive_Rate - 1)
			{
				if (USB_Link_Instance->_Keep_Alive_Bytes_Size > 0)
				{
					#ifdef __linux__
						bool Successful_Sent = USB_Link_Instance->_LibUSB_Handler->Send(USB_Link_Instance->_Keep_Alive_Endpoint, USB_Link_Instance->_Keep_Alive_Bytes, USB_Link_Instance->_Keep_Alive_Bytes_Size);
					#elif _WIN32
						bool Successful_Sent = USB_Link_Instance->_WinUSB_Handler->Send(USB_Link_Instance->_Keep_Alive_Endpoint, USB_Link_Instance->_Keep_Alive_Bytes, USB_Link_Instance->_Keep_Alive_Bytes_Size);
					#endif
					

					if (!Successful_Sent)
					{
						// Something went wrong
						Error = true;
						break;
					}
				}

				Keep_Alive_Timer = 0;
			}
			else
			{
				Keep_Alive_Timer++;
			}
		}


		/////////////////
		// Event Count //
		/////////////////
		if (USB_Link_Instance->_Receive_Event_Count_Clear == true)
		{
			USB_Link_Instance->_Receive_Event_Count = 0;
			USB_Link_Instance->_Receive_Event_Count_Cleared = true;
			while (USB_Link_Instance->_Receive_Event_Count_Clear == true)
			{
				HAL::Common::u_Sleep(1);
			}
			USB_Link_Instance->_Receive_Event_Count_Cleared = false;
		}


		/////////////////
		// Stop Thread //
		/////////////////
		if (USB_Link_Instance->_Stop_Thread == true)
		{
			bool Any_FIFO_Not_Empty = false;
			for (unsigned int i = 0; i < USB_Link_Instance->_Number_Of_Endpoints_Out; i++)
			{
				if (USB_Link_Instance->_TX_FIFO[i]->Fill_Count() != 0)
				{
					Any_FIFO_Not_Empty = true;
					break;
				}
			}

			if (Any_FIFO_Not_Empty == false)
			{
				break;
			}
		}

		if (Error == true)
		{
			break;
		}
		else
		{
			HAL::Common::u_Sleep(Thread_Sleep_Time);
		}
	}

	// DO FINAL STUFF
	#ifdef __linux__
		USB_Link_Instance->_LibUSB_Handler->Close_Device();
		delete USB_Link_Instance->_LibUSB_Handler;
		USB_Link_Instance->_LibUSB_Handler = NULL;
	#elif _WIN32
		USB_Link_Instance->_WinUSB_Handler->Close_Device();
		delete USB_Link_Instance->_WinUSB_Handler;
		USB_Link_Instance->_WinUSB_Handler = NULL;
	#endif

	USB_Link_Instance->_Thread_Running = false;
	USB_Link_Instance->_Thread_Device_Valid = false;

	return 0;
}


bool HAL::USB_Link::Buffer_Read_Thread(USB_Device::Endpoint endpoint, uint8_t* byte)
{
	return Buffer_Read_Thread(endpoint, byte, 1);
}

bool HAL::USB_Link::Buffer_Read_Thread(USB_Device::Endpoint endpoint, uint8_t* bytes, unsigned int size)
{
	int Index = Endpoint_FIFO_Index(endpoint);

	if ((Index != -1) && ((endpoint.Direction == HAL::USB_Device::Transfer_Direction::Host_to_Device) || (endpoint.Direction == HAL::USB_Device::Transfer_Direction::Both)))
	{
		if (_TX_FIFO[Index]->Fill_Count() < size)
		{
			return false;
		}
		else
		{
			for (unsigned int i = 0; i < size; i++)
			{
				bytes[i] = _TX_FIFO[Index]->Read();
			}
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool HAL::USB_Link::Buffer_Write_Thread(USB_Device::Endpoint endpoint, uint8_t byte)
{
	return Buffer_Write_Thread(endpoint, &byte, 1);
}

bool HAL::USB_Link::Buffer_Write_Thread(USB_Device::Endpoint endpoint, uint8_t* bytes, unsigned int size)
{
	int Index = Endpoint_FIFO_Index(endpoint);

	if ((Index != -1) && ((endpoint.Direction == HAL::USB_Device::Transfer_Direction::Device_to_Host) || (endpoint.Direction == HAL::USB_Device::Transfer_Direction::Both)))
	{
		HAL::Timer Timer;

		for (unsigned int i = 0; i < size; i++)
		{
			if ((endpoint.Type == _Receive_Event_Endpoint.Type) && (endpoint.Direction == _Receive_Event_Endpoint.Direction) && (endpoint.Pipe_ID == _Receive_Event_Endpoint.Pipe_ID) && (endpoint.Valid == _Receive_Event_Endpoint.Valid))
			{
				if (_Receive_Event_Byte == bytes[i])
				{
					_Receive_Event_Count++;
					
					if (_Receive_Event_Filter == false)
					{
						while (_RX_FIFO[Index]->Full() == true)
						{
							if (Timer.Elapsed_ms() >= _Timeout_ms)
							{
								return false;
							}
						}
						_RX_FIFO[Index]->Write(bytes[i]);
					}
				}
				else
				{
					while (_RX_FIFO[Index]->Full() == true)
					{
						if (Timer.Elapsed_ms() >= _Timeout_ms)
						{
							return false;
						}
					}
					_RX_FIFO[Index]->Write(bytes[i]);
				}
			}
			else
			{
				while (_RX_FIFO[Index]->Full() == true)
				{
					if (Timer.Elapsed_ms() >= _Timeout_ms)
					{
						return false;
					}
				}
				_RX_FIFO[Index]->Write(bytes[i]);
			}
		}

		return true;
	}
	else
	{
		return false;
	}	
}





/////////////
// Support //
/////////////
int HAL::USB_Link::Endpoint_Index(USB_Device::Endpoint endpoint)
{
	unsigned int Count = 0;
	for (unsigned int i = 0; i < _Number_Of_Endpoints; i++)
	{
		if (	(_Endpoints[i].Direction	== endpoint.Direction)	&&
				(_Endpoints[i].Pipe_ID		== endpoint.Pipe_ID)	&&
				(_Endpoints[i].Type			== endpoint.Type)		&&
				(_Endpoints[i].Valid		== endpoint.Valid)		)
		{
			return Count;
		}
		else
		{
			Count++;
		}
	}
	return -1;
}

int HAL::USB_Link::Endpoint_FIFO_Index(USB_Device::Endpoint endpoint)
{
	unsigned int Count = 0;
	for (unsigned int i = 0; i < _Number_Of_Endpoints; i++)
	{
		if ((_Endpoints[i].Direction == endpoint.Direction) || (_Endpoints[i].Direction == USB_Device::Transfer_Direction::Both))
		{
			if (	(_Endpoints[i].Direction	== endpoint.Direction)	&&
					(_Endpoints[i].Pipe_ID		== endpoint.Pipe_ID)	&&
					(_Endpoints[i].Type			== endpoint.Type)		&&
					(_Endpoints[i].Valid		== endpoint.Valid)		)
			{
				return Count;
			}
			else
			{
				Count++;
			}
		}
	}
	return -1;
}

unsigned int HAL::USB_Link::Calc_In_Endpoints()
{
	unsigned int Count = 0;

	for (unsigned int i = 0; i < _Number_Of_Endpoints; i++)
	{
		if (	(_Endpoints[i].Direction == USB_Device::Transfer_Direction::Device_to_Host)		||
				(_Endpoints[i].Direction == USB_Device::Transfer_Direction::Both)				)
		{
			Count++;
		}
	}

	return Count;
}

unsigned int HAL::USB_Link::Calc_Out_Endpoints()
{
	unsigned int Count = 0;

	for (unsigned int i = 0; i < _Number_Of_Endpoints; i++)
	{
		if (	(_Endpoints[i].Direction == USB_Device::Transfer_Direction::Host_to_Device)		||
				(_Endpoints[i].Direction == USB_Device::Transfer_Direction::Both)				)
		{
			Count++;
		}
	}

	return Count;
}

void HAL::USB_Link::Clean_Up()
{
	if (_Endpoints != NULL)
	{
		delete [] _Endpoints;
		_Endpoints = NULL;
	}

	if (_Endpoints_Active != NULL)
	{
		delete [] _Endpoints_Active;
		_Endpoints_Active = NULL;
	}

	if (_TX_FIFO != NULL)
	{
		for (unsigned int i = 0; i < _Number_Of_Endpoints_Out; i++)
		{
			delete _TX_FIFO[i];
		}
		delete [] _TX_FIFO; 
		_TX_FIFO = NULL;
	}

	if (_RX_FIFO != NULL)
	{
		for (unsigned int i = 0; i < _Number_Of_Endpoints_In; i++)
		{
			delete _RX_FIFO[i];
		}
		delete [] _RX_FIFO;
		_RX_FIFO = NULL;
	}

	if (_Setup_FIFO != NULL)
	{
		delete _Setup_FIFO;
		_Setup_FIFO = NULL;
	}
}