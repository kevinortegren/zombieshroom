#include <RootForce/Include/ConnectingState.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Network/ServerConfig.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;

namespace RootForce
{
	ConnectingState::ConnectingState(NetworkContext& p_networkContext, SharedSystems& p_sharedSystems)
		: m_networkContext(p_networkContext)
		, m_sharedSystems(p_sharedSystems),
		m_loadingScreen(nullptr)
	{}

	ConnectingState::~ConnectingState()
	{
	}

	void ConnectingState::Initialize()
	{
		m_sharedSystems.m_worldSystem = std::shared_ptr<RootForce::WorldSystem>(new RootForce::WorldSystem(g_world, &g_engineContext));
		m_loadingScreen = g_engineContext.m_gui->LoadURL("loading.html");
	}

	void ConnectingState::Enter(const GameStates::PlayData& p_playData)
	{
		// Render a frame of loading screen to indicate the loading state
		while(m_loadingScreen->IsLoading())
			g_engineContext.m_gui->Update();
		g_engineContext.m_renderer->Clear();
		g_engineContext.m_gui->Render(m_loadingScreen);
		g_engineContext.m_renderer->Swap();
        
		// Host
		if (p_playData.Host)
		{
			// Setup the server and connect a local client
			m_networkContext.m_server = std::shared_ptr<RootForce::Network::Server>(new RootForce::Network::Server(g_engineContext.m_logger, g_world, p_playData.ServerInfo, false));
			m_networkContext.m_serverMessageHandler = std::shared_ptr<RootForce::Network::ServerMessageHandler>(new RootForce::Network::ServerMessageHandler(m_networkContext.m_server->GetPeerInterface(), g_world));
			m_networkContext.m_serverMessageHandler->SetNetworkEntityMap(&m_networkContext.m_networkEntityMap);
			m_networkContext.m_server->SetMessageHandler(m_networkContext.m_serverMessageHandler.get());
			m_networkContext.m_client->Connect("127.0.0.1", p_playData.ServerInfo.Password, p_playData.ServerInfo.Port, false);

			// Setup the rules
			ECS::Entity* entity = g_world->GetEntityManager()->CreateEntity();
			g_world->GetTagManager()->RegisterEntity("MatchState", entity);
			RootForce::TDMRuleSet* rules = g_world->GetEntityManager()->CreateComponent<RootForce::TDMRuleSet>(entity);
			rules->ScoreLimit = p_playData.ServerInfo.KillCount;
			rules->TimeLeft = (float)p_playData.ServerInfo.MatchTime;
			rules->TeamScore[1] = 0;
			rules->TeamScore[2] = 0;
		}
		else
		{
			// Connect the client
			m_networkContext.m_client->Connect(p_playData.ClientInfo.Address, p_playData.ClientInfo.Password, p_playData.ClientInfo.Port, true);

			// Setup the rules
			ECS::Entity* ruleEntity = g_world->GetEntityManager()->CreateEntity();
			g_world->GetTagManager()->RegisterEntity("MatchState", ruleEntity);
			RootForce::TDMRuleSet* rules = g_world->GetEntityManager()->CreateComponent<RootForce::TDMRuleSet>(ruleEntity);
			rules->ScoreLimit = 20; //TODO: this will be sent through the  gameState snapshot once this has been implemented
			rules->TimeLeft = 20;   // hardcoded for now so that the game does not crash
			rules->TeamScore[1] = 0;
			rules->TeamScore[2] = 0;
		}

		// Reset the network entity map
		m_networkContext.m_networkEntityMap.clear();

		// Setup the client
		m_networkContext.m_clientMessageHandler->SetNetworkEntityMap(&m_networkContext.m_networkEntityMap);
		m_networkContext.m_clientMessageHandler->SetWorldSystem(m_sharedSystems.m_worldSystem.get());
	}

	void ConnectingState::Exit()
	{
	}

	GameStates::GameStates ConnectingState::Update()
	{
		if (m_networkContext.m_server != nullptr)
			m_networkContext.m_server->Update();
		m_networkContext.m_client->Update();

		ECS::Entity* clientEntity = g_world->GetTagManager()->GetEntityByTag("Client");
		Network::ClientComponent* clientComponent = g_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);
		if (clientComponent->State == RootForce::Network::ClientState::CONNECTED)
			return GameStates::Ingame;
		if (clientComponent->State == RootForce::Network::ClientState::AWAITING_SERVER_INFO)
			return GameStates::Connecting;
		if (clientComponent->State == RootForce::Network::ClientState::AWAITING_USER_CONNECT)
			return GameStates::Connecting;
		if (clientComponent->State == RootForce::Network::ClientState::AWAITING_FIRST_GAMESTATE_DELTA)
			return GameStates::Connecting;
		if (clientComponent->State == RootForce::Network::ClientState::DISCONNECTED_TIMEOUT)
			return GameStates::Menu;
		if (clientComponent->State == RootForce::Network::ClientState::DISCONNECTED_REFUSED)
			return GameStates::Menu;
		if (clientComponent->State == RootForce::Network::ClientState::DISCONNECTED_REFUSED_TOO_MANY_PLAYERS)
			return GameStates::Menu;
		if (clientComponent->State == RootForce::Network::ClientState::DISCONNECTED_SERVER_SHUTDOWN)
			return GameStates::Menu;

		return GameStates::Connecting;
	}
}