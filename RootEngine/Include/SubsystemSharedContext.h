#pragma once

#include <string>
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
		const char* m_workingDirectory;
		const char* m_preferenceDirectory;
		Logging* m_logger;

		MemoryTracker* m_memTracker;

		ConfigManagerInterface* m_configManager;
		ResourceManagerInterface* m_resourceManager;
		ProfilingInterface* m_profiler;
#ifndef COMPILE_LEVEL_EDITOR
#ifdef _DEBUG
		DebugOverlayInterface* m_debugOverlay;
#endif
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