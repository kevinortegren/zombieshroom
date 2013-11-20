#pragma once

#include <ECS/Entity.h>
#include <ECS/ComponentInterface.h>
#include <memory>
#include <vector>
#include <stack>
#include <iostream>
#include <assert.h>

namespace ECS
{
	class EntityManager
	{
	public:
		EntityManager();

		std::shared_ptr<Entity> CreateEntity();
		void RemoveEntity(std::shared_ptr<Entity> p_entity);

		template<class T> 
		std::shared_ptr<T> CreateComponent(std::shared_ptr<Entity> p_entity)
		{
			// Allocate memory for component.
			std::shared_ptr<T> component = std::shared_ptr<T>(new T);

			std::cout << Component<T>::GetTypeId() << std::endl;

			/* Component TypeId is enumerated 0,1,2.. etc.
			So we resize the component type vector to match the ids.
			*/
			if(Component<T>::GetTypeId() >= m_components.size())
				m_components.resize(Component<T>::GetTypeId() + 1);

			// By using the component id we can retrieve the vector of components of that type.
			std::vector<std::shared_ptr<ComponentInterface>>& componentList = m_components[Component<T>::GetTypeId()];

			/* Entity ID is enumerated 0,1,2.. etc.
			So we resize the component vector to match the number of entities. */
			if(p_entity->GetId() >= componentList.size())
				componentList.resize(p_entity->GetId() + 1);

			// Use the entity id in the component vector and store the allocated component. 
			componentList[p_entity->GetId()] = component;

			// Return a pointer to the component for editing. 
			return component;
		}

		template<class T>
		void AddComponent(std::shared_ptr<T> p_component, std::shared_ptr<Entity> p_entity)
		{
			assert(p_entity->GetId() < m_components[Component<T>::GetTypeId()].size());
		
			m_components[Component<T>::GetTypeId()][p_entity->GetId()] = p_component;
		}

		template<class T> 
		void RemoveComponent(std::shared_ptr<Entity> p_entity)
		{
			m_components[Component<T>::GetTypeId()][p_entity->GetId()] = nullptr;
		}

		template<class T>
		void RemoveAllComponentsOfType()
		{
			for(auto itr = m_components[Component<T>::GetTypeId()].begin(); itr != m_components[Component<T>::GetTypeId()].end(); ++itr)
			{
				(*itr) = nullptr;
			}
		}

		void RemoveAllComponents(std::shared_ptr<Entity> p_entity);
		

	private:
		int m_nextID;
		std::vector<std::shared_ptr<Entity>> m_entities;
		std::stack<int> m_recyledIds;
		std::vector<std::vector<std::shared_ptr<ComponentInterface>>> m_components;
	};
}