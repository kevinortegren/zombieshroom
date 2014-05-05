#include <Utility\ECS\Include\GroupManager.h>
#include <Utility/ECS/Include/World.h>
#include <iostream>

ECS::GroupManager::GroupManager(World* p_world)
	: m_world(p_world)
{}

void ECS::GroupManager::RegisterEntity(const std::string& p_group, ECS::Entity* p_entity)
{
	std::pair<std::string, ECS::Entity*> pair;
	pair.first = p_group;
	pair.second = p_entity;
	m_groups.insert(pair);

	Message m;
	m.m_type = MessageType::ENTITY_ADDED_TO_GROUP;
	m.m_entity = p_entity;
	m.m_tagGroupName = p_group;
	m_world->m_messages.push_back(m);
}

void ECS::GroupManager::UnregisterEntity(const std::string& p_group, ECS::Entity* p_entity)
{
	bool found = false;
	for (auto itr = m_groups.begin(); itr != m_groups.end();)
	{
		// you have to do this because iterators are invalidated
		std::multimap<std::string, ECS::Entity*>::iterator erase_iter = itr++;

		// removes all even values
		if (erase_iter->second == p_entity)
		{
			found = true;
			m_groups.erase(erase_iter);
		}
	}

	if (found)
	{
		Message m;
		m.m_type = MessageType::ENTITY_REMOVED_FROM_GROUP;
		m.m_entity = p_entity;
		m.m_tagGroupName = p_group;
		m_world->m_messages.push_back(m);
	}
}

bool ECS::GroupManager::IsEntityInGroup(ECS::Entity* p_entity, const std::string& p_group)
{
	GroupRange range = GetEntitiesInGroup(p_group);
	for(auto itr = range.first; itr != range.second; ++itr)
	{
		if(p_entity == (*itr).second)
			return true;
	}
	return false;
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
