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
	}
}

/*
TEST(Messages, Serialize)
{
	RootForce::Network::MessageChat m;
	m.Type = RootForce::Network::MessageChat::TYPE_DEBUG;
	m.SenderID = 5;
	m.Message = "Hello world";

	uint8_t* buffer = new uint8_t[m.GetSerializedSize()];
	m.Serialize(buffer);

	RootForce::Network::MessageChat m2 = *(RootForce::Network::MessageChat*) buffer;

	EXPECT_TRUE(m.Type == m2.Type);
	EXPECT_TRUE(m.SenderID == m2.SenderID);
	EXPECT_TRUE(m.Message == m2.Message);
}
*/

#endif
