#pragma once

#include <map>
#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/Transform.h>
#include "glm/glm.hpp"
#include <RootSystems/Include/PlayerSystem.h>

namespace RootForce
{

	namespace WaterState
	{
		enum WaterState
		{
			OVER_WATER,
			EDGE_WATER,
			UNDER_WATER,
		};
	}

	struct WaterCollider : public ECS::Component<WaterCollider>
	{
		WaterCollider(): m_edgeWaterTime(0.0f), m_radius(1), m_disturbPower(0.5f), m_disturbInterval(0.3f){}

		int						m_radius;
		float					m_disturbPower;
		float					m_edgeWaterTime;
		float					m_disturbInterval;
		glm::vec3				m_prevPos;
		WaterState::WaterState	m_waterState;
	};

	struct WaterSystem : public ECS::EntitySystem
	{
		WaterSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_context);
		~WaterSystem();
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
		void CreateWater();
		void Disturb(float p_x, float p_z, float p_power, int p_radius );
		void ParseCommands(std::stringstream* p_data);

		ECS::Entity* CreateDefaultWater();

	private:

		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::WaterCollider> m_waterCollider;

		ECS::World*						m_world;
		Render::ComputeJob				m_computeJob;
		RootForce::Renderable*			m_renderable;
		Render::EffectInterface*		m_effect;			
		Render::TextureInterface*		m_texture[3];
		Render::TextureInterface*		m_normalTexture;
		Logging::LoggingInterface*		m_logger;
		RootEngine::GameSharedContext*	m_context;

		bool		m_wireFrame, m_pause, m_playerWaterDeath, m_showDebugDraw;
		int			m_texSize, m_gridSize;	
		float		m_timeStep, m_dt, m_dx, m_speed, m_damping, m_mk1, m_mk2, m_mk3, m_scale, m_totalTime;;	
		float*		m_textureData;
		glm::vec4	m_waterOptions;

		void CalculateWaterConstants();
		bool ValidValues();
		void CreateWaterMesh();
		glm::vec2 WorldSpaceToWaterSpace(glm::vec2 p_worldSpace);

		void SetDamping(float p_damping);
		void SetSpeed(float p_speed);

		void IncreaseDamping();
		void IncreaseSpeed();

		void DecreaseDamping();
		void DecreaseSpeed();

		void TogglePause();
		void ToggleWireFrame();
		void ToggleReflections();
		void ToggleRefractions();
		void ToggleNormalMaps();
		void ToggleCollideDeath();
		void ToggleCollisionDebugDraw();

		void ResetWater();

		void SaveWater();
		void LoadWater();

		void PlayWaterSound(int p_radius, glm::vec3 p_pos);
	};
}