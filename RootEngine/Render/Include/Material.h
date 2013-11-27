#pragma once

#include <RootEngine/Render/Include/Texture.h>
#include <RootEngine/Render/Include/Effect.h>

namespace Render
{
	struct Material
	{
		Render::Texture* m_diffuseMap;
		Render::Texture* m_specularMap;
		Render::Texture* m_normalMap;
		Render::Effect* m_effect;
	};
}