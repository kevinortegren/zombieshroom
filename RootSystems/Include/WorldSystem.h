#pragma once

#include <Utility\ECS\Include\Component.h>
#include <Utility\ECS\Include\EntitySystem.h>
#include <array>

#define PLAYER_NUM_ABILITIES 1

namespace RootForce
{
	class WorldSystem : public ECS::VoidSystem
	{
	public:
		WorldSystem(ECS::World* p_world)
			: ECS::VoidSystem(p_world) {}
		void CreateWorld(const std::string& p_worldName);
		void Process();
	};
}
