#include <RootEngine\Include\ConfigManager.h>
#include <sstream>
#include <fstream>

#include <RootEngine\Include\Logging\Logging.h>
extern Logging g_logger;

namespace RootEngine
{
	void ConfigManager::LoadConfig(const std::string& p_filepath)
	{
		try
		{
			std::ifstream file(p_filepath, std::ifstream::in);
			if(!file.good())
			{
				CreateDefaultConfig();
				StoreConfig(p_filepath);
				g_logger.LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "Couldnt find config, created one.");
				return;
			}

			YAML::Parser parser(file);
			YAML::Node doc;
			parser.GetNextDocument(doc);

			Parse(doc, "ScreenWidth");
			Parse(doc, "ScreenHeight");
		}
		catch(YAML::ParserException& e) {
			g_logger.LogText(LogTag::GENERAL, LogLevel::FATAL_ERROR, "Config parser error: %s", e.what());
		}
	}

	void ConfigManager::StoreConfig(const std::string& p_filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		for(auto itr = m_values.begin(); itr != m_values.end(); ++itr)
		{
			out << YAML::Key << itr->first << YAML::Value << itr->second; 
		}
		out << YAML::EndMap;

		std::ofstream file;
		file.open("config.yaml");
		file << out.c_str();
	}

	void ConfigManager::CreateDefaultConfig()
	{
		// Default values.
		m_values["ScreenWidth"] = "1280";
		m_values["ScreenHeight"] = "720";
	}

	bool ConfigManager::GetConfigValueAsBool(const std::string& p_key)
	{
		if(m_values.find(p_key) == m_values.end())
			if( m_values[p_key] == "True" || m_values[p_key] == "true" || m_values[p_key] == "1" )
			{
				return true;
			}
			else
			{
				return false;
			}
	}

	int ConfigManager::GetConfigValueAsInteger(const std::string& p_key)
	{
		if(m_values.find(p_key) == m_values.end())
			return 0;

		int r;
		std::stringstream ss( m_values[p_key] );
		ss >> std::dec >> r;
		return r;
	}

	float ConfigManager::GetConfigValueAsFloat(const std::string& p_key)
	{
		if(m_values.find(p_key) == m_values.end())
			return 0;

		float r;
		std::stringstream ss( m_values[p_key] );
		ss >> std::defaultfloat >> r;
		return r;
	}

	std::string ConfigManager::GetConfigValueAsString(const std::string& p_key)
	{
		if(m_values.find(p_key) == m_values.end())
			return m_values[p_key];
	}

	void ConfigManager::Parse(const YAML::Node& p_node, const std::string& p_key)
	{
		if(p_node.FindValue(p_key) != nullptr)
			p_node[p_key] >> m_values[p_key];
	}
}