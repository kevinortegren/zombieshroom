#include <Server.h>
#include <External/Include/RakNet/BitStream.h>
#include <External/Include/RakNet/RakNetTypes.h>
namespace RootEngine
{
	namespace Network
	{
		Server::Server(void)
		{
		}

		Server::~Server(void)
		{
		}

		Message* Server::PollMessage()
		{
			if(m_message.size() < 1)
				return nullptr;
			Message* message = *(m_message.end()-1);
			m_message.pop_back();
			return message;
		}

		bool Server::Transmit( Message p_message, RakNet::RakNetGUID p_guid, bool p_broadcast )
		{
			if(p_message.DataSize > UINT_MAX)
				g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Attempting to send package with size higher than uint32. Package splitting not yet implemented.");

			uint32_t numBytesSent = 0;
			if(p_message.DataSize > 0)
			{
				RakNet::BitStream bitstream;
				bitstream.Write(NON_RAKNET_MESSAGE_ID);
				bitstream.Write(p_message.RecipientID);
				bitstream.Write(p_message.MessageID);
				bitstream.Write(p_message.DataSize);
				bitstream.Write((const char*)p_message.Data, (unsigned int)p_message.DataSize);

				numBytesSent = m_peerInterface->Send( &bitstream, HIGH_PRIORITY, p_message.Reliability, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}
			return numBytesSent == p_message.DataSize + 8 + 1 + 1 + 1;
		}

		void Server::ParseNonRaknetPacket( RakNet::Packet* p_packet )
		{
			Message* message = new Message;
			RakNet::BitStream bitstream(p_packet->data, p_packet->length, false);
			bitstream.IgnoreBytes(1); // skip reading data[0] again
			bitstream.Read(message->RecipientID);
			bitstream.Read(message->MessageID);
			bitstream.Read(message->DataSize);
			bitstream.Read((char*)message->Data, (unsigned int)message->DataSize);

			m_message.push_back(message);
		}

	}
}