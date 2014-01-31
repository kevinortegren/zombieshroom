#include <Utility\ECS\Include\World.h>

ECS::EntityManager* ECS::World::GetEntityManager() 
{ 
	return &m_entityManager; 
}
ECS::EntitySystemManager* ECS::World::GetSystemManager() 
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

ECS::EntityImporter* ECS::World::GetEntityImporter()
{
	return &m_importer;
}

ECS::EntityExporter* ECS::World::GetEntityExporter()
{
	return &m_exporter;
}

ECS::Storage* ECS::World::GetStorage()
{
	return &m_storage;
}


