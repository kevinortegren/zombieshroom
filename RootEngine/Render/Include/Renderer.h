#pragma once

#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Camera.h>

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
	};

	class GLRenderer : public RendererInterface
	{
	public:
		static GLRenderer* GetInstance();

		void Startup();
		void Shutdown();
		void SetupSDLContext(SDL_Window* p_window);
		void Render();

	private:
		GLRenderer();
		~GLRenderer();

		void Clear();
		void Swap();

		void SetAttributes();
		bool CheckExtension(const char* p_extension);
		int GetAvailableVideoMemory(); //Returns VRAM in kilobytes

		static GLRenderer* s_rendererInstance;
		SDL_GLContext m_glContext;
		SDL_Window* m_window;

		// Temporary.

		Buffer m_buffer;
		Effect m_effect;
		VertexAttributes m_attributes;

		Buffer m_uniforms;

		glm::mat4 m_world;
		float m_angle;

		struct 
		{
			glm::mat4 m_world;
			glm::mat4 m_normal;
		} m_uniformVars;

		// Camera stuff.

		Camera m_camera;
		Buffer m_camerBuffer;

		struct
		{
			glm::mat4 m_projection;
			glm::mat4 m_view;

		} m_cameraVars;
	};
}

extern "C"
{
	typedef Render::RendererInterface* (*CREATERENDERER)(RootEngine::SubsystemSharedContext);
	RENDERSYS_DLL_EXPORT Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context);
}
