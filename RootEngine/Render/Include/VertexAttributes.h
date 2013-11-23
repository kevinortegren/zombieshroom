#pragma once

#include <GL/glew.h>

namespace Render
{
	class VertexAttributes
	{
	public:
		VertexAttributes() :
			m_stride(0) { }
		~VertexAttributes();

		void Bind();
		void Unbind();

		void Init(size_t p_numAttribs);

		void SetFormat(GLuint p_location, GLint p_elementCount, GLint p_elementSize, GLenum p_type, GLenum p_normalized, GLuint p_offset);
		
		void SetVertexBuffer(GLuint p_bufferId, GLuint p_offset);
		void SetVertexBuffer(GLuint p_bufferId, GLuint p_bindingIndex, GLuint p_location, GLuint p_offset);

		size_t GetNumAttributes() const { return m_numAttribs; }
		size_t GetStride() const { return m_stride; }

	private:
		GLuint m_vao;
		size_t m_numAttribs;
		GLsizei m_stride;
	};
}