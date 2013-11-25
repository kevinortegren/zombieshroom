#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <RootEngine/Include/Memory/MemoryTracker.h>
#include <Psapi.h>
#include <RootEngine/Include/Logging/Logging.h>

void MemoryTracker::LogProcessMemoryInfo() 
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	hProcess = GetCurrentProcess();
	if (NULL == hProcess) 
	{
		m_logger->LogText("GetCurrentProcess() failed.");
		return;
	}

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		if (pmc.WorkingSetSize > m_max) 
			m_max = pmc.WorkingSetSize;

		size_t MiB = pmc.WorkingSetSize / (1024 * 1024);
		size_t KiB = (pmc.WorkingSetSize - MiB * 1024 * 1024) / 1024;
		size_t B = pmc.WorkingSetSize - MiB * 1024 * 1024 - KiB * 1024;
		m_logger->LogText("Working set size: %d MiB, %d KiB, %d B", MiB, KiB, B);

		MiB = pmc.PeakWorkingSetSize / (1024 * 1024);
		KiB = (pmc.PeakWorkingSetSize - MiB * 1024 * 1024) / 1024;
		B = pmc.PeakWorkingSetSize - MiB * 1024 * 1024 - KiB * 1024;
		m_logger->LogText("Peak working set size: %d MiB, %d KiB, %d B", MiB, KiB, B);
	}
}


