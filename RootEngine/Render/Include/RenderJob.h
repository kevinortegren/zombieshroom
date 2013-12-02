#pragma once

#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Material.h>
#include <glm/glm.hpp>
#include <memory>

namespace Render
{
	struct Uniforms
	{
		glm::mat4 m_world;
		glm::mat4 m_normal;
	};

	struct RenderJob
	{
		std::shared_ptr<Mesh> m_mesh;

		Material* m_material;	
		Uniforms m_uniforms;
	};
}