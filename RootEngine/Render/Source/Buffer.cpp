#include <RootEngine/Render/Include/Buffer.h>

Render::Buffer::Buffer()
	: m_numElements(0), m_size(0)
{

}

Render::Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_id);
}

void Render::Buffer::Init(GLenum p_type)
{
	m_type = p_type;

	glGenBuffers(1, &m_id);
}

/*void Render::GraphicsBuffer::SetVertexAttribPointer(GLuint p_index, GLuint p_location, GLint p_size, GLenum p_type, GLboolean p_normalized, GLsizei p_stride, GLvoid* p_pointer)
{
	glBindVertexBuffer(0, buff, baseOffset, sizeof(Vertex));

	assert(p_location < m_numAttribs);

	BindVertexArray();
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[p_index]);
	glVertexAttribPointer( p_location, p_size, p_type, p_normalized, p_stride, p_pointer );
	UnbindVertexArray();

	glVertexAttribFormat(p_location, 3, GL_FLOAT, GL_FALSE, offsetof(position, Vertex));
	glVertexAttribBinding(p_location, 0);

	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(normal, Vertex));
	glVertexAttribBinding(1, 0);

	glVertexAttribFormat(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(color, Vertex));
	glVertexAttribBinding(2, 0);

}*/

void Render::Buffer::BufferData(size_t p_elementCount, size_t p_elementSize, void* p_data)
{
	glBindBuffer(m_type, m_id);
	glBufferData(m_type, p_elementCount * p_elementSize, p_data, GL_STATIC_DRAW);

	m_numElements += p_elementCount;
	m_size += p_elementCount * p_elementSize;
}

void Render::Buffer::BufferSubData(size_t p_offset, size_t p_length, void* p_data)
{
	glBindBuffer(m_type, m_id);
	glBufferSubData(m_type, p_offset, p_length, p_data);
}
