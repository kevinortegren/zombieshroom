#include <RootEngine/Render/Include/Mesh.h>

namespace Render
{
	Mesh::Mesh()
		: m_primitive(GL_TRIANGLES) {}

	void Mesh::CreateIndexBuffer(unsigned int* p_indices, unsigned int p_numberOfIndices)
	{
		m_elementBuffer->Init(GL_ELEMENT_ARRAY_BUFFER);
		m_elementBuffer->BufferData(p_numberOfIndices, sizeof(GLuint), p_indices);
	}

	void Mesh::CreateVertexBuffer1P(Vertex1P* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->Init(GL_ARRAY_BUFFER);
		m_vertexBuffer->BufferData(p_numberOfVertices, 3 * sizeof(float), p_vertices);
		m_vertexAttributes->Init(1);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	}

	void Mesh::CreateVertexBuffer1P1UV(Vertex1P1UV* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->Init(GL_ARRAY_BUFFER);
		m_vertexBuffer->BufferData(p_numberOfVertices, 5 * sizeof(float), p_vertices);
		m_vertexAttributes->Init(2);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (char*)0 + 3 * sizeof(float));
	}

	void Mesh::CreateVertexBuffer1P1N(Vertex1P1N* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->Init(GL_ARRAY_BUFFER);
		m_vertexBuffer->BufferData(p_numberOfVertices, 6 * sizeof(float), p_vertices);
		m_vertexAttributes->Init(2);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)0 + 3 * sizeof(float));
	}

	void Mesh::CreateVertexBuffer1P1C(Vertex1P1C* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->Init(GL_ARRAY_BUFFER);
		m_vertexBuffer->BufferData(p_numberOfVertices, 7 * sizeof(float), p_vertices);
		m_vertexAttributes->Init(2);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (char*)0 + 3 * sizeof(float));
	}

	void Mesh::CreateVertexBuffer1P1N1UV(Vertex1P1N1UV* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->Init(GL_ARRAY_BUFFER);
		m_vertexBuffer->BufferData(p_numberOfVertices, 8 * sizeof(float), p_vertices);
		m_vertexAttributes->Init(3);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)0 + 3 * sizeof(float));
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)0 + 6 * sizeof(float));
	}

	void Mesh::Bind()
	{
		m_vertexAttributes->Bind();

		if(m_elementBuffer != nullptr)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer->GetBufferId());
		}
	}

	void Mesh::Unbind()
	{
		m_vertexAttributes->Unbind();
	}

	void Mesh::Draw()
	{
		if(m_elementBuffer != nullptr)
		{
			glDrawElements(m_primitive, m_elementBuffer->GetBufferSize(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(m_primitive, 0, m_vertexBuffer->GetBufferSize());
		}
	}

	void Mesh::DrawInstanced(GLsizei p_instances)
	{
		glDrawElementsInstanced(GL_TRIANGLES, m_elementBuffer->GetBufferSize(), GL_UNSIGNED_INT, 0, p_instances);
	}

	GLenum Mesh::GetPrimitiveType()
	{
		return m_primitive;
	}

	void Mesh::SetPrimitiveType( GLenum p_type )
	{
		m_primitive = p_type;
	}

	void Mesh::SetElementBuffer(std::shared_ptr<BufferInterface> p_buffer)
	{
		m_elementBuffer = p_buffer;
	}

	void Mesh::SetVertexBuffer(std::shared_ptr<BufferInterface> p_buffer)
	{
		m_vertexBuffer = p_buffer;
	}

	void Mesh::SetVertexAttribute(std::shared_ptr<VertexAttributesInterface> p_attribute)
	{
		m_vertexAttributes = p_attribute;
	}

	std::shared_ptr<BufferInterface> Mesh::GetVertexBuffer()
	{
		return m_vertexBuffer;
	}

	std::shared_ptr<BufferInterface> Mesh::GetElementBuffer()
	{
		return m_elementBuffer;
	}

	std::shared_ptr<VertexAttributesInterface> Mesh::GetVertexAttribute()
	{
		return m_vertexAttributes;
	}

}