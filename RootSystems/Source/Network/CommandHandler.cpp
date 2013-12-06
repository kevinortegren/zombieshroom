#include "Network/CommandHandler.h"

namespace RootSystems
{
	void CommandHandler::ExecuteCommand(std::vector<std::string> p_command)
	{
		if( p_command[0].compare("quit") == 0
			|| p_command[0].compare("exit") == 0 )
		{

		}
	}
}