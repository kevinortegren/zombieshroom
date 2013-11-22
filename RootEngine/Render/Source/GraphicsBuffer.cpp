#include <RootEngine/Render/Include/GraphicsBuffer.h>

void Render::GraphicsBuffer::Init(int p_numAttribs)
{
	glGenBuffers(1, &m_vbo);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	

	for(size_t i = 0; i < p_numAttribs; ++i)
	{
		glEnableVertexAttribArray(i);
	}
}

void Render::GraphicsBuffer::BindVertexArray()
{
	glBindVertexArray(m_vao);
}

void Render::GraphicsBuffer::UnbindVertexArray()
{
	glBindVertexArray(0);
}

void Render::GraphicsBuffer::SetVertexAttribPointer(GLuint p_location, GLint p_size, GLenum p_type, GLboolean p_normalized, GLsizei p_stride, GLvoid* p_pointer)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer( p_location, p_size, p_type, p_normalized, p_stride, p_pointer );
}

void Render::GraphicsBuffer::BufferData(GLuint p_vbo, size_t p_elementCount, size_t p_elementSize, void* p_data)
{
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
	glBufferData(GL_ARRAY_BUFFER, p_elementCount * p_elementSize, p_data, GL_STATIC_DRAW);
}