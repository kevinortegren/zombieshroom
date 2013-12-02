#pragma once

#include <ECS/World.h>

#include <Light.h>
#include <Transform.h>

#include <RootEngine/Render/Include/Renderer.h>

namespace RootForce
{
	class PointLightSystem : public ECS::ComponentSystem
	{
	public:
		PointLightSystem(ECS::World* p_world, Render::RendererInterface* p_renderer)
			: ECS::ComponentSystem(p_world), m_renderer(p_renderer)
		{
			SetUsage<PointLight>();
			SetUsage<Transform>();
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity, float dt);
		void End();

	private:	
		ECS::ComponentMapper<PointLight> m_plights;
		ECS::ComponentMapper<Transform> m_transforms;
		Render::RendererInterface* m_renderer;
		int m_lightCount;
	};
}