#pragma once

#include <RootEngine/Include/Logging/Logging.h>

namespace RootEngine
{
	struct MemInfo
	{
		MemInfo()
		{
			m_workingSetMiB = m_workingSetKiB = m_workingSetB = m_peakSetMiB = m_peakSetKiB = m_peakSetB = 0;
		}
		size_t m_workingSetMiB;
		size_t m_workingSetKiB;
		size_t m_workingSetB;
		size_t m_peakSetMiB;
		size_t m_peakSetKiB;
		size_t m_peakSetB;
		size_t m_max;
	};

	class MemoryTrackingInterface
	{
	public:
		virtual void LogProcessMemoryInfo() = 0;
		virtual MemInfo* GetProcessMemInfo() = 0;
	};
	
	class MemoryTracker : public MemoryTrackingInterface 
	{
	public:
		MemoryTracker(Logging* p_logger);
		~MemoryTracker();
		//Uses GetProcessMemoryInfo to log working set size and peak working set size. Calling this method uses a bunch of KiB in itself the first two times
		void LogProcessMemoryInfo();
		//Get a pointer to the meminfo struct
		MemInfo* GetProcessMemInfo();
	private:

		void UpdateMemory();

		MemInfo m_memInfo;
		Logging* m_logger;
	};
}