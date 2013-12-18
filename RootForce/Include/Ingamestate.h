#pragma once
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/LightSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/AbilitySystem.h>
#include <RootSystems/Include/Network/MessageHandler.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>
#include <RootSystems/Include/Components.h>
#include "GameStates.h"
#include "ChatSystem.h"

namespace RootForce
{
	class Ingamestate
	{
	public:
		Ingamestate();
		void Initialize(RootEngine::GameSharedContext* p_engineContext, std::shared_ptr<RootForce::Network::MessageHandler> p_networkHandler, std::shared_ptr<ECS::World> p_world, GameStates::PlayData p_playData);
		void Update(float p_deltaTime);
		void Render();
	private:
		RootEngine::GameSharedContext* m_engineContext;
		RootForce::ChatSystemInterface* m_chat;

		std::shared_ptr<ECS::World> m_world;

		std::shared_ptr<RootForce::Network::MessageHandler> m_networkHandler;

		bool m_displayNormals; // TODO: May not be needed?
		bool m_displayPhysicsDebug; // TODO: May not be needed?

		//Game systems
		std::shared_ptr<RootForce::PlayerControlSystem> m_playerControlSystem;
		std::shared_ptr<RootForce::PlayerSystem> m_playerSystem;
		RootForce::PhysicsSystem* m_physicsSystem;
		RootForce::ScriptSystem* m_scriptSystem;
		RootForce::CameraSystem* m_cameraSystem;
		RootForce::LookAtSystem* m_lookAtSystem;
		RootForce::ThirdPersonBehaviorSystem* m_thirdPersonBehaviorSystem;
		std::shared_ptr<RootForce::RenderingSystem> m_renderingSystem;
		std::shared_ptr<RootForce::PointLightSystem> m_pointLightSystem;
		std::shared_ptr<RootForce::AbilitySystem> m_abilitySystem;
	};
}