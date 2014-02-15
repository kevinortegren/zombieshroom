#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/RenderToTexture.h>
#include <RootSystems/Include/DataStructures/Quad.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <random>

// Cell defines.
#define BOTANY_MAX_CELLS 1000

#define BOTANY_VERTEX_SIZE 28
#define BOTANY_MAX_POINTS_PER_CELL 100000
#define BOTANY_CELL_SIZE BOTANY_VERTEX_SIZE * BOTANY_MAX_POINTS_PER_CELL

// Mesh defines.
#define BOTANY_MESHES_SIZE 100
#define BOTANY_MESHES_PER_CELL 5

namespace RootForce
{
	struct BotanyCell
	{
		BotanyCell()
			: m_meshSize(0) {}

		int m_meshSize;
		int m_meshIndices[BOTANY_MESHES_PER_CELL];
	};

	class BotanySystem : public ECS::VoidSystem
	{
	public:
		BotanySystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::VoidSystem(p_world), m_engineContext(p_engineContext) {}

		void Initialize();
		void SetQuadTree(QuadTree* p_quadTree);
		void Process();

	private:
		void Construct(QuadNode* p_node);

		// Cell collection.
		BotanyCell m_cells[BOTANY_MAX_CELLS];

		int m_meshCount;
		Render::MeshInterface* m_meshes[BOTANY_MESHES_SIZE];
		int m_cellDirectory[BOTANY_MESHES_SIZE];

		RootEngine::GameSharedContext* m_engineContext;	
		Render::EffectInterface* m_effect;
		Render::Material* m_material;
		QuadTree* m_quadTree;
	};
}