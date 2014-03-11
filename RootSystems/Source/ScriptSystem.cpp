#include <RootSystems/Include/ScriptSystem.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Include/Logging/Logging.h>

namespace RootForce
{
	ScriptSystem::ScriptSystem(ECS::World* p_world) 
		: ECS::EntitySystem(p_world), m_world(p_world)
	{
		SetUsage<RootForce::Script>();
	}

	ScriptSystem::~ScriptSystem()
	{
	}

	void ScriptSystem::Init()
	{
		m_scripts.Init(m_world->GetEntityManager());
	}

	void ScriptSystem::Begin()
	{
	}

	void ScriptSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		RootForce::Script* script = m_scripts.Get(p_entity);
		
		g_engineContext.m_script->SetFunction(script->Name, "OnUpdate");
		g_engineContext.m_script->AddParameterUserData(p_entity, sizeof(ECS::Entity*), "Entity");
		g_engineContext.m_script->ExecuteScript();
	}

	void ScriptSystem::End()
	{
	}
}
