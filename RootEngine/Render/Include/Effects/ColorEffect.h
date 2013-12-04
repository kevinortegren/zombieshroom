#pragma once

#include <RootEngine\Render\Include\Effect.h>

namespace Render
{
	struct ColorData : public EffectData
	{
		glm::vec4 m_color;
	};

	class ColorEffect : public EffectInstance
	{
		void Apply();
	};
}