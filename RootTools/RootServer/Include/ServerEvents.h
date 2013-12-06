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
			SET_VALUE,
			SHUTDOWN,
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