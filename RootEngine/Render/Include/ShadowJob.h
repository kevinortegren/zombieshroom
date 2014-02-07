#pragma once

#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Effect.h>

namespace Render
{
	struct ShadowJob
	{
		MeshInterface* m_mesh;
		EffectInterface* m_effect;
	};
}