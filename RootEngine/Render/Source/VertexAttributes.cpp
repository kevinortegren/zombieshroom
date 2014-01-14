#include <RootEngine/Render/Include/VertexAttributes.h>

#include <GL/glew.h>
#include <assert.h>

namespace Render
{
	VertexAttributes::~VertexAttributes()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void VertexAttributes::Bind()
	{
		glBindVertexArray(m_vao);
	}

	void VertexAttributes::Unbind()
	{
		glBindVertexArray(0);
	}

	void VertexAttributes::Init(size_t p_numAttribs)
	{
		glGenVertexArrays(1, &m_vao);
		Bind();

		for(size_t i = 0; i < p_numAttribs; ++i) 
		{
			glEnableVertexAttribArray(i);
		}

		m_numAttribs = p_numAttribs;
		Unbind();
	}

	void VertexAttributes::SetFormat(GLuint p_location, GLint p_elementCount, GLint p_elementSize, GLenum p_type, GLenum p_normalized, GLuint p_offset)
	{
		Bind();
		glVertexAttribFormat(p_location, p_elementCount, p_type, p_normalized, p_offset);

		m_stride += p_elementCount * p_elementSize;
		Unbind();
	}

	void VertexAttributes::SetVertexBuffer(GLuint p_bufferId, GLuint p_offset)
	{
		Bind();
		for(size_t i = 0; i < m_numAttribs; ++i) 
		{
			glBindVertexBuffer(0, p_bufferId, p_offset, m_stride);
			glVertexAttribBinding(i, 0);
		}
		Unbind();
	}

	void VertexAttributes::SetVertexBuffer(GLuint p_bufferId, GLuint p_bindingIndex, GLuint p_location, GLuint p_offset)
	{
		Bind();
		glBindVertexBuffer(p_bindingIndex, p_bufferId, p_offset, m_stride);
		glVertexAttribBinding(p_location, p_bindingIndex);
		Unbind();
	}

	void VertexAttributes::SetVertexAttribPointer(GLuint p_bufferId, GLuint p_location, GLint p_size, GLenum p_type, GLboolean p_normalized, GLsizei p_stride, GLvoid* p_pointer)
	{
		assert(p_location < m_numAttribs);

		Bind();
		glBindBuffer(GL_ARRAY_BUFFER,p_bufferId);
		if(p_type == GL_UNSIGNED_INT)
			glVertexAttribIPointer( p_location, p_size, p_type, p_stride, p_pointer );
		else
			glVertexAttribPointer( p_location, p_size, p_type, p_normalized, p_stride, p_pointer );
		Unbind();
	}

}