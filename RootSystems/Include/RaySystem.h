#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <Utility/ECS/Include/World.h>

namespace RootForce
{
	struct RayComponent : public ECS::Component<RayComponent>
	{
		ECS::Entity* HitEntity;
		glm::vec3 StartPosition;
		glm::vec3 Direction;
		glm::vec3 HitPos;
		float Distance;
		//Some kind of renderthingymajig
		bool Render;
		RayComponent()
			: HitEntity(nullptr),
			  StartPosition(glm::vec3(0,0,0)),
			  Direction(glm::vec3(0,0,0)),
			  HitPos(glm::vec3(0,0,0)),
			  Distance(0.0f),
			  Render(false)
		{}
	};

	class RaySystem : public ECS::EntitySystem
	{
	public:
		RaySystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{ 
			SetUsage<RootForce::RayComponent>();
			SetUsage<RootForce::Collision>();
		}
		void Init();
		void ProcessEntity(ECS::Entity* p_entity);

	private:


		RootEngine::GameSharedContext* m_engineContext;
		ECS::ComponentMapper<RootForce::Collision> m_collision;
		ECS::ComponentMapper<RootForce::RayComponent> m_ray;
	};
}