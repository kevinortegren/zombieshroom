#pragma once

#include <map>
#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/Script.h>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	struct ScriptSystem : public ECS::EntitySystem
	{
		ScriptSystem(ECS::World* p_world);
		~ScriptSystem();
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();

	private:
		ECS::ComponentMapper<RootForce::Script> m_scripts;

		ECS::World*	m_world;
	};
}