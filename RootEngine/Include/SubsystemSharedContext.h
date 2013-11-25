#pragma once

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/Memory/MemoryTracker.h>

namespace RootEngine
{
	struct SubsystemSharedContext
	{
		Logging* m_logger;
		MemoryTracker* m_memTracker;
	};

	/** This class needs to be implemented by all subsystem interfaces */
	class SubsystemInterface
	{
	public:
		virtual void Startup() = 0;
		virtual void Shutdown() = 0;
	};
}