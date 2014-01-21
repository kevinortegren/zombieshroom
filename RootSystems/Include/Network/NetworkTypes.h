#pragma once

#include <cstdint>
#include <map>
#include <Utility/ECS/Include/Entity.h>


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
				NONE = 0xFFFFFFFD,
				CONNECT,
				ALL,
			};
		}

		/** Specifies reserved sequence IDs. */
		namespace ReservedSequenceID
		{
			enum ReservedSequenceID
			{
				NONE = 0xFFFE,
				ALL,
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
				SequenceID_t SequenceID;
				ActionID_t ActionID;
			};

			SynchronizedID_t SynchronizedID;

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
	}
}