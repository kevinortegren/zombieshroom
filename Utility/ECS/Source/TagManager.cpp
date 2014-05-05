#include <Utility\ECS\Include\TagManager.h>
#include <Utility/ECS/Include/World.h>

ECS::TagManager::TagManager(World* p_world)
	: m_world(p_world)
{}

void ECS::TagManager::RegisterEntity(const std::string& p_tag, Entity* p_entity)
{
	m_tags[p_tag] = p_entity;

	Message m;
	m.m_type = MessageType::TAG_ADDED;
	m.m_entity = p_entity;
	m.m_tagGroupName = p_tag;
	m_world->m_messages.push_back(m);
}

void ECS::TagManager::Unregister(const std::string& p_tag)
{
	if (m_tags.find(p_tag) != m_tags.end())
	{
		Message m;
		m.m_type = MessageType::TAG_REMOVED;
		m.m_entity = m_tags[p_tag];
		m.m_tagGroupName = p_tag;
		m_world->m_messages.push_back(m);
	}

	m_tags[p_tag] = nullptr;
}

ECS::Entity* ECS::TagManager::GetEntityByTag(const std::string& p_tag)
{
	if (m_tags.find(p_tag) == m_tags.end())
		return nullptr;

	return m_tags[p_tag];
}

void ECS::TagManager::UnregisterAll()
{
	m_tags.clear();
}
