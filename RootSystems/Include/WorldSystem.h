#pragma once

#include <RootSystems/Include/DataStructures/Quad.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Frustum.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <array>

namespace RootForce
{
	class WorldSystem : public ECS::VoidSystem
	{
	public:
		WorldSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::VoidSystem(p_world), m_engineContext(p_engineContext), m_showDebug(false), m_staticMesh(nullptr) {}

#ifndef COMPILE_LEVEL_EDITOR
		void LoadWorld(const std::string& p_worldName);
		void CreatePlayerCamera();
#endif

		void SubdivideTree();

		void SetAmbientLight(glm::vec4 p_ambient);
		void CreateSun();
		void CreateSkyBox();
		
		void BuildStaticShadowMesh();
		void AddStaticEntitiesToPhysics();
		
		void Process();
		void ShowDebug(bool p_value);

		QuadTree* GetQuadTree();

	private:
		RootEngine::GameSharedContext* m_engineContext;
		QuadTree m_quadTree;
		bool m_showDebug;
		unsigned m_culledNodes;

		Render::MeshInterface* m_staticMesh;
	};
}
