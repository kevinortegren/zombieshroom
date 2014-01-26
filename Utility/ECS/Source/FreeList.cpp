#include "FreeList.h"

void FreeList::Initialize(void* p_start, unsigned p_elementSize, unsigned p_numElements)
{
	union 
	{
		void* as_void;
		char* as_char;
		FreeList* as_self;
	};

	as_void = p_start;
	m_next = as_self;

	FreeList* runner = m_next;
	for (unsigned i = 0; i < p_numElements; ++i)
	{	
		runner->m_next = as_self;
		runner = as_self;
		as_char += p_elementSize;
	}

	runner->m_next = nullptr;
};

void* FreeList::Obtain()
{
  if (m_next == nullptr)
  {
    return nullptr;
  }
 
  FreeList* head = m_next;
  m_next = head->m_next;
  return head;
}
 
void FreeList::Free(void* ptr)
{
  FreeList* head = static_cast<FreeList*>(ptr);
  head->m_next = m_next;
  m_next = head;
}