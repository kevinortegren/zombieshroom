#include "RemoteServer.h"

namespace RootEngine
{
	namespace Network
	{
		RemoteServer::RemoteServer()
		{
		}
		RemoteServer::~RemoteServer()
		{
		}

		bool RemoteServer::Send(Message p_message)
		{
			return Transmit(p_message, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}

		bool RemoteServer::ConnectTo( const char* p_ip , USHORT p_port)
		{
			RakNet::SocketDescriptor sd;
			m_peerInterface->Startup(1, &sd, 1);

			g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Connecting to: %s:%u", p_ip, p_port);
			RakNet::ConnectionAttemptResult result = m_peerInterface->Connect( p_ip, p_port, 0, 0 );
			if(result == RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED)
				return true;
			return false;
		}
	}
}