#include <RootSystems/Include/Network/Messages.h>
#include <cstring>

namespace RootForce
{
	namespace Network
	{
		int EntityCreated::GetSerializedSize() const
		{
			int size = 0;
			size += sizeof(TemporaryID);
			size += sizeof(SynchronizedID);

			return size;
		}

		void EntityCreated::Serialize(uint8_t* buffer) const
		{
			memcpy(buffer, &TemporaryID, sizeof(TemporaryID));
			memcpy(buffer + sizeof(TemporaryID), &SynchronizedID, sizeof(SynchronizedID));
		}

		void EntityCreated::Deserialize(uint8_t* buffer)
		{
			memcpy(&TemporaryID, buffer, sizeof(TemporaryID));
			memcpy(&SynchronizedID, buffer + sizeof(TemporaryID), sizeof(SynchronizedID));
		}



		int EntityRemoved::GetSerializedSize() const
		{
			int size = 0;
			size += sizeof(SynchronizedID);

			return size;
		}

		void EntityRemoved::Serialize(uint8_t* buffer) const
		{
			memcpy(buffer, &SynchronizedID, sizeof(SynchronizedID));
		}

		void EntityRemoved::Deserialize(uint8_t* buffer)
		{
			memcpy(&SynchronizedID, buffer, sizeof(SynchronizedID));
		}



		int ComponentUpdated::GetSerializedSize() const
		{
			int size = 0;
			size += sizeof(SynchronizedID);
			size += sizeof(ComponentTypeID);

			return size;
		}

		void ComponentUpdated::Serialize(uint8_t* buffer) const
		{

		}

		void ComponentUpdated::Deserialize(uint8_t* buffer)
		{

		}



		int MessageGameStateSnapshot::GetSerializedSize() const
		{
			return 0;
		}

		void MessageGameStateSnapshot::Serialize(uint8_t* buffer) const
		{

		}

		void MessageGameStateSnapshot::Deserialize(uint8_t* buffer)
		{

		}



		int MessageChat::GetSerializedSize() const
		{
			int size = 0;
			size += sizeof(Type);
			size += sizeof(SenderID);
			size += strlen(Message) + 1;

			return size;
		}

		void MessageChat::Serialize(uint8_t* buffer) const
		{
			memcpy(buffer, &Type, sizeof(Type));
			memcpy(buffer + sizeof(Type), &SenderID, sizeof(SenderID));
			memcpy(buffer + sizeof(Type) + sizeof(SenderID), Message, strlen(Message) + 1);
		}

		
		void MessageChat::Deserialize(uint8_t* buffer)
		{
			memcpy(&Type, buffer, sizeof(Type));
			memcpy(&SenderID, buffer + sizeof(Type), sizeof(SenderID));
			Message = (char*) buffer + sizeof(Type) + sizeof(SenderID);
		}
		



		int MessageUserInfo::GetSerializedSize() const
		{
			int size = 0;
			size += strlen(PlayerName) + 1;

			return size;
		}

		void MessageUserInfo::Serialize(uint8_t* buffer) const
		{
			memcpy(buffer, PlayerName, strlen(PlayerName) + 1);
		}



		int MessageUserConnected::GetSerializedSize() const
		{
			int size = 0;
			size += sizeof(UserID);
			size += UserInfo.GetSerializedSize();

			return size;
		}

		void MessageUserConnected::Serialize(uint8_t* buffer) const
		{
			memcpy(buffer, &UserID, sizeof(UserID));
			UserInfo.Serialize(&buffer[sizeof(UserID)]);
		}



		int MessageUserDisconnected::GetSerializedSize() const
		{
			int size = 0;
			size += sizeof(UserID);

			return size;
		}

		void MessageUserDisconnected::Serialize(uint8_t* buffer) const
		{
			memcpy(buffer, &UserID, sizeof(UserID));
		}



		int MessageUserCommandOrient::GetSerializedSize() const
		{
			int size = 0;
			size += sizeof(Orientation);

			return size;
		}

		void MessageUserCommandOrient::Serialize(uint8_t* buffer) const
		{
			memcpy(buffer, &Orientation[0], sizeof(Orientation));
		}



		int MessageUserCommandSelectAbility::GetSerializedSize() const
		{
			int size = 0;
			size += sizeof(Slot);

			return size;
		}

		void MessageUserCommandSelectAbility::Serialize(uint8_t* buffer) const
		{
			memcpy(buffer, &Slot, sizeof(Slot));
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