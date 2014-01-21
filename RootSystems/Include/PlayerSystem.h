#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <array>

#define PLAYER_NUM_ABILITIES 3

namespace RootForce
{
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
		Network::UserID_t LastDamageSourceID;
		bool IsDead;
		bool WantsRespawn;
		float RespawnDelay;
	};


	struct PlayerComponent : public ECS::Component<PlayerComponent>
	{
		std::string Name;
		int TeamID;

		std::array<std::string, PLAYER_NUM_ABILITIES> AbilityScripts;
		int SelectedAbility;

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
