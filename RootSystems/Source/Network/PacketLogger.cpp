#include <RootSystems/Include/Network/PacketLogger.h>
#include <RootEngine/Include/GameSharedContext.h>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	namespace Network
	{
		PacketLogger::PacketLogger(LogTag::LogTag p_tag)
			: m_tag(p_tag)
		{}

		void PacketLogger::WriteLog(const char* p_string)
		{
			g_engineContext.m_logger->LogText(m_tag, LogLevel::PACKET_PRINT, "Packet: %s", p_string);
		}
	}
}