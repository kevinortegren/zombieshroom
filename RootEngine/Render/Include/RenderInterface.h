#pragma once

#include <SDL2/SDL.h>

#if defined(_WINDLL)
#define RENDERSYS_DLL_EXPORT __declspec(dllexport)
#else
#define RENDERSYS_DLL_EXPORT __declspec(dllimport)
#endif

namespace Render
{
	class RendererInterface abstract
	{
	public:
		virtual void SetupSDLContext(SDL_Window* p_window) = 0;
		virtual void Render() = 0;
		virtual void Cleanup() = 0;
	};

}
