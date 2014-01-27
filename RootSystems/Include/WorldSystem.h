#pragma once

#include <RootSystems/Include/DataStructures/Quad.h>

#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Frustum.h>
#include <array>

#define PLAYER_NUM_ABILITIES 1

namespace RootForce
{
	class WorldSystem : public ECS::VoidSystem
	{
	public:
		WorldSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::VoidSystem(p_world), m_engineContext(p_engineContext), m_showDebug(false) {}

		void CreateWorld(const std::string& p_worldName);
		void Process();
		void ShowDebug(bool p_value);
	private:
		RootEngine::GameSharedContext* m_engineContext;
		QuadTree m_quadTree;
		bool m_showDebug;
		unsigned m_culledNodes;
	};
}
