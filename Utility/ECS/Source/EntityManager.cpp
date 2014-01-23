#include <Utility\ECS\Include\EntityManager.h>
#include <Utility\ECS\Include\EntitySystemManager.h>

ECS::EntityManager::EntityManager(EntitySystemManager* p_systemManager)
	: m_nextID(0), m_systemManager(p_systemManager)
{
	m_components.resize(s_numComponents + 1);	
}

ECS::Entity* ECS::EntityManager::CreateEntity()
{
	int id = m_nextID;
	if(m_recyledIds.size() > 0)
	{
		id = m_recyledIds.top();
		m_recyledIds.pop();
		m_nextID--;
	}

	std::shared_ptr<ECS::Entity> entity = std::shared_ptr<ECS::Entity>(new ECS::Entity(id));
	m_entities.push_back(entity);
	m_nextID++;
	return entity.get();
}

void ECS::EntityManager::RemoveEntity(ECS::Entity* p_entity)
{
	m_recyledIds.push(p_entity->m_id);
	for (unsigned int i = 0; i < m_entities.size(); i++)
	{
		if(m_entities.at(i)->GetId() == p_entity->GetId())
		{
			m_entities.erase(m_entities.begin() + i);
			break;
		}
	}
}

std::vector<std::pair<unsigned int, ECS::ComponentInterface*>> ECS::EntityManager::GetAllComponents(Entity* p_entity)
{
	std::vector<std::pair<unsigned int, ECS::ComponentInterface*>> components;

	for (unsigned int type = 0; type < m_components.size(); ++type)
	{
		if (p_entity->m_id < m_components[type].size() && m_components[type][p_entity->m_id] != nullptr)
		{
			std::pair<unsigned int, ECS::ComponentInterface*> c;
			c.first = type;
			c.second = m_components[type][p_entity->m_id].get();
			
			components.push_back(c);
		}
	}

	return components;
}

void ECS::EntityManager::RemoveAllComponents(Entity* p_entity)
{
	for(size_t i = 0; i < m_components.size(); ++i) 
	{
		if(m_components[i].size() > p_entity->m_id) {
			m_components[i][p_entity->m_id] = nullptr;
			p_entity->m_componentTypes.set(i, 0);
		}
	}

	m_systemManager->RemoveEntityFromSystems(p_entity);
}

std::vector<std::shared_ptr<ECS::ComponentInterface>>& ECS::EntityManager::GetComponentList(int p_typeId)
 {  
	 return m_components[p_typeId]; 
}

void ECS::EntityManager::RemoveAllEntitiesAndComponents()
{
	int size = m_entities.size();
	for(int i = m_entities.size() - 1; i > -1; i--)
	{
		RemoveAllComponents(m_entities[i].get());
		m_recyledIds.push(m_entities[i]->GetId());
	}
	m_entities.clear();
}

std::vector<ECS::Entity*> ECS::EntityManager::GetAllEntities()
{
	std::vector<ECS::Entity*> result(m_entities.size());
	for (int i = 0; i < m_entities.size(); ++i)
	{
		result[i] = m_entities[i].get();
	}

	return result;
}
