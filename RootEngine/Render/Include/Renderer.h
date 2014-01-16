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

#define RENDER_TEXTURE_DIFFUSE 0
#define RENDER_TEXTURE_SPECULAR 1
#define RENDER_TEXTURE_NORMAL 2

#define RENDER_TEXTURE_RANDOM 4
#define RENDER_TEXTURE_DEPTH 5

namespace Render
{
	class RendererInterface : public RootEngine::SubsystemInterface
	{
	public:
		virtual void SetupSDLContext(SDL_Window* p_window) = 0;
		virtual void SetResolution(int p_width, int p_height) = 0;

		virtual void SetViewMatrix(glm::mat4 p_viewMatrix) = 0;
		virtual void SetProjectionMatrix(glm::mat4 p_projectionMatrix) = 0;

		virtual void AddShadowcaster(const Render::Shadowcaster& p_shadowcaster) = 0;
		
		virtual void AddRenderJob(const RenderJob& p_job) = 0;
		virtual void AddLine(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color) = 0;
		virtual void Clear() = 0;
		virtual void Render() = 0;
		virtual void Swap() = 0;
		virtual void DisplayNormals(bool p_display) = 0;

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;

		// Resource creation.
		virtual std::shared_ptr<BufferInterface> CreateBuffer() = 0;
		virtual std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() = 0;
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
		void AddShadowcaster(const Render::Shadowcaster& p_shadowcaster);

		void Clear();
		void AddRenderJob(const RenderJob& p_job);
		void AddLine(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color);
		void Render();
		void Swap();
		void DisplayNormals(bool p_display) { m_displayNormals = p_display; }
		bool CheckExtension(const char* p_extension);

		virtual int GetWidth();
		virtual int GetHeight();

		std::shared_ptr<BufferInterface> CreateBuffer() { return std::shared_ptr<BufferInterface>(new Buffer); }
		std::shared_ptr<VertexAttributesInterface> CreateVertexAttributes() { return std::shared_ptr<VertexAttributesInterface>(new VertexAttributes); }
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

		void RenderGeometry();

		void GeometryPass();
		void LightingPass();
		void ForwardPass();
		void Output();

		int GetAvailableVideoMemory(); //Returns currently accessible VRAM in kilobytes

		SDL_GLContext m_glContext;
		SDL_Window* m_window;
		int m_width;
		int m_height;

		GLuint m_fbo;
		GLuint m_color;

		std::vector<RenderJob> m_jobs;
		unsigned m_renderFlags;
		
		GeometryBuffer m_geometryPass;
		LineRenderer m_lineRenderer;

		Mesh m_fullscreenQuad;
	
		ParticleSystemHandler m_particles;
		LightingDevice m_lighting;
		ShadowDevice m_shadowDevice;
		
		std::map<Material*, std::vector<MeshInterface*>> m_materialMeshMap; //For optimization by means of material sorting
		
		struct
		{
			glm::mat4 m_projection;
			glm::mat4 m_view;
			glm::mat4 m_invView;
			glm::mat4 m_invProj;
			glm::mat4 m_invViewProj;

		} m_cameraVars;

		Buffer m_cameraBuffer;
		Buffer m_uniforms;

		std::shared_ptr<TechniqueInterface> m_renderTech;
		std::shared_ptr<TechniqueInterface> m_normalTech;

		bool m_displayNormals;
	};
}

extern "C"
{
	typedef Render::RendererInterface* (*CREATERENDERER)(RootEngine::SubsystemSharedContext);
	RENDERSYS_DLL_EXPORT Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context);
}
