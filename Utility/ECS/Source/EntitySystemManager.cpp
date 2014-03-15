#include <Utility\ECS\Include\EntitySystemManager.h>
#include <algorithm>

void ECS::EntitySystemManager::Clear()
{
	for(auto itr = m_systems.begin(); itr != m_systems.end(); ++itr)
	{
		(*itr)->m_entitiesToRemove.clear();
		(*itr)->m_activeEntities.clear();
	}
}

void ECS::EntitySystemManager::AddEntityToSystems(Entity* p_entity)
{
	for(auto itr = m_systems.begin(); itr != m_systems.end(); ++itr)
	{
		if(((*itr)->m_flag & p_entity->m_flag) == (*itr)->m_flag)
		{
			// Add it to the active entities.
			(*itr)->m_activeEntities.insert(p_entity);

			//TODO: Probely not needed.

			// If it still exists in the remove list, remove it from that as well.
			/*auto it = std::find((*itr)->m_entitiesToRemove.begin(), (*itr)->m_entitiesToRemove.end(), p_entity);
			while (it != (*itr)->m_entitiesToRemove.end())
			{
				(*itr)->m_entitiesToRemove.erase(it);
				it = std::find((*itr)->m_entitiesToRemove.begin(), (*itr)->m_entitiesToRemove.end(), p_entity);
			}*/
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
				(*itr)->m_entitiesToRemove.insert(p_entity);
			}
		}
	}
}

