#include <DeserializationSystem.h>
#include <Network/Messages.h>
#include <Network/NetworkTypes.h>
#include <Network/NetworkComponents.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>

extern RootEngine::GameSharedContext g_engineContext;
extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

namespace RootForce
{
	DeserializationSystem::DeserializationSystem(ECS::World* p_world)
		: ECS::VoidSystem(p_world)
	{

	}

	void DeserializationSystem::SetData(const RakNet::BitStream* p_bs)
	{
		unsigned int size = p_bs->GetNumberOfUnreadBits() / 8;
		unsigned int readOffset = p_bs->GetReadOffset() / 8;

		m_data.resize(size);
		memcpy(&m_data[0], p_bs->GetData() + readOffset, size); 
	}

	void DeserializationSystem::Process()
	{
		if (!m_data.empty())
		{
			RakNet::BitStream bs(&m_data[0], m_data.size(), false);

			ECS::Entity* clientEntity = m_world->GetTagManager()->GetEntityByTag("Client");
			assert(clientEntity != nullptr);

			Network::ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);
			assert(clientComponent != nullptr);

			Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(m_world->GetTagManager()->GetEntityByTag("Player"));	
			assert(network != nullptr);

			// Deserialize and set to connected if we were awaiting our first game state snapshot.
			NetworkMessage::DeserializeWorld(&bs, m_world, g_networkEntityMap, network->ID.UserID);

			if (clientComponent->State == Network::ClientState::AWAITING_FIRST_GAMESTATE_DELTA)
			{
				clientComponent->State = Network::ClientState::CONNECTED;
			}

			m_data.clear();
		}
	}
}