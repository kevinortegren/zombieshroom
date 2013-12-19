#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>

namespace RootForce
{
	namespace GameStates
	{
		enum GameStates
		{
			Menu,
			Connecting,
			Ingame,
			Exit
		};

		struct PlayData
		{
			bool Host;
			int16_t p_port;
			std::string p_address;
			std::string serverName;
			std::string mapName;
			std::string password;
			int maxPlayers;
			int matchLength;
			int killcount;
		};
	}
}