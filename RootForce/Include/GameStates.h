#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/MatchStateSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/RespawnSystem.h>

#ifndef COMPILE_LEVEL_EDITOR

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
			RootSystems::ServerConfig ServerInfo;

			struct
			{
				std::string Address;
				std::string Password;
				uint16_t Port;
			} ClientInfo;
		};
	}

	struct NetworkContext
	{
		std::shared_ptr<RootForce::Network::Server> m_server;
		std::shared_ptr<RootForce::Network::Client> m_client;
		std::shared_ptr<RootForce::Network::ServerMessageHandler> m_serverMessageHandler;
		std::shared_ptr<RootForce::Network::ClientMessageHandler> m_clientMessageHandler;
	};

	struct SharedSystems
	{
		// System responsible for keeping track of match rules
		std::shared_ptr<RootForce::MatchStateSystem> m_matchStateSystem;

		// System responsible for creating the world.
		std::shared_ptr<RootForce::WorldSystem> m_worldSystem;
		RootForce::AbilityRespawnSystem* m_abilitySpawnSystem;
		// TODO: Add more

		// System responsible for respawning and spawning players.
		RootSystems::RespawnSystem* m_respawnSystem;
	};
}

#endif