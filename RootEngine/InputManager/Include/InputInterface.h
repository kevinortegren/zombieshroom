#pragma once
//#include <Windows.h>
#include <WinSock2.h>
#include <SDL2/SDL.h>
#include <RootEngine/Include/SubsystemSharedContext.h>
#include "KeyStateMouseEnum.h"
#include <glm/glm.hpp>

#if defined(_WINDLL)
#define SUBSYSTEM_DLL_EXPORT __declspec(dllexport)
#else
#define SUBSYSTEM_DLL_EXPORT __declspec(dllimport)
#endif
namespace RootEngine
{
	namespace InputManager
	{

		class InputInterface abstract : public RootEngine::SubsystemInterface
		{
		public:
			// Called from the loop calling SDL_PollEvent for events of types SDL_KEYDOWN, SDL_KEYUP, SDL_MOUSEMOTION, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP
			// Updates the internal input status data, later polled with the functions available below
			virtual void HandleInput(SDL_Event& p_event) = 0;

			// Returns the key state of a specific keyboard key (SDL_SCANCODE_XXX) or mouse button
			virtual KeyState::KeyState GetKeyState(SDL_Scancode p_key) = 0;
			virtual KeyState::KeyState GetKeyState(MouseButton::MouseButton p_button) = 0;

			// Returns the global and delta mouse position
			virtual glm::ivec2 GetGlobalMousePos() = 0;
			virtual glm::ivec2 GetDeltaMousePos() = 0;

		};
	}
}
extern "C" 
{
	typedef RootEngine::InputManager::InputInterface* (* CREATEINPUTINTERFACE)(RootEngine::SubsystemSharedContext);
	SUBSYSTEM_DLL_EXPORT RootEngine::InputManager::InputInterface* CreateInputSystem(RootEngine::SubsystemSharedContext p_context);
}