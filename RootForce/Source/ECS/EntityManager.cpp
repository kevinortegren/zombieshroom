#include <ECS/EntityManager.h>

ECS::EntityManager::EntityManager()
	: m_nextID(0)
{

}

std::shared_ptr<ECS::Entity> ECS::EntityManager::CreateEntity()
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
	return entity;
}

void ECS::EntityManager::RemoveEntity(std::shared_ptr<ECS::Entity> p_entity)
{
	m_recyledIds.push(p_entity->GetId());
	m_entities.erase(m_entities.begin() + p_entity->GetId());
}