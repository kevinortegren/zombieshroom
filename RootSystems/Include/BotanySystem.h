#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/RenderToTexture.h>
#include <RootSystems/Include/DataStructures/Quad.h>
#include <RootSystems/Include/RenderingSystem.h>

namespace RootForce
{
	class BotanySystem : public ECS::VoidSystem
	{
	public:
		BotanySystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::VoidSystem(p_world), m_engineContext(p_engineContext) {}

		void Initialize();

		void CalculatePaintedAABB();
		void DensityRenderToTexture(RootForce::RenderingSystem* m_renderingSystem);

		void Process();

	private:
		RootEngine::GameSharedContext* m_engineContext;
		Render::EffectInterface* m_effect;

		Render::RenderToTextureInterface* m_density;

		
		AABB m_aabb;

		int m_width;
		int m_height;

		glm::mat4 m_proj;
		glm::mat4 m_view;

		std::map<ECS::Entity*, MatrixData> m_matrices;
	};
}