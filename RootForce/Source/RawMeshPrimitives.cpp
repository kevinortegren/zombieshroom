#include <RootForce/Include/RawMeshPrimitives.h>

using glm::vec3;
using glm::vec2;

namespace Utility
{
	Cube::Cube(int p_vertexType)
	{
		Init(p_vertexType);
	}

	Cube::~Cube()
	{
		delete [] m_vertices;
		m_vertices = nullptr;
		delete [] m_indices;
		m_indices = nullptr;
	}

	void Cube::Init(int p_vertexType)
	{
		if(p_vertexType == Render::VertexType::VERTEXTYPE_1P)
		{
			m_numberOfVertices = 8;
			m_vertices = new Render::Vertex1P[8];
			m_numberOfIndices = 36;
			m_indices = new unsigned int[36];

			vec3 max = vec3(1.0f);
			vec3 min = vec3(-1.0f);

			m_vertices[0].m_pos = vec3(max);
			m_vertices[1].m_pos = vec3(max.x, min.y, max.z);						 
			m_vertices[2].m_pos = vec3(min.x, max.y, min.z);
			m_vertices[3].m_pos = vec3(min);						 
			m_vertices[4].m_pos = vec3(min.x, max.y, max.z);
			m_vertices[5].m_pos = vec3(min.x, min.y, max.z);		 
			m_vertices[6].m_pos = vec3(max.x, max.y, min.z);
			m_vertices[7].m_pos = vec3(max.x, min.y, min.z);

			m_indices[0] = 0; m_indices[1] = 4; m_indices[2] = 1;
			m_indices[3] = 5; m_indices[4] = 1; m_indices[5] = 4;

			m_indices[6] = 2; m_indices[7] = 5; m_indices[8] = 4;
			m_indices[9] = 2; m_indices[10] = 3; m_indices[11] = 5;

			m_indices[12] = 6; m_indices[13] = 3; m_indices[14] = 2;
			m_indices[15] = 6; m_indices[16] = 7; m_indices[17] = 3;

			m_indices[18] = 0; m_indices[19] = 7; m_indices[20] = 6;
			m_indices[21] = 0; m_indices[22] = 1; m_indices[23] = 7;

			m_indices[24] = 0; m_indices[25] = 2; m_indices[26] = 4;
			m_indices[27] = 0; m_indices[28] = 6; m_indices[29] = 2;

			m_indices[30] = 1; m_indices[31] = 5; m_indices[32] = 3;
			m_indices[33] = 1; m_indices[34] = 3; m_indices[35] = 7;
		}
	}

	ScreenQuad::ScreenQuad(int p_vertexType)
	{
		Init(p_vertexType);
	}

	ScreenQuad::~ScreenQuad()
	{

	}

	void ScreenQuad::Init(int p_vertexType)
	{
		if(p_vertexType == Render::VertexType::VERTEXTYPE_1P)
		{
			m_numberOfVertices = 4;
			m_vertices = new Render::Vertex1P[4];
			m_numberOfIndices = 6;
			m_indices = new unsigned int[6];

			m_vertices[0].m_pos = glm::vec3(-1.0f, -1.0f, 0.0f);
			m_vertices[1].m_pos = glm::vec3(+1.0f, -1.0f, 0.0f);
			m_vertices[2].m_pos = glm::vec3(-1.0f, +1.0f, 0.0f);
			m_vertices[3].m_pos = glm::vec3(+1.0f, +1.0f, 0.0f);

			m_indices[0] = 0; m_indices[1] = 1; m_indices[2] = 2;
			m_indices[3] = 2; m_indices[4] = 1; m_indices[5] = 3;
		}
	}
}