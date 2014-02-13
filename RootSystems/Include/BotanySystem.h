#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/RenderToTexture.h>
#include <RootSystems/Include/DataStructures/Quad.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <random>

namespace RootForce
{
	struct BotanyCell
	{
		Render::Buffer* m_buffer;
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
		RootEngine::GameSharedContext* m_engineContext;
		
		Render::EffectInterface* m_effect;
		Render::Material* m_material;

		QuadTree* m_quadTree;

		std::vector<Render::MeshInterface*> m_meshes;
		std::vector<float> m_distances;
	};
}