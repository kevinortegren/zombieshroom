#pragma once

#include <glm/gtc/quaternion.hpp>
#include <gtest/gtest.h>
#include <stdint.h>

namespace RootForce
{
	namespace Network
	{
		/** Define the message types */
		namespace MessageType
		{
			enum MessageType
			{
				GameStateSnapshot,
				Chat,
				UserConnected,
				UserDisconnected,
				UserInfo,
				UserCommandMoveForward,
				UserCommandMoveBackward,
				UserCommandStrafeRight,
				UserCommandStrafeLeft,
				UserCommandOrient,
				UserCommandSelectAbility,
				UserCommandActivateAbility,
				UserCommandDeactivateAbility,
				UserCommandPickUpAbility,
				UserCommandJump,
				UserCommandStopJumping
			};
		}

		struct MessageGameStateSnapshot;
		struct MessageChat;
		struct MessageUserConnected;
		struct MessageUserDisconnected;
		struct MessageUserInfo;
		struct MessageUserCommandOrient;
		struct MessageUserCommandSelectAbility;


		/** Header for the game state snapshot message. Following in the data stream is the actual updated components. */
		struct MessageGameStateSnapshot
		{
			// TODO: This needs to be developed in conjunction with the game logic.
		};

		/** Sent when a chat message is entered. This will be sent to the server and then sent to the given recipients. */
		struct MessageChat
		{
			enum MessageType
			{
				TYPE_CHAT,
				TYPE_SERVER_MESSAGE,
				TYPE_DEBUG
			};

			MessageType Type;
			int8_t SenderID;
			const char* Message;
		};
		
		/** Sent to the server when connecting (in order to identify yourself). Also sent from the server as part of the MessageUserConnected message. */
		struct MessageUserInfo
		{
			const char* PlayerName;
		};

		/** Sent to all connected clients when a client connects. Also sent to the connecting client for each connected client. */
		struct MessageUserConnected
		{
			int8_t UserID;
			MessageUserInfo UserInfo;
		};


		/** Sent to all connected clients when a client disconnects. */
		struct MessageUserDisconnected
		{
			int8_t UserID;
		};

		/** Sent to the server when the player reorients the character. */
		struct MessageUserCommandOrient
		{
			glm::quat Orientation;
		};

		/** Sent to the server when the player selects a new ability. */
		struct MessageUserCommandSelectAbility
		{
			uint8_t Slot;
		};
	}
}

TEST(NetworkMessage, SerializeOverlay)
{
	RootForce::Network::MessageChat m;
	m.Type = RootForce::Network::MessageChat::TYPE_SERVER_MESSAGE;
	m.SenderID = 10;
	m.Message = "Hello world";

	unsigned char* serializedData = (unsigned char*) &m;
	RootForce::Network::MessageChat m2 = *((RootForce::Network::MessageChat*) serializedData);

	EXPECT_TRUE(m.Type == m2.Type);
	EXPECT_TRUE(m.SenderID == m2.SenderID);
	EXPECT_TRUE(strcmp(m.Message, m2.Message) == 0);
}


TEST(NetworkMessage, SerializeCopy)
{
	RootForce::Network::MessageChat m;
	m.Type = RootForce::Network::MessageChat::TYPE_SERVER_MESSAGE;
	m.SenderID = 10;
	m.Message = "Hello world";
	
	size_t size = 0;
	size += sizeof(m.Type);
	size += sizeof(m.SenderID);
	size += strlen(m.Message);

	unsigned char* serializedData = new unsigned char[size];
	memcpy(serializedData, &m, size);

	RootForce::Network::MessageChat m2;
	memcpy(&m2, serializedData, size);

	EXPECT_TRUE(m.Type == m2.Type);
	EXPECT_TRUE(m.SenderID == m2.SenderID);
	EXPECT_TRUE(strcmp(m.Message, m2.Message) == 0);
}