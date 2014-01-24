#pragma once

#include <yaml-cpp\yaml.h>
#include <map>
#include <string>

namespace RootEngine
{
	struct ConfigManagerInterface
	{
		virtual void StoreConfig(const std::string& p_filepath) = 0;

		virtual bool GetConfigValueAsBool(const std::string& p_key) = 0;
		virtual int GetConfigValueAsInteger(const std::string& p_key) = 0;
		virtual float GetConfigValueAsFloat(const std::string& p_key) = 0;
		virtual std::string GetConfigValueAsString(const std::string& p_key) = 0;
		virtual std::map<std::string, std::string> GetConfigValuePairs() = 0;
		
		virtual void SetConfigValue(const std::string& p_key, const std::string& p_value) = 0;
	};

	class ConfigManager : public ConfigManagerInterface
	{
	public:
		void LoadConfig(const std::string& p_filepath); 
		void StoreConfig(const std::string& p_filepath);
		
		void CreateDefaultConfig();

		bool GetConfigValueAsBool(const std::string& p_key);
		int GetConfigValueAsInteger(const std::string& p_key);
		float GetConfigValueAsFloat(const std::string& p_key);
		std::string GetConfigValueAsString(const std::string& p_key);
		virtual std::map<std::string, std::string> GetConfigValuePairs();
		
		virtual void SetConfigValue(const std::string& p_key, const std::string& p_value);

	private:

		void Parse(const YAML::Node& p_node, const std::string& p_key);
		std::map<std::string, std::string> m_values;
	};
}