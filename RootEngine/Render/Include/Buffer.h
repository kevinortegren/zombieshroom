#pragma once

#include <GL/glew.h>

namespace Render
{
	class Buffer
	{
	public:
		Buffer();
		~Buffer();

		void Init(GLenum p_type);

		void BufferData(size_t p_elementCount, size_t p_elementSize, void* p_data);
		void BufferSubData(size_t p_offset, size_t p_length, void* p_data);

		GLuint GetBufferId() const { return m_id; }
		GLenum GetBufferType() const { return m_type; }
		size_t GetNumElements() const { return m_numElements; }
		size_t GetBufferSize() const { return m_size; }

	private:
		GLuint m_id;
		GLenum m_type;
		size_t m_numElements;
		size_t m_size;
	};
}
