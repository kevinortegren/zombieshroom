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

	struct AnimUniforms
	{
		std::vector<glm::mat4> m_bones;
	};

	struct RenderJob
	{
		Render::MeshInterface* m_mesh;

		Material* m_material;	
		Uniforms m_uniforms;
		AnimUniforms m_animUniforms;
	};
}