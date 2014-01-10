#include <Utility\ECS\Include\GroupManager.h>
#include <iostream>

void ECS::GroupManager::RegisterEntity(const std::string& p_group, ECS::Entity* p_entity)
{
	std::pair<std::string, ECS::Entity*> pair;
	pair.first = p_group;
	pair.second = p_entity;
	m_groups.insert(pair);
}

void ECS::GroupManager::UnregisterEntity(const std::string& p_group, ECS::Entity* p_entity)
{
	for (auto itr = m_groups.begin(); itr != m_groups.end();)
	{
		// you have to do this because iterators are invalidated
		std::multimap<std::string, ECS::Entity*>::iterator erase_iter = itr++;

		// removes all even values
		if (erase_iter->second == p_entity)
			m_groups.erase(erase_iter);
	}

}

void ECS::GroupManager::PrintEntitiesInGroup(const std::string& p_group)
{
	GroupRange range = m_groups.equal_range(p_group);

	for (std::multimap<std::string, ECS::Entity*>::iterator itr = range.first; itr != range.second; ++itr)
	{
		//std::cout << (*itr).second->GetId() << std::endl;
	}
}

ECS::GroupManager::GroupRange ECS::GroupManager::GetEntitiesInGroup(const std::string& p_group)
{
	return m_groups.equal_range(p_group);
}

void ECS::GroupManager::UnregisterAll()
{
	m_groups.clear();
}
