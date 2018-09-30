#ifndef USB_LINK_H_
#define USB_LINK_H_

#include <stdint.h>
#include <string>

#include "Common.h"
#include "FIFO.hpp"
#include "Timer.h"

#include "USB_Device.h"

#ifdef __linux__
	#include "LibUSB_Handler.h"
	#include <pthread.h>
#elif _WIN32
	#include "WinUSB_Handler.h"
#endif

namespace HAL
{
	class USB_Link
	{
		public:
			USB_Link();
			~USB_Link();

			unsigned int Number_of_Devices(std::string guid);

			bool Open_Device(std::string guid, unsigned int buffer_size, unsigned int device_index = 0);
			void Close_Device();

			bool Open();
			bool Closed();

			unsigned int Number_Of_Endpoints();
			unsigned int Get_Endpoints(HAL::USB_Device::Endpoint* endpoints_buffer);
			HAL::USB_Device::Endpoint Find_Endpoint(HAL::USB_Device::Transfer_Direction direction, HAL::USB_Device::Pipe_Type type);

			void Activate_Endpoint(HAL::USB_Device::Endpoint endpoint);
			void Deactivate_Endpoint(HAL::USB_Device::Endpoint endpoint);
			bool Endpoint_Active(HAL::USB_Device::Endpoint endpoint);

			void Set_Read_Poll_Rate(unsigned int rate);

			void Setup_Keep_Alive(HAL::USB_Device::Endpoint endpoint, unsigned int rate, uint8_t byte);
			void Setup_Keep_Alive(HAL::USB_Device::Endpoint endpoint, unsigned int rate, uint8_t* bytes, unsigned int size);

			void Setup_Receive_Event(HAL::USB_Device::Endpoint endpoint, uint8_t byte, bool filter);
			unsigned int Receive_Event_Occured();

			bool Control_Transfer(HAL::USB_Device::Setup_Packet setup_packet);
			bool Control_Transfer(HAL::USB_Device::Setup_Packet setup_packet, uint8_t* data);

			unsigned int Buffer_Fill_Count(HAL::USB_Device::Endpoint endpoint);
			void Buffer_Clear(HAL::USB_Device::Endpoint endpoint);

			bool Buffer_Write(HAL::USB_Device::Endpoint endpoint, uint8_t byte);
			bool Buffer_Write(HAL::USB_Device::Endpoint endpoint, uint8_t* bytes, unsigned int size);
			bool Buffer_Read(HAL::USB_Device::Endpoint endpoint, uint8_t* byte);
			bool Buffer_Read(HAL::USB_Device::Endpoint endpoint, uint8_t* bytes, unsigned int size);

		private:
			std::string _GUID;
			unsigned int _Device_Index;

			////////////
			// Thread //
			////////////
			#ifdef __linux__
				LibUSB_Handler* _LibUSB_Handler;
				pthread_t _Thread_Handle;
				static void* Thread(void* arguments);
			#elif _WIN32
				WinUSB_Handler* _WinUSB_Handler;
				HANDLE _Thread_Handle;
				static DWORD WINAPI Thread(LPVOID lpParam);
			#endif
			

			unsigned int _Number_Of_Endpoints;
			unsigned int _Number_Of_Endpoints_In;
			unsigned int _Number_Of_Endpoints_Out;
			HAL::USB_Device::Endpoint* _Endpoints;
			bool* _Endpoints_Active;

			bool _Thread_Running;
			bool _Thread_Device_Valid;
			bool _Stop_Thread;

			unsigned int _Read_Poll_Rate;

			HAL::USB_Device::Endpoint _Keep_Alive_Endpoint;
			unsigned int _Keep_Alive_Rate;
			uint8_t* _Keep_Alive_Bytes;
			unsigned int _Keep_Alive_Bytes_Size;

			HAL::USB_Device::Endpoint _Receive_Event_Endpoint;
			uint8_t _Receive_Event_Byte;
			bool _Receive_Event_Filter;
			unsigned int _Receive_Event_Count;
			bool _Receive_Event_Count_Clear;
			bool _Receive_Event_Count_Cleared;


			////////////
			// Buffer //
			////////////
			HAL::FIFO<uint8_t>** _TX_FIFO;
			HAL::FIFO<uint8_t>** _RX_FIFO;
			HAL::FIFO<HAL::USB_Device::Setup_Packet>* _Setup_FIFO;

			bool Buffer_Read_Thread(HAL::USB_Device::Endpoint endpoint, uint8_t* byte);
			bool Buffer_Read_Thread(HAL::USB_Device::Endpoint endpoint, uint8_t* bytes, unsigned int size);
			bool Buffer_Write_Thread(HAL::USB_Device::Endpoint endpoint, uint8_t byte);
			bool Buffer_Write_Thread(HAL::USB_Device::Endpoint endpoint, uint8_t* bytes, unsigned int size);


			/////////////
			// Support //
			/////////////
			const double _Timeout_ms = 2000.0;
			int Endpoint_Index(HAL::USB_Device::Endpoint endpoint);
			int Endpoint_FIFO_Index(HAL::USB_Device::Endpoint endpoint);
			unsigned int Calc_In_Endpoints();
			unsigned int Calc_Out_Endpoints();
			void Clean_Up();
	};
}

#endif