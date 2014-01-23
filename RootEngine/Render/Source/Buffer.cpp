#include <RootEngine/Render/Include/Buffer.h>

Render::Buffer::Buffer(GLenum p_type)
	: m_numElements(0), m_size(0), m_elementSize(0)
{
	m_type = p_type;
	glGenBuffers(1, &m_id);
}

Render::Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_id);
}

void Render::Buffer::BufferData(size_t p_elementCount, size_t p_elementSize, void* p_data)
{
	glBindBuffer(m_type, m_id);
	glBufferData(m_type, p_elementCount * p_elementSize, p_data, GL_DYNAMIC_DRAW);

	m_elementSize = p_elementSize;
	m_numElements = p_elementCount;
	m_size = p_elementCount * p_elementSize;
}

void Render::Buffer::BufferSubData(size_t p_offset, size_t p_length, void* p_data)
{
	glBindBuffer(m_type, m_id);
	glBufferSubData(m_type, p_offset, p_length, p_data);
}
