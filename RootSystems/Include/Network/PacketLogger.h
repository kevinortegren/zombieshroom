#pragma once

#include <RakNet/PacketLogger.h>
#include <RootEngine/Include/Logging/Logging.h>

namespace RootForce
{
	namespace Network
	{
		class PacketLogger : public RakNet::PacketLogger
		{
		public:
			PacketLogger(LogTag::LogTag p_tag);

			void WriteLog(const char* p_string);
		private:
			LogTag::LogTag m_tag;
		};
	}
}

