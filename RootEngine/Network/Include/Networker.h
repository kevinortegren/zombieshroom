#pragma once

#include <RakNet\RakPeerInterface.h>

#include <thread>
#include <string>

#define DEFAULT_PORT 5613
namespace Network
{
class Networker abstract
{
	public:
		Networker(void);
		~Networker(void);

		// Signals the networking system to finish the loop and then waits for the thread to finish
		void Join();
		// Send a string to all connected parties
		void Send( std::string p_str );
		
	protected:
		RakNet::RakPeerInterface* m_peer;
		bool m_isServer;

		virtual void ProcessPacket( RakNet::Packet* p_packet ) = 0;
		
	private:
		std::thread* m_thr;
		bool m_running;

		void Loop();
		void PrintIP( RakNet::SystemAddress p_addr );
	};
}

