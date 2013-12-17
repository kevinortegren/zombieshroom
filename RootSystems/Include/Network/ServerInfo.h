#pragma once

#include <stdint.h>
#include <RakNet/BitStream.h>

namespace RootSystems
{
	// Contains the information sent to a client on a LAN-discovery request
	struct ServerInfo
	{
		RakNet::RakString Name;
		RakNet::RakString MapFile;
		uint8_t NumPlayers;
		uint8_t MaxPlayers;
		bool PasswordProtected;

		void Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			bs->Serialize(writeToBitstream, Name);
			bs->Serialize(writeToBitstream, MapFile);
			bs->Serialize(writeToBitstream, NumPlayers);
			bs->Serialize(writeToBitstream, MaxPlayers);
			bs->Serialize(writeToBitstream, PasswordProtected);
		}
	};
	
	// Inner struct, includes the address of the server which sent the info
	// The RootEngine::Network always appends IP and Port to the server discovery message
	struct ServerInfoInternal
	{
		RakNet::RakString Name;
		RakNet::RakString MapFile;
		uint8_t NumPlayers;
		uint8_t MaxPlayers;
		bool PasswordProtected;
		char IP[16];
		USHORT Port;
	};
}