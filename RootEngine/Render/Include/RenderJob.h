#pragma once

#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
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
		std::shared_ptr<BufferInterface> m_vertexBuffer;
		std::shared_ptr<BufferInterface> m_indexBuffer;
		std::shared_ptr<VertexAttributesInterface> m_attributes;
		Uniforms* m_uniforms;
	};
}