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
const ECS::GroupManager* ECS::World::GetGroupManager() const
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

const std::vector<ECS::Message>& ECS::World::GetMessages() const
{
	return m_messages;
}

void ECS::World::TestMessaging()
{
	auto msgs = GetMessages();

	for(auto itr = msgs.begin(); itr != msgs.end(); ++itr) {

		//std::cout << "Message Type: " << itr->m_type << " Entity ID: " << itr->m_entity->GetId() << " Component Type: " << itr->m_compType << std::endl;
	}

}

