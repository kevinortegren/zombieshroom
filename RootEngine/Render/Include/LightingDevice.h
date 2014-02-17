#pragma once

#include <RootEngine\Render\Include\Mesh.h>
#include <RootEngine\Render\Include\Effect.h>
#include <RootEngine\Render\Include\Light.h>
#include <RootEngine\Render\Include\Texture.h>
#include <RootEngine\Render\Include\GeometryBuffer.h>

#define RENDER_MAX_DIRECTIONALLIGHTS 5
#define RENDER_MAX_POINTLIGHTS 500

namespace Render
{
	namespace BackgroundBlend
	{
		enum BackgroundBlend
		{
			ADDATIVE,
			ALPHABLEND
		};
	}

	class LightingDevice
	{
	public:
		friend class GLRenderer;
		LightingDevice();

		void Init(GLRenderer* p_renderer, int p_width, int p_height, GeometryBuffer* p_gbuffer, Mesh* p_fullscreenQuad);

		void SetAmbientLight(const glm::vec4& p_color);
		void AddDirectionalLight(const DirectionalLight& p_light, int index);
		void AddPointLight(const PointLight& p_light, int index);

		void BufferLights();
		void Ambient();
		void Directional();
		void PointLightStencil();
		void PointLightRender();
		void PointLightFSQ();
		void BackgroundBlend(BackgroundBlend::BackgroundBlend p_mode);

		void Clear();
		void ResetLights();

		void Resize(int p_width, int p_height);
		
	private:

		// Light data.
		struct
		{
			glm::vec4 m_ambient;
			DirectionalLight m_dlights[RENDER_MAX_DIRECTIONALLIGHTS];
			PointLight m_plights[RENDER_MAX_POINTLIGHTS];
		
		} m_lightVars;

		BufferInterface* m_lights;

		size_t m_numDirectionalLights;
		size_t m_numPointLights;

		GLuint m_fbo;
		TextureInterface* m_la;

		// Meshes.
		MeshInterface* m_fullscreenQuad;
		MeshInterface* m_unitSphere;
		MeshInterface* m_triangle;

		// Programs.
		std::shared_ptr<ProgramInterface> m_ambient;
		std::shared_ptr<ProgramInterface> m_directional;
		std::shared_ptr<ProgramInterface> m_pointLightFullScreen;
		std::shared_ptr<ProgramInterface> m_pointLightStencil;
		std::shared_ptr<ProgramInterface> m_pointLight;
		std::shared_ptr<ProgramInterface> m_backgroundAddative;
		std::shared_ptr<ProgramInterface> m_backgroundAlphaBlend;

		// TODO: Add gpu timer.
	};
}