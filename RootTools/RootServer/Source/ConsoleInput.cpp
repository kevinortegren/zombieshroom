#include <ConsoleInput.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace RootServer
{
	void ParseWord(EventData& p_ev, std::stringstream& p_ss)
	{
		if(p_ss.eof())
			return;

		std::string name;
		std::getline(p_ss, name, ' ');
		p_ev.Data = (uint8_t*)new char[name.size()];
		memcpy(p_ev.Data, name.data(), name.size());
		p_ev.DataSize = name.size();
	}

	void ParseRemaining(EventData& p_ev, std::stringstream& p_ss)
	{
		if(p_ss.eof())
			return;

		std::string name = p_ss.str();
		p_ev.Data = (uint8_t*)new char[name.size()];
		memcpy(p_ev.Data, name.data(), name.size());
		p_ev.DataSize = name.size();
	}

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

	EventData ConsoleInput::PollEvent()
	{
		if(m_eventBuffer.size() < 1)
			return EventData();
		EventData cmd = *(m_eventBuffer.end()-1);
		m_eventBuffer.pop_back();
		return cmd;
	}

	void ConsoleInput::Process()
	{
		while(!m_shouldExit)
		{
			std::string line;
			std::getline(std::cin, line);
			std::stringstream ss(line);
			EventData ev;
			
			std::string evtyp;
			std::getline(ss, evtyp, ' ');

			if(evtyp.compare("kick") == 0)
			{
				ev.EventType = ServerEvents::KICK_PLAYER;
				ParseWord(ev, ss);
			}
			else if(evtyp.compare("say") == 0)
			{
				ev.EventType = ServerEvents::BROADCAST_MESSAGE;
				ParseRemaining(ev, ss);
			}
			else if(evtyp.compare("reset") == 0)
			{
				ev.EventType = ServerEvents::RESET_SERVER;
			}
			else
			{
				std::cout << "Unknown command!" << std::endl;
				continue;
			}

			/*while( !ss.eof() )
			{
				std::string tmp;
				std::getline(ss, tmp, ' ');
				ev.push_back(tmp);
			}*/

			std::cout << std::endl;
			m_eventBuffer.push_back( ev );
			
			if( command[0].compare("quit") == 0
				|| command[0].compare("exit") == 0 )
			{
				m_shouldExit = true;
			}
		}
	}
}
