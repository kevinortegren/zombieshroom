#pragma once

#include <RootEngine/Render/Include/Vertex.h>

namespace Utility
{
	class RawMesh
	{
	public:
		virtual void Init(int p_vertexType) = 0;

		Render::Vertex* m_vertices;
		unsigned int m_numberOfVertices;
		unsigned int* m_indices;
		unsigned int m_numberOfIndices;
	};

	class Cube : public RawMesh
	{
	public:
		Cube(int p_vertexType);
		~Cube();
		void Init(int p_vertexType);
	private:

	};

	class ScreenQuad : public RawMesh
	{
	public:
		ScreenQuad(int p_vertexType);
		~ScreenQuad();
		void Init(int p_vertexType);
	};
}