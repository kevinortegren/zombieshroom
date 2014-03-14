#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/DebugOverlay/DebugOverlay.h>
#include <Network/NetworkDebugSystem.h>
#include <sstream>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	namespace Network
	{
		NetworkDebugSystem::NetworkDebugSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{
			SetUsage<NetworkComponent>();
			SetUsage<PlayerComponent>();
			SetUsage<Transform>();
		}

		void NetworkDebugSystem::Init()
		{
			m_networks.Init(m_world->GetEntityManager());
			m_players.Init(m_world->GetEntityManager());
			m_transforms.Init(m_world->GetEntityManager());
		}

		void NetworkDebugSystem::ProcessEntity(ECS::Entity* p_entity)
		{
#ifdef _DEBUG
			NetworkComponent* network = m_networks.Get(p_entity);
			PlayerComponent* player = m_players.Get(p_entity);
			Transform* transform = m_transforms.Get(p_entity);

			std::stringstream ss;
			ss << network->ID.UserID << ": " << player->Name << " (" << transform->m_position.x << ", " << transform->m_position.y << ", " << transform->m_position.z << ") ";
			ss << "(" << player->AbilityScripts[0].Name << ", " << player->AbilityScripts[1].Name << ", " << player->AbilityScripts[2].Name << ") ";
			ss << "<br/>";

			g_engineContext.m_debugOverlay->AddHTMLToBuffer(ss.str().c_str());
#endif
		}
	}
}