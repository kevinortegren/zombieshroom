#pragma once

#include <glm/glm.hpp>

namespace Render
{
	namespace VertexType
	{
		enum VertexType
		{
			VERTEXTYPE_1P,
			VERTEXTYPE_1P1UV,
			VERTEXTYPE_1P1N,
			VERTEXTYPE_1P1C,
			VERTEXTYPE_1P1N1UV
		};
	}

	struct Vertex abstract
	{
		glm::vec3 m_pos;
	};

	struct Vertex1P : public Vertex
	{
	};

	struct Vertex1P1UV : public Vertex
	{
		glm::vec2 m_UV;
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

	struct Vertex1P1N1UV1T1BT : public Vertex1P1N1UV
	{
		glm::vec3 m_tangent;
		glm::vec3 m_bitangent;
	};
}