#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/Transform.h>

namespace RootSystems
{
	class RespawnSystem : public ECS::EntitySystem
	{
	public:
		RespawnSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world)
			, m_engineContext(p_engineContext)
		{ 
			SetUsage<RootForce::HealthComponent>();
			SetUsage<RootForce::Collision>();
		}
		void Init();
		void ProcessEntity(ECS::Entity* p_entity);
	private:
		RootForce::Transform* GetRandomSpawnpoint();
		RootEngine::GameSharedContext* m_engineContext;

		ECS::ComponentMapper<RootForce::HealthComponent> m_health;
		ECS::ComponentMapper<RootForce::Collision> m_collision;

		ECS::GroupManager::GroupRange m_spawnPoints;
	};
}