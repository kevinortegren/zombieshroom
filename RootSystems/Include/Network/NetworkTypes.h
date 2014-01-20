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
				ActionID_t ActionID;
				SequenceID_t SequenceID;
			};

			SynchronizedID_t SynchronizedID;
		};


		/*
			Defines a map between network entity IDs and local entities.
		*/
		typedef std::map<NetworkEntityID, ECS::Entity*> NetworkEntityMap;
	}
}