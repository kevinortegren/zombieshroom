#pragma once

#include <windows.h>

namespace Utility
{
	class CPUTimer
	{		
	public:
		CPUTimer();

		void Start();
		void Stop();
		double GetTime();

	private:

		LARGE_INTEGER freq;
		LARGE_INTEGER start;
		LARGE_INTEGER stop;
	};
}
