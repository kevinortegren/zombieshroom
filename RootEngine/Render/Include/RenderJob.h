#pragma once

#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Material.h>
#include <glm/glm.hpp>
#include <memory>

namespace Render
{
	namespace RenderFlags
	{
		enum RenderFlags
		{
			RENDER_TRANSFORMFEEDBACK = 0x01,
			RENDER_IGNORE_CASTSHADOW = 0x02
		};
	}

	struct RenderJob
	{
		RenderJob()
			: m_flags(0), m_renderPass(2) {}

		MeshInterface* m_mesh;
		Material* m_material;	
		std::map<Semantic::Semantic, void*> m_params;
		unsigned m_flags;
		unsigned m_renderPass;
	};
}