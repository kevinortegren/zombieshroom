#include "GameLogicSystem.h"

namespace RootForce
{


	//GameLogicSystem::GameLogicSystem( ECS::World* p_world )
	//{

	//}

	void GameLogicSystem::Process()
	{
		//State: team death match
		//loop through all members of a team, adding their score together.
		//If it equals or exceeeds the score limit of rule entity, set match over equal to true
		//loop second team
		//withdraw deltatime from rule entity's TimeLeft

		//State: free for all
		//Not to be implemented yet ;) 
	}

	void GameLogicSystem::SetLoggingInterface( Logging* p_logger )
	{
		m_logger = p_logger;
	}

}