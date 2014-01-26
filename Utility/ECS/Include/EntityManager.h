#pragma once

#include <Utility\ECS\Include\Entity.h>
#include <Utility\ECS\Include\Component.h>
#include <Utility\ECS\Include\ComponentAllocator.h>
#include <memory>
#include <vector>
#include <stack>
#include <iostream>
#include <assert.h>

namespace ECS
{
	class EntitySystemManager;

	class EntityManager
	{
	public:
		friend class World;
		friend class EntityExporter;
		friend class EntityImporter;

		EntityManager(EntitySystemManager* p_systemManager);

		Entity* CreateEntity();
		void RemoveEntity(ECS::Entity* p_entity);
		void RemoveAllEntitiesAndComponents();

		ComponentAllocator* GetAllocator();

		template<class T> 
		T* CreateComponent(Entity* p_entity)
		{
			// Check that the type id is initialized, otherwise the Cthulu will win the war!
			if(Component<T>::GetTypeId() == UINT_MAX)
			{
				printf("Attempting to create a component without initialized type id. See ECS::Component<T>::SetTypeId(..)");
				return nullptr;
			}

			T* component = new (m_allocator.Allocate<T>()) T;

			/* Component TypeId is enumerated 0,1,2.. etc.
			So we resize the component type vector to match the ids.
			*/
			if(Component<T>::GetTypeId() >= m_components.size())
				m_components.resize(Component<T>::GetTypeId() + 1);

			// By using the component id we can retrieve the vector of components of that type.
			std::vector<ComponentInterface*>& componentList = m_components[Component<T>::GetTypeId()];

			/* Entity ID is enumerated 0,1,2.. etc.
			So we resize the component vector to match the number of entities. */
			if(p_entity->m_id >= componentList.size())
				componentList.resize(p_entity->m_id + 1);

			// Use the entity id in the component vector and store the allocated component. 
			componentList[p_entity->m_id] = component;

			// Flag the entity to use the component.
			p_entity->m_componentTypes.set(Component<T>::GetTypeId());

			m_systemManager->AddEntityToSystems(p_entity);

			return component;
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
				m_allocator.Free<T>(m_components[Component<T>::GetTypeId()][p_entity->m_id]);

				m_components[Component<T>::GetTypeId()][p_entity->m_id] = nullptr;

				p_entity->m_componentTypes.set(Component<T>::GetTypeId(), 0);

				m_systemManager->RemoveEntityFromSystems(p_entity);
			}
		}

		template<class T>
		T* GetComponent(Entity* p_entity)
		{
			if (m_components.size() <= Component<T>::GetTypeId())
				return nullptr;
			if(p_entity->m_id >= m_components[Component<T>::GetTypeId()].size())
				return nullptr;

			return static_cast<T*>(m_components[Component<T>::GetTypeId()][p_entity->m_id]);
		}

		std::vector<std::pair<unsigned int, ComponentInterface*>> GetAllComponents(Entity* p_entity);

		template<class T>
		void RemoveAllComponentsOfType()
		{
			for(auto itr = m_entities.begin(); itr != m_entities.end(); ++itr)
			{
				RemoveComponent<T>((*itr).get());
			}
		}

		void RemoveAllComponents(Entity* p_entity);
		
		std::vector<ComponentInterface*>& GetComponentList(int p_typeId);

		int GetNumEntities() const { return m_entities.size(); }

		std::vector<ECS::Entity*> GetAllEntities();
	private:

		EntitySystemManager* m_systemManager;
		int m_nextID;
		std::vector<std::shared_ptr<Entity>> m_entities;
		std::stack<int> m_recyledIds;
		std::vector<std::vector<ComponentInterface*>> m_components; // CompID, EntityId, CompType.

		ComponentAllocator m_allocator;
	};
}