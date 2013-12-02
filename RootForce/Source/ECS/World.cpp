#include <ECS/World.h>

ECS::EntityManager* ECS::World::GetEntityManager() 
{ 
	return &m_entityManager; 
}
ECS::ComponentSystemManager* ECS::World::GetSystemManager() 
{ 
	return &m_systemManager; 
}
ECS::TagManager* ECS::World::GetTagManager()
{
	return &m_tagManager;
}
ECS::GroupManager* ECS::World::GetGroupManager()
{
	return &m_groupManager;
}
void ECS::World::SetDelta(float p_dt)
{
	m_dt = p_dt;
}
float ECS::World::GetDelta()
{
	return m_dt;
}

