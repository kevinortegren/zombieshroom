#include <RootSystems/Include/Network/MessageHandlers.h>

namespace RootForce
{
	namespace Network
	{
		MessageHandler::~MessageHandler() {}

		void ClientMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			switch (p_id)
			{
				
			}
		}
	}
}