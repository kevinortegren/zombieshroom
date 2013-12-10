#pragma once

#include <glm/gtc/quaternion.hpp>
#include <stdint.h>
#include <vector>

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

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
			void Deserialize(uint8_t* buffer);
		};

		struct EntityRemoved
		{
			uint16_t SynchronizedID;

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
			void Deserialize(uint8_t* buffer);
		};

		struct ComponentUpdated
		{
			uint16_t SynchronizedID;
			unsigned int ComponentTypeID;
			uint8_t* ComponentData;

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
			void Deserialize(uint8_t* buffer);
		};


		/** Header for the game state snapshot message. Following in the data stream is the actual updated components. */
		struct MessageGameStateSnapshot
		{
			std::vector<EntityCreated> CreatedEntities;
			std::vector<EntityRemoved> RemovedEntities;
			std::vector<ComponentUpdated> UpdatedComponents;

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
			void Deserialize(uint8_t* buffer);
		};

		/** Sent when a chat message is entered. This will be sent to the server and then sent to the given recipients. */
		struct MessageChat
		{
			/*
			enum MessageType
			{
				TYPE_CHAT,
				TYPE_SERVER_MESSAGE,
				TYPE_DEBUG
			};
			*/
			static const uint8_t TYPE_CHAT = 0;
			static const uint8_t TYPE_SERVER_MESSAGE = 1;
			static const uint8_t TYPE_DEBUG = 2;


			//MessageType Type;
			uint8_t Type;
			int8_t SenderID;
			char* Message;

			int GetSerializedSize() const;
			void Serialize(uint8_t* buffer) const;
			void Deserialize(uint8_t* buffer);
		};
		
		/** Sent to the server when connecting (in order to identify yourself). Also sent from the server as part of the MessageUserConnected message. */
		struct MessageUserInfo
		{
			char* PlayerName;

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