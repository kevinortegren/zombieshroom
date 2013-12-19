#pragma once
#include "GameStates.h"
namespace RootForce
{
	namespace GameStates
	{
		enum GameStates
		{
			Menu,
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