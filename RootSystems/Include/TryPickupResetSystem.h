#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/ParticleSystem.h>

namespace RootForce
{
	class TryPickupResetSystem : public ECS::EntitySystem
	{
	public:
		TryPickupResetSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{ 
			SetUsage<RootForce::TryPickupComponent>();
		}
		void Init();
		void ProcessEntity(ECS::Entity* p_entity);
	private:

		RootEngine::GameSharedContext* m_engineContext;

		ECS::ComponentMapper<RootForce::TryPickupComponent> m_tryPickupComponents;
	};
}