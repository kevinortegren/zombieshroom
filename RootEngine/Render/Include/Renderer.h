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

		bool CheckExtension(const char* p_extension);

	private:
		GLRenderer();
		~GLRenderer();

		void Clear();
		void Swap();

		void SetAttributes();
	
		int GetAvailableVideoMemory(); //Returns VRAM in kilobytes

		static GLRenderer* s_rendererInstance;
		SDL_GLContext m_glContext;
		SDL_Window* m_window;
		GLuint m_vao;
		// Temporary.

		Buffer m_buffer;
		Effect m_effect;
		VertexAttributes m_attributes;

		glm::mat4 m_world;
		float m_angle;

		struct 
		{
			glm::mat4 m_world;
			glm::mat4 m_normal;
		} m_uniformVars;

		Buffer m_uniforms;

		// Camera stuff.
		Camera m_camera;
		
		struct
		{
			glm::mat4 m_projection;
			glm::mat4 m_view;

		} m_cameraVars;

		Buffer m_camerBuffer;
	};
}

extern "C"
{
	typedef Render::RendererInterface* (*CREATERENDERER)(RootEngine::SubsystemSharedContext);
	RENDERSYS_DLL_EXPORT Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context);
}
