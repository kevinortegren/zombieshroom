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
		m_sharedSystems.m_worldSystem = std::shared_ptr<RootForce::WorldSystem>(new RootForce::WorldSystem(g_world, &g_engineContext));
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
			rules->TimeLeft = (float)p_playData.MatchLength;
			rules->TeamScore[1] = 0;
			rules->TeamScore[2] = 0;
			// Setup server info response
			RootForce::Network::MessagePlayData response;
			response.ServerName = p_playData.ServerName.c_str();
			response.MapName = p_playData.MapName.c_str();
			response.MaxPlayers = (uint8_t) p_playData.MaxPlayers;
			response.MatchLength = (uint16_t) p_playData.MatchLength;
			response.KillCount = (uint8_t) p_playData.Killcount;

			m_networkContext.m_serverMessageHandler->SetPlayDataResponse(response);
		}
		else
		{
			m_networkContext.m_client->Connect(p_playData.Address.c_str(), p_playData.Port);

			ECS::Entity* ruleEntity = g_world->GetEntityManager()->CreateEntity();
			g_world->GetTagManager()->RegisterEntity("MatchState", ruleEntity);
			RootForce::TDMRuleSet* rules = g_world->GetEntityManager()->CreateComponent<RootForce::TDMRuleSet>(ruleEntity);;
			rules->ScoreLimit = 20; //TODO: this will be sent through the  gameState snapshot once this has been implemented
			rules->TimeLeft = 20;   // hardcoded for now so that the game does not crash
			rules->TeamScore[1] = 0;
			rules->TeamScore[2] = 0;
		}

		// Reset the network entity map
		m_networkContext.m_networkEntityMap = std::shared_ptr<RootForce::Network::NetworkEntityMap>(new RootForce::Network::NetworkEntityMap);

		// Set the network entity map on both message handlers
		m_networkContext.m_client->SetNetworkEntityMap(m_networkContext.m_networkEntityMap.get());
		m_networkContext.m_clientMessageHandler->SetNetworkEntityMap(m_networkContext.m_networkEntityMap.get());
		m_networkContext.m_clientMessageHandler->SetPlayerSystem(m_sharedSystems.m_playerSystem.get());
		m_networkContext.m_clientMessageHandler->SetWorldSystem(m_sharedSystems.m_worldSystem.get());
		if (m_networkContext.m_serverMessageHandler != nullptr)
			m_networkContext.m_serverMessageHandler->SetNetworkEntityMap(m_networkContext.m_networkEntityMap.get());

		// Set the connection message handler on the client.
		m_networkContext.m_client->SetMessageHandler(m_networkContext.m_clientMessageHandler.get());
		if (m_networkContext.m_server != nullptr)
			m_networkContext.m_server->SetMessageHandler(m_networkContext.m_serverMessageHandler.get());
	}

	void ConnectingState::Exit()
	{}

	GameStates::GameStates ConnectingState::Update()
	{
		if (m_networkContext.m_server != nullptr)
			m_networkContext.m_server->Update();
		m_networkContext.m_client->Update();

		if (m_networkContext.m_clientMessageHandler->GetClientState() == RootForce::Network::ClientState::CONNECTED)
			return GameStates::Ingame;
		if (m_networkContext.m_clientMessageHandler->GetClientState() == RootForce::Network::ClientState::CONNECTION_FAILED)
			return GameStates::Menu;
		if (m_networkContext.m_clientMessageHandler->GetClientState() == RootForce::Network::ClientState::CONNECTION_FAILED_TOO_MANY_PLAYERS)
			return GameStates::Menu;
		if (m_networkContext.m_clientMessageHandler->GetClientState() == RootForce::Network::ClientState::CONNECTION_LOST)
			return GameStates::Menu;

		return GameStates::Connecting;
	}
}