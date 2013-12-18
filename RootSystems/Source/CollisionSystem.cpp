#include <RootSystems\Include\CollisionSystem.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	void CollisionSystem::Init()
	{
		m_scripts.Init(m_world->GetEntityManager());
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
			Script* script = m_scripts.Get(p_entity);
			Action action(ActionType::ACTION_COLLIDE);
			action.m_data.m_collision.m_entityId = (*itr);

			script->m_actions.push_back(action);
		}

		cr->m_collidedEntityId.clear();
	}

	void CollisionSystem::End()
	{

	}
}