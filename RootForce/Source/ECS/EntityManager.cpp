#include <ECS/EntityManager.h>
#include <ECS/ComponentSystemManager.h>

ECS::EntityManager::EntityManager(ComponentSystemManager* p_systemManager)
	: m_nextID(0), m_systemManager(p_systemManager) 
{
	m_components.resize(10);	
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
	m_entities.erase(m_entities.begin() + p_entity->m_id);
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