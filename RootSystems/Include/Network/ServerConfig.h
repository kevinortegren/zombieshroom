#pragma once

#include <string>
#include <stdint.h>

namespace RootSystems
{
	namespace GameMode
	{
		enum GameMode
		{
			// Team deathmatch
			DEATHMATCH,
			// Free For All deathmatch
			FREEFORALL
		};
	}

	struct ServerConfig
	{
		// Max number of players to accept (hard limit on 12 players currently)
		uint8_t MaxPlayers;
		// Port number
		uint16_t Port;
		// The name of the server
		std::string ServerName;
		// Password to connect to server
		std::string Password;
		// Level file name
		std::string MapName;
		// Game mode, see enum for details
		GameMode::GameMode GameMode;
		// Match length in seconds
		uint32_t MatchTime;
		// Victory condition on kill count; 0 = no kill-based victory condition
		uint32_t KillCount;

		ServerConfig()
		{
			MaxPlayers = 12;
			Port = 5567;
			ServerName = "Server";
			Password = "";
			MapName = "Nyaste";
			GameMode = GameMode::DEATHMATCH;
			MatchTime = 15*60; // 15 min
			KillCount = 0; // no killcount-victory
		}
	};
}