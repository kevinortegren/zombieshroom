#pragma once

#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/LightSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/ParticleSystem.h>
#include <RootSystems/Include/Script.h>
#include <RootSystems/Include/ShadowSystem.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/AnimationSystem.h>
#include <RootSystems/Include/MatchStateSystem.h>

namespace RootForce
{
	namespace ComponentType
	{
		enum ComponentType
		{
			RENDERABLE,
			TRANSFORM,
			POINTLIGHT,
			CAMERA,
			HEALTH,
			PLAYERCONTROL,
			PHYSICS,
			NETWORK,
			LOOKATBEHAVIOR,
			THIRDPERSONBEHAVIOR,
			SCRIPT,
			COLLISION,
			COLLISIONRESPONDER,
			PLAYER,
			ANIMATION,
			PARTICLE,
			TDMRULES, //TODO: name might be inappropriate?
			PLAYERACTION,
			PLAYERPHYSICS,
			ENTITYSTATE,
			SHADOWCASTER,
			DIRECTIONALLIGHT,
			SERVERINFORMATION,
			CLIENT
		};
		
		inline void Initialize()
		{
			RootForce::Renderable::SetTypeId(RootForce::ComponentType::RENDERABLE);
			RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
			RootForce::PointLight::SetTypeId(RootForce::ComponentType::POINTLIGHT);
			RootForce::HealthComponent::SetTypeId(RootForce::ComponentType::HEALTH);
			RootForce::PlayerControl::SetTypeId(RootForce::ComponentType::PLAYERCONTROL);
			RootForce::Physics::SetTypeId(RootForce::ComponentType::PHYSICS);
			RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
			RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
			RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
			RootForce::ThirdPersonBehavior::SetTypeId(RootForce::ComponentType::THIRDPERSONBEHAVIOR);
			RootForce::Script::SetTypeId(RootForce::ComponentType::SCRIPT);
			RootForce::Collision::SetTypeId(RootForce::ComponentType::COLLISION);
			RootForce::CollisionResponder::SetTypeId(RootForce::ComponentType::COLLISIONRESPONDER);
			RootForce::Player::SetTypeId(RootForce::ComponentType::PLAYER);
			RootForce::Animation::SetTypeId(RootForce::ComponentType::ANIMATION);
			RootForce::TDMRuleSet::SetTypeId(RootForce::ComponentType::TDMRULES);
			RootForce::ParticleEmitter::SetTypeId(RootForce::ComponentType::PARTICLE);
			RootForce::PlayerActionComponent::SetTypeId(RootForce::ComponentType::PLAYERACTION);
			RootForce::PlayerPhysics::SetTypeId(RootForce::ComponentType::PLAYERPHYSICS);
			RootForce::StateComponent::SetTypeId(RootForce::ComponentType::ENTITYSTATE);
			RootForce::Shadowcaster::SetTypeId(RootForce::ComponentType::SHADOWCASTER);
			RootForce::DirectionalLight::SetTypeId(RootForce::ComponentType::DIRECTIONALLIGHT);
			RootForce::Network::ServerInformationComponent::SetTypeId(RootForce::ComponentType::SERVERINFORMATION);
			RootForce::Network::ClientComponent::SetTypeId(RootForce::ComponentType::CLIENT);
		};

		inline void InitializeServerComponents()
		{
			RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
			RootForce::HealthComponent::SetTypeId(RootForce::ComponentType::HEALTH);
			RootForce::Physics::SetTypeId(RootForce::ComponentType::PHYSICS);
			RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
			RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
			RootForce::Script::SetTypeId(RootForce::ComponentType::SCRIPT);
			RootForce::Collision::SetTypeId(RootForce::ComponentType::COLLISION);
			RootForce::CollisionResponder::SetTypeId(RootForce::ComponentType::COLLISIONRESPONDER);
			RootForce::Player::SetTypeId(RootForce::ComponentType::PLAYER);
			RootForce::TDMRuleSet::SetTypeId(RootForce::ComponentType::TDMRULES);
			RootForce::PlayerActionComponent::SetTypeId(RootForce::ComponentType::PLAYERACTION);
			RootForce::PlayerPhysics::SetTypeId(RootForce::ComponentType::PLAYERPHYSICS);
            RootForce::StateComponent::SetTypeId(RootForce::ComponentType::ENTITYSTATE);
			RootForce::Network::ServerInformationComponent::SetTypeId(RootForce::ComponentType::SERVERINFORMATION);
		}
	}

	
}