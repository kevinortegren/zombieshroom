#pragma once

#include <RootEngine/Include/Logging/Logging.h>

class MemoryTrackingInterface
{
public:
	virtual void LogProcessMemoryInfo() = 0;
};

// Doubtful if class is even needed anymore, since it's just one method. Keeping it like this in case we want more sophisticated memory tracking.
class MemoryTracker : public MemoryTrackingInterface 
{
public:
	MemoryTracker(Logging* p_logger) : m_logger(p_logger), m_max(0) {}

	//Uses GetProcessMemoryInfo to log working set size and peak working set size. Calling this method uses a bunch of KiB in itself the first two times
	void LogProcessMemoryInfo();
private:
	size_t m_max;

	Logging* m_logger;
};