#pragma once

#include <stdint.h>
#include <string>
#include <RakNet/BitStream.h>

namespace RootSystems
{
	static const int SERVER_INFO_NAME_SIZE = 128;
	static const int SERVER_INFO_MAP_FILE_SIZE = 64;

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
		
		
		ServerInfoInternal() {}

		ServerInfoInternal(const ServerInfoInternal& p_copy)
		{
			(*this) = p_copy;
		}

		ServerInfoInternal& operator=(const ServerInfoInternal& p_copy)
		{
			Name = p_copy.Name;
			MapFile = p_copy.MapFile;
			NumPlayers = p_copy.NumPlayers;
			MaxPlayers = p_copy.MaxPlayers;
			PasswordProtected = p_copy.PasswordProtected;
			strcpy(IP, p_copy.IP);
			Port = p_copy.Port;

			return (*this);
		}

		void SetInfo(const ServerInfo& p_info, const RakNet::Packet* p_packet)
		{
			Name = p_info.Name;
			MapFile = p_info.MapFile;
			NumPlayers = p_info.NumPlayers;
			MaxPlayers = p_info.MaxPlayers;
			PasswordProtected = p_info.PasswordProtected;
			strcpy(IP, p_packet->systemAddress.ToString(false));
			Port = p_packet->systemAddress.GetPort();
		}
	};
}