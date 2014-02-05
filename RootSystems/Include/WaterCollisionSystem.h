#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/WaterSystem.h>
#include "glm/glm.hpp"

namespace RootForce
{
	namespace WaterState
	{
		enum WaterState
		{
			OVER_WATER,
			EDGE_WATER,
			UNDER_WATER,
		};
	}

	struct WaterCollider : public ECS::Component<WaterCollider>
	{
		WaterCollider(): m_edgeWaterTime(0.0f), m_radius(0.0f), m_disturbPower(1.0f), m_disturbInterval(0.3f){}

		float					m_radius;
		float					m_disturbPower;
		float					m_edgeWaterTime;
		float					m_disturbInterval;
		glm::vec3				m_prevPos;
		WaterState::WaterState	m_waterState;
	};
}

namespace RootSystems
{
	class WaterCollsionSystem : public ECS::EntitySystem
	{
	public:

		WaterCollsionSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext, RootForce::WaterSystem* p_waterSystem);

		void Init();
		void ProcessEntity(ECS::Entity* p_entity);

	private:

		RootEngine::GameSharedContext* m_engineContext;

		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::WaterCollider> m_waterCollider;

		RootForce::WaterSystem* m_waterSystem;
	};
}