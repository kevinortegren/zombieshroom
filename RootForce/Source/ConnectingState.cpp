#include <RootForce/Include/ConnectingState.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/Messages.h>

namespace RootForce
{
	extern RootEngine::GameSharedContext g_engineContext;
	extern ECS::World* g_world;


	ClientConnectingMessageHandler::ClientConnectingMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger, RootForce::Network::NetworkEntityMap* p_networkEntityMap)
		: MessageHandler(p_peer, p_logger)
		, m_networkEntityMap(p_networkEntityMap)
		, m_playerSystem(nullptr)
		, m_connectionConfirmed(false)
	{}

	bool ClientConnectingMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
	{
		switch (p_id)
		{
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				m_logger->LogText(LogTag::NETWORK, LogLevel::SUCCESS, "CLIENT: Connection accepted");

				// Create temporary entity for player
				const std::string PLAYER_NAME = "John Doe"; // TODO: Get this from config/options.

				m_playerSystem->CreatePlayer();
				ECS::Entity* entity = g_world->GetTagManager()->GetEntityByTag("Player");
				Network::TemporaryId_t tId = m_networkEntityMap->AddEntity(entity);

				Network::NetworkClientComponent* clientComponent = g_world->GetEntityManager()->CreateComponent<Network::NetworkClientComponent>(entity);
				clientComponent->Name = PLAYER_NAME;
				clientComponent->State = Network::NetworkClientComponent::EXPECTING_USER_INFO;
				clientComponent->UserID = -1;

				Network::NetworkComponent* networkComponent = g_world->GetEntityManager()->CreateComponent<Network::NetworkComponent>(entity);

				// Send a user info message to the server
				Network::MessageUserInfo m;
				m.PlayerEntity.EntityType = Network::EntityCreated::TYPE_PLAYER;
				m.PlayerEntity.SynchronizedID = Network::SYNCHRONIZED_ID_NONE;
				m.PlayerEntity.TemporaryID = tId;
				m.PlayerName = PLAYER_NAME.c_str();

				RakNet::BitStream bs;
				bs.Write((RakNet::MessageID) Network::MessageType::UserInfo);
				m.Serialize(true, &bs);

				m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peer->GetSystemAddressFromIndex(0), false);
			} return true;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
			{
				m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "CLIENT: Connection refused: server full");
			} return true;

			case ID_CONNECTION_ATTEMPT_FAILED:
			{
				m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "CLIENT: Connection attempt failed");
			} return true;

			case Network::MessageType::UserConnected:
			{
				Network::MessageUserConnected m;
				m.Serialize(false, p_bs);

				// Check whether we have a temporary entity already for the connected player (would imply that the connected player is us).
				if (m.UserInfo.PlayerEntity.TemporaryID == Network::TEMPORARY_ID_NONE)
				{
					// This is not a response to a join request. We do not have a temporary entity for this player. Create an entity.
					ECS::Entity* entity = g_world->GetEntityManager()->CreateEntity();
					Network::TemporaryId_t tId = m_networkEntityMap->AddEntity(entity);
					Network::SynchronizedId_t sId = m.UserInfo.PlayerEntity.SynchronizedID;
					m_networkEntityMap->SetSynchronizedId(tId, sId);
					m_networkEntityMap->AssociatePlayerEntityWithUserID(m.UserID, entity);
						
					Network::NetworkClientComponent* clientComponent = g_world->GetEntityManager()->CreateComponent<Network::NetworkClientComponent>(entity);
					clientComponent->Name = m.UserInfo.PlayerName.C_String();
					clientComponent->State = Network::NetworkClientComponent::CONNECTED;
					clientComponent->UserID = m.UserID;

					Network::NetworkComponent* networkComponent = g_world->GetEntityManager()->CreateComponent<Network::NetworkComponent>(entity);

					// Log the connection
					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "CLIENT: Remote player connected (ID: %d, Name: %s)", m.UserID, m.UserInfo.PlayerName.C_String());
				}
				else
				{
					// This is a response to our join request, we should have a temporary player entity.
					ECS::Entity* entity = m_networkEntityMap->GetTemporaryEntity(m.UserInfo.PlayerEntity.TemporaryID);
					assert(entity != nullptr);

					// Synchronize our temporary entity with the ID the server has.
					m_networkEntityMap->SetSynchronizedId(m.UserInfo.PlayerEntity.TemporaryID, m.UserInfo.PlayerEntity.SynchronizedID);
					m_networkEntityMap->AssociatePlayerEntityWithUserID(m.UserID, entity);

					// Update the user info
					Network::NetworkClientComponent* clientComponent = g_world->GetEntityManager()->GetComponent<Network::NetworkClientComponent>(entity);
					clientComponent->State = Network::NetworkClientComponent::CONNECTED;
					clientComponent->UserID = m.UserID;

					// Log the connection
					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "CLIENT: We have connected (ID: %d, Name: %s)", m.UserID, m.UserInfo.PlayerName.C_String());
				}
			} return true;
		}

		return false;
	}



	ConnectingState::ConnectingState(std::shared_ptr<RootForce::Network::Client>& p_client, std::shared_ptr<RootForce::Network::Server>& p_server, std::shared_ptr<RootForce::Network::NetworkEntityMap>& p_networkEntityMap)
		: m_client(p_client)
		, m_server(p_server)
		, m_networkEntityMap(p_networkEntityMap)
	{}

	void ConnectingState::Initialize()
	{
		
	}

	void ConnectingState::Enter()
	{
		// Reset the network entity map
		m_networkEntityMap = std::shared_ptr<RootForce::Network::NetworkEntityMap>(new RootForce::Network::NetworkEntityMap);

		// Create a network message handlers for connection messages.
		m_serverMessageHandler = std::shared_ptr<RootForce::Network::ServerMessageHandler>(new RootForce::Network::ServerMessageHandler(m_server->GetPeerInterface(), g_engineContext.m_logger, g_world));
		m_clientMessageHandler = std::shared_ptr<RootForce::Network::ClientMessageHandler>(new RootForce::ClientConnectingMessageHandler(m_client->GetPeerInterface(), g_engineContext.m_logger, m_networkEntityMap.get()));

		m_serverMessageHandler->SetNetworkEntityMap(m_networkEntityMap.get());

		// Set the connection message handler on the client.
		m_client->SetMessageHandler(m_clientMessageHandler.get());
		m_server->SetMessageHandler(m_serverMessageHandler.get());
	}

	void ConnectingState::Exit()
	{}

	GameStates::GameStates ConnectingState::Update()
	{
		m_server->Update();
		m_client->Update();

		return GameStates::Connecting;
	}
}