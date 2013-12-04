#include <Client.h>
#include <External/Include/RakNet/BitStream.h>
namespace RootEngine
{
	namespace Network
	{
		Client::Client(RakNet::SystemAddress p_sysAddress)
		{
			m_sysAddress = p_sysAddress;
		}


		Client::~Client(void)
		{
		}

		bool Client::Send( const Message& p_message )
		{
			if(p_message.DataSize > UINT_MAX)
				g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Attempting to send package with size higher than uint32. Package splitting not yet implemented.");

			uint32_t numBytesSent = 0;
			if(p_message.DataSize > 0)
			{
				RakNet::BitStream bitstream;
				bitstream.Write((RakNet::MessageID)39);
				bitstream.Write(p_message.RecipientID);
				bitstream.Write(p_message.MessageID);
				bitstream.Write(p_message.DataSize);
				bitstream.Write((const char*)p_message.Data, (unsigned int)p_message.DataSize);

				numBytesSent = m_peerInterface->Send( &bitstream, HIGH_PRIORITY, p_message.Reliability, 0, m_sysAddress, true);
			}
			return numBytesSent == p_message.DataSize + 8 + 1 + 1 + 1;
		}

	}
}