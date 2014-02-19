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
#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems/Include/ChatSystem.h>
#endif
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
		void CreateWater(float p_height);
		void Disturb(float p_x, float p_z, float p_power, int p_radius );
		void InitDisturb();
		
		void CalculateWaterConstants();

		float GetWaterHeight();
		void SetWaterHeight(float p_height);

		#ifndef COMPILE_LEVEL_EDITOR
		void ParseCommands(RootForce::ChatSystem* p_chat, std::stringstream* p_data);
		#endif

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

		bool		m_wireFrame, m_pause;
		int			m_texSize, m_gridSize;	
		float		m_timeStep, m_dt, m_dx, m_speed, m_damping, m_mk1, m_mk2, m_mk3, m_scale, m_totalTime;;	
		float*		m_textureData;
		glm::vec4	m_waterOptions;

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
		void ToggleDepth();
		void ToggleNormalMaps();

		void ResetWater();
	};
}