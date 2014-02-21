#pragma once

#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Effect.h>

namespace Render
{
	namespace ShadowTechnique
	{
		enum ShadowTechnique
		{
			SHADOW_OPAQUE,
			SHADOW_TRANSPARANT,
			SHADOW_ANIMATED
		};
	}

	struct ShadowJob
	{
		ShadowJob() : m_mesh(nullptr), m_technique(ShadowTechnique::SHADOW_OPAQUE) {}

		MeshInterface* m_mesh;
		ShadowTechnique::ShadowTechnique m_technique;
	};
}