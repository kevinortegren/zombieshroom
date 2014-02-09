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
	class LightingDevice
	{
	public:
		friend class GLRenderer;
		LightingDevice();
		void Init(GLRenderer* p_renderer, int p_width, int p_height, GeometryBuffer* p_gbuffer);

		void SetAmbientLight(const glm::vec4& p_color);
		void AddDirectionalLight(const DirectionalLight& p_light, int index);
		void AddPointLight(const PointLight& p_light, int index);

		void Clear();
		void Process(Mesh& p_fullscreenQuad, int p_backgroundEffect);
		void Resize(int p_width, int p_height);
		void ClearLights();

	private:

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

		GLuint m_fbo;
		TextureInterface* m_la;

		MeshInterface* m_unitSphere;
	};
}