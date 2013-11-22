#pragma once

#include <RootEngine/Render/Include/GraphicsBuffer.h>
#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <SDL2/SDL.h>

#if defined(_WINDLL)
#define RENDERSYS_DLL_EXPORT __declspec(dllexport)
#else
#define RENDERSYS_DLL_EXPORT __declspec(dllimport)
#endif

namespace Render
{
	class RendererInterface : public RootEngine::SubsystemInterface
	{
	public:
		virtual void SetupSDLContext(SDL_Window* p_window) = 0;
		virtual void Render() = 0;
		virtual void Cleanup() = 0;
	};

	class GLRenderer : public RendererInterface
	{
	public:
		static GLRenderer* GetInstance();

		void Startup();
		void Shutdown();
		void SetupSDLContext(SDL_Window* p_window);
		void Render();
		void Cleanup();

	private:
		GLRenderer();
		~GLRenderer();

		void SetAttributes();
		bool CheckExtension(const char* p_extension);
		int GetAvailableVideoMemory(); //Returns VRAM in kilobytes

		static GLRenderer* s_rendererInstance;
		SDL_GLContext m_glContext;

		GraphicsBuffer m_buffer;
		Effect m_effect;
		SDL_Window* m_window;
	};
}

extern "C"
{
	typedef Render::RendererInterface* (*CREATERENDERER)(RootEngine::SubsystemSharedContext);
	RENDERSYS_DLL_EXPORT Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context);
}
