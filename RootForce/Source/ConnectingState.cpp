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
		m_loadingScreen = g_engineContext.m_gui->LoadURL("Loading", "loading.html");
	}

	void ConnectingState::Enter(const GameStates::PlayData& p_playData)
	{
		// Render a frame of loading screen to indicate the loading state
		m_loadingScreen->WaitLoading();
		g_engineContext.m_renderer->Clear();
		g_engineContext.m_gui->Render(m_loadingScreen);
		g_engineContext.m_renderer->Swap();
		
		// Create a server information entity. Used for local information on a local server and used to store server info response on a remote client.
		ECS::Entity* serverInfoEntity = g_world->GetEntityManager()->CreateEntity();
		g_world->GetEntityManager()->CreateComponent<Network::ServerInformationComponent>(serverInfoEntity);
		g_world->GetTagManager()->RegisterEntity("ServerInformation", serverInfoEntity);
        
		// Destroy any existing network entities.
		Network::DeleteEntities(g_networkEntityMap, Network::NetworkEntityID(Network::ReservedUserID::ALL, Network::ReservedActionID::ALL, Network::ReservedSequenceID::ALL), g_world->GetEntityManager()); 
		g_networkEntityMap.clear();
		Network::NetworkComponent::s_sequenceIDMap.clear();

		// Create the match state which will keep track of the rules.
		g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("MatchState"), "OnCreate");
		g_engineContext.m_script->AddParameterNumber(Network::ReservedUserID::NONE);
		g_engineContext.m_script->AddParameterNumber(Network::ReservedActionID::NONE);
		g_engineContext.m_script->ExecuteScript();

		ECS::Entity* matchStateEntity = g_world->GetTagManager()->GetEntityByTag("MatchState");
		RootForce::TDMRuleSet* rules = g_world->GetEntityManager()->GetComponent<RootForce::TDMRuleSet>(matchStateEntity);
		
		// Create a new client.
		m_networkContext.m_client = std::shared_ptr<RootForce::Network::Client>(new RootForce::Network::Client(g_engineContext.m_logger, g_world));
		m_networkContext.m_clientMessageHandler = std::shared_ptr<RootForce::Network::ClientMessageHandler>(new RootForce::Network::ClientMessageHandler(m_networkContext.m_client->GetPeerInterface(), g_world));
		m_networkContext.m_client->SetMessageHandler(m_networkContext.m_clientMessageHandler.get());

		if (p_playData.Host)
		{
			// Setup the server and connect a local client.
			m_networkContext.m_server = std::shared_ptr<RootForce::Network::Server>(new RootForce::Network::Server(g_engineContext.m_logger, g_world, p_playData.ServerInfo));
			m_networkContext.m_serverMessageHandler = std::shared_ptr<RootForce::Network::ServerMessageHandler>(new RootForce::Network::ServerMessageHandler(m_networkContext.m_server->GetPeerInterface(), g_world));
			
			m_networkContext.m_server->Initialize(m_sharedSystems.m_worldSystem.get(), p_playData.ServerInfo, false);
			m_networkContext.m_server->SetMessageHandler(m_networkContext.m_serverMessageHandler.get());
			m_networkContext.m_client->Connect("127.0.0.1", p_playData.ServerInfo.Password, p_playData.ServerInfo.Port, false);

			// Setup the rules.
			rules->ScoreLimit = p_playData.ServerInfo.KillCount;
			rules->TimeLeft = (float)p_playData.ServerInfo.MatchTime;

			// Set the server peer on the respawn system if we are hosting.
			m_sharedSystems.m_respawnSystem->SetServerPeer(m_networkContext.m_server->GetPeerInterface());
		}
		else
		{
			// Connect the client.
			m_networkContext.m_client->Connect(p_playData.ClientInfo.Address, p_playData.ClientInfo.Password, p_playData.ClientInfo.Port, true);

			// Set the server peer to null on the respawn system, since we're a dedicated client.
			m_sharedSystems.m_respawnSystem->SetServerPeer(nullptr);

			// Set the world system on the client, so that it can load a level when receiving the level name from the server.
			m_networkContext.m_clientMessageHandler->SetWorldSystem(m_sharedSystems.m_worldSystem.get());
		}

		// Always set the client peer on the respawning system (this will only not be set for a dedicated server).
		m_sharedSystems.m_respawnSystem->SetClientPeer(m_networkContext.m_client->GetPeerInterface());
	}

	void ConnectingState::Exit()
	{
	}

	GameStates::GameStates ConnectingState::Update()
	{
		if (m_networkContext.m_server != nullptr)
			m_networkContext.m_server->Update();
		m_networkContext.m_client->Update();

		m_sharedSystems.m_respawnSystem->Process();

		ECS::Entity* clientEntity = g_world->GetTagManager()->GetEntityByTag("Client");
		Network::ClientComponent* clientComponent = g_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);
		
		assert(!Network::ClientState::IsUnconnected(clientComponent->State));
		if (Network::ClientState::IsConnected(clientComponent->State))
			return GameStates::Ingame;
		if (Network::ClientState::IsConnecting(clientComponent->State))
			return GameStates::Connecting;
		if (Network::ClientState::IsDisconnected(clientComponent->State))
			return GameStates::Menu;

		return GameStates::Connecting;
	}
}