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

		void CalculatePaintedAABB();
		void DensityRenderToTexture(RootForce::RenderingSystem* m_renderingSystem);
		void Process();

	private:
		RootEngine::GameSharedContext* m_engineContext;
		Render::EffectInterface* m_effect;

		Render::RenderToTextureInterface* m_density;

		// Bounds of painted objects.
		AABB m_aabb;

		// Dimensions of the plane grid.
		int m_width;
		int m_height;

		std::uniform_real_distribution<float> m_floatDistrubution;
		std::default_random_engine m_generator;

		glm::mat4 m_proj;
		glm::mat4 m_view;

		std::map<ECS::Entity*, MatrixData> m_matrices;

		QuadTree* m_quadTree;


	};
}