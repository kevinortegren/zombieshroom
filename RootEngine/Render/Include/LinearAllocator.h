#pragma once

namespace Render
{
	class LinearAllocator
	{
	public:
		LinearAllocator(unsigned int stackSize_bytes );
		LinearAllocator(unsigned int stackSize_bytes, unsigned int alignment);
		~LinearAllocator();

		void* Alloc( unsigned int size_bytes );
		void Clear();

	private:
		void* m_mem;
		void* m_marker;
	};
}