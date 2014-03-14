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
			ADDITIVE,
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
		DirectionalLight* GetDirectionalLight();

		void SSAO();
		void Clear();
		void Process(Mesh& p_fullscreenQuad);
		void Resize(int p_width, int p_height);
		void BufferLights();
		void Ambient();
		void Directional();
		void Point();

		void ShowAmbient(bool p_value);
		void ShowDirectional(bool p_value);
		void ShowPointLights(bool p_value);
		void ShowBackgroundBlend(bool p_value);
		void ShowSSAO(bool p_value);

		void BackgroundBlend(BackgroundBlend::BackgroundBlend p_mode);

		void ResetLights();

	private:

		void PointLightStencil();
		void PointLightRender();
		void PointLightFSQ();

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

		std::shared_ptr<TechniqueInterface> m_deferredTech;
		std::shared_ptr<TechniqueInterface> m_ssaoTech;

		GLuint m_fbo;
		TextureInterface* m_la;
		TextureInterface* m_ssaoTex;
		TextureInterface* m_noiseSSAOTex;
		glm::vec4 m_kernel[16];
		float Random(float p_low, float p_high);
		void SetupSSAO();
		void BeginSSAO();


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
		std::shared_ptr<ProgramInterface> m_backgroundAdditive;
		std::shared_ptr<ProgramInterface> m_backgroundAlphaBlend;
		std::shared_ptr<ProgramInterface> m_pointLightFSQ;

		bool m_showAmbient;
		bool m_showDirectional;
		bool m_showPointlights;
		bool m_showBackgroundBlend;
		bool m_showSSAO;
	};
}