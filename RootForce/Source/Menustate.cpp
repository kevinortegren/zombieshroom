#include <RootForce/Include/MenuState.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;

namespace RootForce
{	
	MenuState::MenuState(NetworkContext& p_networkContext)
		: m_networkContext(p_networkContext)
	{}

	void MenuState::Initialize(const std::string& p_workingDir)
	{
        m_workingDir = p_workingDir;

		// Initialize the LAN-list
		m_lanList = std::shared_ptr<RootSystems::LanList>(new RootSystems::LanList);
	}

	void MenuState::Enter()
	{
		// Allow the mouse to be moved while in the menu
		g_engineContext.m_inputSys->LockMouseToCenter(false);

		// Initialize the menu
		m_menu = std::shared_ptr<Menu>(new Menu(g_engineContext.m_gui->LoadURL("menu.html"), g_engineContext.m_gui->GetDispatcher(), g_engineContext));

		// Reset the menu
        m_menu->LoadDefaults(g_engineContext.m_configManager, m_workingDir);

		// Destroy any existing server/client and setup a new network client so we can search for LAN-servers
		m_networkContext.m_client = std::shared_ptr<RootForce::Network::Client>(new RootForce::Network::Client(g_engineContext.m_logger, g_world));
		m_networkContext.m_server = nullptr;
		m_networkContext.m_clientMessageHandler = std::shared_ptr<RootForce::Network::ClientMessageHandler>(new RootForce::Network::ClientMessageHandler(m_networkContext.m_client->GetPeerInterface(), g_world));
		m_networkContext.m_serverMessageHandler = nullptr;
		m_networkContext.m_networkEntityMap.clear();
		m_networkContext.m_client->SetMessageHandler(m_networkContext.m_clientMessageHandler.get());

		// Set the LAN list on the message handler
		m_networkContext.m_clientMessageHandler->SetLanList(m_lanList.get());
	}

	void MenuState::Exit()
	{
		// Remove the LAN-list from the message handler
		m_networkContext.m_clientMessageHandler->SetLanList(nullptr);

		// Free menu resources
		g_engineContext.m_gui->DestroyView(m_menu->GetView());
		m_menu.reset();
	}

	GameStates::GameStates MenuState::Update()
	{
		g_engineContext.m_gui->Update();

		// Update the client - querying for network messages and updating the message handler.
		m_networkContext.m_client->Update();

		// Fill the LAN list if the message handler has received any server response.
		std::vector<std::pair<uint64_t, RootSystems::ServerInfoInternal>> lanList = m_lanList->GetList();
		for(unsigned int i = 0; i < lanList.size(); i++)
			m_menu->AddServer(lanList.at(i));

		// Handle GUI events
		MenuEvent::MenuEvent event = m_menu->PollEvent();
		GameStates::GameStates result = GameStates::Exit;
		switch (event.type)
		{
			case MenuEvent::EventType::Exit:
			{
				result = GameStates::Exit;
			} break;

			case MenuEvent::EventType::Refresh:
			{
				m_networkContext.m_client->PingNetwork(m_lanList.get(), 5567);
				result = GameStates::Menu;
			} break;

			case MenuEvent::EventType::Host:
			{
				// Retrieve hosting data and go into a connecting state.
				m_playData.Host = true;
				m_playData.ServerInfo.ServerName = Awesomium::ToString(event.data[0].ToString());
				m_playData.ServerInfo.Port = event.data[1].ToInteger();
				m_playData.ServerInfo.Password = Awesomium::ToString(event.data[2].ToString());
				m_playData.ServerInfo.MaxPlayers = event.data[3].ToInteger();
				m_playData.ServerInfo.MatchTime = event.data[4].ToInteger();
				m_playData.ServerInfo.KillCount = event.data[5].ToInteger();
				m_playData.ServerInfo.MapName = Awesomium::ToString(event.data[6].ToString());

				result = GameStates::Connecting;
			} break;

			case MenuEvent::EventType::Connect:
			{
				// Retrieve connection data and go into a connecting state.
				m_playData.Host = false;
				m_playData.ClientInfo.Address = Awesomium::ToString(event.data[1].ToString());
				m_playData.ClientInfo.Password = "";
				m_playData.ClientInfo.Port = event.data[0].ToInteger();

				result = GameStates::Connecting;
			} break;

			default:
			{
				result = GameStates::Menu;
			} break;
		}

		// Render the GUI
		g_engineContext.m_renderer->Clear();
		g_engineContext.m_gui->Render(m_menu->GetView());
		g_engineContext.m_renderer->Swap();

		return result;
	}

	const GameStates::PlayData& MenuState::GetPlayData() const
	{
		return m_playData;
	}
}