#pragma once

#include <RootEngine/Include/YamlParser.h>

namespace RootEngine
{
	class EffectParser : public RootEngine::YamlParser
	{
	public:
		void Process(const YAML::Node& p_node);
	};
}