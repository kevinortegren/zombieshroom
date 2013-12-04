#pragma once
#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Camera.h>
#include <RootEngine/Render/Include/RenderJob.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/GeometryBuffer.h>
#include <RootEngine/Render/Include/Line.h>
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <RootEngine/Render/Include/Light.h>
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
		virtual void AddDirectionalLight(const DirectionalLight& p_light, int index) = 0;
		virtual void AddPointLight(const PointLight& p_light, int index) = 0;
		virtual void SetAmbientLight(const glm::vec4& p_color) = 0;
		virtual void AddRenderJob(const RenderJob& p_job) = 0;
		virtual void AddLine(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color) = 0;
		virtual void Clear() = 0;
		virtual void Render() = 0;
		virtual void RenderLines() = 0;
		virtual void Swap() = 0;

		// Resource creation.
		virtual std::shared_ptr<BufferInterface> CreateBuffer() = 0;
		virtual std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() = 0;

		virtual MeshInterface* CreateMesh() = 0;
		virtual EffectInterface* CreateEffect() = 0;
		virtual TextureInterface* CreateTexture() = 0;
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
		void AddDirectionalLight(const DirectionalLight& p_light, int index);
		void AddPointLight(const PointLight& p_light, int index);
		void SetAmbientLight(const glm::vec4& p_color);
		void Clear();
		void AddRenderJob(const RenderJob& p_job);
		void AddLine(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color);
		void Render();
		void RenderLines();
		void Swap();
		bool CheckExtension(const char* p_extension);

		std::shared_ptr<BufferInterface> CreateBuffer() { return std::shared_ptr<BufferInterface>(new Buffer); }
		std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() { return std::shared_ptr<VertexAttributesInterface>(new VertexAttributes); }

		MeshInterface*		CreateMesh() { return new Mesh; }
		EffectInterface*	CreateEffect() { return new Effect; } //Remember to delete
		TextureInterface*	CreateTexture() { return new Texture; } //Remember to delete

	private:

		void GeometryPass();
		void LightingPass();
		void BindMaterial(Material* p_material);
	
		int GetAvailableVideoMemory(); //Returns currently accessible VRAM in kilobytes

		static GLRenderer* s_rendererInstance;
		SDL_GLContext m_glContext;
		SDL_Window* m_window;

		GeometryBuffer m_gbuffer;

		Mesh m_fullscreenQuad;
		Mesh m_lineMesh;

		std::vector<RenderJob> m_jobs;
		std::vector<Line> m_lines;

		Buffer m_uniforms;
		Buffer m_lights;
		Buffer m_cameraBuffer;

		Camera m_camera;
	
		struct
		{
			glm::mat4 m_projection;
			glm::mat4 m_view;
			glm::mat4 m_invView;
			glm::mat4 m_invProj;
			glm::mat4 m_invViewProj;

		} m_cameraVars;

		struct
		{
			glm::vec4 m_ambient;
			DirectionalLight m_dlights[16];
			PointLight m_plights[16];
		
		} m_lightVars;

		size_t m_numDirectionalLights;
		size_t m_numPointLights;


		std::shared_ptr<TechniqueInterface> m_lightingTech;
		std::shared_ptr<TechniqueInterface> m_debugTech;

		//debug

		GLuint m_debugFbo;
		GLuint m_testHandle;
		//GLuint m_testHandle;
	};
}

extern "C"
{
	typedef Render::RendererInterface* (*CREATERENDERER)(RootEngine::SubsystemSharedContext);
	RENDERSYS_DLL_EXPORT Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context);
}
