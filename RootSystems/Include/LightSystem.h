#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Transform.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Frustum.h>

namespace RootForce
{
	struct PointLight : public ECS::Component<PointLight>
	{
		glm::vec4 m_color;
		glm::vec3 m_attenuation;
		float m_range;
	};

	struct DirectionalLight : public ECS::Component<DirectionalLight>
	{
		glm::vec4 m_color;
	};

	class PointLightSystem : public ECS::EntitySystem
	{
	public:
		PointLightSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world), m_context(p_engineContext)
		{
			SetUsage<PointLight>();
			SetUsage<Transform>();
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();

	private:	

		void LightStressTest();

		float CalculateScale(PointLight* p_pl);

		ECS::ComponentMapper<PointLight> m_plights;
		ECS::ComponentMapper<Transform> m_transforms;
		RootEngine::GameSharedContext* m_context;

		Frustum* m_frustum;
		int m_lightCount;
		bool m_test;
	};

	class DirectionalLightSystem : public ECS::EntitySystem
	{
	public:
		DirectionalLightSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world), m_context(p_engineContext)
		{
			SetUsage<DirectionalLight>();
			SetUsage<Transform>();
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
	private:
		ECS::ComponentMapper<DirectionalLight> m_dlights;
		ECS::ComponentMapper<Transform> m_transforms;
		RootEngine::GameSharedContext* m_context;
		int m_lightCount;

	};
}