#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems\Include\ScriptSystem.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

void RootForce::ScriptSystem::Init()
{
	m_scripts.Init(m_world->GetEntityManager());
}

void RootForce::ScriptSystem::Begin()
{

}

void RootForce::ScriptSystem::ProcessEntity(ECS::Entity* p_entity)
{
	Script* script = m_scripts.Get(p_entity);

	for(auto itr = script->m_actions.begin(); itr != script->m_actions.end(); ++itr)
	{
		g_engineContext.m_script->SetFunction(script->m_name, "Process");
		g_engineContext.m_script->AddParameterNumber((*itr).m_action);

		if((*itr).m_action == RootForce::ActionType::ACTION_CREATE)
		{
		}
		else if((*itr).m_action == RootForce::ActionType::ACTION_COLLIDE)
		{
			g_engineContext.m_script->AddParameterNumber((*itr).m_data.m_collision.m_entityId);
		}

		g_engineContext.m_script->ExecuteScript();
	}

	script->m_actions.clear();
}

void RootForce::ScriptSystem::End()
{

}
#endif
