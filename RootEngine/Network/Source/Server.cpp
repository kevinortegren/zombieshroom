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

	}
}