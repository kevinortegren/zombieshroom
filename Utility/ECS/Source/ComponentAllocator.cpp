#include "ComponentAllocator.h"

namespace ECS
{
	ComponentAllocator::ComponentAllocator(size_t p_numComponents)
	{
		m_lists.resize(p_numComponents);
	}

	ComponentAllocator::~ComponentAllocator()
	{
		for(auto itr = m_lists.begin(); itr != m_lists.end(); ++itr)
		{
			if((*itr).second != nullptr)
			{
				delete (*itr).second;
				(*itr).second = nullptr;
			}
		}
	}
}