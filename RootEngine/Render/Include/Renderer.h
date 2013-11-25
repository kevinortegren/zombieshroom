#pragma once

#include <RootEngine/Render/Include/Buffer.h>

#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Camera.h>
#include <RootEngine/Render/Include/RenderJob.h>
#include <RootEngine/Render/Include/Mesh.h>

#include <RootEngine/Include/SubsystemSharedContext.h>
#include <SDL2/SDL.h>
#include <memory>

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
		virtual void AddRenderJob(RenderJob* p_job) = 0;
		virtual void Render() = 0;
		virtual std::shared_ptr<BufferInterface> CreateBuffer() = 0;
		virtual std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() = 0;
		virtual std::shared_ptr<MeshInterface> CreateMesh() = 0;

	};

	class GLRenderer : public RendererInterface
	{
	public:
		static GLRenderer* GetInstance();

		void Startup();
		void Shutdown();
		void SetupSDLContext(SDL_Window* p_window);
		void AddRenderJob(RenderJob* p_job);
		void Render();

		bool CheckExtension(const char* p_extension);

		std::shared_ptr<BufferInterface> CreateBuffer() { return std::shared_ptr<BufferInterface>(new Buffer); }
		std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() { return std::shared_ptr<VertexAttributesInterface>(new VertexAttributes); }
		std::shared_ptr<MeshInterface> CreateMesh() { return std::shared_ptr<MeshInterface>(new Mesh); }

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

		std::vector<RenderJob*> m_jobs;

		// Effect.

		Effect m_effect;
		Buffer m_uniforms;

		// Camera
		Camera m_camera;
		Buffer m_camerBuffer;
		struct
		{
			glm::mat4 m_projection;
			glm::mat4 m_view;

		} m_cameraVars;

		// Lights.
		Buffer m_lights;
		struct
		{
			glm::vec3 m_direction;

		} m_lightVars;
	};
}

extern "C"
{
	typedef Render::RendererInterface* (*CREATERENDERER)(RootEngine::SubsystemSharedContext);
	RENDERSYS_DLL_EXPORT Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context);
}
