#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Transform.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <stdexcept>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	namespace Network
	{
		Client::Client(Logging* p_logger, ECS::World* p_world)
			: m_world(p_world)
			, m_logger(p_logger)
			, m_peer(nullptr)
			, m_messageHandler(nullptr)
			, m_chatSystem(nullptr)
		{
			m_peer = RakNet::RakPeerInterface::GetInstance();

			RakNet::SocketDescriptor sd;
			if (m_peer->Startup(1, &sd, 1) != RakNet::RAKNET_STARTED)
				throw std::runtime_error("Failed to create RakNet client");

			// Create a client entity
			ECS::Entity* clientEntity = m_world->GetEntityManager()->CreateEntity();
			m_world->GetEntityManager()->CreateComponent<Network::ClientComponent>(clientEntity);
			m_world->GetTagManager()->RegisterEntity("Client", clientEntity);

			// Let scripts know that we have a client and require client components on our entities.
			g_engineContext.m_script->SetGlobalNumber("IsClient", true);

			// Client-cruiser operational!
			m_logger->LogText(LogTag::CLIENT, LogLevel::INIT_PRINT, "Client initialized successfully");
		}

		Client::~Client()
		{
			m_peer->Shutdown(1000);
			RakNet::RakPeerInterface::DestroyInstance(m_peer);
		}

		void Client::PingNetwork(RootSystems::LanList* p_list, unsigned short p_port)
		{
			m_peer->Ping("255.255.255.255", p_port, false);
			p_list->Start();
		}

		bool Client::Connect(const std::string& p_address, const std::string& p_password, unsigned short p_port, bool p_isRemote)
		{
			ECS::Entity* clientEntity = m_world->GetTagManager()->GetEntityByTag("Client");
			Network::ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);
			clientComponent->IsRemote = p_isRemote;
			clientComponent->State = ClientState::UNCONNECTED;
			clientComponent->Name = g_engineContext.m_configManager->GetConfigValueAsString("Name").c_str();

			if (m_peer->Connect(p_address.c_str(), p_port, p_password.c_str(), p_password.size() + 1) != RakNet::CONNECTION_ATTEMPT_STARTED)
				return false;

			m_peer->SetOccasionalPing(true);

			return true;
		}

		void Client::SetMessageHandler(MessageHandler* p_messageHandler)
		{
			m_messageHandler = p_messageHandler;
		}

		void Client::SetChatSystem(RootForce::ChatSystem* p_chatSystem)
		{
			m_chatSystem = p_chatSystem;
		}

		void Client::Update()
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
						m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Message handler neglected to parse message with ID: %u", id);
					}
				}
				else
				{
					m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "No message handler to parse message with ID: %u", id);
				}
			}

			// Deallocate the temporary list
			for (size_t i = 0; i < packets.size(); ++i)
			{
				m_peer->DeallocatePacket(packets[i]);
			}

			// TODO: Maybe put outside?
			// Send a chat message
			if (m_chatSystem != nullptr)
			{
				std::string chatMessage = m_chatSystem->PollMessage();
				if (chatMessage != "")
				{
					ECS::Entity* player = m_world->GetTagManager()->GetEntityByTag("Player");
					NetworkComponent* network = m_world->GetEntityManager()->GetComponent<NetworkComponent>(player);

					NetworkMessage::Chat c;
					c.Message = chatMessage.c_str();
					c.Sender = network->ID.UserID;
					c.Type = NetworkMessage::Chat::TYPE_CHAT;

					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::Chat);
					c.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peer->GetSystemAddressFromIndex(0), false);
				}
			}
		}

		RakNet::RakPeerInterface* Client::GetPeerInterface()
		{
			return m_peer;
		}
	}
}