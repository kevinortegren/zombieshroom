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
#include <RootEngine/Render/Include/ParticleSystem.h>
#include <RootEngine/Render/Include/LightingDevice.h>
#include <WinSock2.h>
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
		
		virtual void SetViewMatrix(glm::mat4 p_viewMatrix) = 0;
		virtual void SetProjectionMatrix(glm::mat4 p_projectionMatrix) = 0;
		
		virtual void AddRenderJob(const RenderJob& p_job) = 0;
		virtual void AddLine(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color) = 0;
		virtual void Clear() = 0;
		virtual void Render() = 0;
		virtual void RenderLines() = 0;
		virtual void Swap() = 0;
		virtual void DisplayNormals(bool p_display) = 0;

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;

		// Resource creation.
		virtual std::shared_ptr<BufferInterface> CreateBuffer() = 0;
		virtual std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() = 0;
		virtual std::shared_ptr<EffectParams> CreateEffectParams() = 0;
		virtual std::shared_ptr<MeshInterface> CreateMesh() = 0;
		virtual std::shared_ptr<EffectInterface> CreateEffect() = 0;
		virtual std::shared_ptr<TextureInterface> CreateTexture() = 0;
		virtual std::shared_ptr<Material> CreateMaterial() = 0;

		// Particle systems.
		virtual ParticleSystem* CreateParticleSystem(const ParticleSystemDescription& p_desc) = 0;
		virtual void BeginTransform(float dt) = 0;
		virtual void EndTransform() = 0;

		// Lighting.
		virtual void SetAmbientLight(const glm::vec4& p_color) = 0;
		virtual void AddDirectionalLight(const DirectionalLight& p_light, int index) = 0;
		virtual void AddPointLight(const PointLight& p_light, int index) = 0;
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

		void SetViewMatrix(glm::mat4 p_viewMatrix);
		void SetProjectionMatrix(glm::mat4 p_projectionMatrix);

		void Clear();
		void AddRenderJob(const RenderJob& p_job);
		void AddLine(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color);
		void Render();
		void RenderLines();
		void Swap();
		void DisplayNormals(bool p_display) { m_displayNormals = p_display; }
		bool CheckExtension(const char* p_extension);

		virtual int GetWidth();
		virtual int GetHeight();

		std::shared_ptr<BufferInterface> CreateBuffer() { return std::shared_ptr<BufferInterface>(new Buffer); }
		std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() { return std::shared_ptr<VertexAttributesInterface>(new VertexAttributes); }
		std::shared_ptr<EffectParams> CreateEffectParams() { return std::shared_ptr<EffectParams>(new EffectParams); }
		std::shared_ptr<MeshInterface> CreateMesh() { return std::shared_ptr<MeshInterface>(new Mesh); }
		std::shared_ptr<EffectInterface> CreateEffect() { return std::shared_ptr<EffectInterface>(new Effect); }
		std::shared_ptr<TextureInterface> CreateTexture() { return std::shared_ptr<TextureInterface>(new Texture); }
		std::shared_ptr<Material> CreateMaterial();

		ParticleSystem* CreateParticleSystem(const ParticleSystemDescription& p_desc);
		void BeginTransform(float dt);
		void EndTransform();

		void SetAmbientLight(const glm::vec4& p_color);
		void AddDirectionalLight(const DirectionalLight& p_light, int index);
		void AddPointLight(const PointLight& p_light, int index);

	private:

		void GeometryPass();
		void LightingPass();
		void ForwardPass();

		void BindMaterial(Material* p_material);
	
		int GetAvailableVideoMemory(); //Returns currently accessible VRAM in kilobytes

		static GLRenderer* s_rendererInstance;
		SDL_GLContext m_glContext;
		SDL_Window* m_window;
		int m_width;
		int m_height;

		GeometryBuffer m_gbuffer;
		Mesh m_lineMesh;

		std::map<Material*, std::vector<MeshInterface*>> m_materialMeshMap; //For optimization by means of material sorting
		std::vector<RenderJob> m_jobs;
		std::vector<Line> m_lines;

		Buffer m_uniforms;
		Buffer m_cameraBuffer;
		Buffer m_animuniforms;
		struct
		{
			glm::mat4 m_projection;
			glm::mat4 m_view;
			glm::mat4 m_invView;
			glm::mat4 m_invProj;
			glm::mat4 m_invViewProj;

		} m_cameraVars;

		std::shared_ptr<TechniqueInterface> m_debugTech;
		std::shared_ptr<TechniqueInterface> m_normalTech;

		bool m_displayNormals;

		ParticleSystemHandler m_particles;
		LightingDevice m_lighting;
	};
}

extern "C"
{
	typedef Render::RendererInterface* (*CREATERENDERER)(RootEngine::SubsystemSharedContext);
	RENDERSYS_DLL_EXPORT Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context);
}
