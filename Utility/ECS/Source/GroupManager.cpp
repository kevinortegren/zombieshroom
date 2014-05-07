#include <Utility\ECS\Include\GroupManager.h>
#include <Utility/ECS/Include/World.h>
#include <iostream>

ECS::GroupManager::GroupManager(World* p_world)
	: m_world(p_world)
{}

void ECS::GroupManager::RegisterEntity(const std::string& p_group, ECS::Entity* p_entity)
{
	if(p_entity == nullptr)
		return;

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
	for (auto it = m_groups.begin(); it != m_groups.end();)
	{
		if (it->second == p_entity && it->first == p_group)
		{
			Message m;
			m.m_type = MessageType::ENTITY_REMOVED_FROM_GROUP;
			m.m_entity = p_entity;
			m.m_tagGroupName = p_group;
			m_world->m_messages.push_back(m);

			it = m_groups.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void ECS::GroupManager::UnregisterEntityFromAllGroups(ECS::Entity* p_entity)
{
	for (auto it = m_groups.begin(); it != m_groups.end();)
	{
		if (it->second == p_entity)
		{
			Message m;
			m.m_type = MessageType::ENTITY_REMOVED_FROM_GROUP;
			m.m_entity = p_entity;
			m.m_tagGroupName = it->first;
			m_world->m_messages.push_back(m);

			it = m_groups.erase(it);
		}
		else
		{
			it++;
		}
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
