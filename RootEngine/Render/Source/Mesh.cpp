#include <RootEngine/Render/Include/Mesh.h>

namespace Render
{
	Mesh::Mesh()
	{

	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Init(Vertex1P* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices)
	{
		m_vertexBuffer.Init(GL_ARRAY_BUFFER);
		m_elementBuffer.Init(GL_ELEMENT_ARRAY_BUFFER);
		m_vertexBuffer.BufferData(p_numberOfVertices, 3 * sizeof(float), p_vertices);
		m_elementBuffer.BufferData(p_numberOfIndices, sizeof(GLuint), p_indices);
		m_vertexAttributes.Init(1);
		m_vertexAttributes.SetVertexAttribPointer(m_vertexBuffer.GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	}

	void Mesh::Init(Vertex1P1UV* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices)
	{
		m_vertexBuffer.Init(GL_ARRAY_BUFFER);
		m_elementBuffer.Init(GL_ELEMENT_ARRAY_BUFFER);
		m_vertexBuffer.BufferData(p_numberOfVertices, 5 * sizeof(float), p_vertices);
		m_elementBuffer.BufferData(p_numberOfIndices, sizeof(GLuint), p_indices);
		m_vertexAttributes.Init(2);
		m_vertexAttributes.SetVertexAttribPointer(m_vertexBuffer.GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		m_vertexAttributes.SetVertexAttribPointer(m_vertexBuffer.GetBufferId(), 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (char*)0 + 3 * sizeof(float));
	}

	void Mesh::Init(Vertex1P1N* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices)
	{
		m_vertexBuffer.Init(GL_ARRAY_BUFFER);
		m_elementBuffer.Init(GL_ELEMENT_ARRAY_BUFFER);
		m_vertexBuffer.BufferData(p_numberOfVertices, 6 * sizeof(float), p_vertices);
		m_elementBuffer.BufferData(p_numberOfIndices, sizeof(GLuint), p_indices);
		m_vertexAttributes.Init(2);
		m_vertexAttributes.SetVertexAttribPointer(m_vertexBuffer.GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		m_vertexAttributes.SetVertexAttribPointer(m_vertexBuffer.GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)0 + 3 * sizeof(float));
	}

	void Mesh::Init(Vertex1P1C* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices)
	{
		m_vertexBuffer.Init(GL_ARRAY_BUFFER);
		m_elementBuffer.Init(GL_ELEMENT_ARRAY_BUFFER);
		m_vertexBuffer.BufferData(p_numberOfVertices, 7 * sizeof(float), p_vertices);
		m_elementBuffer.BufferData(p_numberOfIndices, sizeof(GLuint), p_indices);
		m_vertexAttributes.Init(2);
		m_vertexAttributes.SetVertexAttribPointer(m_vertexBuffer.GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
		m_vertexAttributes.SetVertexAttribPointer(m_vertexBuffer.GetBufferId(), 1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (char*)0 + 3 * sizeof(float));
	}

	void Mesh::Init(Vertex1P1N1UV* p_vertices, unsigned int p_numberOfVertices, unsigned int* p_indices, unsigned int p_numberOfIndices)
	{
		m_vertexBuffer.Init(GL_ARRAY_BUFFER);
		m_elementBuffer.Init(GL_ELEMENT_ARRAY_BUFFER);
		m_vertexBuffer.BufferData(p_numberOfVertices, 8 * sizeof(float), p_vertices);
		m_elementBuffer.BufferData(p_numberOfIndices, sizeof(GLuint), p_indices);
		m_vertexAttributes.Init(3);
		m_vertexAttributes.SetVertexAttribPointer(m_vertexBuffer.GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		m_vertexAttributes.SetVertexAttribPointer(m_vertexBuffer.GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)0 + 3 * sizeof(float));
		m_vertexAttributes.SetVertexAttribPointer(m_vertexBuffer.GetBufferId(), 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)0 + 6 * sizeof(float));
	}

	void Mesh::Bind()
	{
		m_vertexAttributes.Bind();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer.GetBufferId());
	}

	void Mesh::Unbind()
	{
		m_vertexAttributes.Unbind();
	}

	void Mesh::DrawArrays()
	{
		glDrawElements(GL_TRIANGLES, m_elementBuffer.GetBufferSize(), GL_UNSIGNED_INT, 0);
	}
}