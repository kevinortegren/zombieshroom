#include <RootForce/Include/ConnectingState.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/Messages.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;

namespace RootForce
{
	ConnectingState::ConnectingState(NetworkContext& p_networkContext, SharedSystems& p_sharedSystems)
		: m_networkContext(p_networkContext)
		, m_sharedSystems(p_sharedSystems)
	{}

	void ConnectingState::Initialize()
	{
		
	}

	void ConnectingState::Enter(const GameStates::PlayData& p_playData)
	{
		// Connect the client and create a server if necessary
		if (p_playData.Host)
		{
			m_networkContext.m_server = std::shared_ptr<RootForce::Network::Server>(new RootForce::Network::Server(g_engineContext.m_logger, p_playData.ServerName, p_playData.Port, p_playData.MaxPlayers));
			m_networkContext.m_serverMessageHandler = std::shared_ptr<RootForce::Network::ServerMessageHandler>(new RootForce::Network::ServerMessageHandler(m_networkContext.m_server->GetPeerInterface(), g_engineContext.m_logger, g_world));
			m_networkContext.m_client->Connect("127.0.0.1", p_playData.Port);

			
			ECS::Entity* entity = g_world->GetEntityManager()->CreateEntity();
			g_world->GetTagManager()->RegisterEntity("MatchState", entity);
			RootForce::TDMRuleSet* rules = g_world->GetEntityManager()->CreateComponent<RootForce::TDMRuleSet>(entity);;
			rules->ScoreLimit = p_playData.Killcount;
			rules->TimeLeft = p_playData.MatchLength;
			rules->TeamOneScore = 0;
			rules->TeamTwoScore = 0;
		}
		else
		{
			m_networkContext.m_client->Connect(p_playData.Address.c_str(), p_playData.Port);

			ECS::Entity* ruleEntity = g_world->GetEntityManager()->CreateEntity();
			g_world->GetTagManager()->RegisterEntity("MatchState", ruleEntity);
			RootForce::TDMRuleSet* rules = g_world->GetEntityManager()->CreateComponent<RootForce::TDMRuleSet>(ruleEntity);;
			rules->ScoreLimit = 20; //TODO: this will be sent through the  gameState snapshot once this has been implemented
			rules->TimeLeft = 20;   // hardcoded for now so that the game does not crash
			rules->TeamOneScore = 0;
			rules->TeamTwoScore = 0;
		}

		// Reset the network entity map
		m_networkContext.m_networkEntityMap = std::shared_ptr<RootForce::Network::NetworkEntityMap>(new RootForce::Network::NetworkEntityMap);

		// Set the network entity map on both message handlers
		m_networkContext.m_clientMessageHandler->SetNetworkEntityMap(m_networkContext.m_networkEntityMap.get());
		m_networkContext.m_clientMessageHandler->SetPlayerSystem(m_sharedSystems.m_playerSystem.get());
		m_networkContext.m_serverMessageHandler->SetNetworkEntityMap(m_networkContext.m_networkEntityMap.get());

		// Set the connection message handler on the client.
		m_networkContext.m_client->SetMessageHandler(m_networkContext.m_clientMessageHandler.get());
		m_networkContext.m_server->SetMessageHandler(m_networkContext.m_serverMessageHandler.get());
	}

	void ConnectingState::Exit()
	{}

	GameStates::GameStates ConnectingState::Update()
	{
		m_networkContext.m_server->Update();
		m_networkContext.m_client->Update();

		if (m_networkContext.m_clientMessageHandler->GetClientState() == RootForce::Network::ClientState::CONNECTED)
			return GameStates::Ingame;

		return GameStates::Connecting;
	}
}