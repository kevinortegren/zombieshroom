#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility\ECS\Include\Component.h>
#include <Utility\ECS\Include\EntitySystem.h>
#include <array>

#define PLAYER_NUM_ABILITIES 1

namespace RootForce
{
	namespace Abilitiy
	{
		enum Ability
		{
			ABILITY_TEST
		};
	}

	struct Player : public ECS::Component<Player>
	{
		std::string Name; 
		int Health;
		int TeamID;
		int LastDamageSourceID;
	};

	struct UserAbility : public ECS::Component<UserAbility>
	{
		std::array<Abilitiy::Ability, PLAYER_NUM_ABILITIES> Abilities;
		Abilitiy::Ability SelectedAbility;
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
