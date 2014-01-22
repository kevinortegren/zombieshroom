#pragma once

namespace Render
{
	namespace Semantic
	{
		enum Semantic
		{
			NORMAL,
			MODEL,
			BONES,
			SHADOW,
			POSITION,
			LIFETIMEMIN,
			LIFETIMEMAX,
			SPEEDMIN,
			SPEEDMAX,
			SIZEMIN,
			SIZEMAX,
			SIZEEND,
			COLOR,
			COLOREND,
			GRAVITY,
			DIRECTION,
			SPREAD,
			SPAWNTIME,
			TRANSPOSITION,
		};
	}

	namespace TextureSemantic
	{
		enum TextureSemantic
		{
			DIFFUSE,
			SPECULAR,
			NORMAL,
			DEPTH,
			TEXTUREMAP,
			TEXTURE_R,
			TEXTURE_G,
			TEXTURE_B
		};
	}
}