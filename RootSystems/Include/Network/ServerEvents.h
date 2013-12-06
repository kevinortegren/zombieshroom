#pragma once

#include <stdint.h>

namespace RootServer
{
	namespace ServerEvents
	{
		enum ServerEvents
		{
			NO_EVENT,
			NEW_PLAYER,
			REMOVE_PLAYER,
			KICK_PLAYER,
			BROADCAST_MESSAGE,
			RESET_SERVER,
			SET_MAXPLAYERS,
			SET_PASSWORD,
			SET_MAP,
			SET_GAMEMODE,
			SET_MATCHTIME,
			SET_KILLCOUNT,
			SHUTDOWN,
			CHEATMODE,
		};
	}

	struct EventData
	{
		ServerEvents::ServerEvents EventType;
		uint32_t DataSize;
		uint8_t* Data;

		EventData()
			: EventType(ServerEvents::NO_EVENT),
			DataSize(0),
			Data(0)
		{}
	};
}