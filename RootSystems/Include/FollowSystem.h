#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Transform.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Network/NetworkComponents.h>

namespace RootForce
{
	struct FollowComponent : public ECS::Component<FollowComponent>
	{
		//ECS::Entity* Target;
		Network::NetworkEntityID TargetID;
		float Offset;
		FollowComponent()
			: TargetID(),
			  Offset(0.0f)
		{}
	};

	class FollowSystem : public ECS::EntitySystem
	{
	public:
		FollowSystem(ECS::World* p_world/*, RootEngine::GameSharedContext* p_engineContext, std::string p_workingDir*/)
			: ECS::EntitySystem(p_world)
			//, m_engineContext(p_engineContext)
		{ 
			SetUsage<RootForce::FollowComponent>();
			SetUsage<RootForce::Transform>();
		}
		void Init();
		void ProcessEntity(ECS::Entity* p_entity);

	private:


		RootEngine::GameSharedContext* m_engineContext;
		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::FollowComponent> m_target;
	};
}