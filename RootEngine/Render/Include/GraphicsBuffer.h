#pragma once

#include <GL/glew.h>

namespace Render
{
	struct GraphicsBuffer
	{
		GLuint m_vao;
		GLuint m_ebo;
		GLuint m_vbo;
		size_t m_numVertices;
		size_t m_numIndices;
	};

	inline void BindVertexArray(GraphicsBuffer* p_buffer)
	{
		glBindVertexArray(p_buffer->m_vao);
	}

	inline void InitGraphicsBuffer(GraphicsBuffer* p_buffer, size_t p_numAttribs)
	{
		glGenVertexArrays(1, &p_buffer->m_vao);
		glBindVertexArray(p_buffer->m_vao);

		glGenBuffers(1, &p_buffer->m_vbo);

		for(size_t i = 0; i < p_numAttribs; ++i)
		{
			glEnableVertexAttribArray(i);
		}
	}

	inline void SetVertexAtribPointer( GLuint p_vbo, GLuint p_location, GLint p_size, GLenum p_type, GLboolean p_normalized, GLsizei p_stride, GLvoid* p_pointer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
		glVertexAttribPointer( p_location, p_size, p_type, p_normalized, p_stride, p_pointer );
	}

	inline void BufferData(GLuint p_vbo, size_t p_elementCount, size_t p_elementSize, void* p_data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
		glBufferData(GL_ARRAY_BUFFER, p_elementCount * p_elementSize, p_data, GL_STATIC_DRAW);
	}
}
