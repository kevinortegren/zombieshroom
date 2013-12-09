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
			else if(evtyp.compare("set_maxplayers") == 0)
			{
				ev.EventType = ServerEvents::SET_MAXPLAYERS;
				ParseInt<uint8_t>(ev, ss);
			}
			else if(evtyp.compare("set_password") == 0)
			{
				ev.EventType = ServerEvents::SET_PASSWORD;
				ParseWord(ev, ss);
			}
			else if(evtyp.compare("set_map") == 0)
			{
				ev.EventType = ServerEvents::SET_MAP;
				ParseWord(ev, ss);
			}
			else if(evtyp.compare("set_gamemode") == 0)
			{
				ev.EventType = ServerEvents::SET_GAMEMODE;
				ParseInt<uint8_t>(ev, ss);
			}
			else if(evtyp.compare("set_matchtime") == 0)
			{
				ev.EventType = ServerEvents::SET_MATCHTIME;
				ParseInt<uint32_t>(ev, ss);
			}
			else if(evtyp.compare("set_killcount") == 0)
			{
				ev.EventType = ServerEvents::SET_KILLCOUNT;
				ParseInt<uint32_t>(ev, ss);
			}
			else if(evtyp.compare("shutdown") == 0)
			{
				ev.EventType = ServerEvents::SHUTDOWN;
			}
			else if(evtyp.compare("pancakesocks") == 0)
			{
				ev.EventType = ServerEvents::CHEATMODE;
				ParseWord(ev, ss);
			}
			else
			{
				std::cout << "Unknown command!" << std::endl;
				continue;
			}

			m_eventBuffer.push_back( ev );
			
			if(ev.EventType == ServerEvents::SHUTDOWN)
			{
				m_shouldExit = true;
			}
		}
	}
	
	void ConsoleInput::ParseWord(EventData& p_ev, std::stringstream& p_ss)
	{
		if(p_ss.eof())
			return;

		std::string name;
		std::getline(p_ss, name, ' ');
		p_ev.Data = (uint8_t*)new char[name.size()];
		memcpy(p_ev.Data, name.data(), name.size());
		p_ev.DataSize = name.size();
	}

	void ConsoleInput::ParseRemaining(EventData& p_ev, std::stringstream& p_ss)
	{
		if(p_ss.eof())
			return;

		std::string name = p_ss.str();
		p_ev.Data = (uint8_t*)new char[name.size()];
		memcpy(p_ev.Data, name.data(), name.size());
		p_ev.DataSize = name.size();
	}

	template<class T>
	void ConsoleInput::ParseInt(EventData& p_ev, std::stringstream& p_ss)
	{
		if(p_ss.eof())
			return;

		p_ev.Data = (uint8_t*)new T;
		if(typeid(T) == typeid(uint8_t))
		{
			uint16_t tmp;
			p_ss >> tmp;
			*(T*)p_ev.Data = (uint8_t)tmp;
		}
		else
			p_ss >> *(T*)p_ev.Data;
		p_ev.DataSize = sizeof(T);
	}
}
