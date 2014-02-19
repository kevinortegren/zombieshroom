#include <Utility\ECS\Include\EntitySystemManager.h>

void ECS::EntitySystemManager::AddEntityToSystems(Entity* p_entity)
{
	for(auto itr = m_systems.begin(); itr != m_systems.end(); ++itr)
	{
		if(((*itr)->m_flag & p_entity->m_flag) == (*itr)->m_flag)
		{
			(*itr)->m_activeEntities.insert(p_entity);
			//(*itr)->Init();
		}
	}
}

void ECS::EntitySystemManager::RemoveEntityFromSystems(Entity* p_entity)
{
	for(auto itr = m_systems.begin(); itr != m_systems.end(); ++itr)
	{
		if((*itr)->m_activeEntities.find(p_entity) != (*itr)->m_activeEntities.end())
		{
			if(((*itr)->m_flag & p_entity->m_flag) != (*itr)->m_flag)
			{
				(*itr)->m_activeEntities.erase(p_entity);
				//(*itr)->Init();
			}
		}
	}
}

