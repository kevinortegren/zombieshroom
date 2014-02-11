#include <Utility\ECS\Include\EntityManager.h>
#include <Utility\ECS\Include\EntitySystemManager.h>

ECS::EntityManager::EntityManager(EntitySystemManager* p_systemManager)
	: m_nextID(0), m_systemManager(p_systemManager), m_allocator(ECS_MAX_COMPONENTS)
{
	m_entities.resize(ECS_MAX_ENTITIES);
	m_components.resize(ECS_MAX_COMPONENTS);	
}

ECS::Entity* ECS::EntityManager::CreateEntity()
{
	int id = m_nextID;
	if(m_recycledIds.size() > 0)
	{
		id = m_recycledIds.top();
		m_recycledIds.pop();
		m_nextID--;
	}

	ECS::Entity* e = &m_entities[id];
	e->m_id = id;
	e->m_flag = 0;
	m_nextID++;

	return e;
}

void ECS::EntityManager::RemoveEntity(ECS::Entity* p_entity)
{
	m_recycledIds.push(p_entity->m_id);

	p_entity->m_id = -1;
	p_entity->m_flag = 0;
	
	p_entity = nullptr;
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
	if (p_entity != nullptr && p_entity->m_id != -1)
	{
		for(unsigned i = 0; i < m_components.size(); ++i)
		{
			if(p_entity->m_id > -1 && p_entity->m_id < (int)m_components[i].size())
			{
				if(m_components[i][p_entity->m_id] != nullptr)
				{
					m_allocator.FreePtrFromList(m_components[i][p_entity->m_id], i);
					m_components[i][p_entity->m_id] = nullptr;
				}
			}
		}

		p_entity->m_flag = 0;
		m_systemManager->RemoveEntityFromSystems(p_entity);
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
		RemoveAllComponents(&m_entities[i]);
	}

	// Clear stack.
	while (!m_recycledIds.empty() )
	{
		m_recycledIds.pop();
	}

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
