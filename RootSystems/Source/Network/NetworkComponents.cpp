#include <Network/NetworkComponents.h>

#ifndef COMPILE_LEVEL_EDITOR

extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

namespace RootForce
{
	namespace Network
	{
		NetworkComponent::SequenceIDMap NetworkComponent::s_sequenceIDMap;

		void NetworkComponent::SetID(ECS::Entity* p_entity, UserID_t p_userID, ActionID_t p_actionID)
		{
			ID.UserID = p_userID;
			ID.ActionID = p_actionID;

			UserActionKey_t key = (((UserActionKey_t) ID.UserID) << 48) | (((UserActionKey_t) ID.ActionID) << 16);
			ID.SequenceID = s_sequenceIDMap[key]++;

			g_networkEntityMap[ID] = p_entity;
		}

		void NetworkComponent::SetID(ECS::Entity* p_entity, NetworkComponent* p_parent)
		{
			ID.UserID = p_parent->ID.UserID;
			ID.ActionID = p_parent->ID.ActionID;
				
			UserActionKey_t key = (((UserActionKey_t) ID.UserID) << 48) | (((UserActionKey_t) ID.ActionID) << 16);
			ID.SequenceID = s_sequenceIDMap[key]++;

			g_networkEntityMap[ID] = p_entity;
		}

		void NetworkComponent::ResetSequenceForUser(UserID_t p_id)
		{
			std::vector<SequenceIDMap::iterator> destroy;
			for (SequenceIDMap::iterator it = s_sequenceIDMap.begin(); it != s_sequenceIDMap.end(); it++)
			{
				UserID_t id = (UserID_t) ((it->first & 0xFFFF000000000000) >> 48);
				if (id == p_id)
				{
					destroy.push_back(it);
				}
			}

			for (SequenceIDMap::iterator it : destroy)
			{
				s_sequenceIDMap.erase(it);
			}
		}



		bool ClientState::IsConnecting(ClientState p_state)
		{
			return p_state == ClientState::AWAITING_CONNECTION_ACCEPT ||
				   p_state == ClientState::SERVER_AWAITING_USER_INFO ||
				   p_state == ClientState::AWAITING_SERVER_INFO ||
				   p_state == ClientState::SERVER_AWAITING_MAP_LOAD_STATUS ||
				   p_state == ClientState::AWAITING_USER_CONNECT ||
				   p_state == ClientState::AWAITING_FIRST_GAMESTATE_DELTA ||
				   p_state == ClientState::AWAITING_SPAWN_POINT;
		}

		bool ClientState::IsConnected(ClientState p_state)
		{
			return p_state == ClientState::CONNECTED;
		}

		bool ClientState::IsDisconnected(ClientState p_state)
		{
			return p_state == ClientState::DISCONNECTED_TIMEOUT ||
				   p_state == ClientState::DISCONNECTED_REFUSED ||
				   p_state == ClientState::DISCONNECTED_REFUSED_TOO_MANY_PLAYERS ||
				   p_state == ClientState::DISCONNECTED_REFUSED_INVALID_PASSWORD ||
				   p_state == ClientState::DISCONNECTED_SERVER_SHUTDOWN;
		}

		bool ClientState::IsUnconnected(ClientState p_state)
		{
			return p_state == ClientState::UNCONNECTED;
		}



		ClientComponent::ClientComponent()
			: State(ClientState::UNCONNECTED)
			, IsRemote(false) {}
	}
}

#endif