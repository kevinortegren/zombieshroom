#pragma once

#include <glm/glm.hpp>

namespace Render
{
	struct Vertex1P
	{
		glm::vec4 m_pos;
	};

	struct Vertex1P1C
	{
		glm::vec4 m_pos;
		glm::vec4 m_color;
	};

	struct Vertex1P1N1UV
	{
		glm::vec4 m_pos;
		glm::vec3 m_normal;
		glm::vec2 m_UV;
	};
}