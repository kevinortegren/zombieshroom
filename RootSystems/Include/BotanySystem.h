#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/DataStructures/Quad.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <random>

#define BOTANY_MAX_CELLS 1000
#define BOTANY_VERTEX_SIZE 28
#define BOTANY_MAX_POINTS_PER_CELL 5000
#define BOTANY_CELL_SIZE BOTANY_VERTEX_SIZE * BOTANY_MAX_POINTS_PER_CELL
#define BOTANY_MESHES_SIZE 200
#define BOTANY_MESHES_PER_CELL 5
#define BOTANY_VERTICES_PER_TERRAIN_CHUNK 500
#define BOTANY_CULL_RANGE 100

namespace RootForce
{
	struct BotanyCell
	{
		BotanyCell()
			: m_meshSize(0) {}

		int m_meshSize;
		int m_meshIndices[BOTANY_MESHES_PER_CELL];
	};

	struct BotanyTextures
	{
		std::string m_diffuse;
		std::string m_translucency;
		std::string m_billboard;
		std::string m_terrainTexture;
	};

	class BotanySystem : public ECS::VoidSystem
	{
	public:
		BotanySystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::VoidSystem(p_world), m_engineContext(p_engineContext), m_initialized(false) {}

		void Initialize(BotanyTextures& m_textures);
		void Process();
		void Divide();

		void SetGrassFactor(float p_grassFactor);
		void SetLOD1Distance(float p_distance);
		void SetLOD2Distance(float p_distance);


		void ParseCommands(std::stringstream* p_ss);

	private:
		void Construct(QuadNode* p_node);

		QuadTree m_quadTree;

		// Cell collection.
		BotanyCell m_cells[BOTANY_MAX_CELLS];

		int m_meshCount;
		Render::MeshInterface* m_meshes[BOTANY_MESHES_SIZE];
		int m_cellDirectory[BOTANY_MESHES_SIZE];

		RootEngine::GameSharedContext* m_engineContext;	
		Render::EffectInterface* m_effect;
		Render::Material* m_material;
		bool m_initialized;

		bool m_show;

		struct
		{
			float m_grassFactor;

		} m_updateUniforms;

		struct
		{
			glm::vec3 m_playerPosition;
			float m_padd0;
			float m_lod1Distance;
			float m_lod2Distance;
		} m_renderUniforms;


	};
}