#include <Networker.h>

#include <RakNet/RakNetTypes.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>

#include <cstdio>

Networker::Networker(void)
{
	m_peer = RakNet::RakPeerInterface::GetInstance( );
	m_thr = new std::thread( [this]() {this->Loop();} );
	m_running = true;
}


Networker::~Networker(void)
{
	RakNet::RakPeerInterface::DestroyInstance( m_peer );
}

void Networker::Loop()
{
	RakNet::Packet* packet;

	while( m_running )
	{
		for( packet = m_peer->Receive( );
			packet;
			m_peer->DeallocatePacket( packet ), packet = m_peer->Receive( ) )
		{
			switch( packet->data[0] )
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf( "A client has disconnected." );
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf( "A client was lost to the internet." );
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf( "A wandering client has arrived." );
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf( "Our connection request has been accepted." );
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf( "A connection is incoming." );
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf( "The server is drunk! (full)" );
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if( m_isServer )
				{
					printf( "A client has disconnected." );
				}
				else
				{
					printf( "Connection lost." );
				}
				break;
			case ID_CONNECTION_LOST:
				if( m_isServer )
				{
					printf( "A client was lost to the internet." );
				}
				else
				{
					printf( "Connection lost." );
				}
				break;
			default:
				//printf( "Message with identifier %i has arrived.", packet->data[0] );
				ProcessPacket( packet );
				break;
			}
			PrintIP( packet->systemAddress );
		}

		Sleep( 500 );
	}
}

void Networker::Join()
{
	m_running = false;
	m_thr->join();
}

void Networker::Send( std::string p_str )
{
	if( p_str.length() > 0 )
	{
		std::string tmp;
		tmp += (char)29;
		tmp.append( p_str );
		m_peer->Send( tmp.data(), tmp.length(), HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
	}
}

void Networker::PrintIP( RakNet::SystemAddress p_addr )
{
	IN_ADDR addr = p_addr.address.addr4.sin_addr;
	USHORT port = p_addr.address.addr4.sin_port;
	printf( " From %u.%u.%u.%u:%u\n", addr.S_un.S_un_b.s_b1, addr.S_un.S_un_b.s_b2, addr.S_un.S_un_b.s_b3, addr.S_un.S_un_b.s_b4 , port);
}
