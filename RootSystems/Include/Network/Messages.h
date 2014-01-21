#pragma once

#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <stdint.h>
#include <vector>
#include <Utility/ECS/Include/Entity.h>
#include <RootSystems/Include/ComponentTypes.h>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/PlayerSystem.h>

namespace RootForce
{
	namespace NetworkMessage
	{
		/** Define the message types */
		namespace MessageType
		{
			enum MessageType
			{
				GameStateDelta = ID_USER_PACKET_ENUM + 1,
				Chat,
				UserConnected,
				UserDisconnected,
				UserInformation,
				PlayerCommand,
				DestroyEntities,
				SpawnUser,
				LoadMap,
				LoadMapStatus,
				SetMaxPlayers,
				SetGameMode,
				SetMatchTime,
				SetKillCount,
				ServerInformation
			};
		}

		/*
			Sent at regular intervals from the server to the clients.
		*/
		struct GameStateDelta
		{
			struct SerializableComponent
			{
				ComponentType::ComponentType Type;
				unsigned int DataSize;
				char* Data;

				SerializableComponent();
				~SerializableComponent();
				void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
				bool SerializeComponent(ECS::ComponentInterface* p_component, ComponentType::ComponentType p_type);
				ECS::ComponentInterface* DeserializeComponent(ECS::Entity* p_entity, ECS::EntityManager* p_entityManager);
			};

			struct SerializableEntity
			{
				Network::NetworkEntityID ID;
				RakNet::RakString ScriptName;
				std::vector<SerializableComponent> Components;

				void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
				bool SerializeEntity(ECS::Entity* p_entity, ECS::World* p_world, const Network::NetworkEntityMap& p_map);
				ECS::Entity* DeserializeEntity(ECS::World* p_world, Network::NetworkEntityMap& p_map);
			};

			std::vector<SerializableEntity> Entities;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};
		

		/* 
			Sent when a chat message is entered. This will be sent to the server and then sent to the given recipients. 
		*/
		struct Chat
		{
			static const uint8_t TYPE_CHAT = 0;
			static const uint8_t TYPE_SERVER_MESSAGE = 1;
			static const uint8_t TYPE_DEBUG = 2;

			uint8_t Type;
			Network::UserID_t Sender;
			RakNet::RakString Message;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent to all clients when a user connects to the server. Also sent to a connecting client for each already connected client.
		*/
		struct UserConnected
		{
			Network::UserID_t User;
			bool IsYou;
			RakNet::RakString Name;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent to all clients when a client disconnects or the connection to the client is lost.
		*/
		struct UserDisconnected
		{
			Network::UserID_t User;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent to the server on connect
		*/
		struct UserInformation
		{
			RakNet::RakString Name;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent every frame from the client to the server. Also forwarded by the server to all other clients.
		*/
		struct PlayerCommand
		{
			Network::UserID_t User;
			PlayerActionComponent Action;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};
		
		/*
			Sent by the server to destroy a set of entities matching the given ID on a client.
		*/
		struct DestroyEntities
		{
			Network::NetworkEntityID ID;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by the server to a client to spawn the player associated with the given UserID on the map.
		*/
		struct SpawnUser
		{
			Network::UserID_t User;
			unsigned int SpawnPointIndex;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by the server to a client to load a specific map.
		*/
		struct LoadMap
		{
			RakNet::RakString MapName;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by the client to the server to announce its map loading status.
		*/
		struct LoadMapStatus
		{
			static const uint8_t STATUS_LOADING = 0;
			static const uint8_t STATUS_COMPLETED = 1;
			static const uint8_t STATUS_FAILED_MAP_NOT_FOUND = 2;
			static const uint8_t STATUS_FAILED_INVALID_HASH = 3;

			uint8_t Status;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by the server to a client to change the maximum amount of players.
		*/
		struct SetMaxPlayers
		{
			Network::UserID_t MaxPlayers;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by the server to a client to change the game mode.
		*/
		struct SetGameMode
		{
			uint8_t GameMode;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by the server to a client to change the match time.
		*/
		struct SetMatchTime
		{
			uint32_t Seconds;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by the server to a client to change the kill count.
		*/
		struct SetKillCount
		{
			uint32_t Count;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};


		/*
			Sent by the server to a connecting client or as a response to a ping.
		*/
		struct ServerInformation
		{
			RakNet::RakString ServerName;
			RakNet::RakString MapName;
			Network::UserID_t CurrentPlayers;
			Network::UserID_t MaxPlayers;
			bool PasswordProtected;
			bool IsDedicated;
			uint8_t GameMode;
			uint32_t MatchTimeSeconds;
			uint32_t KillCount;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};
	}
}