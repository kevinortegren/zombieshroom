#pragma once

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/Memory/MemoryTracker.h>
#include <RootEngine/Include/DebugOverlay/DebugOverlay.h>

namespace RootEngine
{
	class ResourceManager;

	struct SubsystemSharedContext
	{
		Logging* m_logger;
		MemoryTracker* m_memTracker;
		DebugOverlayInterface* m_debugOverlay;
		ResourceManager* m_resourceManager;
	};

	/** This class needs to be implemented by all subsystem interfaces */
	class SubsystemInterface
	{
	public:
		virtual void Startup() = 0;
		virtual void Shutdown() = 0;
	};
}