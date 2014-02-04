#pragma once

#include <RootEngine/Render/Include/Texture.h>
#include <RootEngine/Render/Include/Effect.h>

namespace Render
{
	struct Material
	{
		Material(int p_id) : m_id(p_id), m_effect(nullptr), m_tileFactor(0) {}

		// ID.
		int m_id;

		// Material Data.
		float m_tileFactor;

		// Effect.
		Render::EffectInterface* m_effect;

		// Tetxure map.
		std::map<Render::TextureSemantic::TextureSemantic, Render::TextureInterface*> m_textures;
	};
}