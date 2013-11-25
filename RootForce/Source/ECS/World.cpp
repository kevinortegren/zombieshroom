#include <ECS/World.h>

ECS::EntityManager* ECS::World::GetEntityManager() 
{ 
	return &m_entityManager; 
}
ECS::ComponentSystemManager* ECS::World::GetSystemManager() 
{ 
	return &m_systemManager; 
}