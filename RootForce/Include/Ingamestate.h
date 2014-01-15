#pragma once
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/LightSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/AnimationSystem.h>
#include <RootSystems/Include/MatchStateSystem.h>
#include <RootSystems/Include/ActionSystem.h>
#include <RootSystems/Include/RespawnSystem.h>
#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>
#include <RootSystems/Include/Components.h>

#include <RootForce/Include/GameStates.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/ChatSystem.h>
#include <RootForce/Include/HUD.h>
#include <thread>
namespace RootForce
{
	class IngameState
	{
	public:
		IngameState(NetworkContext& p_networkContext, SharedSystems& p_sharedSystems);

		void Initialize();
		void Enter();
		void Exit();
		GameStates::GameStates Update(float p_deltaTime);
	private:
		NetworkContext& m_networkContext;

		bool m_displayNormals; // TODO: May not be needed?
		bool m_displayPhysicsDebug; // TODO: May not be needed?

		std::shared_ptr<RootForce::HUD> m_hud;

		// Game systems
		SharedSystems& m_sharedSystems;
		std::shared_ptr<RootForce::PlayerControlSystem> m_playerControlSystem;
		RootForce::PhysicsSystem* m_physicsSystem;
		RootForce::CollisionSystem* m_collisionSystem;
		RootForce::RenderingSystem* m_renderingSystem;
		RootForce::PointLightSystem* m_pointLightSystem;
		RootForce::ParticleSystem* m_particleSystem;
		RootForce::CameraSystem* m_cameraSystem;
		RootForce::LookAtSystem* m_lookAtSystem;
		RootForce::ThirdPersonBehaviorSystem* m_thirdPersonBehaviorSystem;
		RootForce::AnimationSystem* m_animationSystem;
		RootForce::MatchStateSystem* m_gameLogicSystem;
		RootSystems::ActionSystem* m_actionSystem;
		RootSystems::RespawnSystem* m_respawnSystem;

	};
}