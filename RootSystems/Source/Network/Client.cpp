#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <stdexcept>

namespace RootForce
{
	namespace Network
	{
		Client::Client(Logging* p_logger)
			: m_logger(p_logger)
			, m_peer(nullptr)
			, m_messageHandler(nullptr)
			, m_chatSystem(nullptr)
		{
			m_peer = RakNet::RakPeerInterface::GetInstance();

			RakNet::SocketDescriptor sd;
			if (m_peer->Startup(1, &sd, 1) != RakNet::RAKNET_STARTED)
				throw std::runtime_error("Failed to create RakNet client");

			m_logger->LogText(LogTag::NETWORK, LogLevel::INIT_PRINT, "Client initialized successfully");
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

		bool Client::Connect(const char* p_address, unsigned short p_port)
		{
			if (m_peer->Connect(p_address, p_port, nullptr, 0) != RakNet::CONNECTION_ATTEMPT_STARTED)
				return false;

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
			for (RakNet::Packet* packet = m_peer->Receive(); packet; m_peer->DeallocatePacket(packet), packet = m_peer->Receive())
			{
				RakNet::MessageID id;
				RakNet::BitStream bs(packet->data, packet->length, false);
				bs.Read(id);

				if (m_messageHandler != nullptr)
					m_messageHandler->ParsePacket(id, &bs, packet);
			}

			// Send a chat message
			if (m_chatSystem != nullptr)
			{
				std::string chatMessage = m_chatSystem->PollMessage();
				if (chatMessage != "")
				{
					MessageChat c;
					c.Message = chatMessage.c_str();
					c.SenderID = -1;
					c.Type = MessageChat::TYPE_CHAT;

					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) MessageType::ChatToServer);
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