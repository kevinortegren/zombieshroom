#pragma once

#include <Utility/ECS/Include/World.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/Transform.h>

namespace RootForce
{
	class TransformInterpolationSystem : public ECS::EntitySystem
	{
	public:
		TransformInterpolationSystem(ECS::World* p_world);

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
	private:
		ECS::ComponentMapper<Transform> m_transforms;
	};
}