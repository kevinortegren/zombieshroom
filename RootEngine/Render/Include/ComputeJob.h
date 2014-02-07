#pragma once

#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/Texture.h>
#include <glm/glm.hpp>
#include <memory>

namespace Render
{

	struct ComputeJob
	{
		ComputeJob()
			: m_groupDim(1) {}
		Render::EffectInterface* m_effect;	
		std::map<int, Render::TextureInterface*> m_textures;
		std::map<Semantic::Semantic, void*> m_params;
		glm::uvec3 m_groupDim;
	};
}