#pragma once

#include <stdint.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakString.h>
#include <RootSystems/Include/Components.h>
#include <Utility/ECS/Include/Entity.h>
#include <Utility/ECS/Include/Component.h>

namespace RootForce
{
	namespace Network
	{
		typedef uint16_t UserID_t;
		typedef uint32_t ActionID_t;
		typedef uint16_t SequenceID_t;
		typedef uint64_t SynchronizedID_t;
		typedef uint64_t UserActionKey_t;

		/** Specifies reserved user IDs. Connected peers will start counting from index 0. */
		namespace ReservedUserID
		{
			enum ReservedUserID
			{
				UNCONNECTED_USER = 0xFFFF
			};
		};
		
		/** Specifies reserved action IDs. Local action IDs will start counting from ACTION0. */
		namespace ReservedActionID
		{
			enum ReservedActionID
			{
				NONE,
				CONNECT,
				ACTION0
			};
		}


		/*
			Uniquely identifies an entity across all peers.

			UserID - Identifies the peer, creator of the entity.
			ActionID - Identifies the specific user command that created this entity.
			SequenceID - Increases for every entity that an action created. Starts at 0.

			SynchronizedID - A combination of all above, creating a unique entity ID across all peers.
		*/
		union NetworkEntityID
		{
			struct
			{
				UserID_t UserID;
				ActionID_t ActionID;
				SequenceID_t SequenceID;
			};

			SynchronizedID_t SynchronizedID;
		};

		/*
			Used to associate a NetworkEntityID with a local entity.
		*/
		typedef std::map<NetworkEntityID, ECS::Entity*> NetworkEntityMap;
		typedef std::map<UserActionKey_t, SequenceID_t> SequenceIDMap;
		



		/* 
			Add this component onto every entity that you want to synchronize over the network.

			The AssociatedInNetworkEntityMap field is read by the NetworkRegistrationSystem and entities
			are added to a network entity map if the field is false.
		*/
		struct NetworkComponent
		{
			static SequenceIDMap s_sequenceIDMap;
			
			NetworkEntityID ID;
			bool AssociatedInNetworkEntityMap;
			
			NetworkComponent(UserID_t p_userID, ActionID_t p_actionID)
			{
				ID.UserID = p_userID;
				ID.ActionID = p_actionID;

				UserActionKey_t key = (((UserActionKey_t) ID.UserID) << 48) | (((UserActionKey_t) ID.ActionID) << 16);
				ID.SequenceID = s_sequenceIDMap[key]++;

				AssociatedInNetworkEntityMap = false;
			}

			NetworkComponent(NetworkComponent* p_parent)
			{
				ID.UserID = p_parent->ID.UserID;
				ID.ActionID = p_parent->ID.ActionID;
				
				UserActionKey_t key = (((UserActionKey_t) ID.UserID) << 48) | (((UserActionKey_t) ID.ActionID) << 16);
				ID.SequenceID = s_sequenceIDMap[key]++;

				AssociatedInNetworkEntityMap = false;
			}
		};

		void SerializeEntity(RakNet::BitStream* p_bs, ECS::Entity* p_entity, const std::string& p_entityScript, const NetworkEntityMap& p_map, ECS::World* p_world);
		ECS::Entity* DeserializeEntity(RakNet::BitStream* p_bs, const NetworkEntityMap& p_map, ECS::World* p_world);


		/*
			Defines a serialized component that can be sent across the network
		*/
		/*
		struct SerializedComponent
		{
			ComponentType::ComponentType Type;
			unsigned int DataSize;
			unsigned char* Data;

			void Serialize(bool p_write, RakNet::BitStream* p_bs);
			void SerializeComponent(ECS::ComponentInterface* p_component, ComponentType::ComponentType p_type);
			void DeserializeComponent(ECS::ComponentInterface* p_component);
		};
		*/

		/*
			Defines a serialized entity that can be sent across the network
		*/
		/*
		struct SerializedEntity
		{
			NetworkEntityID ID;
			RakNet::RakString Script;
			std::vector<SerializedComponent> Components;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
			void SerializeEntity(ECS::Entity* p_entity);
			void DeserializeEntity(ECS::Entity* p_entity);
		};
		*/

		/*
			Sent occasionally to clients to synchronize entities.
		*/
		/*
		struct MessageDeltaGameState
		{
			std::vector<SerializedEntity> Entities;
		};
		*/
	}
}