#pragma once
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/LightSystem.h>
#include <RootSystems/Include/Shadowcaster.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/AnimationSystem.h>
#include <RootSystems/Include/RagdollSystem.h>
#include <RootSystems/Include/MatchStateSystem.h>
#include <RootSystems/Include/ActionSystem.h>
#include <RootSystems/Include/RespawnSystem.h>
#include <RootSystems/Include/StateSystem.h>
#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>
#include <RootSystems/Include/Components.h>
#include <RootSystems/Include/ConcurrentTest.h>
#include <RootSystems/Include/WaterSystem.h>
#include <RootSystems/Include/AbilityRespawnSystem.h>
#include <RootSystems/Include/TryPickupResetSystem.h>
#include <RootSystems/Include/SoundSystem.h>

#include <RootForce/Include/GameStates.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/ChatSystem.h>
#include <RootForce/Include/HUD.h>
#include <RootForce/Include/IngameMenu.h>
#include <thread>
namespace RootForce
{
	class IngameState
	{
	public:
		IngameState(NetworkContext& p_networkContext, SharedSystems& p_sharedSystems, Keymapper* p_keymapper);

		void Initialize();
		void Enter();
		void Exit();
		GameStates::GameStates Update(float p_deltaTime);
	private:
		NetworkContext& m_networkContext;

		bool m_displayNormals;
		bool m_displayPhysicsDebug; 
		bool m_displayWorldDebug;
		bool m_displayIngameMenu;

		std::shared_ptr<RootForce::HUD> m_hud;
		std::shared_ptr<RootForce::IngameMenu> m_ingameMenu;

		// Game systems
		SharedSystems& m_sharedSystems;
		std::shared_ptr<RootForce::PlayerControlSystem> m_playerControlSystem;
		RootForce::PhysicsTransformCorrectionSystem*	m_physicsTransformCorrectionSystem;
		RootForce::PhysicsSystem*						m_physicsSystem;
		RootForce::CollisionSystem*						m_collisionSystem;
		RootForce::ShadowSystem*						m_shadowSystem;
		RootForce::RenderingSystem*						m_renderingSystem;
		RootForce::PointLightSystem*					m_pointLightSystem;
		RootForce::ParticleSystem*						m_particleSystem;
		RootForce::CameraSystem*						m_cameraSystem;
		RootForce::LookAtSystem*						m_lookAtSystem;
		RootForce::ThirdPersonBehaviorSystem*			m_thirdPersonBehaviorSystem;
		RootForce::AnimationSystem*						m_animationSystem;
		RootForce::RagdollSystem*						m_ragdollSystem;
		RootForce::MatchStateSystem*					m_gameLogicSystem;
		RootForce::WaterSystem*							m_waterSystem;
		RootSystems::ActionSystem*						m_actionSystem;
		RootSystems::StateSystem*						m_stateSystem;
		RootForce::DirectionalLightSystem* 				m_directionlLightSystem;
		RootForce::TryPickupResetSystem* 				m_tryPickupResetSystem;
		RootForce::SoundSystem*							m_soundSystem;

		Keymapper* m_keymapper;
	};
}