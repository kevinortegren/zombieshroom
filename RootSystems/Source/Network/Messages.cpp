#ifndef COMPILE_LEVEL_EDITOR

#include <RootSystems/Include/Network/Messages.h>
#include <cstring>

namespace RootForce
{
	namespace Network
	{
		void EntityCreated::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			bs->Serialize(writeToBitstream, TemporaryID);
			bs->Serialize(writeToBitstream, SynchronizedID);
			bs->Serialize(writeToBitstream, EntityType);
		}



		void EntityRemoved::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			bs->Serialize(writeToBitstream, SynchronizedID);
		}




		void MessageGameStateSnapshot::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			// TODO: Implement
		}



		void MessageChat::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			bs->Serialize(writeToBitstream, Type);
			bs->Serialize(writeToBitstream, SenderID);
			bs->Serialize(writeToBitstream, Message);
		}
		



		void MessageUserInfo::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			bs->Serialize(writeToBitstream, PlayerName);
			PlayerEntity.Serialize(writeToBitstream, bs);
		}



		void MessagePlayData::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			bs->Serialize(writeToBitstream, ServerName);
			bs->Serialize(writeToBitstream, MapName);
			bs->Serialize(writeToBitstream, MaxPlayers);
			bs->Serialize(writeToBitstream, MatchLength);
			bs->Serialize(writeToBitstream, KillCount);
		}


		void MessageUserConnected::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			bs->Serialize(writeToBitstream, UserID);
			UserInfo.Serialize(writeToBitstream, bs);
		}



		void MessageUserDisconnected::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			bs->Serialize(writeToBitstream, UserID);
		}



		void MessageUserCommandOrient::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			for (int i = 0; i < 4; ++i)
				bs->Serialize(writeToBitstream, Orientation[i]);
		}



		void MessageUserCommandSelectAbility::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			bs->Serialize(writeToBitstream, Slot);
		}



		void HACK_MessageTransformUpdate::Serialize(bool writeToBitstream, RakNet::BitStream* bs)
		{
			bs->Serialize(writeToBitstream, EntityID);
			for (int i = 0; i < 3; ++i)
				bs->Serialize(writeToBitstream, Position[i]);
			for (int i = 0; i < 4; ++i)
				bs->Serialize(writeToBitstream, Orientation[i]);
		}
	}
}

#endif
