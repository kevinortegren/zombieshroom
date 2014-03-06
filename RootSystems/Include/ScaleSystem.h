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


namespace RootForce
{
	struct Scalable : public ECS::Component<Scalable>
	{
		Scalable() : MaxScale(0.0f), ScaleSpeed(0.0f), CurScale(0.0f){}
		float MaxScale;
		float ScaleSpeed;
		float CurScale;
	};

	struct ScaleSystem : public ECS::EntitySystem
	{
		ScaleSystem(ECS::World* p_world);
		~ScaleSystem();
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();

	private:

		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::Scalable> m_scalable;

		ECS::World*	m_world;
	};
}