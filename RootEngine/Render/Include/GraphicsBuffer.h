#pragma once

#include <GL/glew.h>

namespace Render
{
	class GraphicsBuffer
	{
	public:

		GraphicsBuffer() : 
			m_vao(0),
			m_ebo(0),
			m_vbo(0),
			m_numVertices(0),
			m_numIndices(0)
		{}

		void Init(int p_numAttribs);
		void BindVertexArray();
		void UnbindVertexArray();

		void SetVertexAttribPointer(GLuint p_location, GLint p_size, GLenum p_type, GLboolean p_normalized, GLsizei p_stride, GLvoid* p_pointer);

		void BufferData(GLuint p_vbo, size_t p_elementCount, size_t p_elementSize, void* p_data);

		GLuint m_vao;
		GLuint m_ebo;
		GLuint m_vbo;
		size_t m_numVertices;
		size_t m_numIndices;
	};
}
