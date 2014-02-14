#pragma once

#include <Utility/ECS/Include/Component.h>
#include <glm/glm.hpp>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <array>

#define PLAYER_NUM_ABILITIES 3

namespace RootForce
{
	const float JUMP_TIME_LIMIT = 1.0f;

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

	namespace AbilityState
	{
		enum AbilityState
		{
			OFF,
			START_CHARGING,
			CHARGING,
			START_CHANNELING,
			CHANNELING,
			STOP_CHANNELING,
			STOP_CHARGING_AND_CHANNELING
		};
	}


#ifndef COMPILE_LEVEL_EDITOR
	struct PlayerActionComponent : public ECS::Component<PlayerActionComponent>
	{
		Network::ActionID_t ActionID;
		float MovePower;
		float StrafePower;
		glm::vec2 Angle;

		float JumpTime;

		bool TryPickup;

		float AbilityTime;
		uint8_t SelectedAbility;

		bool WantRespawn;

		PlayerActionComponent() 
			: ActionID(Network::ReservedActionID::NONE)
			, MovePower(0.0f)
			, StrafePower(0.0f)
			, JumpTime(0.0f)
			, AbilityTime(0.0f)
			, SelectedAbility(1)
			, WantRespawn(false)
			, TryPickup(false)
		{}
	};
#endif

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

#ifndef COMPILE_LEVEL_EDITOR
	struct HealthComponent : public ECS::Component<HealthComponent>
	{	
		int Health;
		Network::UserID_t LastDamageSourceID;
		bool IsDead;
		bool WantsRespawn;
		float RespawnDelay;
	};
#endif

	struct AbilityInfo
	{
		std::string Name;
		float Cooldown;
		bool OnCooldown;
		int Charges;

		AbilityInfo()
			: Name(""), Cooldown(0.0f), OnCooldown(false), Charges(-1)
		{}
	};

	struct PlayerComponent : public ECS::Component<PlayerComponent>
	{
		std::string Name;
		int TeamID;

		std::array<AbilityInfo, PLAYER_NUM_ABILITIES> AbilityScripts;
		int SelectedAbility;
		AbilityState::AbilityState AbilityState;

		int Score;
		int Deaths;

		PlayerComponent()
		{
			TeamID = 0;
			SelectedAbility = 0;
			AbilityState = AbilityState::OFF;
			Score = 0;
			Deaths = 0;
		}
	};
}
