#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems/Include/CollisionSystem.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootEngine/Physics/Include/RootPhysics.h>


namespace RootForce
{
	void CollisionSystem::Init()
	{
		m_scripts.Init(m_world->GetEntityManager());
		m_responders.Init(m_world->GetEntityManager());
	}

	void CollisionSystem::Begin()
	{
		int i = 0;
	}

	void CollisionSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		CollisionResponder* cr = m_responders.Get(p_entity);

		for(auto itr = cr->m_collisions.begin(); itr != cr->m_collisions.end(); ++itr)
		{
			Script* script = m_scripts.Get(p_entity);
			if(script->Name.compare("AbilitySpawnPoint") == 0)
				int i = 0;
			
			m_engineContext->m_script->SetFunction(script->Name, "OnCollide");
			m_engineContext->m_script->AddParameterUserData(p_entity, sizeof(ECS::Entity*), "Entity");
			m_engineContext->m_script->AddParameterUserData((*itr).first, sizeof(ECS::Entity*), "Entity");
			m_engineContext->m_script->ExecuteScript();

			if(p_entity->GetId() == -1)
				return;
		}

		cr->m_collisions.clear();
	}

	void CollisionSystem::End()
	{

	}

	Collision::~Collision()
	{
		g_engineContext.m_physics->RemoveObject(*m_handle);
	}

}
#endif
