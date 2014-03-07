#pragma once

#ifndef COMPILE_LEVEL_EDITOR

#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <stdint.h>
#include <vector>
#include <Utility/ECS/Include/Entity.h>
#include <RootSystems/Include/ComponentTypes.h>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ECS
{
	class World;
}

namespace RootForce
{
	/** Forward declare serializable components */
	struct Transform;
	struct HealthComponent;
	struct Physics;
	struct LookAtBehavior;
	struct Script;
	struct PlayerComponent;
	struct TDMRuleSet;
	struct PlayerPhysics;
	struct AbilitySpawnComponent;
	struct TimerComponent;
	struct FollowComponent;
	struct HomingComponent;
	struct DamageAndKnockback;
	struct StatChange;

	namespace Network
	{
		struct NetworkComponent;
	}

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
				JumpStart,
				JumpStop,
				AbilityEvent,
				AbilityCooldownOff,
				AbilityTryClaim,
				AbilityClaimedBy,
				RespawnRequest,
				Suicide,
				DestroyEntities,
				SpawnUser,
				LoadMap,
				LoadMapStatus,
				SetMaxPlayers,
				SetGameMode,
				SetMatchTime,
				SetKillCount,
				ServerInformation,
				TimeUp,
				AbilitySpawn,
				Death,
				PlayerTeamSelect,
				PlayerNameChange,
			};
		}


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
			uint8_t TeamID;

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

			float MovePower;
			float StrafePower;
			glm::vec2 Angle;
			float JumpTime;
			uint8_t SelectedAbility;

			glm::vec3 Position;
			glm::quat Orientation;
			glm::quat AimingDeviceOrientation;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by a client when a jump is started. Forwarded by the server.
		*/
		struct JumpStart
		{
			Network::UserID_t User;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by a client when a jump has ended. Forwarded by the server.
		*/
		struct JumpStop
		{
			Network::UserID_t User;
			float Time;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by a client when an ability event is done. Forwarded by the server.
		*/
		struct AbilityEvent
		{
			Network::UserID_t User;
			RootForce::AbilityEvent Event;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent to the client whenever the cooldown for a certain ability on the server reaches zero.
		*/
		struct AbilityCooldownOff
		{
			Network::UserID_t User;
			uint8_t AbilityIndex;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent to the server when a player wants to pick up an ability
		*/		
		struct AbilityTryClaim
		{
			Network::UserID_t User;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent to all clients when a player has successfully picked up an ability
		*/
		struct AbilityClaimedBy
		{
			Network::UserID_t User;
			Network::NetworkEntityID AbilitySpawnPointID;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		/*
			Sent by the client to the server when a respawn is request. Server will respond with a spawn user if the request is approved.
		*/
		struct RespawnRequest
		{
			Network::UserID_t User;		// Pretty much useless.

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};
		
		struct Suicide
		{
			Network::UserID_t User;

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
			RakNet::RakString AbilityPack;
			Network::UserID_t CurrentPlayers;
			Network::UserID_t MaxPlayers;
			bool PasswordProtected;
			bool IsDedicated;
			uint8_t GameMode;
			uint32_t MatchTimeSeconds;
			uint32_t KillCount;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};


		/*
			Sent by the server when a timer component reaches 0. 
		*/
		struct TimeUp 
		{
			Network::NetworkEntityID ID;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		struct AbilitySpawn
		{
			Network::NetworkEntityID ID;
			RakNet::RakString AbilityName;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		struct Death
		{
			Network::UserID_t User;
			Network::UserID_t LastDamageSource;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		struct PlayerTeamSelect
		{
			Network::NetworkEntityID UserID;
			uint8_t TeamID;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};

		struct PlayerNameChange
		{
			RakNet::RakString Name;
			Network::UserID_t UserID;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
		};
		/*
			Serialize functions for components
		*/
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, Transform* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, HealthComponent* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, Physics* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, Network::NetworkComponent* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, LookAtBehavior* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, Script* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, PlayerComponent* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, TDMRuleSet* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, PlayerPhysics* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, AbilitySpawnComponent* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, TimerComponent* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, FollowComponent* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, HomingComponent* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, DamageAndKnockback* p_c);
		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, StatChange* p_c);

		/*
			Returns true if the specified component can be serialized. False otherwise.
		*/
		bool CanSerializeComponent(ComponentType::ComponentType p_type);

		/*
			Serialize a general component into a bitstream. Returns true if the component can be serialized, false otherwise.

			Components are serialized in the following way:
				- Type : ComponentType
				- Data
			The data is serialized differently for different component types.
		*/
		bool SerializeComponent(RakNet::BitStream* p_bs, ECS::ComponentInterface* p_component, ComponentType::ComponentType p_type);

		/*
			Deserialize a component. This will create a new component in the given entity manager if the component doesn't exist on the given entity.
			Returns nullptr on error. Do not deserialize transform on our own player entity.

			The serialized data in the bitstream is assumed to be in the following order:
				- Type : ComponentType
				- Data
			Where data can be deserialized differently depending on type.
		*/
		ECS::ComponentInterface* DeserializeComponent(RakNet::BitStream* p_bs, ECS::Entity* p_entity, ECS::EntityManager* p_entityManager, bool p_isSelf);

		/*
			Check whether an entity can be serialized. This will check whether it exists in the network entity map and whether it has a script component.
		*/
		bool CanSerializeEntity(ECS::Entity* p_entity, ECS::EntityManager* p_entityManager, const Network::NetworkEntityMap& p_map);
		
		/*
			Serialize an entity and all of its serializable components. The entity needs to have a script component registered in the given entity manager.
			The entity does also need to be registered in the given network entity map. Returns false if these requirements are not met.

			Entities are serialized in the following way:
				- ID : NetworkEntityID
				- ScriptName : RakString
				- NumberOfComponents : int
				- Components
			Where components are serialized differently depending on type. See SerializeComponent for more information.
		*/
		bool SerializeEntity(RakNet::BitStream* p_bs, ECS::Entity* p_entity, ECS::EntityManager* p_entityManager, const Network::NetworkEntityMap& p_map);

		/*
			Deserialize an entity and all of its serialized components. This will create an entity in the entity manager
			if the deserialized ID cannot be found in the given network entity map. A deserialized entity will be added to the network entity map. Do not deserialize transform on our own player entity.

			The serialized data in the bitstream is assumed to be in the following order:
				- ID : NetworkEntityID
				- ScriptName : RakString
				- NumberOfComponents : int
				- Components
			Where components are deserialized differently depending on their type. See DeserializeComponent for more information.
		*/
		ECS::Entity* DeserializeEntity(RakNet::BitStream* p_bs, ECS::EntityManager* p_entityManager, Network::NetworkEntityMap& p_map, Network::UserID_t p_self);

		/*
			Serialize all entities in the world along with all their components that can be serialized.

			The world is serialized in the following way:
				- NumberOfEntities
				- Entities
			See SerializeEntity for how entities are serialized.
		*/
		void SerializeWorld(RakNet::BitStream* p_bs, ECS::World* p_world, const Network::NetworkEntityMap& p_map);

		/*
			Deserialize a world, creating any entities that are not existing, and updating all serializable components. Do not deserialize transform on our own player entity.

			The serialized data in the bitstream is assumed to be in the following order:
				- NumberOfEntities
				- Entities
			See DeserializeEntity for how entities are serialized.
		*/
		void DeserializeWorld(RakNet::BitStream* p_bs, ECS::World* p_world, Network::NetworkEntityMap& p_map, Network::UserID_t p_self);
	}
}

#endif