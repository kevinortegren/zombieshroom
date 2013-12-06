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
		// Password to connect to server
		std::string Password;
		// Level file name
		std::string LevelFile;
		// Game mode, see enum for details
		GameMode::GameMode GameMode;
		// Match length in seconds
		uint32_t GameTime;
		// Victory condition on kill count; 0 = no kill-based victory condition
		uint32_t KillCount;

		ServerConfig()
		{
			MaxPlayers = 12;
			Port = 5567;
			Password = "";
			LevelFile = "level.lev"; // ToDo: Set to correct value
			GameMode = GameMode::DEATHMATCH;
			GameTime = 15*60; // 15 min
			KillCount = 0; // no killcount-victory
		}
	};
}