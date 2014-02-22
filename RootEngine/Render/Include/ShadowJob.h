#pragma once

#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Effect.h>

namespace Render
{
	namespace ShadowTechnique
	{
		enum ShadowTechnique
		{
			SHADOW_NONE,
			SHADOW_OPAQUE,
			SHADOW_TRANSPARANT,
			SHADOW_DYNAMIC,
			SHADOW_ANIMATED
		};
	}

	struct ShadowJob
	{
		ShadowJob() : m_mesh(nullptr), m_technique(ShadowTechnique::SHADOW_NONE) {}

		MeshInterface* m_mesh;
		ShadowTechnique::ShadowTechnique m_technique;
		
		std::map<Semantic::Semantic, void*> m_params;
		std::map<Render::TextureSemantic::TextureSemantic, Render::TextureInterface*> m_textures;
	};
}