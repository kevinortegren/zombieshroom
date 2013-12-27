#include "GameLogicSystem.h"

namespace RootForce
{


	//GameLogicSystem::GameLogicSystem( ECS::World* p_world )
	//{

	//}

	void GameLogicSystem::Process()
	{

	}

	bool GameLogicSystem::IsMatchOver()
	{
		return false;
	}

	void GameLogicSystem::SetLoggingInterface( Logging* p_logger )
	{
		m_logger = p_logger;
	}

}