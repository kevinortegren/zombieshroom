#pragma once

#include <RootEngine/Render/Include/Texture.h>
#include <RootEngine/Render/Include/Effect.h>

namespace Render
{
	struct Material
	{
	private:
		Material() : 
			m_effect(nullptr), m_tileFactor(0)
		{}
	public:
		friend class GLRenderer;
		std::map<Render::TextureSemantic::TextureSemantic, Render::TextureInterface*> m_textures;
		float m_tileFactor;
		Render::EffectInterface* m_effect;
	};
}