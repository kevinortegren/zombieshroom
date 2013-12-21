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
			RENDER_TRANSFORMFEEDBACK = 0x01
		};
	}
	struct Uniforms
	{
		glm::mat4 m_world;
		glm::mat4 m_normal;
	};

	struct RenderJob
	{
		RenderJob()
			: m_flags(0) {}

		Render::MeshInterface* m_mesh;

		Material* m_material;	
		Uniforms m_uniforms;

		unsigned m_flags;
	};
}