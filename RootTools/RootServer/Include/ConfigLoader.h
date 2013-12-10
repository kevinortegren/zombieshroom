#pragma once
#include <RootSystems/Include/Network/ServerConfig.h>
#include <RootEngine/Include/Logging/Logging.h>

namespace RootServer
{
	class ConfigLoader
	{
	public:
		ConfigLoader(Logging* p_logger);
		RootSystems::ServerConfig Load(std::string p_filename);
	private:
		Logging* m_logger;
	};
}