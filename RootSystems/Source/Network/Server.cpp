#include <RootSystems/Include/Network/Server.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <stdexcept>

namespace RootForce
{
	namespace Network
	{
		Server::Server(Logging* p_logger, const std::string& p_name, unsigned short p_port, unsigned int p_maxConnections)
			: m_logger(p_logger)
		{
			// Setup the server
			m_peer = RakNet::RakPeerInterface::GetInstance();

			RakNet::SocketDescriptor sd(p_port, nullptr);
			if (m_peer->Startup(p_maxConnections, &sd, 1) != RakNet::RAKNET_STARTED)
				throw std::runtime_error("Failed to create RakNet server");

			m_peer->SetMaximumIncomingConnections(p_maxConnections);

			// Setup the ping response (for network discovery)
			RootSystems::ServerInfo info;
			m_info.Name = p_name.c_str();
			m_info.MapFile = "";
			m_info.MaxPlayers = p_maxConnections;
			m_info.NumPlayers = 0;
			m_info.PasswordProtected = false;

			UpdatePingResponse();

			// Notify the user of the firepower of this fully armed and operational server!
			m_logger->LogText(LogTag::NETWORK, LogLevel::INIT_PRINT, "Server initialized successfully");
		}

		Server::~Server()
		{
			m_peer->Shutdown(1000);
			RakNet::RakPeerInterface::DestroyInstance(m_peer);
		}

		RootSystems::ServerInfo Server::GetServerInfo() const
		{
			return m_info;
		}

		void Server::SetServerInfo(const RootSystems::ServerInfo& p_info)
		{
			m_info = p_info;

			UpdatePingResponse();
		}

		void Server::Update()
		{
			for (RakNet::Packet* packet = m_peer->Receive(); packet; m_peer->DeallocatePacket(packet), packet = m_peer->Receive())
			{
				RakNet::MessageID id;
				RakNet::BitStream bs(packet->data, packet->length, false);
				bs.Read(id);
			}
		}

		void Server::UpdatePingResponse()
		{
			RakNet::BitStream bs;
			m_info.Serialize(true, &bs);

			m_peer->SetOfflinePingResponse((const char*)bs.GetData(), bs.GetNumberOfBytesUsed());
		}
	}
}