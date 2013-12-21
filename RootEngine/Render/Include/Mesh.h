#pragma once

#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Vertex.h>
#include <vector>
#include <memory>

namespace Render
{
	class MeshInterface
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Draw() = 0;
		virtual void DrawInstanced(GLsizei p_instances) = 0;
		virtual void DrawTransformFeedback() = 0;

		//Get functions
		virtual std::shared_ptr<BufferInterface> GetVertexBuffer() = 0;
		virtual std::shared_ptr<BufferInterface> GetElementBuffer() = 0;
		virtual std::shared_ptr<VertexAttributesInterface> GetVertexAttribute() = 0;
		virtual GLenum GetPrimitiveType() = 0;
		virtual GLuint GetTransformFeedback() = 0;

		//Init functions
		virtual void SetVertexBuffer(std::shared_ptr<BufferInterface> p_buffer) = 0;
		virtual void SetElementBuffer(std::shared_ptr<BufferInterface> p_buffer) = 0;
		virtual void SetVertexAttribute(std::shared_ptr<VertexAttributesInterface> p_attribute) = 0;
		virtual void SetTransformFeedback() = 0;
		virtual void SetPrimitiveType(GLenum p_type) = 0;

		//Create functions
		virtual void CreateIndexBuffer(unsigned int* p_indices, unsigned int p_numberOfIndices) = 0;
		virtual void CreateVertexBuffer1P(Vertex1P* p_vertices, unsigned int p_numberOfVertices) = 0;
		virtual void CreateVertexBuffer1P1UV(Vertex1P1UV* p_vertices, unsigned int p_numberOfVertices) = 0;
		virtual void CreateVertexBuffer1P1N(Vertex1P1N* p_vertices, unsigned int p_numberOfVertices) = 0;
		virtual void CreateVertexBuffer1P1C(Vertex1P1C* p_vertices, unsigned int p_numberOfVertices) = 0;
		virtual void CreateVertexBuffer1P1N1UV(Vertex1P1N1UV* p_vertices, unsigned int p_numberOfVertices) = 0;
		virtual void CreateVertexBuffer1P1N1UV1T1BT(Vertex1P1N1UV1T1BT* p_vertices, unsigned int p_numberOfVertices) = 0;
	};

	class Mesh : public MeshInterface
	{
	public:
		Mesh();
		~Mesh();

		//Get functions
		std::shared_ptr<BufferInterface> GetVertexBuffer();
		std::shared_ptr<BufferInterface> GetElementBuffer();
		std::shared_ptr<VertexAttributesInterface> GetVertexAttribute();
		GLenum GetPrimitiveType();
		GLuint GetTransformFeedback();

		//Set functions
		void SetVertexBuffer(std::shared_ptr<BufferInterface> p_buffer);
		void SetElementBuffer(std::shared_ptr<BufferInterface> p_buffer);
		void SetVertexAttribute(std::shared_ptr<VertexAttributesInterface> p_attribute);
		void SetPrimitiveType(GLenum p_type);
		void SetTransformFeedback();

		//Create functions
		void CreateIndexBuffer(unsigned int* p_indices, unsigned int p_numberOfIndices);
		void CreateVertexBuffer1P(Vertex1P* p_vertices, unsigned int p_numberOfVertices);
		void CreateVertexBuffer1P1UV(Vertex1P1UV* p_vertices, unsigned int p_numberOfVertices);
		void CreateVertexBuffer1P1N(Vertex1P1N* p_vertices, unsigned int p_numberOfVertices);
		void CreateVertexBuffer1P1C(Vertex1P1C* p_vertices, unsigned int p_numberOfVertices);
		void CreateVertexBuffer1P1N1UV(Vertex1P1N1UV* p_vertices, unsigned int p_numberOfVertices);
		void CreateVertexBuffer1P1N1UV1T1BT(Vertex1P1N1UV1T1BT* p_vertices, unsigned int p_numberOfVertices);

		void Bind();
		void Unbind();
		void Draw();
		void DrawInstanced(GLsizei p_instances);
		void DrawTransformFeedback();

	private:
		std::shared_ptr<BufferInterface> m_vertexBuffer;
		std::shared_ptr<BufferInterface> m_elementBuffer;
		std::shared_ptr<VertexAttributesInterface> m_vertexAttributes;
		
		GLuint m_transformFeedback;
		GLenum m_primitive;
	};

}