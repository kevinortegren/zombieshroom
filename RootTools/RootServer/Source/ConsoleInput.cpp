#include <ConsoleInput.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace RootServer
{
	void ConsoleInput::Startup()
	{
		m_shouldExit = false;
		m_thread = new std::thread( [this](){ this->Process(); } );
	}

	void ConsoleInput::Shutdown()
	{
		m_shouldExit = true;
		m_thread->join();
	}

	std::vector<std::string> ConsoleInput::PollCommand()
	{
		if(m_commandBuffer.size() < 1)
			return std::vector<std::string>();
		std::vector<std::string> cmd = *(m_commandBuffer.end()-1);
		m_commandBuffer.pop_back();
		return cmd;
	}

	void ConsoleInput::Process()
	{
		while(!m_shouldExit)
		{
			std::string line;
			std::getline(std::cin, line);
			std::stringstream ss(line);
			std::vector<std::string> command;
			while( !ss.eof() )
			{
				std::string tmp;
				std::getline(ss, tmp, ' ');
				command.push_back(tmp);
			}

			std::cout << "Command received";
			for( uint32_t i = 0; i < command.size(); i++ )
			{
				std::cout << " " << command[i];
			}
			std::cout << std::endl;
			m_commandBuffer.push_back( command );
			
			if( command[0].compare("quit") == 0
				|| command[0].compare("exit") == 0 )
			{
				m_shouldExit = true;
			}
		}
	}
}
