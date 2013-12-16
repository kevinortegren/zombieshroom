#include <RootSystems\Include\CollisionSystem.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	void CollisionSystem::Init()
	{
		m_colliders.Init(m_world->GetEntityManager());
		m_responders.Init(m_world->GetEntityManager());
	}

	void CollisionSystem::Begin()
	{

	}

	void CollisionSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		CollisionResponder* cr = m_responders.Get(p_entity);

		for(auto itr = cr->m_collidedEntityId.begin(); itr != cr->m_collidedEntityId.end(); ++itr)
		{
			if((*itr) != 3)
			g_engineContext.m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "%d collided with %d", p_entity->GetId(), (*itr));

		}

		cr->m_collidedEntityId.clear();
	}

	void CollisionSystem::End()
	{

	}
}