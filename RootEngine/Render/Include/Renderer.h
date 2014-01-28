#pragma once
#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Camera.h>
#include <RootEngine/Render/Include/RenderJob.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/GeometryBuffer.h>
#include <RootEngine/Render/Include/LineRenderer.h>
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <RootEngine/Render/Include/Light.h>
#include <RootEngine/Render/Include/ParticleSystem.h>
#include <RootEngine/Render/Include/LightingDevice.h>
#include <RootEngine/Render/Include/Shadowcaster.h>
#include <RootEngine/Render/Include/ShadowDevice.h>
#include <WinSock2.h>
#include <SDL2/SDL.h>
#include <memory>


#if defined(_WINDLL)
#define RENDERSYS_DLL_EXPORT __declspec(dllexport)
#else
#define RENDERSYS_DLL_EXPORT __declspec(dllimport)
#endif

#define RENDER_MAX_BONES 20
#define RENDER_UNIFORMS_SIZE 1408

#define RENDER_SLOT_PERFRAME 0
#define RENDER_SLOT_PEROBJECT 1
#define RENDER_SLOT_LIGHTS 2
#define RENDER_SLOT_PEREFFECT 3 

namespace Render
{
	class RendererInterface : public RootEngine::SubsystemInterface
	{
	public:

		// Init
		virtual void SetupSDLContext(SDL_Window* p_window) = 0;
		virtual void SetResolution(int p_width, int p_height) = 0;
		
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		// Camera
		virtual void SetViewMatrix(glm::mat4 p_viewMatrix) = 0;
		virtual void SetProjectionMatrix(glm::mat4 p_projectionMatrix) = 0;

		// Shadows
		virtual void AddShadowcaster(const Render::Shadowcaster& p_shadowcaster, int p_index) = 0;
		
		// Rendering
		virtual void AddRenderJob(const RenderJob& p_job) = 0;
		virtual void AddLine(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color) = 0;
		virtual void Clear() = 0;
		virtual void Render() = 0;
		virtual void Swap() = 0;
		virtual void DisplayNormals(bool p_display) = 0;

		// Resource creation.
		virtual BufferInterface* CreateBuffer(GLenum p_type) = 0;
		virtual void ReleaseBuffer(BufferInterface* p_buffer) = 0;

		virtual TextureInterface* CreateTexture() = 0;
		virtual void ReleaseTexture(TextureInterface* p_texture) = 0;

		virtual std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() = 0;
		virtual std::shared_ptr<MeshInterface> CreateMesh() = 0;
		virtual std::shared_ptr<EffectInterface> CreateEffect() = 0;
		virtual std::shared_ptr<Material> CreateMaterial() = 0;

		// Particle systems.
		virtual ParticleSystem* CreateParticleSystem() = 0;
		virtual void SetParticleUniforms(Technique* p_technique, std::map<Render::Semantic::Semantic, void*> p_params) = 0;
		virtual void BeginTransform(float dt) = 0;
		virtual void EndTransform() = 0;

		// Lighting.
		virtual void SetAmbientLight(const glm::vec4& p_color) = 0;
		virtual void AddDirectionalLight(const DirectionalLight& p_light, int p_index) = 0;
		virtual void AddPointLight(const PointLight& p_light, int index) = 0;
	};

	class GLRenderer : public RendererInterface
	{
	public:
		GLRenderer();
		void Startup();
		void Shutdown();
		void SetupSDLContext(SDL_Window* p_window);
		void SetResolution(int p_width, int p_height);

		int GetWidth() const;
		int GetHeight() const;

		void SetViewMatrix(glm::mat4 p_viewMatrix);
		void SetProjectionMatrix(glm::mat4 p_projectionMatrix);

		void AddShadowcaster(const Render::Shadowcaster& p_shadowcaster, int p_index);

		void Clear();
		void AddRenderJob(const RenderJob& p_job);
		void AddLine(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color);
		void Render();
		void Swap();
		void DisplayNormals(bool p_display) { m_displayNormals = p_display; }
		
		BufferInterface* CreateBuffer(GLenum p_type);
		void ReleaseBuffer(BufferInterface* p_buffer);

		TextureInterface* CreateTexture();
		void ReleaseTexture(TextureInterface* p_texture);

		std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes();
		std::shared_ptr<MeshInterface> CreateMesh();
		std::shared_ptr<EffectInterface> CreateEffect();
		std::shared_ptr<Material> CreateMaterial();

		void InitialziePostProcesses();

		ParticleSystem* CreateParticleSystem();
		void SetParticleUniforms(Technique* p_technique, std::map<Render::Semantic::Semantic, void*> p_params);
		void BeginTransform(float dt);
		void EndTransform();

		void SetAmbientLight(const glm::vec4& p_color);
		void AddDirectionalLight(const DirectionalLight& p_light, int index);
		void AddPointLight(const PointLight& p_light, int index);

		static std::map<Semantic::Semantic, unsigned> s_sizes;

	private:

		bool CheckExtension(const char* p_extension);
		void InitializeSemanticSizes();
		void ProcessRenderJobs();

		void GeometryPass();
		void ShadowPass();
		void LightingPass();
		void ForwardPass();
		void PostProcessPass();
		void Output();

		int GetAvailableVideoMemory(); //Returns currently accessible VRAM in kilobytes
		void PrintResourceUsage();

		unsigned m_renderFlags;

		std::vector<RenderJob> m_jobs;
		std::vector<Render::BufferInterface*> m_buffers;
		std::vector<Render::TextureInterface*> m_textures;

		int m_renderAllocations;

		SDL_GLContext m_glContext;
		SDL_Window* m_window;
		int m_width;
		int m_height;

		// Scratch framebuffer.
		GLuint m_fbo;
		GLuint m_activeTarget;
		GLenum m_activeTexture;

		Render::TextureInterface* m_color0;
		Render::TextureInterface* m_color1;
		Render::TextureInterface* m_color2;

		GLuint m_glowFbo;
		Render::TextureInterface* m_glow;

		GeometryBuffer m_gbuffer;
		LineRenderer m_lineRenderer;
		ParticleSystemHandler m_particles;
		LightingDevice m_lighting;
		ShadowDevice m_shadowDevice;
		
		Mesh m_fullscreenQuad;
		std::vector<Render::EffectInterface*> m_postProcessEffects;

		std::map<Material*, std::vector<MeshInterface*>> m_materialMeshMap; //For optimization by means of material sorting
		
		struct
		{
			glm::mat4 m_projection;
			glm::mat4 m_view;
			glm::mat4 m_invView;
			glm::mat4 m_invProj;
			glm::mat4 m_invViewProj;

		} m_cameraVars;

		BufferInterface* m_cameraBuffer;
		BufferInterface* m_uniforms;

		std::shared_ptr<TechniqueInterface> m_renderTech;
		bool m_displayNormals;
	};
}

extern "C"
{
	typedef Render::RendererInterface* (*CREATERENDERER)(RootEngine::SubsystemSharedContext);
	RENDERSYS_DLL_EXPORT Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context);
}
