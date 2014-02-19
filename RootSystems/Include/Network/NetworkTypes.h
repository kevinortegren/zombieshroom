#pragma once

#ifndef COMPILE_LEVEL_EDITOR

#include <cstdint>
#include <map>

namespace ECS
{
	class Entity;
	class EntityManager;
}

namespace RootForce
{
	namespace Network
	{
		typedef uint16_t UserID_t;
		typedef uint32_t ActionID_t;
		typedef uint16_t SequenceID_t;
		typedef uint64_t SynchronizedID_t;
		typedef uint64_t UserActionKey_t;

		static_assert(sizeof(uint8_t) == 1, "Invalid maximum number of bits in uint8_t");
		static_assert(sizeof(uint16_t) == 2, "Invalid maximum number of bits in uint16_t");
		static_assert(sizeof(uint32_t) == 4, "Invalid maximum number of bits in uint32_t");
		static_assert(sizeof(uint64_t) == 8, "Invalid maximum number of bits in uint64_t");

		/** Specifies reserved user IDs. Connected peers will start counting from index 0. */
		namespace ReservedUserID
		{
			enum ReservedUserID
			{
				NONE = 0xFFFD,
				UNCONNECTED,
				ALL,
			};
		};
		
		/** Specifies reserved action IDs. */
		namespace ReservedActionID
		{
			enum ReservedActionID
			{
				NONE = 0xFFFFFFFC,
				ABILITYSPAWN,
				CONNECT,
				ALL,
			};
		}

		/** Specifies reserved sequence IDs. */
		namespace ReservedSequenceID
		{
			enum ReservedSequenceID
			{
				NONE = 0xFFFD,
				CLIENT_ENTITY,
				ALL,
			};
		}

		const SequenceID_t SEQUENCE_PLAYER_ENTITY = 0;
		const SequenceID_t SEQUENCE_AIMING_DEVICE_ENTITY = 1;

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
				SequenceID_t SequenceID;
				ActionID_t ActionID;
			};

			SynchronizedID_t SynchronizedID;

			NetworkEntityID::NetworkEntityID()
			{
				UserID = ReservedUserID::NONE;
				ActionID = ReservedActionID::NONE;
				SequenceID = ReservedSequenceID::NONE;
			}

			NetworkEntityID::NetworkEntityID(UserID_t p_user, ActionID_t p_action, SequenceID_t p_sequence)
			{
				UserID = p_user;
				ActionID = p_action;
				SequenceID = p_sequence;
			}

			bool operator<(const NetworkEntityID& p_rhs) const
			{
				return SynchronizedID < p_rhs.SynchronizedID;
			}

			bool operator==(const NetworkEntityID& p_rhs) const
			{
				return SynchronizedID == p_rhs.SynchronizedID;
			}
		};


		/*
			Defines a map between network entity IDs and local entities.
		*/
		typedef std::map<NetworkEntityID, ECS::Entity*> NetworkEntityMap;
		ECS::Entity* FindEntity(NetworkEntityMap& p_map, const NetworkEntityID& p_id);

		/*
			Remove a set of entities from the network entity map and delete them.
		*/
		void DeleteEntities(NetworkEntityMap& p_map, NetworkEntityID p_id, ECS::EntityManager* p_entityManager);
	}
}

#endif