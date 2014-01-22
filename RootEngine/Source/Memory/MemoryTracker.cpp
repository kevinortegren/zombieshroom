#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <RootEngine/Include/Memory/MemoryTracker.h>
#include <Psapi.h>

namespace RootEngine
{

	MemoryTracker::MemoryTracker( Logging* p_logger ) : m_logger(p_logger)
	{
		m_memInfo = new MemInfo();
	}

	MemoryTracker::~MemoryTracker()
	{
		delete m_memInfo;
	}

	void MemoryTracker::LogProcessMemoryInfo() 
	{
		UpdateMemory();
		m_logger->LogText("Working set size: %d MiB, %d KiB, %d B", m_memInfo->m_workingSetMiB, m_memInfo->m_workingSetKiB, m_memInfo->m_workingSetB);
		m_logger->LogText("Peak working set size: %d MiB, %d KiB, %d B", m_memInfo->m_peakSetMiB, m_memInfo->m_peakSetKiB, m_memInfo->m_peakSetB);
	}
	MemInfo* MemoryTracker::GetProcessMemInfo()
	{
		UpdateMemory();
		return m_memInfo;
	}

	void MemoryTracker::UpdateMemory()
	{
		HANDLE hProcess;
		PROCESS_MEMORY_COUNTERS pmc;

		hProcess = GetCurrentProcess();
		if (NULL == hProcess) 
		{
			m_logger->LogText(LogTag::GENERAL, LogLevel::WARNING, "GetCurrentProcess() failed.");
			return;
		}

		if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		{
			if (pmc.WorkingSetSize > m_memInfo->m_max) 
				m_memInfo->m_max = pmc.WorkingSetSize;

			m_memInfo->m_workingSetMiB	= pmc.WorkingSetSize / (1024 * 1024);
			m_memInfo->m_workingSetKiB	= (pmc.WorkingSetSize - m_memInfo->m_workingSetMiB * 1024 * 1024) / 1024;
			m_memInfo->m_workingSetB	= pmc.WorkingSetSize - m_memInfo->m_workingSetMiB * 1024 * 1024 - m_memInfo->m_workingSetKiB * 1024;
			
			m_memInfo->m_peakSetMiB = pmc.PeakWorkingSetSize / (1024 * 1024);
			m_memInfo->m_peakSetKiB = (pmc.PeakWorkingSetSize - m_memInfo->m_peakSetMiB * 1024 * 1024) / 1024;
			m_memInfo->m_peakSetB = pmc.PeakWorkingSetSize - m_memInfo->m_peakSetMiB * 1024 * 1024 - m_memInfo->m_peakSetKiB * 1024;
			
		}
	}
}
