#pragma once

#include <RootEngine/Include/Logging/Logging.h>

namespace RootEngine
{
	struct SubsystemSharedContext
	{
		Logging* m_logger;
	};

	/** This class needs to be implemented by all subsystem interfaces */
	class SubsystemInterface
	{
	public:
		virtual void Startup() = 0;
		virtual void Shutdown() = 0;
	};
}