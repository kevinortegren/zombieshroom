#pragma once

#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/FreeList.h>
#include <vector>
#include <assert.h>

namespace ECS
{
	class ComponentAllocator
	{
	public:
		ComponentAllocator(size_t p_numComponents);
		~ComponentAllocator();

		template<class T>
		void CreateList(size_t p_numElements)
		{
			assert(sizeof(T) >= 8 && "Component size must atleast be 8 bytes.");

			unsigned id = Component<T>::GetTypeId();

			void* mem = operator new(p_numElements * sizeof(T));

			m_lists[id].first.Initialize(mem, sizeof(T), p_numElements);
			m_lists[id].second = mem;
		}

		template<class T>
		T* Allocate()
		{
			unsigned id = Component<T>::GetTypeId();

			T* ptr = static_cast<T*>(m_lists[id].first.Obtain());

			return ptr;
		}

		template<class T>
		void Free(T* p_ptr)
		{
			static_cast<T*>(ptr)->~T();

			unsigned id = Component<T>::GetTypeId();

			m_lists[id].first.Free(p_ptr);
		}

		void FreePtrFromList(void* p_ptr, int p_id);

		std::vector<std::pair<FreeList, void*>> m_lists;
	};
}