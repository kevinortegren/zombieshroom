#pragma once

#include <cstdint>
#include <map>

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
			Add this component onto every entity that you want to synchronize over the network.

			The AssociatedInNetworkEntityMap field is read by the NetworkRegistrationSystem and entities
			are added to a network entity map if the field is false.
		*/
		struct NetworkComponent : public ECS::Component<NetworkComponent>
		{
			typedef std::map<UserActionKey_t, SequenceID_t> SequenceIDMap;
			static SequenceIDMap s_sequenceIDMap;
			
			NetworkEntityID ID;
			bool AssociatedInNetworkEntityMap;
			
			NetworkComponent()
			{
				ID.UserID = ReservedUserID::NONE;
				ID.ActionID = ReservedActionID::NONE;
				ID.SequenceID = ReservedSequenceID::NONE;
				AssociatedInNetworkEntityMap = false;
			}

			void SetID(UserID_t p_userID, ActionID_t p_actionID)
			{
				ID.UserID = p_userID;
				ID.ActionID = p_actionID;

				UserActionKey_t key = (((UserActionKey_t) ID.UserID) << 48) | (((UserActionKey_t) ID.ActionID) << 16);
				ID.SequenceID = s_sequenceIDMap[key]++;
			}

			void SetID(NetworkComponent* p_parent)
			{
				ID.UserID = p_parent->ID.UserID;
				ID.ActionID = p_parent->ID.ActionID;
				
				UserActionKey_t key = (((UserActionKey_t) ID.UserID) << 48) | (((UserActionKey_t) ID.ActionID) << 16);
				ID.SequenceID = s_sequenceIDMap[key]++;
			}
		};
	}
}