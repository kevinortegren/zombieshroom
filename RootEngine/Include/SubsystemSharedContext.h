#pragma once


#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/Memory/MemoryTracker.h>
#include <RootEngine/Include/ConfigManager.h>
#include <RootEngine/Include/Profiling.h>
#ifndef COMPILE_LEVEL_EDITOR
#include <RootEngine/Include/DebugOverlay/DebugOverlayInterface.h>
#endif
namespace RootEngine
{
	class ResourceManagerInterface;

	struct SubsystemSharedContext
	{
		Logging* m_logger;

		MemoryTracker* m_memTracker;

		ConfigManagerInterface* m_configManager;
		ResourceManagerInterface* m_resourceManager;
		ProfilingInterface* m_profiler;
#ifndef COMPILE_LEVEL_EDITOR
		DebugOverlayInterface* m_debugOverlay;
#endif
	};

	/** This class needs to be implemented by all subsystem interfaces */
	class SubsystemInterface
	{
	public:
		virtual void Startup() = 0;
		virtual void Shutdown() = 0;
	};
}