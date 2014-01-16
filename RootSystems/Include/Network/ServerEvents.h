#pragma once

#include <stdint.h>
#include <string>

namespace RootServer
{
	namespace UserCommands
	{
		enum UserCommands
		{
			// There is no event! Or spoon!
			NO_EVENT,
			// Data[0] : <char*> player name
			KICK_PLAYER,
			// Data[0] : <char*> message
			BROADCAST_MESSAGE,
			// <no data>
			RESET_SERVER,
			// Data[0] : <uint8_t> max players
			SET_MAXPLAYERS,
			// Data[0] : <char*> password
			SET_PASSWORD,
			// Data[0] : <char*> map
			SET_MAP,
			// Data[0] : <uint8_t> game mode ID
			SET_GAMEMODE,
			// Data[0] : <uint32_t> match time in seconds
			SET_MATCHTIME,
			// Data[0] : <uint32_t> victory kill count
			SET_KILLCOUNT,
			// <no data>
			SHUTDOWN,
			// Data[0] : <char*> debug draw entity group to toggle
			CLIENT_DEBUGDRAW,
			// <no data>
			CLIENT_SUICIDE,
			// <no data>
			CLIENT_RAGEQUIT,
			// Data[0] : <char*> some super secret cheat mode argument
			CLIENT_SUPERSECRETCHEATMODE,
		};
	}

	struct EventData
	{
		UserCommands::UserCommands EventType;
		uint32_t DataSize;
		uint8_t* Data;

		EventData()
			: EventType(UserCommands::NO_EVENT),
			DataSize(0),
			Data(0)
		{}
	};

	EventData EventFromString(std::string p_string);
}