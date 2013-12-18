#include "Menustate.h"

namespace RootForce
{

	Menustate::Menustate()
	{
	}


	void Menustate::Initialize(RootEngine::GameSharedContext* p_engineContext, RootForce::Network::Client* p_client, RootForce::Network::ClientMessageHandler* p_clientMessageHandler)
	{
		m_engineContext = p_engineContext;
		m_client = p_client;

		//Initiate Gui
		m_engineContext->m_gui->Initialize(m_engineContext->m_configManager->GetConfigValueAsInteger("ScreenWidth"),
			m_engineContext->m_configManager->GetConfigValueAsInteger("ScreenHeight"));

		//Initiate Menu
		m_menu = std::shared_ptr<Menu>(new Menu(p_engineContext->m_gui->LoadURL("menu.html"), p_engineContext->m_gui->GetDispatcher(), *m_engineContext));

		// Initialize the LAN-list
		m_lanList = std::shared_ptr<RootSystems::LanList>(new RootSystems::LanList);
		p_clientMessageHandler->SetLanList(m_lanList.get());
	}

	GameStates::GameStates Menustate::Update()
	{
		m_engineContext->m_gui->Update();

		MenuEvent::MenuEvent event = m_menu->PollEvent();

		m_client->Update();

		std::vector<std::pair<uint64_t, RootSystems::ServerInfoInternal>> lanList = m_lanList->GetList();
		for(int i = 0; i < lanList.size(); i++)
			m_menu->AddServer(lanList.at(i));

		GameStates::GameStates result = GameStates::Exit;
		switch (event.type)
		{
			case MenuEvent::EventType::Exit:
				result = GameStates::Exit;
				break;
			case MenuEvent::EventType::Refresh:
				m_client->PingNetwork(m_lanList.get(), 5567);
				result = GameStates::Menu;
				break;
			case MenuEvent::EventType::Host:
				m_playData.Host = true;
				m_playData.p_port = event.data[0].ToInteger();
				m_menu->Hide();
				result = GameStates::Ingame;
				break;
			case MenuEvent::EventType::Connect:
				m_playData.Host = false;
				m_playData.p_port = event.data[0].ToInteger();
				m_playData.p_address = Awesomium::ToString(event.data[1].ToString());
				m_menu->Hide();
				result = GameStates::Ingame;
				break;
			default:
				m_menu->Unhide();
				result = GameStates::Menu;
				break;
		}

		m_engineContext->m_renderer->Clear();

		m_engineContext->m_gui->Render();

		m_engineContext->m_renderer->Swap();

		return result;
	}
}