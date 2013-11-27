#pragma once

#include <RootEngine/Render/Include/Texture.h>
#include <RootEngine/Render/Include/Effect.h>

namespace Render
{
	struct Material
	{
		Render::TextureInterface* m_diffuseMap;
		Render::TextureInterface* m_specularMap;
		Render::TextureInterface* m_normalMap;
		Render::EffectInterface* m_effect;
	};
}