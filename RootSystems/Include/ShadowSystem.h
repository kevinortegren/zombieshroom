#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Shadowcaster.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/DataStructures/Quad.h>

#ifndef COMPILE_LEVEL_EDITOR

#define SHADOWSYSTEM_CULL
//#define SHADOWSYSTEM_DEBUG
#endif

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

		void SetQuadTree(QuadTree* p_quadTree);

		ECS::ComponentMapper<Shadowcaster> m_shadowcasters;
		ECS::ComponentMapper<Transform> m_transforms;

	private:
		
		glm::mat4 OrthoProjectionFromFrustum(Frustum* p_frustum, glm::mat4 p_lightViewMatrix);
		
		float m_maxWorldX;
		float m_minWorldX;
		float m_minWorldY;
		float m_maxWorldZ;
		float m_minWorldZ;
		float m_maxWorldY;

		// World AABB points.
		glm::vec3 worldCorners[8];

		QuadTree* m_quadTree;
	};
}