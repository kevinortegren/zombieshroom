#pragma once

#include <glm/glm.hpp>

namespace Render
{
	struct Vertex abstract
	{
		glm::vec3 m_pos;
	};

	struct Vertex1P : public Vertex
	{
	};

	struct Vertex1P1N : public Vertex
	{
		glm::vec3 m_normal;
	};

	struct Vertex1P1C : public Vertex
	{
		glm::vec4 m_color;
	};

	struct Vertex1P1N1UV : public Vertex
	{
		glm::vec3 m_normal;
		glm::vec2 m_UV;
	};
}