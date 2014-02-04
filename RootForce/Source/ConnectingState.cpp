#include <RootForce/Include/ConnectingState.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Network/ServerConfig.h>
#include <RootSystems/Include/Script.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/GUI/Include/guiInstance.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;
extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

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
		
		ECS::Entity* serverInfoEntity = g_world->GetEntityManager()->CreateEntity();
		g_world->GetEntityManager()->CreateComponent<Network::ServerInformationComponent>(serverInfoEntity);
		g_world->GetTagManager()->RegisterEntity("ServerInformation", serverInfoEntity);
        

		// Reset the network entity map
		g_networkEntityMap.clear();

		g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("MatchState"), "OnCreate");
		g_engineContext.m_script->AddParameterNumber(Network::ReservedUserID::NONE);
		g_engineContext.m_script->AddParameterNumber(Network::ReservedActionID::NONE);
		g_engineContext.m_script->ExecuteScript();

		ECS::Entity* matchStateEntity = g_world->GetTagManager()->GetEntityByTag("MatchState");
		RootForce::TDMRuleSet* rules = g_world->GetEntityManager()->GetComponent<RootForce::TDMRuleSet>(matchStateEntity);

		m_networkContext.m_client = std::shared_ptr<RootForce::Network::Client>(new RootForce::Network::Client(g_engineContext.m_logger, g_world));
		m_networkContext.m_clientMessageHandler = std::shared_ptr<RootForce::Network::ClientMessageHandler>(new RootForce::Network::ClientMessageHandler(m_networkContext.m_client->GetPeerInterface(), g_world));
		m_networkContext.m_client->SetMessageHandler(m_networkContext.m_clientMessageHandler.get());

		// Host
		if (p_playData.Host)
		{
			if(!m_networkContext.m_server)
			{
				// Setup the server and connect a local client
				m_networkContext.m_server = std::shared_ptr<RootForce::Network::Server>(new RootForce::Network::Server(g_engineContext.m_logger, g_world, m_sharedSystems.m_worldSystem.get(), p_playData.ServerInfo, false));
				m_networkContext.m_serverMessageHandler = std::shared_ptr<RootForce::Network::ServerMessageHandler>(new RootForce::Network::ServerMessageHandler(m_networkContext.m_server->GetPeerInterface(), g_world));
				m_networkContext.m_server->SetMessageHandler(m_networkContext.m_serverMessageHandler.get());
				m_networkContext.m_client->Connect("127.0.0.1", p_playData.ServerInfo.Password, p_playData.ServerInfo.Port, false);
			}

			// Setup the rules
			rules->ScoreLimit = p_playData.ServerInfo.KillCount;
			rules->TimeLeft = (float)p_playData.ServerInfo.MatchTime;
		}
		else
		{
			// Connect the client
			m_networkContext.m_client->Connect(p_playData.ClientInfo.Address, p_playData.ClientInfo.Password, p_playData.ClientInfo.Port, true);
		}

		// Setup the client
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