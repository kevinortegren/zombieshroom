#include <RootForce/Include/MenuState.h>

namespace RootForce
{
	extern RootEngine::GameSharedContext g_engineContext;
	extern ECS::World* g_world;


	ClientMenuMessageHandler::ClientMenuMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger, RootSystems::LanList* p_lanList)
		: RootForce::Network::MessageHandler(p_peer, p_logger)
		, m_lanList(p_lanList) {}

	bool ClientMenuMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
	{
		switch (p_id)
		{
			case ID_UNCONNECTED_PONG:
			{
				p_bs->IgnoreBytes(4); // Timestamp

				RootSystems::ServerInfo info;
				info.Serialize(false, p_bs);

				RootSystems::ServerInfoInternal internalInfo;
				internalInfo.SetInfo(info, p_packet);

				m_lanList->AddServer(internalInfo);
			} return true;
		}

		return false;
	}


	MenuState::MenuState(std::shared_ptr<RootForce::Network::Client>& p_client, std::shared_ptr<RootForce::Network::Server>& p_server)
		: m_client(p_client)
		, m_server(p_server)
	{}

	void MenuState::Initialize(const std::string& p_workingDir)
	{
        m_workingDir = p_workingDir;

        // Initialize the menu
        m_menu = std::shared_ptr<Menu>(new Menu(g_engineContext.m_gui->LoadURL("menu.html"), g_engineContext.m_gui->GetDispatcher(), g_engineContext));
        
		// Initialize the LAN-list
		m_lanList = std::shared_ptr<RootSystems::LanList>(new RootSystems::LanList);

		// Create a network message handler for the menu
		m_menuMessageHandler = std::shared_ptr<RootForce::ClientMenuMessageHandler>(new RootForce::ClientMenuMessageHandler(m_client->GetPeerInterface(), g_engineContext.m_logger, m_lanList.get()));
	}

	void MenuState::Enter()
	{
		// Allow the mouse to be moved while in the menu
		g_engineContext.m_inputSys->LockMouseToCenter(false);

		// Reset the menu
        m_menu->LoadDefaults(g_engineContext.m_configManager, m_workingDir);

		// Destroy any existing server
		m_server.reset();

		// Set the menu message handler on the client
		m_client->SetMessageHandler(m_menuMessageHandler.get());
	}

	void MenuState::Exit()
	{
		// Free menu resources
		m_menu.reset();
	}

	GameStates::GameStates MenuState::Update()
	{
		g_engineContext.m_gui->Update();

		// Update the client - querying for network messages and updating the message handler.
		m_client->Update();

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
				m_client->PingNetwork(m_lanList.get(), 5567);
				result = GameStates::Menu;
			} break;

			case MenuEvent::EventType::Host:
			{
				// Host a server and go into connecting state, waiting for the local client to successfully connect.
				m_server = std::shared_ptr<RootForce::Network::Server>(new RootForce::Network::Server(g_engineContext.m_logger, "Local Server", event.data[0].ToInteger(), 12));
				m_client->Connect("127.0.0.1", event.data[0].ToInteger());

				m_menu->Hide();
				result = GameStates::Connecting;
			} break;

			case MenuEvent::EventType::Connect:
			{
				// Connect to a server and go into a connecting state, waiting for an accept from the server.
				m_client->Connect(Awesomium::ToString(event.data[1].ToString()).c_str(), event.data[0].ToInteger());

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
		g_engineContext.m_gui->Render();
		g_engineContext.m_renderer->Swap();

		return result;
	}
}