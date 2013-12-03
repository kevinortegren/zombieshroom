#pragma once

#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Vertex.h>
#include <memory>

namespace Render
{
	class MeshInterface
	{
		virtual void CreateIndexBuffer(unsigned int* p_indices, unsigned int p_numberOfIndices) = 0;
		virtual void CreateVertexBuffer1P(Vertex1P* p_vertices, unsigned int p_numberOfVertices) = 0;
		virtual void CreateVertexBuffer1P1UV(Vertex1P1UV* p_vertices, unsigned int p_numberOfVertices) = 0;
		virtual void CreateVertexBuffer1P1N(Vertex1P1N* p_vertices, unsigned int p_numberOfVertices) = 0;
		virtual void CreateVertexBuffer1P1C(Vertex1P1C* p_vertices, unsigned int p_numberOfVertices) = 0;
		virtual void CreateVertexBuffer1P1N1UV(Vertex1P1N1UV* p_vertices, unsigned int p_numberOfVertices) = 0;
	};

	class Mesh : public MeshInterface
	{
	friend class GLRenderer;
	public:
		Mesh();

		void CreateIndexBuffer(unsigned int* p_indices, unsigned int p_numberOfIndices);
		void CreateVertexBuffer1P(Vertex1P* p_vertices, unsigned int p_numberOfVertices);
		void CreateVertexBuffer1P1UV(Vertex1P1UV* p_vertices, unsigned int p_numberOfVertices);
		void CreateVertexBuffer1P1N(Vertex1P1N* p_vertices, unsigned int p_numberOfVertices);
		void CreateVertexBuffer1P1C(Vertex1P1C* p_vertices, unsigned int p_numberOfVertices);
		void CreateVertexBuffer1P1N1UV(Vertex1P1N1UV* p_vertices, unsigned int p_numberOfVertices);

		std::shared_ptr<BufferInterface> m_vertexBuffer;
		std::shared_ptr<BufferInterface> m_elementBuffer;
		std::shared_ptr<VertexAttributesInterface> m_vertexAttributes;
		GLenum m_primitive;
	private:		
		void Bind();
		void Unbind();
		void Draw();
		void DrawInstanced(GLsizei p_instances);
	};

}