#pragma once

#include <GL/glew.h>

namespace Render
{
	class GraphicsBuffer
	{
	public:
		GraphicsBuffer();
		~GraphicsBuffer();

		void Init(int p_numAttribs);
		void Init(int p_numVBO, int p_numAttribs);

		void BindVertexArray();
		void UnbindVertexArray();

		void SetVertexAttribPointer(GLuint p_index, GLuint p_location, GLint p_size, GLenum p_type, GLboolean p_normalized, GLsizei p_stride, GLvoid* p_pointer);

		void BufferData(GLuint p_index, size_t p_elementCount, size_t p_elementSize, void* p_data);
		void BufferSubData(GLuint p_index, size_t p_offset, size_t p_length, void* p_data);

		void BufferIndexData( size_t p_elementCount, const GLvoid *data );

		GLuint m_vao;
		GLuint* m_vbo;
		GLuint m_ebo;
		
		size_t m_numVertices;
		size_t m_numIndices;
		size_t m_numAttribs;
		size_t m_numVBO;
	};
}
