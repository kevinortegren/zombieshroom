#pragma once

namespace Render
{
	namespace Semantic
	{
		enum Semantic
		{
			NORMAL,
			MODEL,
			BONES
		};
	}

	static std::map<Semantic::Semantic, unsigned> s_sizes;

	inline void InitializeSemanticSizes()
	{
		s_sizes[Semantic::MODEL] = 64;
		s_sizes[Semantic::NORMAL] = 64;
		s_sizes[Semantic::BONES] = 1280;
	}
}