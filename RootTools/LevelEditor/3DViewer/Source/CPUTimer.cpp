#include "CPUTimer.h"

using namespace Utility;

CPUTimer::CPUTimer()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	
	/*
	Some dual core systems have a problem where the different CPUs return different
	QueryPerformanceCounter values. So when this thread is schedule on the other CPU
	in a later frame, we could even get a negative frameTime. To solve this we force
	the main thread to always run on CPU 0.
	*/
	SetThreadAffinityMask(GetCurrentThread(), 1);
}

void CPUTimer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&start);
}

void CPUTimer::Stop()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&stop);
}

double CPUTimer::GetTime()
{
	// miliseconds
	return (double)(stop.QuadPart - start.QuadPart) / ((double)freq.QuadPart/1000.0);
}