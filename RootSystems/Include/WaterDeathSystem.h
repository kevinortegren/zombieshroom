#pragma once

#include <map>
#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/Transform.h>
#include "glm/glm.hpp"
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/WaterSystem.h>

namespace RootForce
{
	struct WaterDeathSystem : public ECS::EntitySystem
	{
		WaterDeathSystem(ECS::World* p_world);
		~WaterDeathSystem();
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();

	private:

		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::WaterCollider> m_waterCollider;
		ECS::ComponentMapper<RootForce::HealthComponent> m_health;

		ECS::World*	m_world;

		float m_waterHeight;
	};
}