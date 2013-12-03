#include <Utility\ECS\Include\EntitySystemManager.h>

ECS::EntitySystemManager::~EntitySystemManager()
{
	for(auto itr = m_systems.begin(); itr != m_systems.end(); ++itr)
	{
		delete (*itr).second;
	}
}

void ECS::EntitySystemManager::AddEntityToSystems(Entity* p_entity)
{
	for(auto itr = m_systems.begin(); itr != m_systems.end(); ++itr)
	{
		// Validate entity against system.
		bool insert = true;
		for(size_t i = 0; i < (*itr).second->m_componentTypes.size(); ++i)
		{
			if((*itr).second->m_componentTypes.at(i))
				if(!p_entity->m_componentTypes.at(i))
					insert = false; // Validation fails.
		}

		// Validation sucess insert the entity in the system.
		if(insert)
		{
			(*itr).second->m_activeEntities.insert(p_entity);
			(*itr).second->Init();
		}
	}
}

void ECS::EntitySystemManager::RemoveEntityFromSystems(Entity* p_entity)
{
	for(auto itr = m_systems.begin(); itr != m_systems.end(); ++itr)
	{
		// Validate entity against system.
		for(size_t i = 0; i < (*itr).second->m_componentTypes.size(); ++i)
		{
			if((*itr).second->m_componentTypes.at(i))
				if(!p_entity->m_componentTypes.at(i))
				{
					// Everytime validation fails for a system agains a entity, if the entity exist remove it.
					if((*itr).second->m_activeEntities.find(p_entity) != (*itr).second->m_activeEntities.end())
					{
						(*itr).second->m_activeEntities.erase(p_entity);
						(*itr).second->Init();
					}
				}
		}
	}
}

