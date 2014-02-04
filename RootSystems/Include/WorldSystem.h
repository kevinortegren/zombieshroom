#pragma once

#include <RootSystems/Include/DataStructures/Quad.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Frustum.h>
#include <array>

namespace RootForce
{
	class WorldSystem : public ECS::VoidSystem
	{
	public:
		WorldSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::VoidSystem(p_world), m_engineContext(p_engineContext), m_showDebug(false) {}

#ifndef COMPILE_LEVEL_EDITOR
		void LoadWorld(const std::string& p_worldName);
#endif

		void SetAmbientLight(glm::vec3 p_ambient);
		void CreateSun();
		void CreateSkyBox();
		void CreatePlayerCamera();
		void AddStaticEntitiesToPhysics();
		
		void Process();
		void ShowDebug(bool p_value);

		QuadTree* GetQuadTree();

	private:
		RootEngine::GameSharedContext* m_engineContext;
		QuadTree m_quadTree;
		bool m_showDebug;
		unsigned m_culledNodes;
	};
}
