#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Shadowcaster.h>
#include <RootSystems/Include/Transform.h>

namespace RootForce
{
	class ShadowSystem : public ECS::EntitySystem
	{
	public:
		ShadowSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{
			SetUsage<Shadowcaster>();
			SetUsage<Transform>();
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();

		void SetAABB(const RootForce::AABB& p_worldAABB)
		{
			m_worldAABB = p_worldAABB;
		}

		ECS::ComponentMapper<Shadowcaster> m_shadowcasters;
		ECS::ComponentMapper<Transform> m_transforms;

	private:
		int m_shadowcasterCount;
		RootForce::AABB m_worldAABB;
	};
}