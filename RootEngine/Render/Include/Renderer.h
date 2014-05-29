#pragma once

#include <RootEngine/Include/SubsystemSharedContext.h>

#include <RootEngine/Render/Include/Camera.h>

#include <RootEngine/Render/Include/RenderJob.h>
#include <RootEngine/Render/Include/ComputeJob.h>

#include <RootEngine/Render/Include/RenderResourceManager.h>
#include <RootEngine/Render/Include/LinearAllocator.h>

#include <RootEngine/Render/Include/Light.h>
#include <RootEngine/Render/Include/Shadowcaster.h>
#include <RootEngine/Render/Include/ShadowDevice.h>
#include <RootEngine/Render/Include/GeometryBuffer.h>
#include <RootEngine/Render/Include/LightingDevice.h>
#include <RootEngine/Render/Include/GlowDevice.h>
#include <RootEngine/Render/Include/LineRenderer.h>
#include <RootEngine/Render/Include/ParticleSystem.h>

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

#define RENDER_USE_COMPUTE

#ifdef _DEBUG
#define GLCheck(F) \
	F;			   \
	{   		   \
		GLenum error = glGetError(); \
		if (error != GL_NO_ERROR) {  \
			Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::NON_FATAL_ERROR, "GLCheck error: %s", gluErrorString(error)); \
		}		   \
	}
#else
#define GLCheck(F) F
#endif


namespace Render
{
	extern RootEngine::SubsystemSharedContext g_context;
	class RendererInterface : public RootEngine::SubsystemInterface
	{
	public:

		// Init
		virtual void SetupSDLContext(SDL_Window* p_window) = 0;
		virtual void SetResolution(bool p_fullscreen, int p_width, int p_height) = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual SDL_Window* GetWindow() = 0;

		// Camera
		virtual void SetViewMatrix(glm::mat4 p_viewMatrix) = 0;
		virtual void SetProjectionMatrix(glm::mat4 p_projectionMatrix) = 0;
		virtual void SetCameraPosition(glm::vec3 p_camPos) = 0;

		virtual const glm::mat4& GetProjectionMatrix() = 0;
		virtual const glm::mat4& GetViewMatrix() = 0;


		// Shadows
		virtual void AddShadowcaster(const Render::Shadowcaster& p_shadowcaster, int p_index) = 0;
		virtual void AddShadowJob(Render::ShadowJob& p_shadowJob) = 0;

		// Rendering
		virtual void AddRenderJob(RenderJob& p_job) = 0;	
		virtual void AddLine(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color) = 0;
		virtual void Clear() = 0;
		virtual void Render() = 0;
		virtual void Swap() = 0;
		virtual void DisplayNormals(bool p_display) = 0;

		// Resource Management.
		virtual void GetResourceUsage(int& p_bufferUsage, int& p_textureUsage, int& p_numBuffers, int& p_numTextures) = 0;
		virtual BufferInterface* CreateBuffer(GLenum p_type) = 0;
		virtual void ReleaseBuffer(BufferInterface* p_buffer) = 0;
		virtual TextureInterface* CreateTexture() = 0;
		virtual void ReleaseTexture(TextureInterface* p_texture) = 0;
		virtual Material* CreateMaterial(const std::string& p_name) = 0;
		virtual VertexAttributesInterface* CreateVertexAttributes() = 0;
		virtual MeshInterface* CreateMesh() = 0;
		virtual EffectInterface* CreateEffect() = 0;
		virtual std::string GetStringFromMaterial(Material* p_material) = 0;

		// Particle systems.
		virtual ParticleSystem* CreateParticleSystem() = 0;
		virtual void FreeParticleSystem(ParticleSystemInterface* p_particleSys) = 0;
		virtual void SetParticleUniforms(ParticleSystemInterface* p_system, Technique* p_technique, std::map<Render::Semantic::Semantic, void*> p_params) = 0;
		virtual void BeginTransform(float dt) = 0;
		virtual void EndTransform() = 0;

		// Lighting.
		virtual void SetAmbientLight(const glm::vec4& p_color) = 0;
		virtual void AddDirectionalLight(const DirectionalLight& p_light, int p_index) = 0;
		virtual void AddPointLight(const PointLight& p_light, int index) = 0;

		virtual void Compute(ComputeJob* p_job) = 0;

		virtual DirectionalLight* GetDirectionalLight() = 0;

		virtual void ParseCommands(std::stringstream* p_ss) = 0;
		virtual void ClearJobs() = 0;
		virtual void CleanResources(unsigned p_flag) = 0;
		virtual void RemoveMesh(MeshInterface* p_mesh) = 0;
		virtual void RemoveVAO(VertexAttributesInterface* p_vao) = 0;

	};

	class GLRenderer : public RendererInterface
	{
	public:
		
		GLRenderer();
		~GLRenderer();
		void Startup();
		void Shutdown();

		// Init
		void SetupSDLContext(SDL_Window* p_window);
		void SetResolution(bool p_fullscreen, int p_width, int p_height);
		int GetWidth() const;
		int GetHeight() const;
		SDL_Window* GetWindow() { return m_window; }

		// Camera
		void SetViewMatrix(glm::mat4 p_viewMatrix);
		void SetProjectionMatrix(glm::mat4 p_projectionMatrix);
		void SetCameraPosition(glm::vec3 p_camPos);

		const glm::mat4& GetProjectionMatrix();
		const glm::mat4& GetViewMatrix();

		// Shadows
		void AddShadowcaster(const Render::Shadowcaster& p_shadowcaster, int p_index);
		void AddShadowJob(Render::ShadowJob& p_shadowJob);

		// Rendering
		void Clear();
		void AddRenderJob(RenderJob& p_job);
		void AddLine(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color);
		void Render();
		void Swap();
		void DisplayNormals(bool p_display) { m_displayNormals = p_display; }
		
		// Resource Management.
		void GetResourceUsage(int& p_bufferUsage, int& p_textureUsage, int& p_numBuffers, int& p_numTextures);
		BufferInterface* CreateBuffer(GLenum p_type);
		void ReleaseBuffer(BufferInterface* p_buffer);
		TextureInterface* CreateTexture();
		void ReleaseTexture(TextureInterface* p_texture);
		Material* CreateMaterial(const std::string& p_name);
		VertexAttributesInterface* CreateVertexAttributes();
		MeshInterface* CreateMesh();
		EffectInterface* CreateEffect();
		std::string GetStringFromMaterial(Material* p_material);

		// Particle systems.
		ParticleSystem* CreateParticleSystem();
		void FreeParticleSystem(ParticleSystemInterface* p_particleSys);
		void SetParticleUniforms(ParticleSystemInterface* p_system, Technique* p_technique, std::map<Render::Semantic::Semantic, void*> p_params);
		void BeginTransform(float dt);
		void EndTransform();

		// Lighting.
		void SetAmbientLight(const glm::vec4& p_color);
		void AddDirectionalLight(const DirectionalLight& p_light, int index);
		void AddPointLight(const PointLight& p_light, int index);

		void Compute(ComputeJob* p_job);

		DirectionalLight* GetDirectionalLight();
		
		void InitialziePostProcesses();

		static std::map<Semantic::Semantic, unsigned> s_sizes;
		static std::map<TextureSemantic::TextureSemantic, unsigned> s_textureSlots;

		void ParseCommands(std::stringstream* p_ss);

		void ClearJobs();
		void CleanResources(unsigned p_flag);
		void RemoveMesh(MeshInterface* p_mesh);
		void RemoveVAO(VertexAttributesInterface* p_vao);

		void PrintOpenGLMaxValues();

		glm::vec3 m_camPos;

		struct SortOnDistanceFunctor
		{ 
			SortOnDistanceFunctor( const GLRenderer& renderer ) : m_renderer(renderer) { }
			const GLRenderer& m_renderer;

			bool operator()(const RenderJob* a, const RenderJob* b)
			{ 
				float aDist = glm::distance(a->m_position, m_renderer.m_camPos);
				float bDist = glm::distance(b->m_position, m_renderer.m_camPos);
				if( aDist > bDist ) return true;
				return false;
			}
		};

	private:

		void BindForwardFramebuffer();
		void BindForwardDepthAndColor();
		void CopyDepthAndColor();
		void SwapForwardFramebuffer();

		void UnbindTexture(TextureSemantic::TextureSemantic p_semantic);

		bool CheckExtension(const char* p_extension);
		void InitializeSemanticSizes();
		void ProcessRenderJobs(std::vector<RenderJob*>& p_jobs);

		void Sorting();
		void ShadowPass();
		void GeometryPass(int p_layer);	
		void LightingPass(int p_layer);
		void ForwardPass();
		void PostProcessPass();
		void Output();

		int GetAvailableVideoMemory(); //Returns currently accessible VRAM in kilobytes
		void PrintResourceUsage();

		SDL_GLContext m_glContext;
		SDL_Window* m_window;
		int m_width;
		int m_height;

		unsigned m_renderFlags;
	
		std::vector<RenderJob*> m_forwardJobs;
		std::vector<RenderJob*> m_jobs;
		LinearAllocator m_allocator;

		int m_sjobCount[RENDER_SHADOW_CASCADES];
		std::vector<ShadowJob> m_sjobs;

		int m_activeForwardFramebuffer;
		GLuint m_forwardFramebuffers[2];
		Render::TextureInterface* m_forwardColors[2];
		Render::TextureInterface* m_forwardDepth[2];

		RenderResourceManager m_resources;
		GeometryBuffer m_gbuffer;
		LineRenderer m_lineRenderer;
		ParticleSystemHandler m_particles;
		LightingDevice m_lighting;
		ShadowDevice m_shadowDevice;
		GlowDevice m_glowDevice;	
		Mesh m_fullscreenQuad;

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

		Render::EffectInterface* m_renderEffect;

		std::shared_ptr<TechniqueInterface> m_fullscreenQuadTech;

		bool m_layers[2];
		bool m_displayNormals;
		bool m_showForward;


	};
}

extern "C"
{
	typedef Render::RendererInterface* (*CREATERENDERER)(RootEngine::SubsystemSharedContext);
	RENDERSYS_DLL_EXPORT Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context);
}
