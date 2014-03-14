#include <RootEngine/Include/ConfigManager.h>
#include <sstream>
#include <fstream>
#include <SDL2/SDL_scancode.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/InputManager/Include/KeyStateMouseEnum.h>

extern Logging g_logger;

namespace RootEngine
{
	void ConfigManager::LoadConfig(const std::string& p_filepath)
	{
		try
		{
			std::ifstream file(p_filepath, std::ifstream::in);
			CreateDefaultConfig();

			if(!file.good())
			{
				StoreConfig(p_filepath);
				g_logger.LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "Couldnt find config, created one.");
				return;
			}

			YAML::Parser parser(file);
			YAML::Node doc;
			parser.GetNextDocument(doc);

			for(auto itr = doc.begin(); itr != doc.end(); ++itr)
			{
				std::string key, val;
				itr.first() >> key;
				itr.second() >> val;
				m_values[key] = val;
			}
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
		file.open(p_filepath);
		file << out.c_str();
	}

	void ConfigManager::CreateDefaultConfig()
	{
		// Default values.
		m_values["settings-player-name"] = "Player";
		m_values["settings-resolution"] = "1280x720";
		m_values["settings-fullscreen"] = "false";
		m_values["settings-glow"] = "true";
		m_values["settings-grass"] = "high";
		m_values["settings-shadows"] = "high";
		m_values["settings-water"] = "high";
		m_values["settings-key-Move_Forwards"] = "26";
		m_values["settings-key-Move_Backwards"] = "22";
		m_values["settings-key-Strafe_Left"] = "4";
		m_values["settings-key-Strafe_Right"] = "7";
		m_values["settings-key-Jump"] = "44";
		m_values["settings-key-Activate_Ability"] = "490";
		m_values["settings-key-Select_Ability1"] = "30";
		m_values["settings-key-Select_Ability2"] = "31";
		m_values["settings-key-Select_Ability3"] = "32";
		m_values["settings-key-Pick_up_Ability"] = std::to_string(SDL_SCANCODE_LSHIFT);
		m_values["settings-key-Activate_Push_Ability"] = std::to_string(RootEngine::InputManager::MouseButton::RIGHT);
		m_values["settings-key-Switch_Ability_Forward"] = std::to_string(SDL_SCANCODE_Q);
		m_values["settings-key-Scroll_Ability_Forward"] = std::to_string(RootEngine::InputManager::MouseButton::X1);
		m_values["settings-key-Scroll_Ability_Backward"] = std::to_string(RootEngine::InputManager::MouseButton::X2);
		m_values["settings-mouse-sensitivity"] = "0.2f";
		m_values["settings-mouse-invert"] = "false";
		m_values["ServerName"] = "RootForce server";
		m_values["ServerMapfile"] = "Nyaste.world";
		m_values["AbilityPack"] = "Standard.txt";
		m_values["ServerPort"] = "5567";
		m_values["ServerPassword"] = "";
		m_values["ServerMaxPlayers"] = "12";
		m_values["ServerMatchLength"] = "15";
		m_values["ServerKillVictory"] = "0";
	}

	bool ConfigManager::GetConfigValueAsBool(const std::string& p_key)
	{
		if(m_values.find(p_key) != m_values.end())
			if( m_values[p_key] == "True" || m_values[p_key] == "true" || m_values[p_key] == "1" )
			{
				return true;
			}

		return false;
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
		if(m_values.find(p_key) != m_values.end())
			return m_values[p_key];
		return std::string("");
	}
	std::map<std::string, std::string> ConfigManager::GetConfigValuePairs()
	{
		return m_values;
	}
		
	void ConfigManager::SetConfigValue(const std::string& p_key, const std::string& p_value)
	{
		m_values[p_key] = p_value;
	}

	void ConfigManager::Parse(const YAML::Node& p_node, const std::string& p_key)
	{
		if(p_node.FindValue(p_key) != nullptr)
			p_node[p_key] >> m_values[p_key];
	}
}