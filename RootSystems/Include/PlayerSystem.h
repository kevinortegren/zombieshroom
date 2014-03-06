#pragma once

#include <RakNet/RakString.h>
#include <Utility/ECS/Include/Component.h>
#include <glm/glm.hpp>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <array>
#include <queue>

#define PLAYER_NUM_ABILITIES 4

namespace RootForce
{
	const uint8_t ABILITY_INDEX_NONE = 0xFF;
	const uint8_t PUSH_ABILITY_INDEX = 3;
	const float JUMP_TIME_LIMIT = 0.5f; //Do not fucking change without good reason, I know where you live //Kim

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
			CHARGING,
			CHANNELING,
		};
	}


#ifndef COMPILE_LEVEL_EDITOR
	namespace AbilityEventType
	{
		enum AbilityEventType
		{
			CHARGE_START,
			CHANNELING_START,
			CHANNELING_DONE,
			INTERRUPTED
		};
	}
	
	struct AbilityEvent
	{
		AbilityEventType::AbilityEventType Type;
		Network::ActionID_t ActionID;
		float Time;
		uint8_t ActiveAbility;
		RakNet::RakString ActiveAbilityScript;

		AbilityEvent()
			: Type(AbilityEventType::CHARGE_START)
			, ActionID(Network::ReservedActionID::NONE)
			, Time(0.0f)
			, ActiveAbility(ABILITY_INDEX_NONE) {}
	};

	struct PlayerActionComponent : public ECS::Component<PlayerActionComponent>
	{
		float MovePower;
		float StrafePower;
		glm::vec2 Angle;

		float JumpTime;
		glm::vec3 JumpDir;

		uint8_t SelectedAbility;
		std::queue<AbilityEvent> AbilityEvents;
		AbilityEvent CurrentAbilityEvent;

		bool WantRespawn;

		PlayerActionComponent()
			: MovePower(0.0f)
			, StrafePower(0.0f)
			, JumpTime(0.0f)
			, JumpDir(glm::vec3(0))
			, SelectedAbility(0)
			, WantRespawn(false)
		{}
	};
#endif

	struct PlayerPhysics : public ECS::Component<PlayerPhysics>
	{
		float MovementSpeed;
		float JumpForce;
		float JumpBoostForce;
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
		float Health;
		Network::UserID_t LastDamageSourceID;
		bool IsDead;
		bool WantsRespawn;
		float RespawnDelay;
		int SpawnIndex;
		bool SpawnPointReceived;

		HealthComponent()
		{
			Health = 100.0f;
			LastDamageSourceID = 0;
			IsDead = false;
			WantsRespawn = true;
			RespawnDelay = 0.0f;
			SpawnIndex = -1;
			SpawnPointReceived = false;
		}
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

	struct TryPickupComponent : public ECS::Component<TryPickupComponent>
	{
		union
		{
			bool TryPickup;
			int Padding[2]; // world's most handsome code (components need to have size >= 8?)
		};

		TryPickupComponent()
			: TryPickup(false) {}
	};
}
