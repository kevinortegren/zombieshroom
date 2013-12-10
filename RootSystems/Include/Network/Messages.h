#pragma once

#include <glm/gtc/quaternion.hpp>
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
				ChatToServer,
				ChatToClient,
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


		struct EntityCreated
		{
			uint16_t TemporaryID;
			uint16_t SynchronizedID;
		};

		struct EntityRemoved
		{
			uint16_t SynchronizedID;
		};

		struct UpdatedComponent
		{
			uint16_t SynchronizedID;
			unsigned int ComponentTypeID;
			uint8_t* ComponentData;
		};


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

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
		};
		
		/** Sent to the server when connecting (in order to identify yourself). Also sent from the server as part of the MessageUserConnected message. */
		struct MessageUserInfo
		{
			const char* PlayerName;

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
		};

		/** Sent to all connected clients when a client connects. Also sent to the connecting client for each connected client. */
		struct MessageUserConnected
		{
			int8_t UserID;
			MessageUserInfo UserInfo;

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
		};


		/** Sent to all connected clients when a client disconnects. */
		struct MessageUserDisconnected
		{
			int8_t UserID;

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
		};

		/** Sent to the server when the player reorients the character. */
		struct MessageUserCommandOrient
		{
			glm::quat Orientation;

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
		};

		/** Sent to the server when the player selects a new ability. */
		struct MessageUserCommandSelectAbility
		{
			uint8_t Slot;

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
		};
	}
}