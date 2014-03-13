#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems/Include/ControllerActionSystem.h>
#include <RootSystems/Include/Components.h>


namespace RootForce
{
	void ControllerActionSystem::Init()
	{
		m_controllerActions.Init(m_world->GetEntityManager());
	}

	void ControllerActionSystem::Begin()
	{
		int i = 0;
	}

	void ControllerActionSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		ControllerActions* controllerActions = m_controllerActions.Get(p_entity);
		controllerActions->m_activeActions.clear();
		for(auto it = controllerActions->m_actions.begin(); it != controllerActions->m_actions.end(); ++it)
		{
			auto actionKey = (*it).first;
			if(g_engineContext.m_inputSys->GetKeyState(actionKey).DOWN)
			{

			}
		}
	}

	void ControllerActionSystem::End()
	{

	}
}
#endif
