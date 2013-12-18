#include "Menustate.h"

namespace RootForce
{

	Menustate::Menustate()
	{
	}


	void Menustate::Initialize(RootEngine::GameSharedContext* p_engineContext, std::shared_ptr<RootForce::Network::MessageHandler> p_networkHandler)
	{
		m_engineContext = p_engineContext;
		m_networkHandler = p_networkHandler;
		//Initiate Gui
		m_engineContext->m_gui->Initialize(m_engineContext->m_configManager->GetConfigValueAsInteger("ScreenWidth"),
			m_engineContext->m_configManager->GetConfigValueAsInteger("ScreenHeight"));

		//Initiate Menu
		m_menu = std::shared_ptr<Menu>(new Menu(p_engineContext->m_gui->LoadURL("menu.html"), p_engineContext->m_gui->GetDispatcher()));
	}

	GameStates::GameStates Menustate::Update()
	{
		m_engineContext->m_gui->Update();

		MenuEvent::MenuEvent event = m_menu->PollEvent();

		std::vector<std::pair<uint64_t,RootSystems::ServerInfoInternal*>> lanList = m_networkHandler->GetLanList()->GetList();
		for(int i = 0; i < lanList.size(); i++)
			m_menu->AddServer(lanList.at(i));

		switch (event.type)
		{
		case MenuEvent::EventType::Exit:
			return GameStates::Exit;
			break;
		case MenuEvent::EventType::Refresh:
			m_networkHandler->PingNetwork(5567);
			return GameStates::Menu;
			break;
		case MenuEvent::EventType::Host:
			m_playData.Host = true;
			m_playData.p_port = event.data[0].IsInteger();
			m_menu->Hide();
			return GameStates::Ingame;
			break;
		case MenuEvent::EventType::Connect:
			m_playData.Host = false;
			m_playData.p_port = event.data[0].IsInteger();
			m_playData.p_address = event.data[1].IsString();
			m_menu->Hide();
			return GameStates::Ingame;
			break;
		default:
			m_menu->Unhide();
			return GameStates::Menu;
			break;
		}
	}

	void Menustate::Render()
	{
		m_engineContext->m_renderer->Clear();

		m_engineContext->m_gui->Render();

		m_engineContext->m_renderer->Swap();
	}

}