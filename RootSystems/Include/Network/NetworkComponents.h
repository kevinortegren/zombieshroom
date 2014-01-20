#pragma once

#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/Network/Messages.h>
#include <Utility/ECS/Include/Component.h>

namespace RootForce
{
	namespace Network
	{
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


		/*
			Added on a server entity, holding information about the server.
		*/
		struct ServerInformationComponent : public ECS::Component<ServerInformationComponent>
		{
			RootForce::NetworkMessage::ServerInformation Information;
		};


		/*
			Defines the states a client can be in. Used for synchronized communication.
		*/
		namespace ClientState
		{
			enum ClientState
			{
				UNCONNECTED,
				AWAITING_SERVER_INFO,
				AWAITING_USER_CONNECT,
				AWAITING_FIRST_GAMESTATE_DELTA,
				CONNECTED,
				DISCONNECTED_TIMEOUT,
				DISCONNECTED_REFUSED,
				DISCONNECTED_REFUSED_TOO_MANY_PLAYERS,
				DISCONNECTED_SERVER_SHUTDOWN
			};
		}


		/*
			Added on a client entity, holding information about the client.
		*/
		struct ClientComponent : public ECS::Component<ClientComponent>
		{
			ClientState::ClientState State;
			RakNet::RakString Name;
		};
	}
}