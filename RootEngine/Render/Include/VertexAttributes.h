#pragma once

#include <GL/glew.h>

namespace Render
{
	class VertexAttributesInterface
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0; 
		virtual void Init(size_t p_numAttribs) = 0;
		virtual void SetVertexAttribPointer(GLuint p_bufferId, GLuint p_location, GLint p_size,
			GLenum p_type, GLboolean p_normalized, GLsizei p_stride, GLvoid* p_pointer) = 0;
	};

	class VertexAttributes : public VertexAttributesInterface
	{
	public:
		VertexAttributes() :
			m_stride(0) { }
		~VertexAttributes();

		void Bind();
		void Unbind();
		void Init(size_t p_numAttribs);
		void SetVertexAttribPointer(GLuint p_bufferId, GLuint p_location, GLint p_size,
			GLenum p_type, GLboolean p_normalized, GLsizei p_stride, GLvoid* p_pointer);
		size_t GetNumAttributes() const { return m_numAttribs; }

	private:

		void SetFormat(GLuint p_location, GLint p_elementCount, GLint p_elementSize, GLenum p_type, GLenum p_normalized, GLuint p_offset);	
		void SetVertexBuffer(GLuint p_bufferId, GLuint p_offset);
		void SetVertexBuffer(GLuint p_bufferId, GLuint p_bindingIndex, GLuint p_location, GLuint p_offset);
		size_t GetStride() const { return m_stride; }

		GLuint m_vao;
		size_t m_numAttribs;
		GLsizei m_stride;
	};
}