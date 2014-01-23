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
			ORBITSPEED,
			ORBITRADIUS,
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
			RANDOM,
			TEXTUREMAP,
			TEXTURE_R,
			TEXTURE_G,
			TEXTURE_B
		};
	}
}