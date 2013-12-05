#pragma once

#include <Utility\ECS\Include\World.h>
#include "Transform.h"
#include <RootEngine\Render\Include\Renderer.h>

namespace RootForce
{
	struct PointLight : public ECS::Component<PointLight>
	{
		glm::vec4 m_color;
		glm::vec3 m_attenuation;
		float m_range;
	};

	class PointLightSystem : public ECS::EntitySystem
	{
	public:
		PointLightSystem(ECS::World* p_world, Render::RendererInterface* p_renderer)
			: ECS::EntitySystem(p_world), m_renderer(p_renderer)
		{
			SetUsage<PointLight>();
			SetUsage<Transform>();
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();

	private:	
		ECS::ComponentMapper<PointLight> m_plights;
		ECS::ComponentMapper<Transform> m_transforms;
		Render::RendererInterface* m_renderer;
		int m_lightCount;
	};
}