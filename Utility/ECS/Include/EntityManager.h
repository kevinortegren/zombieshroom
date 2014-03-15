#pragma once

#include <Utility\ECS\Include\Entity.h>
#include <Utility\ECS\Include\Component.h>
#include <Utility\ECS\Include\ComponentAllocator.h>
#include <memory>
#include <vector>
#include <set>
#include <stack>
#include <iostream>
#include <assert.h>

#define ECS_MAX_COMPONENTS 64
#define ECS_MAX_ENTITIES 10000

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

		// Creates an entity incrementing nextID.
		Entity* CreateEntity();

		void RemoveEntity(ECS::Entity* p_entity);
		void RemoveAllEntitiesAndComponents();

		ComponentAllocator* GetAllocator();

		template<class T> 
		T* CreateComponent(Entity* p_entity)
		{
			if(p_entity == nullptr)
				return nullptr;

			assert(Component<T>::GetTypeId() != UINT_MAX);

			// Allocate component.
			T* component = new (m_allocator.Allocate<T>()) T;

			assert(component != nullptr);

			// By using the component id we can retrieve the vector of components of that type.
			std::vector<ComponentInterface*>& componentList = m_components[Component<T>::GetTypeId()];

			/* Entity ID is enumerated 0,1,2.. etc.
			So we resize the component vector to match the number of entities. */
			if(p_entity->m_id >= (int)componentList.size())
				componentList.resize(p_entity->m_id + 1);

			// Use the entity id in the component vector and store the allocated component. 
			componentList[p_entity->m_id] = component;

			// Flag the entity to use the component.
			p_entity->m_flag |= (1ULL << Component<T>::GetTypeId());

			m_systemManager->AddEntityToSystems(p_entity);

			return component;
		}

		// Removes a component from an entity.
		template<class T> 
		void RemoveComponent(Entity* p_entity)
		{
			if(p_entity == nullptr)
				return;

			// Make sure the entity exist within the component list range.
			if((size_t) p_entity->m_id >= m_components[Component<T>::GetTypeId()].size())
				return;

		   //Entity does not have this component. Return to avoid re-toggling.
		   if ((p_entity->GetFlag() & (1ULL << Component<T>::GetTypeId())) == 0)
			   return;

			// If already removed, skip.
			if(m_components[Component<T>::GetTypeId()][p_entity->m_id] != nullptr)
			{
				// Push the type of component and the given entity.
				m_componentsToBeRemoved.insert(std::pair<unsigned int, unsigned int>(Component<T>::GetTypeId(), p_entity->GetId()));

			   p_entity->m_flag &= ~(1ULL << Component<T>::GetTypeId());

				m_systemManager->RemoveEntityFromSystems(p_entity);
			}
		}

		void CleanUp();

		template<class T>
		T* GetComponent(Entity* p_entity)
		{
			if(p_entity == nullptr)
				return nullptr;

			// Make sure the entity exist within the component list range.
			if(p_entity->m_id >= (int)m_components[Component<T>::GetTypeId()].size() || p_entity->m_id == -1)
				return nullptr;

			//Entity does not have this component. Return to avoid re-toggling.
			if ((p_entity->GetFlag() & (1ULL << Component<T>::GetTypeId())) == 0)
				return nullptr;

			return static_cast<T*>(m_components[Component<T>::GetTypeId()][p_entity->m_id]);
		}

		std::vector<std::pair<unsigned int, ComponentInterface*>> GetAllComponents(Entity* p_entity);

		template<class T>
		void RemoveAllComponentsOfType()
		{
			for(auto itr = m_entities.begin(); itr != m_entities.end(); ++itr)
			{
				RemoveComponent<T>(&(*itr));
			}
		}

		void RemoveAllComponents(Entity* p_entity);
		
		std::vector<ComponentInterface*>* GetComponentList(int p_typeId);

		int GetNumEntities() const { return m_entities.size(); }

		std::vector<ECS::Entity*> GetAllEntities();

		const std::set<int> GetEntitiesToBeRemoved() const;
	private:

		EntitySystemManager* m_systemManager;
		int m_nextID;
		std::vector<Entity> m_entities;
		std::stack<int> m_recycledIds;
		std::vector<std::vector<ComponentInterface*>> m_components; // CompID, EntityId, CompType.

		ComponentAllocator m_allocator;

		// Recyling sets.
		std::set<std::pair<unsigned int, unsigned int>> m_componentsToBeRemoved;
		std::set<Entity*> m_entitiesToBeRemoved;
	};
}