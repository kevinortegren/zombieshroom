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
			MK1,
			MK2,
			MK3,
			XMAX,
			YMAX,
			EYEWORLDPOS,
			DX
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
			TEXTURE_B,
			GLOW,
			COMPUTEIN,
			COMPUTEOUT,
			SHADOWDEPTHPCF,
			SHADOWDEPTH,
			COMPUTENORMAL,
			TEXTURE_RN,
			TEXTURE_GN,
			TEXTURE_BN

		};
	}
}