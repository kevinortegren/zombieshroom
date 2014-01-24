#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems/Include/CollisionSystem.h>
#include <RootEngine/Script/Include/RootScript.h>


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

		for(auto itr = cr->m_collidedEntities.begin(); itr != cr->m_collidedEntities.end(); ++itr)
		{
			Script* script = m_scripts.Get(p_entity);
			
			m_engineContext->m_script->SetFunction(script->Name, "OnCollide");
			m_engineContext->m_script->AddParameterUserData(p_entity, sizeof(ECS::Entity*), "Entity");
			m_engineContext->m_script->AddParameterUserData((*itr), sizeof(ECS::Entity*), "Entity");
			m_engineContext->m_script->ExecuteScript();
		}

		cr->m_collidedEntities.clear();
	}

	void CollisionSystem::End()
	{

	}
}
#endif
