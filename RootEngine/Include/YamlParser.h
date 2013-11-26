#pragma once
#include <yaml-cpp/yaml.h>
#include <string>

namespace RootEngine
{
	class YamlParser
	{
	public:
		void Load(const std::string& p_filepath);

		virtual void Process(const YAML::Node& node) = 0;
	};
}