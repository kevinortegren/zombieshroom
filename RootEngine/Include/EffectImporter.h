#pragma once

#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/YamlParser.h>

namespace RootEngine
{
	class EffectImporter : public RootEngine::YamlParser
	{
	public:
		EffectImporter(Render::RendererInterface* p_renderer)
			: m_renderer(p_renderer) {}

		void Process(const YAML::Node& p_node);
		void SetWorkingDirectory(std::string p_workingDirectory);

		Render::EffectInterface* m_effect;
	private:
		Render::RendererInterface* m_renderer;
		std::string m_workingDirectory;
	};
}