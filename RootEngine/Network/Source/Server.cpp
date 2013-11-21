#include <Server.h>

Server::Server(void)
{
	m_isServer = true;
}

Server::~Server(void)
{
}

void Server::Host( USHORT p_port )
{
	printf( "Starting server.\n" );
	RakNet::SocketDescriptor sd( p_port, 0 );
	m_peer->Startup( MAX_CLIENTS, &sd, 1 );

	m_peer->SetMaximumIncomingConnections( MAX_CLIENTS );
}

void Server::ProcessPacket( RakNet::Packet* p_packet )
{
	switch( p_packet->data[0] )
	{
	case 29: // Custom chat
		printf( "User sent a message: %.*s", p_packet->length - 1, &p_packet->data[1] );
		Send( std::string( (const char*)(p_packet->data + 1), (const char*)(p_packet->data + p_packet->length) ) );
		break;
	default:
		printf( "Unknown packet ID: %u", p_packet->data[0] );
		break;
	}
}
