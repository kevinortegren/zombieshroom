#include <ECS/ComponentSystemManager.h>

void ECS::ComponentSystemManager::AddEntityToSystems(std::shared_ptr<ECS::Entity> p_entity)
{
	for(auto itr = m_systems.begin(); itr != m_systems.end(); ++itr)
	{
		// Validate entity against system.
		for(int i = 0; i < (*itr).second->m_componentTypes.size(); ++i)
		{
			if((*itr).second->m_componentTypes.at(i))
				if(!p_entity->m_componentTypes.at(i))
					return; // Validation fails.
		}

		// Validation sucess insert the entity in the system.
		(*itr).second->m_activeEntities.insert(p_entity);
	}
}

void ECS::ComponentSystemManager::RemoveEntityFromSystems(std::shared_ptr<ECS::Entity> p_entity)
{
	for(auto itr = m_systems.begin(); itr != m_systems.end(); ++itr)
	{
		// Validate entity against system.
		for(int i = 0; i < (*itr).second->m_componentTypes.size(); ++i)
		{
			if((*itr).second->m_componentTypes.at(i))
				if(!p_entity->m_componentTypes.at(i))
				{
					// Everytime validation fails for a system agains a entity, if the entity exist remove it.
					if((*itr).second->m_activeEntities.find(p_entity) != (*itr).second->m_activeEntities.end())
						(*itr).second->m_activeEntities.erase(p_entity);
				}
		}
	}
}

void ECS::ComponentSystemManager::InitializeSystems()
{
	for(auto itr = m_systems.begin(); itr != m_systems.end(); ++itr)
	{
		(*itr).second->Init();
	}
}
