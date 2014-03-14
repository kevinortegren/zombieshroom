#include <RootSystems/Include/Components.h>

#ifndef COMPILE_LEVEL_EDITOR

namespace RootForce
{
	namespace ComponentType
	{

		void Initialize()
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
			RootForce::PlayerComponent::SetTypeId(RootForce::ComponentType::PLAYER);
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
			RootForce::Ragdoll::SetTypeId(RootForce::ComponentType::RAGDOLL);
			RootForce::WaterCollider::SetTypeId(RootForce::ComponentType::WATERCOLLIDER);
			RootForce::AbilitySpawnComponent::SetTypeId(RootForce::ComponentType::ABILITYSPAWN);
			RootForce::TryPickupComponent::SetTypeId(RootForce::ComponentType::TRYPICKUPCOMPONENT);
			RootForce::SoundComponent::SetTypeId(RootForce::ComponentType::SOUND);
			RootForce::TimerComponent::SetTypeId(RootForce::ComponentType::TIMER);
			RootForce::FollowComponent::SetTypeId(RootForce::ComponentType::FOLLOW);
			RootForce::HomingComponent::SetTypeId(RootForce::ComponentType::HOMING);
			RootForce::RayComponent::SetTypeId(RootForce::ComponentType::RAY);
			RootForce::DamageAndKnockback::SetTypeId(RootForce::ComponentType::DAMAGEANDKNOCKBACK);
			RootForce::Scalable::SetTypeId(RootForce::ComponentType::SCALABLE);
			RootForce::StatChange::SetTypeId(RootForce::ComponentType::STATCHANGE);
			RootForce::KillAnnouncement::SetTypeId(RootForce::ComponentType::KILLANNOUNCEMENT);
			RootForce::ControllerActions::SetTypeId(RootForce::ComponentType::CONTROLLERACTIONS);
		}

		void InitializeServerComponents()
		{
			RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
			RootForce::HealthComponent::SetTypeId(RootForce::ComponentType::HEALTH);
			RootForce::Physics::SetTypeId(RootForce::ComponentType::PHYSICS);
			RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
			RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
			RootForce::Script::SetTypeId(RootForce::ComponentType::SCRIPT);
			RootForce::Collision::SetTypeId(RootForce::ComponentType::COLLISION);
			RootForce::CollisionResponder::SetTypeId(RootForce::ComponentType::COLLISIONRESPONDER);
			RootForce::PlayerComponent::SetTypeId(RootForce::ComponentType::PLAYER);
			RootForce::TDMRuleSet::SetTypeId(RootForce::ComponentType::TDMRULES);
			RootForce::PlayerActionComponent::SetTypeId(RootForce::ComponentType::PLAYERACTION);
			RootForce::PlayerPhysics::SetTypeId(RootForce::ComponentType::PLAYERPHYSICS);
            RootForce::StateComponent::SetTypeId(RootForce::ComponentType::ENTITYSTATE);
			RootForce::Network::ServerInformationComponent::SetTypeId(RootForce::ComponentType::SERVERINFORMATION);
			RootForce::TimerComponent::SetTypeId(RootForce::ComponentType::TIMER);
			RootForce::AbilitySpawnComponent::SetTypeId(RootForce::ComponentType::ABILITYSPAWN);
			RootForce::TryPickupComponent::SetTypeId(RootForce::ComponentType::TRYPICKUPCOMPONENT);
			RootForce::KillAnnouncement::SetTypeId(RootForce::ComponentType::KILLANNOUNCEMENT);
		}
	}

	template <typename T>
	bool AssertComponentValidity(ECS::EntityManager* p_entityManager, ECS::Entity* p_entity)
	{
		T* component = p_entityManager->GetComponent<T>(p_entity);
		if ((p_entity->GetFlag() & ECS::Component<T>::GetTypeId()) != 0)
		{
			return component != nullptr;
		}
		else
		{
			return component == nullptr;
		}
	}

	bool AssertEntityValid(ECS::EntityManager* p_entityManager, ECS::Entity* p_entity)
	{
		// Assert that the entity has all the components it has flags for.
		if (!AssertComponentValidity<Renderable>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Transform>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<PointLight>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Camera>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<HealthComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<PlayerControl>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Physics>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Network::NetworkComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<LookAtBehavior>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<ThirdPersonBehavior>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Script>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Collision>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<CollisionResponder>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<PlayerComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Animation>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<ParticleEmitter>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<TDMRuleSet>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<PlayerActionComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<PlayerPhysics>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<StateComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Shadowcaster>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<DirectionalLight>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Network::ServerInformationComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Network::ClientComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Ragdoll>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<WaterCollider>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<AbilitySpawnComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<TryPickupComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<SoundComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<TimerComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<FollowComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<HomingComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<RayComponent>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<DamageAndKnockback>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<Scalable>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<StatChange>(p_entityManager, p_entity)) return false;
		if (!AssertComponentValidity<KillAnnouncement>(p_entityManager, p_entity)) return false;
		return true;
	}
}

#endif
