#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/InputManager/Include/InputManager.h>
#include <RootSystems/Include/ControllerActions.h>
#include <Utility/ECS/Include/World.h>
#include <set>
#include <iostream>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	class ControllerActionSystem : public ECS::EntitySystem
	{
	public:
		ControllerActionSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world)
		{

			SetUsage<ControllerActions>();
		}

		ECS::ComponentMapper<ControllerActions> m_controllerActions;

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
	private:
	};

}