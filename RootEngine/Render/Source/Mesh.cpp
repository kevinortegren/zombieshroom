#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/RenderResourceManager.h>

namespace Render
{
	Mesh::Mesh()
		: m_primitive(GL_TRIANGLES), m_vertexBuffer(0), m_elementBuffer(0),m_transformFeedback(0),m_wireFrame(false), m_noCulling(false) {}


	Mesh::~Mesh()
	{
		if(m_transformFeedback != 0)
		{
			glDeleteTransformFeedbacks(1, &m_transformFeedback);
		}
	}

	void Mesh::CreateIndexBuffer(unsigned int* p_indices, unsigned int p_numberOfIndices)
	{
		m_elementBuffer->BufferData(p_numberOfIndices, sizeof(GLuint), p_indices, GL_STATIC_DRAW);
	}

	void Mesh::CreateVertexBuffer1P(Vertex1P* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->BufferData(p_numberOfVertices, 3 * sizeof(float), p_vertices, GL_STATIC_DRAW);
		m_vertexAttributes->Init(1);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	}

	void Mesh::CreateVertexBuffer1P1UV(Vertex1P1UV* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->BufferData(p_numberOfVertices, 5 * sizeof(float), p_vertices, GL_STATIC_DRAW);
		m_vertexAttributes->Init(2);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (char*)0 + 3 * sizeof(float));
	}

	void Mesh::CreateVertexBuffer1P1N(Vertex1P1N* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->BufferData(p_numberOfVertices, 6 * sizeof(float), p_vertices, GL_STATIC_DRAW);
		m_vertexAttributes->Init(2);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)0 + 3 * sizeof(float));
	}

	void Mesh::CreateVertexBuffer1P1C(Vertex1P1C* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->BufferData(p_numberOfVertices, 7 * sizeof(float), p_vertices, GL_STATIC_DRAW);
		m_vertexAttributes->Init(2);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (char*)0 + 3 * sizeof(float));
	}

	void Mesh::CreateVertexBuffer1P1N1UV(Vertex1P1N1UV* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->BufferData(p_numberOfVertices, 8 * sizeof(float), p_vertices, GL_STATIC_DRAW);
		m_vertexAttributes->Init(3);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)0 + 3 * sizeof(float));
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)0 + 6 * sizeof(float));
	}

	void Mesh::CreateVertexBuffer1P1N1UV1T1BT(Vertex1P1N1UV1T1BT* p_vertices, unsigned int p_numberOfVertices)
	{
		m_vertexBuffer->BufferData(p_numberOfVertices, sizeof(Vertex1P1N1UV1T1BT), p_vertices, GL_STATIC_DRAW);
		m_vertexAttributes->Init(5);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT), 0);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT), (char*)0 + 3 * sizeof(float));
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT), (char*)0 + 6 * sizeof(float));
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT), (char*)0 + 8 * sizeof(float));
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT), (char*)0 + 11 * sizeof(float));
	}

	void Mesh::CreateVertexBuffer1P1N1UV1T1BT1BID1W( Vertex1P1N1UV1T1BT1BID1W* p_vertices, unsigned int p_numberOfVertices )
	{
		m_vertexBuffer->BufferData(p_numberOfVertices, sizeof(Vertex1P1N1UV1T1BT1BID1W), p_vertices, GL_STATIC_DRAW);
		m_vertexAttributes->Init(7);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT1BID1W), 0);
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT1BID1W), (char*)0 + 3 * sizeof(float));
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT1BID1W), (char*)0 + 6 * sizeof(float));
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT1BID1W), (char*)0 + 8 * sizeof(float));
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT1BID1W), (char*)0 + 11 * sizeof(float));
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 5, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT1BID1W), (char*)0 + 14 * sizeof(float));
		m_vertexAttributes->SetVertexAttribPointer(m_vertexBuffer->GetBufferId(), 6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex1P1N1UV1T1BT1BID1W), (char*)0 + 18 * sizeof(unsigned int));
	}

	void Mesh::Bind()
	{
		m_vertexAttributes->Bind();

		if(m_elementBuffer != nullptr)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer->GetBufferId());
		}
	}

	void Mesh::BindTransformFeedback()
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback); 
	}

	void Mesh::Unbind()
	{
		m_vertexAttributes->Unbind();
	}

	void Mesh::Draw()
	{
		if(m_elementBuffer != nullptr)
		{
			if(m_wireFrame)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawElements(m_primitive, m_elementBuffer->GetNumElements(), GL_UNSIGNED_INT, 0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else if(m_noCulling)
			{
				glDisable(GL_CULL_FACE);
				glDrawElements(m_primitive, m_elementBuffer->GetNumElements(), GL_UNSIGNED_INT, 0);
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDrawElements(m_primitive, m_elementBuffer->GetNumElements(), GL_UNSIGNED_INT, 0);
			}
		}
		else if(m_transformFeedback != 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer->GetBufferId());
			if(m_wireFrame)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawTransformFeedback(m_primitive, m_transformFeedback);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else if(m_noCulling)
			{
				glDisable(GL_CULL_FACE);
				glDrawTransformFeedback(m_primitive, m_transformFeedback);
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDrawTransformFeedback(m_primitive, m_transformFeedback);
			}	
		}
		else
		{
			glDrawArrays(m_primitive, 0, m_elementBuffer->GetNumElements());
		}
	}

	void Mesh::DrawInstanced(GLsizei p_instances)
	{
		glDrawElementsInstanced(m_primitive, m_elementBuffer->GetNumElements(), GL_UNSIGNED_INT, 0, p_instances);
	}

	GLenum Mesh::GetPrimitiveType()
	{
		return m_primitive;
	}

	GLuint Mesh::GetTransformFeedback()
	{
		return m_transformFeedback;
	}

	void Mesh::SetPrimitiveType( GLenum p_type )
	{
		m_primitive = p_type;
	}

	void Mesh::SetTransformFeedback()
	{
		glGenTransformFeedbacks(1, &m_transformFeedback);
	}

	void Mesh::SetElementBuffer(BufferInterface* p_buffer)
	{
		m_elementBuffer = p_buffer;
	}

	void Mesh::SetVertexBuffer(BufferInterface* p_buffer)
	{
		m_vertexBuffer = p_buffer;
	}

	void Mesh::SetVertexAttribute(VertexAttributesInterface* p_attribute)
	{
		m_vertexAttributes = p_attribute;
	}

	BufferInterface* Mesh::GetVertexBuffer()
	{
		return m_vertexBuffer;
	}

	BufferInterface* Mesh::GetElementBuffer()
	{
		return m_elementBuffer;
	}

	VertexAttributesInterface* Mesh::GetVertexAttribute()
	{
		return m_vertexAttributes;
	}

	void Mesh::SetWireFrame( bool p_wireFrame )
	{
		m_wireFrame = p_wireFrame;
	}

	void Mesh::SetNoCulling( bool p_noCulling )
	{
		m_noCulling = p_noCulling;
	}

	void Mesh::FreeBuffers(RenderResourceManager* p_resources)
	{
		p_resources->ReleaseBuffer(m_vertexBuffer);
		p_resources->ReleaseBuffer(m_elementBuffer);
		p_resources->RemoveVAO(m_vertexAttributes);
	}
}