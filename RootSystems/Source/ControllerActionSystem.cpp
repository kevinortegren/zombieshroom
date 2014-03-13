#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems/Include/ControllerActionSystem.h>
#include <RootSystems/Include/Components.h>
#include <RootEngine/InputManager/Include/InputInterface.h>
#include <SDL2/SDL.h>


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
			int actionKey = (*it).first;

			int keystate = g_engineContext.m_inputSys->GetKeyState((SDL_Scancode) actionKey);
			if(keystate == RootEngine::InputManager::KeyState::DOWN || keystate == RootEngine::InputManager::KeyState::DOWN_EDGE)
			{
				controllerActions->m_activeActions.insert((*it).second);
			}
		}
	}

	void ControllerActionSystem::End()
	{

	}
}
#endif
