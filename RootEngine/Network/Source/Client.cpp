#include "Client.h"

Client::Client(void)
{
	m_isServer = false;
}


Client::~Client(void)
{
}

void Client::ConnectTo( const char* p_ip, USHORT p_port )
{
	RakNet::SocketDescriptor sd;
	m_peer->Startup( 1, &sd, 1 );

	printf( "Connecting to %s:%u...\n", p_ip, p_port );
	m_peer->Connect( p_ip, p_port, 0, 0 );
}

void Client::ProcessPacket( RakNet::Packet* p_packet )
{
	switch( p_packet->data[0] )
	{
	case 29: // Custom chat
		printf( "User sent a message: %.*s", p_packet->length-1, &p_packet->data[1] );
		break;
	default:
		printf( "Unknown packet ID: %u", p_packet->data[0] );
		break;
	}
}
