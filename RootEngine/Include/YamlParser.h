#pragma once
#include <yaml-cpp/yaml.h>
#include <string>

namespace RootEngine
{
	class YamlParserInterface
	{
		virtual void Load(const std::string& p_filepath) = 0;
	};

	class YamlParser : public YamlParserInterface
	{
	public:
		void Load(const std::string& p_filepath);

		virtual void Process(const YAML::Node& node) = 0;
	};
}