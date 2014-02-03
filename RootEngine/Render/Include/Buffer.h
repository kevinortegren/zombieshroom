#pragma once

#include <GL/glew.h>

namespace Render
{
	class BufferInterface
	{
	public:
		virtual void BufferData(size_t p_elementCount, size_t p_elementSize, void* p_data) = 0;
		virtual void BufferSubData(size_t p_offset, size_t p_length, void* p_data) = 0;
		virtual GLuint GetBufferId() const = 0;
		virtual GLenum GetBufferType() const = 0;
		virtual size_t GetBufferSize() const = 0;
		virtual size_t GetElementSize() const  = 0;
		virtual size_t GetNumElements() const = 0;
	};

	class Buffer : public BufferInterface
	{
	public:
		friend class RenderResourceManager;

		void BufferData(size_t p_elementCount, size_t p_elementSize, void* p_data);
		void BufferSubData(size_t p_offset, size_t p_length, void* p_data);

		GLuint GetBufferId() const { return m_id; }
		GLenum GetBufferType() const { return m_type; }
		size_t GetBufferSize() const { return m_size; }
		size_t GetElementSize() const { return m_elementSize; }
		size_t GetNumElements() const { return m_numElements; }

	private:
		Buffer(GLenum p_type);
		~Buffer();

		GLuint m_id;
		GLenum m_type;
		size_t m_size;
		size_t m_elementSize;
		size_t m_numElements;
	};
}
