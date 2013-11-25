#pragma once
//#include <Windows.h>
#include <SDL2/SDL.h>


#if defined(_WINDLL)
#define INPSYS_DLL_EXPORT __declspec(dllexport)
#else
#define INPSYS_DLL_EXPORT __declspec(dllimport)
#endif

// KeyState specifies whether the key is triggered (edge) or held pressed beyond the trigger (non-edge)
namespace KeyState
{
	enum KeyState
	{
		UP,
		DOWN,
		DOWN_EDGE,
		UP_EDGE
	};
}

// MouseButton enum is mapped to overlap the key range at the free (unused) SDL scancode IDs for easier key management
namespace MouseButton
{
	enum MouseButton
	{
		LEFT = 490,
		MIDDLE,
		RIGHT,
		X1,
		X2
	};
}

struct vector2 //Just temporary TODO: replace with real glm::vec2 or similar once implemented in the whole solution
{
	vector2()
	{
	}
	vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	float x;
	float y;
};
namespace InputManager
{
	class InputInterface abstract
	{
	public:
		// Called from the loop calling SDL_PollEvent for events of types SDL_KEYDOWN, SDL_KEYUP, SDL_MOUSEMOTION, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP
		// Updates the internal input status data, later polled with the functions available below
		virtual void HandleInput(SDL_Event& p_event) = 0;

		// Returns the key state of a specific keyboard key (SDL_SCANCODE_XXX) or mouse button
		virtual KeyState::KeyState GetKeyState(SDL_Keycode p_key) = 0;
		virtual KeyState::KeyState GetKeyState(MouseButton::MouseButton p_button) = 0;

		// Returns the global and delta mouse position
		virtual vector2 GetGlobalMousePos() = 0;
		virtual vector2 GetDeltaMousePos() = 0;

	};
}

extern "C" 
{
	typedef InputManager::InputInterface* (* GETINPUTINTERFACE) ();
	INPSYS_DLL_EXPORT InputManager::InputInterface* GetInputInterface();
}