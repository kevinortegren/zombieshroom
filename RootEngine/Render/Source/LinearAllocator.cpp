#include <RootEngine/Render/Include/LinearAllocator.h>
#include <cstdlib>

Render::LinearAllocator::LinearAllocator( unsigned int stackSize_bytes )
{
	m_mem = malloc(stackSize_bytes);
	m_marker = m_mem;
}

Render::LinearAllocator::LinearAllocator(unsigned int stackSize_bytes, unsigned int alignment)
{
	m_mem = _aligned_malloc(stackSize_bytes, alignment);
	m_marker = m_mem;
}

Render::LinearAllocator::~LinearAllocator()
{
	if(m_mem != 0) {
		_aligned_free(m_mem);
		m_mem = 0;
	}
}

void* Render::LinearAllocator::Alloc( unsigned int size_bytes )
{
	void* ptr = (char*)m_marker;
	m_marker = (char*)ptr + size_bytes;
	return ptr;
}

void Render::LinearAllocator::Clear()
{
	m_marker = m_mem;
}