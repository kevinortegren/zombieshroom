#pragma once

#include <RootEngine/Include/YamlParser.h>
#include <RootEngine/Render/Include/Renderer.h>

namespace RootEngine
{
	class EffectImporter : public RootEngine::YamlParser
	{
	public:
		EffectImporter(Render::RendererInterface* p_renderer);
			

		void Process(const YAML::Node& p_node);
		void SetWorkingDirectory(std::string p_workingDirectory);

		Render::EffectInterface* m_effect;
	private:
		Render::RendererInterface* m_renderer;
		std::string m_workingDirectory;
	};
}