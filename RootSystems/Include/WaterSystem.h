#pragma once

#include <map>
#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Include/GameSharedContext.h>
namespace RootForce
{
	struct WaterSystem : public ECS::VoidSystem
	{
		WaterSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_context)
			: ECS::VoidSystem(p_world), m_context(p_context), m_world(p_world)
		{
		}
		void Init();
		void Begin();
		void Process();
		void End();
		void CreateRenderable();
		void Disturb(int p_x, int p_z, float p_power);
		
	private:
		Render::EffectInterface*				m_effect;			
		Render::TextureInterface*				m_texture[2];
		Render::TextureInterface*				m_normalTexture;
		Logging::LoggingInterface*				m_logger;
		RootEngine::GameSharedContext*			m_context;
		Render::ComputeJob						m_computeJob;
		ECS::World*								m_world;
		RootForce::Renderable*					m_renderable;

		float m_mk1, m_mk2, m_mk3;
		float m_timeStep, m_dt;
		int m_maxX, m_maxZ;
	};
}