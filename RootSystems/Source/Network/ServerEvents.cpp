#include <RootSystems/Include/Network/ServerEvents.h>

#include <sstream>

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
		name = name.substr((unsigned)p_ss.tellg());
		p_ev.Data = (uint8_t*)new char[name.size()];
		memcpy(p_ev.Data, name.data(), name.size());
		p_ev.DataSize = name.size();
	}

	template<class T>
	void ParseInt(EventData& p_ev, std::stringstream& p_ss)
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

	EventData EventFromString(std::string p_string)
	{
		std::stringstream ss(p_string);
		EventData ev;

		std::string evtyp;
		std::getline(ss, evtyp, ' ');


		if(evtyp.compare("kick") == 0)
		{
			ev.EventType = UserCommands::KICK_PLAYER;
			ParseWord(ev, ss);
		}
		else if(evtyp.compare("say") == 0)
		{
			ev.EventType = UserCommands::BROADCAST_MESSAGE;
			ParseRemaining(ev, ss);
		}
		else if(evtyp.compare("reset") == 0)
		{
			ev.EventType = UserCommands::RESET_SERVER;
		}
		else if(evtyp.compare("set_maxplayers") == 0)
		{
			ev.EventType = UserCommands::SET_MAXPLAYERS;
			ParseInt<uint8_t>(ev, ss);
		}
		else if(evtyp.compare("set_password") == 0)
		{
			ev.EventType = UserCommands::SET_PASSWORD;
			ParseWord(ev, ss);
		}
		else if(evtyp.compare("set_map") == 0)
		{
			ev.EventType = UserCommands::SET_MAP;
			ParseWord(ev, ss);
		}
		else if(evtyp.compare("set_gamemode") == 0)
		{
			ev.EventType = UserCommands::SET_GAMEMODE;
			ParseInt<uint8_t>(ev, ss);
		}
		else if(evtyp.compare("set_matchtime") == 0)
		{
			ev.EventType = UserCommands::SET_MATCHTIME;
			ParseInt<uint32_t>(ev, ss);
		}
		else if(evtyp.compare("set_killcount") == 0)
		{
			ev.EventType = UserCommands::SET_KILLCOUNT;
			ParseInt<uint32_t>(ev, ss);
		}
		else if(evtyp.compare("shutdown") == 0)
		{
			ev.EventType = UserCommands::SHUTDOWN;
		}
		else if(evtyp.compare("debugdraw") == 0)
		{
			ev.EventType = UserCommands::CLIENT_DEBUGDRAW;
			ParseWord(ev, ss);
		}
		else if(evtyp.compare("kill") == 0 || evtyp.compare("die") == 0 || evtyp.compare("suicide") == 0)
		{
			ev.EventType = UserCommands::CLIENT_SUICIDE;
		}
		else if(evtyp.compare("quit") == 0 || evtyp.compare("q") == 0)
		{
			ev.EventType = UserCommands::CLIENT_RAGEQUIT;
		}
		else if(evtyp.compare("pancakesocks") == 0)
		{
			ev.EventType = UserCommands::CLIENT_SUPERSECRETCHEATMODE;
			ParseWord(ev, ss);
		}
		else
		{
			ev.EventType = UserCommands::NO_EVENT;
		}

		return ev;

	}
}