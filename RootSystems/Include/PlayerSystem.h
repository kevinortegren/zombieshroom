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
		std::array<Abilitiy::Ability, PLAYER_NUM_ABILITIES> m_abilities;
		Abilitiy::Ability m_selectedAbility;
	};

	class PlayerSystem : public ECS::VoidSystem
	{
	public:
		PlayerSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_gameSharedContext)
			: ECS::VoidSystem(p_world)
			, m_gameSharedContext(p_gameSharedContext) {}
		void CreatePlayer();
		void Process();
	private:
		RootEngine::GameSharedContext* m_gameSharedContext;
	};
}
