#pragma once

#include <RootForce\Include\ECS/EntityManager.h>
#include <vector>

namespace ECS
{
	template<class T>
	class ComponentMapper
	{
	public:
		void Init(ECS::EntityManager* p_entityManager)
		{
			m_componentList = p_entityManager->GetComponentList(Component<T>::GetTypeId());
		}

		T* Get(Entity* p_entity)
		{
			return static_cast<T*>(m_componentList->at(p_entity->GetId()).get());
		}
		
	private:
		std::vector<std::shared_ptr<ECS::ComponentInterface>>* m_componentList;
	};
}