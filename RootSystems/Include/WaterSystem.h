#pragma once

#include <map>
#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Include/GameSharedContext.h>

namespace RootForce
{
	struct WaterSystem : public ECS::VoidSystem
	{
		WaterSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_context)
			: ECS::VoidSystem(p_world), m_context(p_context), m_world(p_world), m_wireFrame(false), m_scale(1.0f)
		{}

		void Init();
		void Begin();
		void Process();
		void End();
		void CreateRenderable();
		void Disturb(float p_x, float p_z, float p_power);
		void InitDisturb();
		
		void CalculateWaterConstants();

		void SetDamping(float p_damping);
		void SetSpeed(float p_speed);

		void IncreaseDamping();
		void IncreaseSpeed();

		void DecreaseDamping();
		void DecreaseSpeed();

		void TogglePause();
		void ToggleWireFrame();


	private:

		ECS::World*						m_world;
		Render::ComputeJob				m_computeJob;
		RootForce::Renderable*			m_renderable;
		Render::EffectInterface*		m_effect;			
		Render::TextureInterface*		m_texture[3];
		Render::TextureInterface*		m_normalTexture;
		Logging::LoggingInterface*		m_logger;
		RootEngine::GameSharedContext*	m_context;

		bool	m_wireFrame, m_pause;
		int		m_maxX, m_maxZ;	
		float	m_timeStep, m_dt, m_dx, m_speed, m_damping, m_mk1, m_mk2, m_mk3, m_scale;	

		bool ValidValues();
	};
}