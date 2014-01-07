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

        // Initialize the menu
        m_menu = std::shared_ptr<Menu>(new Menu(g_engineContext.m_gui->LoadURL("menu.html"), g_engineContext.m_gui->GetDispatcher(), g_engineContext));

		// Initialize the LAN-list
		m_lanList = std::shared_ptr<RootSystems::LanList>(new RootSystems::LanList);
	}

	void MenuState::Enter()
	{
		// Allow the mouse to be moved while in the menu
		g_engineContext.m_inputSys->LockMouseToCenter(false);

		// Reset the menu
        m_menu->LoadDefaults(g_engineContext.m_configManager, m_workingDir);

		// Destroy any existing server
		m_networkContext.m_server.reset();

		// Set the LAN list on the message handler
		m_networkContext.m_clientMessageHandler->SetLanList(m_lanList.get());
	}

	void MenuState::Exit()
	{
		// Remove the LAN-list from the message handler
		m_networkContext.m_clientMessageHandler->SetLanList(nullptr);

		// Free menu resources
		//m_menu.reset();
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
				m_playData.ServerName = Awesomium::ToString(event.data[0].ToString());
				m_playData.Port = event.data[1].ToInteger();
				m_playData.Password = Awesomium::ToString(event.data[2].ToString());
				m_playData.MaxPlayers = event.data[3].ToInteger();
				m_playData.MatchLength = event.data[4].ToInteger();
				m_playData.Killcount = event.data[5].ToInteger();

				m_menu->Hide();
				result = GameStates::Connecting;
			} break;

			case MenuEvent::EventType::Connect:
			{
				// Retrieve connection data and go into a connecting state.
				m_playData.Host = false;
				m_playData.Address = Awesomium::ToString(event.data[1].ToString());
				m_playData.Port = event.data[0].ToInteger();

				m_menu->Hide();
				result = GameStates::Connecting;
			} break;

			default:
			{
				m_menu->Unhide();
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