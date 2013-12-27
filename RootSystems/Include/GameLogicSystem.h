#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>

namespace RootForce
{
	class GameLogicSystem : public ECS::VoidSystem
	{
	public:
		GameLogicSystem(ECS::World* p_world);
		void Process();
		bool IsMatchOver();
		void SetLoggingInterface(Logging* p_logger);
	private:
		Logging* m_logger;
	};
}