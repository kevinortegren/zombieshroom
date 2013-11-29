#pragma once

#include <ECS/World.h>

#include <Light.h>
#include <Transform.h>

#include <RootEngine/Render/Include/Renderer.h>

namespace RootForce
{
	class DirectionalLightSystem : public ECS::ComponentSystem
	{
	public:
		DirectionalLightSystem(ECS::World* p_world)
			: ECS::ComponentSystem(p_world)
		{
			SetUsage<DirectionalLight>();
			SetUsage<Transform>();

		}

		void SetRenderInterface(Render::RendererInterface* p_interface)
		{
			m_renderer = p_interface;
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity, float dt);
		void End();
	private:
		ECS::ComponentMapper<DirectionalLight> m_dlights;
		ECS::ComponentMapper<Transform> m_transforms;
		Render::RendererInterface* m_renderer;
	};

	class PointLightSystem : public ECS::ComponentSystem
	{
	public:
		PointLightSystem(ECS::World* p_world)
			: ECS::ComponentSystem(p_world)
		{
			SetUsage<PointLight>();
			SetUsage<Transform>();
		}

		void SetRenderInterface(Render::RendererInterface* p_interface)
		{
			m_renderer = p_interface;
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity, float dt);
		void End();

	private:	
		ECS::ComponentMapper<PointLight> m_plights;
		ECS::ComponentMapper<Transform> m_transforms;
		Render::RendererInterface* m_renderer;
		int lightCount;
	};
}