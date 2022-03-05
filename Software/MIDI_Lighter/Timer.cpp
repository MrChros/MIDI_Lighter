#include "Timer.h"

HAL::Timer::Timer()
{
	#ifdef __linux__
		clock_gettime(CLOCK_REALTIME, &_Start_Time);
	#elif _WIN32
		_Start_Time = GetTickCount64();
	#endif
}

double HAL::Timer::Elapsed_s()
{
	#ifdef __linux__
		timespec Current_Time;
		clock_gettime(CLOCK_REALTIME, &Current_Time);
	
		return ( ((Current_Time.tv_sec - _Start_Time.tv_sec) * 1) + ((Current_Time.tv_nsec - _Start_Time.tv_nsec) / 1000000000) );
	#elif _WIN32
		return ((double)(GetTickCount64() - _Start_Time)) / 1000.0;
	#endif
}

double HAL::Timer::Elapsed_ms()
{
	#ifdef __linux__
		timespec Current_Time;
		clock_gettime(CLOCK_REALTIME, &Current_Time);
	
		return ( ((Current_Time.tv_sec - _Start_Time.tv_sec) * 1000) + ((Current_Time.tv_nsec - _Start_Time.tv_nsec) / 1000000) );
	#elif _WIN32
		return ((double)(GetTickCount64() - _Start_Time));
	#endif
}

void HAL::Timer::Reset()
{
	#ifdef __linux__
		clock_gettime(CLOCK_REALTIME, &_Start_Time);
	#elif _WIN32
		_Start_Time = GetTickCount64();
	#endif
}
