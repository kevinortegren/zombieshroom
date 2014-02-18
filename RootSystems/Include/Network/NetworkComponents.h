#pragma once

#ifndef COMPILE_LEVEL_EDITOR

#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/Network/Messages.h>
#include <Utility/ECS/Include/Component.h>

namespace RootForce
{
	namespace Network
	{
		/* 
			Add this component onto every entity that you want to synchronize over the network.
		*/
		struct NetworkComponent : public ECS::Component<NetworkComponent>
		{
			typedef std::map<UserActionKey_t, SequenceID_t> SequenceIDMap;
			static SequenceIDMap s_sequenceIDMap;

			NetworkEntityID ID;
			
			NetworkComponent()
			{
				ID.UserID = ReservedUserID::NONE;
				ID.ActionID = ReservedActionID::NONE;
				ID.SequenceID = ReservedSequenceID::NONE;
			}

			/*
				Calculate the correct sequence ID and associate the entity in the global network entity map.
			*/
			void SetID(ECS::Entity* p_entity, UserID_t p_userID, ActionID_t p_actionID);
			void SetID(ECS::Entity* p_entity, NetworkComponent* p_parent);
			static void ResetSequenceForUser(UserID_t p_id);
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
				AWAITING_CONNECTION_ACCEPT,
				AWAITING_SERVER_INFO,
				AWAITING_USER_CONNECT,
				AWAITING_FIRST_GAMESTATE_DELTA,
				AWAITING_SPAWN_POINT,
				CONNECTED,
				DISCONNECTED_TIMEOUT,
				DISCONNECTED_REFUSED,
				DISCONNECTED_REFUSED_TOO_MANY_PLAYERS,
				DISCONNECTED_REFUSED_INVALID_PASSWORD,
				DISCONNECTED_SERVER_SHUTDOWN
			};

			/*
				True if the client is currently in the connection sequence.
			*/
			bool IsConnecting(ClientState p_state);

			/*
				True if the client is in a fully connected state.
			*/
			bool IsConnected(ClientState p_state);

			/*
				True if the client has been disconnected from the server.
			*/
			bool IsDisconnected(ClientState p_state);

			/*
				True if the client has not connected yet (Connect() has not been called).
			*/
			bool IsUnconnected(ClientState p_state);
		}


		/*
			Added on a player entity, holding information about the client that owns it.

			State shows if and what synchronous state the client is in.
			
		*/
		struct ClientComponent : public ECS::Component<ClientComponent>
		{
			ClientState::ClientState State;
			RakNet::RakString Name;
			bool IsRemote;

			ClientComponent();
		};
	}
}

#endif