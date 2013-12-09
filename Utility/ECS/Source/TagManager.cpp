#include <Utility\ECS\Include\TagManager.h>

void ECS::TagManager::RegisterEntity(const std::string& p_tag, Entity* p_entity)
{
	m_tags[p_tag] = p_entity;
}

void ECS::TagManager::Unregister(const std::string& p_tag)
{
	m_tags[p_tag] = nullptr;
}

ECS::Entity* ECS::TagManager::GetEntityByTag(const std::string& p_tag)
{
	if (m_tags.find(p_tag) == m_tags.end())
		return nullptr;

	return m_tags[p_tag];
}
