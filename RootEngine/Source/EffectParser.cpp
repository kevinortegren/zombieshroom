#include <RootEngine/Include/EffectParser.h>

namespace RootEngine
{
	void EffectParser::Process(const YAML::Node& p_node)
	{
		std::string n;
		p_node["name"] >> n;
		std::cout << n << std::endl;

		int x;
		p_node["position"][0] >> x;
		std::cout << x << std::endl;
	}
}