#ifndef TIMER_H_
#define TIMER_H_

#include <math.h>
#include <string>

#ifdef __linux__
	#include <ctime>
#elif _WIN32
	#include <Windows.h>
#endif

namespace HAL
{
	class Timer
	{
		public:
			Timer();
			double Elapsed_s();
			double Elapsed_ms();
			void Reset();

		private:
			#ifdef __linux__
				timespec _Start_Time;
			#elif _WIN32
				ULONGLONG _Start_Time;
			#endif
			
	};
}

#endif