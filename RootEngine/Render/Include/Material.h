#pragma once

#include <RootEngine/Render/Include/Texture.h>
#include <RootEngine/Render/Include/Effect.h>

namespace Render
{
	struct Material
	{
		Material() : 
			m_diffuseMap(nullptr),
			m_specularMap(nullptr),
			m_normalMap(nullptr),
			m_effect(nullptr)
		{}

		Render::TextureInterface* m_diffuseMap;
		Render::TextureInterface* m_specularMap;
		Render::TextureInterface* m_normalMap;
		Render::EffectInterface* m_effect;

		std::shared_ptr<Render::EffectParams> m_params;
	};
}