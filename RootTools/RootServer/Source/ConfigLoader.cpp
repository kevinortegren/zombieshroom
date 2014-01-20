#include <ConfigLoader.h>
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace RootServer
{
	ConfigLoader::ConfigLoader(Logging* p_logger)
		: m_logger(p_logger)
	{
	}

	RootSystems::ServerConfig ConfigLoader::Load(std::string p_filename)
	{
		RootSystems::ServerConfig conf;
		std::ifstream file(p_filename.c_str(), std::ifstream::in);
		if(file.good())
		{
			try
			{
				YAML::Parser parser(file);
					
				YAML::Node node;
				parser.GetNextDocument(node);

				unsigned utmp;
				if(node.FindValue("MaxPlayers"))
				{
					node["MaxPlayers"] >> utmp;
					conf.MaxPlayers = (uint8_t)utmp;
				}
				if(node.FindValue("Port"))
				{
					node["Port"] >> utmp;
					conf.Port = (uint16_t)utmp;
				}
				if(node.FindValue("Password"))
					node["Password"]  >> conf.Password;
				if(node.FindValue("LevelFile"))
					node["LevelFile"]  >> conf.MapName;
				if(node.FindValue("GameMode"))
				{
					node["GameMode"]  >> utmp;
					conf.GameMode = (RootSystems::GameMode::GameMode)utmp;
				}
				if(node.FindValue("MatchTime"))
				{
					node["MatchTime"] >> utmp;
					conf.MatchTime = (uint32_t)utmp;
				}
				if(node.FindValue("KillCount"))
				{
					node["KillCount"] >> utmp;
					conf.KillCount = (uint32_t)utmp;
				}
			}
			catch(std::exception e) {
				m_logger->LogText(LogTag::TOOLS, LogLevel::NON_FATAL_ERROR, "Failed to load server config. %s", e.what());
			}
		}
		return conf;
	}
}
