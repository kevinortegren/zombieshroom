#include <RootForce/Include/MenuState.h>

namespace RootForce
{
	extern RootEngine::GameSharedContext g_engineContext;
	extern ECS::World* g_world;

	void Menustate::Initialize(RootForce::Network::Client* p_client)
	{
		// Set the menu message handler on the client
		m_client = p_client;
		m_clientMessageHandler = std::shared_ptr<RootForce::Network::ClientMessageHandler>(new RootForce::Network::ClientMessageHandler(m_client->GetPeerInterface(), g_engineContext.m_logger, g_world));
		m_client->SetMessageHandler(m_clientMessageHandler.get());

		// Initialize the LAN-list
		m_lanList = std::shared_ptr<RootSystems::LanList>(new RootSystems::LanList);
		m_clientMessageHandler->SetLanList(m_lanList.get());
	}

	void Menustate::Enter()
	{
		// Initialize menu
		m_menu = std::shared_ptr<Menu>(new Menu(g_engineContext.m_gui->LoadURL("menu.html"), g_engineContext.m_gui->GetDispatcher(), g_engineContext));
	}

	void Menustate::Exit()
	{
		// Free menu resources
		m_menu.reset();
	}

	GameStates::GameStates Menustate::Update()
	{
		g_engineContext.m_gui->Update();

		// Update the client - querying for network messages and updating the message handler.
		m_client->Update();

		// Fill the LAN list if the message handler has received any server response.
		std::vector<std::pair<uint64_t, RootSystems::ServerInfoInternal>> lanList = m_lanList->GetList();
		for(int i = 0; i < lanList.size(); i++)
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
				m_client->PingNetwork(m_lanList.get(), 5567);
				result = GameStates::Menu;
			} break;

			case MenuEvent::EventType::Host:
			{
				m_playData.Host = true;
				m_playData.p_port = event.data[0].ToInteger();
				m_menu->Hide();
				result = GameStates::Ingame;
			} break;

			case MenuEvent::EventType::Connect:
			{
				m_playData.Host = false;
				m_playData.p_port = event.data[0].ToInteger();
				m_playData.p_address = Awesomium::ToString(event.data[1].ToString());
				m_menu->Hide();
				result = GameStates::Ingame;
			} break;

			default:
			{
				m_menu->Unhide();
				result = GameStates::Menu;
			} break;
		}

		// Render the GUI
		g_engineContext.m_renderer->Clear();

		g_engineContext.m_gui->Render();

		g_engineContext.m_renderer->Swap();

		return result;
	}
}