#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/AnimationSystem.h>

namespace RootForce
{
	namespace Network
	{
		class NetworkDebugSystem : public ECS::EntitySystem
		{
		public:
			NetworkDebugSystem(ECS::World* p_world);

			void Init();
			void ProcessEntity(ECS::Entity* p_entity);
		private:
			ECS::ComponentMapper<NetworkComponent> m_networks;
			ECS::ComponentMapper<PlayerComponent> m_players;
			ECS::ComponentMapper<Transform> m_transforms;
			ECS::ComponentMapper<Animation> m_animations;
			ECS::ComponentMapper<StateComponent> m_states;
		};
	}
}