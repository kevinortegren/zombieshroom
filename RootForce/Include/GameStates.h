#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>
#include <RootSystems/Include/MatchStateSystem.h>

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
			int16_t Port;
			std::string Address;
			std::string ServerName;
			std::string MapName;
			std::string Password;
			int MaxPlayers;
			int MatchLength;
			int Killcount;
		};
	}

	struct NetworkContext
	{
		std::shared_ptr<RootForce::Network::Server> m_server;
		std::shared_ptr<RootForce::Network::Client> m_client;
		std::shared_ptr<RootForce::Network::ServerMessageHandler> m_serverMessageHandler;
		std::shared_ptr<RootForce::Network::ClientMessageHandler> m_clientMessageHandler;
		std::shared_ptr<RootForce::Network::NetworkEntityMap> m_networkEntityMap;
	};

	struct SharedSystems
	{
		// System responsible for updating the player.
		std::shared_ptr<RootForce::PlayerSystem> m_playerSystem;
		std::shared_ptr<RootForce::MatchStateSystem> m_gameLogicSystem;
		// TODO: Add more
	};
}