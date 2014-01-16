#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility\ECS\Include\Component.h>
#include <Utility\ECS\Include\EntitySystem.h>
#include <array>

#define PLAYER_NUM_ABILITIES 3

namespace RootForce
{
	namespace Ability
	{
		enum Ability
		{
			ABILITY_NONE,
			ABILITY_TEST,
		};
	}

	namespace EntityState
	{
		enum EntityState
		{
			ASCENDING,
			DESCENDING,
			LANDING,
			GROUNDED,
			RAGDOLL,
		};
	}

	struct PlayerActionComponent : public ECS::Component<PlayerActionComponent>
	{
		float MovePower;
		float StrafePower;
		bool Jump;
		glm::vec2 Angle;
		bool ActivateAbility;
		uint8_t SelectedAbility;
	};

	struct PlayerPhysics : public ECS::Component<PlayerPhysics>
	{
		float MovementSpeed;
		float JumpForce;
	};

	struct StateComponent : public ECS::Component<StateComponent>
	{
		EntityState::EntityState CurrentState;
		glm::vec3 PrevPosition;
	};

	struct PlayerControl : public ECS::Component<PlayerControl>
	{
		float m_mouseSensitivity;
	};

	struct HealthComponent : public ECS::Component<HealthComponent>
	{	
		int Health;
		int LastDamageSourceID;
		bool IsDead;
		bool WantsRespawn;
		float RespawnDelay;
	};

	struct Identity : public ECS::Component<Identity>
	{
		std::string Name; 
		int TeamID;
	};

	struct UserAbility : public ECS::Component<UserAbility>
	{
		std::array<Ability::Ability, PLAYER_NUM_ABILITIES> Abilities;
		Ability::Ability SelectedAbility;
	};

	struct ScoreComponent : public ECS::Component<ScoreComponent>
	{
		int Score;
		int Deaths;
	};

	class PlayerSystem : public ECS::VoidSystem
	{
	public:
		PlayerSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::VoidSystem(p_world)
			, m_engineContext(p_engineContext) {}
		void CreatePlayer(int p_teamID);
		void Process();
	private:
		RootEngine::GameSharedContext* m_engineContext;
	};
}
