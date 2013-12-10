#pragma once

#include <RootEngine/Include/Profiling.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/Memory/MemoryTracker.h>
#include <RootEngine/Include/DebugOverlay/DebugOverlayInterface.h>
#include <RootEngine/Include/ConfigManager.h>

namespace RootEngine
{
	class ResourceManagerInterface;

	struct SubsystemSharedContext
	{
		Logging* m_logger;
		ProfilingInterface* m_profiler;
		MemoryTracker* m_memTracker;
		DebugOverlayInterface* m_debugOverlay;
		ConfigManagerInterface* m_configManager;
		ResourceManagerInterface* m_resourceManager;
	};

	/** This class needs to be implemented by all subsystem interfaces */
	class SubsystemInterface
	{
	public:
		virtual void Startup() = 0;
		virtual void Shutdown() = 0;
	};
}