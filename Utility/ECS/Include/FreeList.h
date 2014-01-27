#pragma once

struct FreeList
{
	void Initialize(void* p_start, unsigned p_elementSize, unsigned p_numElements);
	void* Obtain();
	void Free(void* p_ptr);

	FreeList* m_next;
};
