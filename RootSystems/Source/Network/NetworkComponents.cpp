#include <Network/NetworkComponents.h>

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

		ClientComponent::ClientComponent()
			: State(ClientState::UNCONNECTED)
			, IsRemote(false) {}
	}
}