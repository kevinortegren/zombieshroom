#pragma once

#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <glm/gtc/quaternion.hpp>
#include <stdint.h>
#include <vector>
#include <RootSystems/Include/Network/NetworkEntityMap.h>

namespace RootForce
{
	namespace Network
	{
		/** Define the message types */
		namespace MessageType
		{
			enum MessageType
			{
				GameStateSnapshot = ID_USER_PACKET_ENUM + 1,
				ChatToServer,
				ChatToClient,
				PlayData,
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
				UserCommandStopJumping,
				HACK_TransformUpdate
			};
		}

		struct MessageGameStateSnapshot;
		struct MessageChat;
		struct MessageUserConnected;
		struct MessageUserDisconnected;
		struct MessageUserInfo;
		struct MessageUserCommandOrient;
		struct MessageUserCommandSelectAbility;


		struct EntityCreated
		{
			static const uint8_t TYPE_PLAYER = 0;

			TemporaryId_t TemporaryID;
			SynchronizedId_t SynchronizedID;
			uint8_t EntityType;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};

		struct EntityRemoved
		{
			uint16_t SynchronizedID;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};


		/** Header for the game state snapshot message. Following in the data stream is the actual updated components. */
		struct MessageGameStateSnapshot
		{
			std::vector<EntityCreated> CreatedEntities;
			std::vector<EntityRemoved> RemovedEntities;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};

		/** Sent when a chat message is entered. This will be sent to the server and then sent to the given recipients. */
		struct MessageChat
		{
			static const uint8_t TYPE_CHAT = 0;
			static const uint8_t TYPE_SERVER_MESSAGE = 1;
			static const uint8_t TYPE_DEBUG = 2;

			uint8_t Type;
			int8_t SenderID;
			RakNet::RakString Message;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};
		
		/** Sent to the server when connecting (in order to identify yourself). Also sent from the server as part of the MessageUserConnected message. */
		struct MessageUserInfo
		{
			RakNet::RakString PlayerName;
			EntityCreated PlayerEntity;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};

		/** Sent to a connecting client, containing static data about the game */
		struct MessagePlayData
		{
			RakNet::RakString ServerName;
			RakNet::RakString MapName;
			uint8_t MaxPlayers;
			uint16_t MatchLength;
			uint8_t KillCount;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};

		/** Sent to all connected clients when a client connects. Also sent to the connecting client for each connected client. */
		struct MessageUserConnected
		{
			int8_t UserID;
			MessageUserInfo UserInfo;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};


		/** Sent to all connected clients when a client disconnects. */
		struct MessageUserDisconnected
		{
			int8_t UserID;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};

		/** Sent to the server when the player reorients the character. */
		struct MessageUserCommandOrient
		{
			glm::quat Orientation;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};

		/** Sent to the server when the player selects a new ability. */
		struct MessageUserCommandSelectAbility
		{
			uint8_t Slot;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};

		/** Sent from client to server (this is a HACK) */
		struct HACK_MessageTransformUpdate
		{
			glm::vec3 Position;
			glm::quat Orientation;

			void Serialize(bool writeToBitstream, RakNet::BitStream* bs);
		};

		// TODO: Add parameters for activate ability. Later.

	}
}