#pragma once

#include <Utility\ECS\Include\World.h>
#include <Utility\ECS\Include\EntitySystem.h>
#include <Utility\ECS\Include\Component.h>
#include <string>
#include <vector>
#include <RootSystems\Include\Action.h>

namespace RootForce
{
	struct Script : public ECS::Component<Script>
	{
		std::string m_name;
		std::vector<Action> m_actions;
	};

	class ScriptSystem : public ECS::EntitySystem
	{
	public:
		ScriptSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{
			SetUsage<Script>();
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();

	private:	
		ECS::ComponentMapper<Script> m_scripts;
	};
}