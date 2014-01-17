#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility\ECS\Include\Component.h>
#include <Utility\ECS\Include\EntitySystem.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/BitStream.h>

#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Orientation.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/PlayerSystem.h>

#include <glm/glm.hpp>

namespace RootForce
{
	struct NetworkContext;
}


namespace RootSystems
{
	class ActionSystem : public ECS::EntitySystem
	{
	public:
		ActionSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world)
			, m_engineContext(p_engineContext) { }
		void Init() { }
		void ProcessEntity(ECS::Entity* p_entity);
	private:
		RootEngine::GameSharedContext* m_engineContext;
	};

}