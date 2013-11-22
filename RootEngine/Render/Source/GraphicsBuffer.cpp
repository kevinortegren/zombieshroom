#include <RootEngine/Render/Include/GraphicsBuffer.h>
#include <assert.h>

Render::GraphicsBuffer::GraphicsBuffer()
{

}

Render::GraphicsBuffer::~GraphicsBuffer()
{
	delete[] m_vbo;
}

void Render::GraphicsBuffer::Init(int p_numAttribs)
{
	Init(1, p_numAttribs);
}

void Render::GraphicsBuffer::Init(int p_numVBO, int p_numAttribs)
{
	m_vbo = new GLuint[p_numVBO];
	glGenBuffers(p_numVBO, m_vbo);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	for(size_t i = 0; i < p_numAttribs; ++i)
		glEnableVertexAttribArray(i);
	
	m_numAttribs = p_numAttribs;
	m_numVBO = p_numVBO;
}

void Render::GraphicsBuffer::BindVertexArray()
{
	glBindVertexArray(m_vao);
}

void Render::GraphicsBuffer::UnbindVertexArray()
{
	glBindVertexArray(0);
}

void Render::GraphicsBuffer::SetVertexAttribPointer(GLuint p_index, GLuint p_location, GLint p_size, GLenum p_type, GLboolean p_normalized, GLsizei p_stride, GLvoid* p_pointer)
{
	assert(p_location < m_numAttribs);

	BindVertexArray();
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[p_index]);
	glVertexAttribPointer( p_location, p_size, p_type, p_normalized, p_stride, p_pointer );
	UnbindVertexArray();
}

void Render::GraphicsBuffer::BufferData(GLuint p_index, size_t p_elementCount, size_t p_elementSize, void* p_data)
{
	assert(p_index < m_numVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[p_index]);
	glBufferData(GL_ARRAY_BUFFER, p_elementCount * p_elementSize, p_data, GL_STATIC_DRAW);
}

void Render::GraphicsBuffer::BufferSubData(GLuint p_index, size_t p_offset, size_t p_length, void* p_data)
{
	assert(p_index < m_numVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[p_index]);
	glBufferSubData(GL_ARRAY_BUFFER, p_offset, p_length, p_data);
}

void Render::GraphicsBuffer::BufferIndexData( size_t p_elementCount, const GLvoid *data )
{
	if( !m_ebo )
		glGenBuffers( 1, &m_ebo );
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, p_elementCount * sizeof(GLuint), data, GL_STATIC_DRAW);
}
