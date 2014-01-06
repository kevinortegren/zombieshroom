#pragma once

#include <memory>
#include <RootForce/Include/GameStates.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>
#include <RootSystems/Include/PlayerSystem.h>

namespace RootForce
{
	/** Handles client messages in the connecting state. Will only care for accept/refuse/user connected messages. */
	class ClientConnectingMessageHandler : public RootForce::Network::MessageHandler
	{
	public:
		ClientConnectingMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger, RootForce::Network::NetworkEntityMap* p_networkEntityMap);

		bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet);
	private:
		RootForce::Network::NetworkEntityMap* m_networkEntityMap;
		RootForce::PlayerSystem* m_playerSystem;

		bool m_connectionConfirmed;
	};


	/** Manages the state while the client is connecting to a server. During this time a player entity will not exist, hence several systems must not run. */
	class ConnectingState
	{
	public:
		ConnectingState(std::shared_ptr<RootForce::Network::Client>& p_client, std::shared_ptr<RootForce::Network::Server>& p_server, std::shared_ptr<RootForce::Network::NetworkEntityMap>& p_networkEntityMap);

		void Initialize();
		void Enter();
		void Exit();
		GameStates::GameStates Update();
	private:
		std::shared_ptr<RootForce::Network::ServerMessageHandler> m_serverMessageHandler;
		std::shared_ptr<RootForce::Network::ClientMessageHandler> m_clientMessageHandler;
		std::shared_ptr<RootForce::Network::Client>& m_client;
		std::shared_ptr<RootForce::Network::Server>& m_server;
		std::shared_ptr<RootForce::Network::NetworkEntityMap>& m_networkEntityMap
	};
}