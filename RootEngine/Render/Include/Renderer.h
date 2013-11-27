#pragma once

#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Camera.h>
#include <RootEngine/Render/Include/RenderJob.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/GeometryBuffer.h>

#include <RootEngine/Include/SubsystemSharedContext.h>

#include <RootEngine/Render/Include/Lights.h>

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
		virtual void SetResolution(int p_width, int p_height) = 0;
		virtual void AddRenderJob(RenderJob* p_job) = 0;
		virtual void AddDirectionalLight(const DirectionalLight& p_light, int index) = 0;
		virtual void SetAmbientLight(const glm::vec4& p_color) = 0;
		virtual void Render() = 0;

		// Resource creation.
		virtual std::shared_ptr<BufferInterface> CreateBuffer() = 0;
		virtual std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() = 0;
		virtual std::shared_ptr<MeshInterface> CreateMesh() = 0;
		virtual std::shared_ptr<Effect> CreateEffect() = 0;
	};

	class GLRenderer : public RendererInterface
	{
	public:
		GLRenderer();
		~GLRenderer();

		void Startup();
		void Shutdown();
		void SetupSDLContext(SDL_Window* p_window);
		void SetResolution(int p_width, int p_height);
		void AddRenderJob(RenderJob* p_job);
		void AddDirectionalLight(const DirectionalLight& p_light, int index);
		void SetAmbientLight(const glm::vec4& p_color);
		void Render();

		bool CheckExtension(const char* p_extension);

		std::shared_ptr<BufferInterface> CreateBuffer() { return std::shared_ptr<BufferInterface>(new Buffer); }
		std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() { return std::shared_ptr<VertexAttributesInterface>(new VertexAttributes); }
		std::shared_ptr<MeshInterface> CreateMesh() { return std::shared_ptr<MeshInterface>(new Mesh); }
		std::shared_ptr<Effect> CreateEffect() { return std::shared_ptr<Effect>(new Effect); }

	private:
		void Clear();
		void Swap();

		void SetAttributes();
	
		int GetAvailableVideoMemory(); //Returns VRAM in kilobytes

		static GLRenderer* s_rendererInstance;
		SDL_GLContext m_glContext;
		SDL_Window* m_window;

		std::vector<RenderJob*> m_jobs;

		GeometryBuffer m_gbuffer;

		Mesh m_fullscreenQuad;

		std::shared_ptr<TechniqueInterface> m_lightingTech;

		std::shared_ptr<EffectInterface> m_deferred;
		std::shared_ptr<EffectInterface> m_output;

		Buffer m_uniforms;
		Buffer m_lights;
		Buffer m_cameraBuffer;

		Camera m_camera;
	
		struct
		{
			glm::mat4 m_projection;
			glm::mat4 m_view;

		} m_cameraVars;

		struct
		{
			glm::vec4 m_ambient;
			DirectionalLight m_lights[16];
		
		} m_lightVars;

		size_t m_numDirectionalLights;
	};
}

extern "C"
{
	typedef Render::RendererInterface* (*CREATERENDERER)(RootEngine::SubsystemSharedContext);
	RENDERSYS_DLL_EXPORT Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context);
}
