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

	private:
		Render::RendererInterface* m_renderer;
	};
}