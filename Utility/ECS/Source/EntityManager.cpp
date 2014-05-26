#include <Utility\ECS\Include\EntityManager.h>
#include <Utility\ECS\Include\World.h>

ECS::EntityManager::EntityManager(World* p_world)
	: m_nextID(0), m_world(p_world), m_allocator(ECS_MAX_COMPONENTS)
{
	m_entities.resize(ECS_MAX_ENTITIES);
	m_components.resize(ECS_MAX_COMPONENTS);	
}

ECS::Entity* ECS::EntityManager::CreateEntity()
{
	int id = m_nextID;
	if(m_recycledIds.size() > 0)
	{
		id = m_recycledIds.back();
		m_recycledIds.pop_back();
		m_nextID--;
	}

	ECS::Entity* e = &m_entities[id];
	e->m_id = id;
	e->m_flag = 0;
	m_nextID++;

	Message m;
	m.m_type = MessageType::ENTITY_ADDED;
	m.m_entity = e;
	m_world->m_messages.push_back(m);

	return e;
}

void ECS::EntityManager::RemoveEntity(ECS::Entity* p_entity)
{
	if(p_entity == nullptr)
		return;

	// Remove the entity from all tags and groups
	m_world->GetTagManager()->Unregister(p_entity);
	m_world->GetGroupManager()->UnregisterEntityFromAllGroups(p_entity);

	Message m;
	m.m_type = MessageType::ENTITY_REMOVED;
	m.m_entity = p_entity;
	m_world->m_messages.push_back(m);

	m_entitiesToBeRemoved.insert(p_entity);

	// Delete components at cleanup stage.
	RemoveAllComponents(p_entity);
}

ECS::ComponentAllocator* ECS::EntityManager::GetAllocator()
{
	return &m_allocator;
}

std::vector<std::pair<unsigned int, ECS::ComponentInterface*>> ECS::EntityManager::GetAllComponents(Entity* p_entity)
{
	std::vector<std::pair<unsigned int, ECS::ComponentInterface*>> components;

	for (unsigned int type = 0; type < m_components.size(); ++type)
	{
		if (p_entity->m_id < (int)m_components[type].size() && m_components[type][p_entity->m_id] != nullptr)
		{
			std::pair<unsigned int, ECS::ComponentInterface*> c;
			c.first = type;
			c.second = m_components[type][p_entity->m_id];
			
			components.push_back(c);
		}
	}

	return components;
}

void ECS::EntityManager::RemoveAllComponents(Entity* p_entity)
{
	if(p_entity == nullptr)
		return;

	for(unsigned i = 0; i < m_components.size(); ++i)
	{
		if(p_entity->m_id < (int)m_components[i].size())
		{
			if(m_components[i][p_entity->m_id] != nullptr)
			{
				Message m;
				m.m_type = MessageType::COMPONENT_REMOVED;
				m.m_entity = p_entity;
				m.m_compType = i;
				m_world->m_messages.push_back(m);

				// Push the type of component and the given entity.
				m_componentsToBeRemoved.insert(std::pair<unsigned int, unsigned int>(i, p_entity->GetId()));
			}
		}
	}	
}

std::vector<ECS::ComponentInterface*>* ECS::EntityManager::GetComponentList(int p_typeId)
 {  
	 return &m_components[p_typeId]; 
}

void ECS::EntityManager::RemoveAllEntitiesAndComponents()
{
	int size = m_nextID;
	for(int i = size - 1; i > -1; i--)
	{
		RemoveEntity(&m_entities[i]);
	}

	ClearRecyledEntitiesStack();

	m_nextID = 0;
}

std::vector<ECS::Entity*> ECS::EntityManager::GetAllEntities()
{
	std::vector<ECS::Entity*> result;
	result.reserve(m_nextID);
	for (int i = 0; i < m_nextID; ++i)
	{
		if(m_entities[i].m_id != -1)
			result.push_back(&m_entities[i]);
	}

	return result;
}

bool ECS::EntityManager::IsEntityActive(ECS::Entity* p_entity)
{
	return (std::find(m_recycledIds.begin(), m_recycledIds.end(), p_entity->GetId()) != m_recycledIds.end());
}

void ECS::EntityManager::CleanUp()
{
	m_world->TestMessaging();

	// Loop through components to remove.
	for(auto itr = m_componentsToBeRemoved.begin(); itr != m_componentsToBeRemoved.end(); ++itr)
	{
		// Resolve component type.
		auto component = m_components[(*itr).first][(*itr).second];

		// Run deconstructor and free from allocator list.
		m_allocator.FreePtrFromList(component, (*itr).first);

		// Null the component at the given slot.
		m_components[(*itr).first][(*itr).second] = nullptr;
	}
	m_componentsToBeRemoved.clear();

	for(auto itr = m_entitiesToBeRemoved.begin(); itr != m_entitiesToBeRemoved.end(); ++itr)
	{
		// Recyle id.
		m_recycledIds.push_back((*itr)->GetId());

		(*itr)->m_flag = 0;

		m_world->m_systemManager.RemoveEntityFromSystems((*itr));
	}

	m_entitiesToBeRemoved.clear();

	m_world->m_messages.clear();
}

const std::set<int> ECS::EntityManager::GetEntitiesToBeRemoved() const
{
	std::set<int> entityIds;

	for(auto itr = m_entitiesToBeRemoved.begin(); itr != m_entitiesToBeRemoved.end(); ++itr)
	{
		entityIds.insert((*itr)->GetId());
	}

	return entityIds;
}

void ECS::EntityManager::ClearRecyledEntitiesStack()
{
	// Clear stack.
	m_recycledIds.clear();
}

