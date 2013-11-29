#pragma once

#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Vertex.h>

namespace Render
{
	namespace Primitive
	{
		enum Primitive
		{
			TRIANGLES,
			LINES
		};
	}

	class MeshInterface
	{
	public:
		virtual void Init(Vertex1P* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices) = 0;
		virtual void Init(Vertex1P1UV* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices) = 0;
		virtual void Init(Vertex1P1N* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices) = 0;
		virtual void Init(Vertex1P1C* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices) = 0;
		virtual void Init(Vertex1P1N1UV* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices) = 0;
		virtual void SetPrimitive(int p_primitive) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Draw() = 0;
	};

	class Mesh : public MeshInterface
	{
	friend class GLRenderer;
	public:
		Mesh();
		~Mesh();
		virtual void Init(Vertex1P* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices);
		virtual void Init(Vertex1P1UV* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices);
		virtual void Init(Vertex1P1N* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices);
		virtual void Init(Vertex1P1C* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices);
		virtual void Init(Vertex1P1N1UV* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices);
		virtual void Bind();
		virtual void Unbind();
		virtual void Draw();
		virtual void SetPrimitive(int p_primitive);

		Buffer m_vertexBuffer;
		Buffer m_elementBuffer;
		VertexAttributes m_vertexAttributes;
		bool m_isIndexed;
		GLenum m_primitive;

	private:

		void DrawInstanced(GLsizei p_instances);
	};
}