#pragma once

#include <ECS/Entity.h>
#include <ECS/Component.h>
#include <memory>
#include <vector>
#include <stack>
#include <iostream>
#include <assert.h>

namespace ECS
{
	class ComponentSystemManager;

	class EntityManager
	{
	public:
		EntityManager(ComponentSystemManager* p_systemManager)
			: m_nextID(0), m_systemManager(p_systemManager) 
		{
			m_components.resize(ECS::s_count);
		}

		Entity* CreateEntity();
		void RemoveEntity(ECS::Entity* p_entity);

		template<class T> 
		T* CreateComponent(Entity* p_entity)
		{
			// Allocate memory for component.
			std::shared_ptr<T> component = std::shared_ptr<T>(new T);
			
			/* Component TypeId is enumerated 0,1,2.. etc.
			So we resize the component type vector to match the ids.
			*/
			if(Component<T>::GetTypeId() >= m_components.size())
				m_components.resize(Component<T>::GetTypeId() + 1);


			// By using the component id we can retrieve the vector of components of that type.
			std::vector<std::shared_ptr<ComponentInterface>>& componentList = m_components[Component<T>::GetTypeId()];

			/* Entity ID is enumerated 0,1,2.. etc.
			So we resize the component vector to match the number of entities. */
			if(p_entity->m_id >= componentList.size())
				componentList.resize(p_entity->m_id + 1);

			// Use the entity id in the component vector and store the allocated component. 
			componentList[p_entity->m_id] = component;

			// Flag the entity to use the component.
			p_entity->m_componentTypes.set(Component<T>::GetTypeId());

			m_systemManager->AddEntityToSystems(p_entity);

			return component.get();
		}

		template<class T>
		void AddComponent(T* p_component, Entity* entity)
		{
			assert(p_entity->m_id < m_components[Component<T>::GetTypeId()].size());
		
			m_components[Component<T>::GetTypeId()][p_entity->m_id] = p_component;

			p_entity->m_componentTypes.set(Component<T>::GetTypeId());

		}

		template<class T> 
		void RemoveComponent(Entity* p_entity)
		{
			if(p_entity->m_id > m_components[Component<T>::GetTypeId()].size())
			{
				m_components[Component<T>::GetTypeId()][p_entity->m_id] = nullptr;

				p_entity->m_componentTypes.set(Component<T>::GetTypeId(), 0);

				m_systemManager->RemoveEntityFromSystems(p_entity);
			}
		}

		template<class T>
		void RemoveAllComponentsOfType()
		{
			for(auto itr = m_entities.begin(); itr != m_entities.end(); ++itr)
			{
				RemoveComponent<T>((*itr));
			}
		}

		void RemoveAllComponents(Entity* p_entity);
		
		std::vector<std::shared_ptr<ComponentInterface>>* GetComponentList(int p_typeId);
		int GetNumEntities() const { return m_entities.size(); }

	private:

		ComponentSystemManager* m_systemManager;
		int m_nextID;
		std::vector<std::shared_ptr<Entity>> m_entities;
		std::stack<int> m_recyledIds;
		std::vector<std::vector<std::shared_ptr<ComponentInterface>>> m_components;
	};
}