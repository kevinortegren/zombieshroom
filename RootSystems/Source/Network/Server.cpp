#include <RootSystems/Include/Network/Server.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <stdexcept>

namespace RootForce
{
	namespace Network
	{
		Server::Server(Logging* p_logger, ECS::World* p_world, const RootSystems::ServerConfig& p_config)
			: m_logger(p_logger)
			, m_world(p_world)
			, m_messageHandler(nullptr)
		{
			// Setup the server
			m_peer = RakNet::RakPeerInterface::GetInstance();

			RakNet::SocketDescriptor sd(p_config.Port, nullptr);
			if (m_peer->Startup(p_config.MaxPlayers, &sd, 1) != RakNet::RAKNET_STARTED)
				throw std::runtime_error("Failed to create RakNet server");

			m_peer->SetMaximumIncomingConnections(p_config.MaxPlayers);
			m_peer->SetOccasionalPing(true);
			m_peer->SetIncomingPassword(p_config.Password.c_str(), p_config.Password.size() + 1);

			// Setup the ping response (for network discovery)
			ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
			Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);

			serverInfo->Information.ServerName = p_config.ServerName.c_str();
			serverInfo->Information.MapName = p_config.MapName.c_str();
			serverInfo->Information.CurrentPlayers = 0;
			serverInfo->Information.MaxPlayers = p_config.MaxPlayers;
			serverInfo->Information.PasswordProtected = p_config.Password != "";
			serverInfo->Information.GameMode = p_config.GameMode;
			serverInfo->Information.MatchTimeSeconds = p_config.MatchTime;
			serverInfo->Information.KillCount = p_config.KillCount;

			UpdatePingResponse();

			// Notify the user of the firepower of this fully armed and operational server!
			m_logger->LogText(LogTag::SERVER, LogLevel::INIT_PRINT, "Server initialized successfully");
		}

		Server::~Server()
		{
			m_peer->Shutdown(1000);
			RakNet::RakPeerInterface::DestroyInstance(m_peer);
		}

		const NetworkMessage::ServerInformation& Server::GetServerInformation() const
		{
			return m_information;
		}

		void Server::SetServerInformation(const NetworkMessage::ServerInformation& p_information)
		{
			m_information = p_information;
			UpdatePingResponse();
		}

		void Server::SetMessageHandler(MessageHandler* p_messageHandler)
		{
			m_messageHandler = p_messageHandler;
		}

		void Server::Update()
		{
			// Store all incoming packets in a temporary list (to defer parsing of packets that arrive during parsing)
			std::vector<RakNet::Packet*> packets;
			for (RakNet::Packet* packet = m_peer->Receive(); packet; packet = m_peer->Receive())
			{
				packets.push_back(packet);
			}

			// Parse all incoming packets
			for (size_t i = 0; i < packets.size(); ++i)
			{
				RakNet::Packet* packet = packets[i];

				RakNet::MessageID id;
				RakNet::BitStream bs(packet->data, packet->length, false);
				bs.Read(id);

				if (m_messageHandler != nullptr)
				{
					if (!m_messageHandler->ParsePacket(id, &bs, packet))
					{
						m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Message handler neglected to parse message with ID: %u", id);
					}
				}
				else
				{
					m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "No message handler to parse message with ID: %u", id);
				}
			}

			// Deallocate the temporary list
			for (size_t i = 0; i < packets.size(); ++i)
			{
				m_peer->DeallocatePacket(packets[i]);
			}
		}

		RakNet::RakPeerInterface* Server::GetPeerInterface()
		{
			return m_peer;
		}

		void Server::UpdatePingResponse()
		{
			ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
			Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);

			RakNet::BitStream bs;
			serverInfo->Information.Serialize(true, &bs);

			const unsigned char* data = bs.GetData();
			unsigned int size = bs.GetNumberOfBytesUsed();

			m_peer->SetOfflinePingResponse((const char*)bs.GetData(), bs.GetNumberOfBytesUsed());
		}
	}
}