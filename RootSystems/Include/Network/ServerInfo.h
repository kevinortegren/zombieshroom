#pragma once

#include <stdint.h>
#include <RootEngine/Network/Include/Server.h>

namespace RootSystems
{
#pragma pack(push, 1) // Disable data padding so structs can be read and written as a whole
	// Contains the information sent to a client on a LAN-discovery request
	struct ServerInfo
	{
		char Name[255];
		char Mapfile[64];
		uint8_t NumPlayers;
		uint8_t MaxPlayers;
		bool PasswordProtected;
	};
	
	// Inner struct, includes the address of the server which sent the info
	// The RootEngine::Network always appends IP and Port to the server discovery message
	struct ServerInfoInternal
	{
		char Name[255];
		char Mapfile[64];
		uint8_t NumPlayers;
		uint8_t MaxPlayers;
		bool PasswordProtected;
		char IP[16];
		USHORT Port;
	};
#pragma pack(pop)
}