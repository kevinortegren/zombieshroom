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
			SHADOW
		};
	}

	static std::map<Semantic::Semantic, unsigned> s_sizes;

	inline void InitializeSemanticSizes()
	{
		s_sizes[Semantic::MODEL] = 64;
		s_sizes[Semantic::NORMAL] = 64;
		s_sizes[Semantic::BONES] = 1280;
		s_sizes[Semantic::SHADOW] = 64;
	}
}