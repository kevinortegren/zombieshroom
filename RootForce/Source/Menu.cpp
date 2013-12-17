#include "Menu.h"

namespace RootForce
{

	RootForce::Menu::Menu( Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher )
	{
		m_view = p_view;
		
		Awesomium::JSValue result = m_view->CreateGlobalJavascriptObject(Awesomium::WSLit("Menu"));

		if(result.IsObject() && result.ToObject().type() != Awesomium::kJSObjectType_Local)
		{
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Exit"), JSDelegate(this, &Menu::ExitEvent));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Host"), JSDelegate(this, &Menu::HostEvent));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Connect"), JSDelegate(this, &Menu::ConnectEvent));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Refresh"), JSDelegate(this, &Menu::RefreshEvent));
		}
		m_view->set_js_method_handler(p_dispatcher);
		m_view->Focus();
	}

	void RootForce::Menu::Hide()
	{
		m_view->ExecuteJavascript(Awesomium::WSLit("Hide();"), Awesomium::WebString());
		//m_view->PauseRendering(); // Calling this should save some processing power, but it doesn't wait until the view is updated as transparent
	}
	void RootForce::Menu::Unhide()
	{
		m_view->ExecuteJavascript(Awesomium::WSLit("Unhide();"), Awesomium::WebString());
		//m_view->ResumeRendering();
	}
	void RootForce::Menu::AddServer(std::pair<uint64_t,RootSystems::ServerInfoInternal*> p_serverInfo)
	{
		std::string command = "AddServer('";
		command = command + p_serverInfo.second->IP + ":" + std::to_string(p_serverInfo.second->Port) + "','";
		command = command + p_serverInfo.second->Name + "',";
		command = command + std::to_string(p_serverInfo.second->NumPlayers) + ",";
		command = command + std::to_string(p_serverInfo.second->MaxPlayers) + ",";
		command = command + std::to_string(p_serverInfo.first/1000) + ",'";
		command = command + (p_serverInfo.second->PasswordProtected?"Yes":"No") + "');";

		m_view->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WebString());
	}

	MenuEvent::MenuEvent RootForce::Menu::PollEvent()
	{
		MenuEvent::MenuEvent event;
		if(m_event.size() < 1)
		{
			event.type = MenuEvent::EventType::NoEvent;
			return event;
		}
		event = m_event.at(0); 
		m_event.erase(m_event.begin());
		return event;
	}


	void Menu::ExitEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Exit;

		m_event.push_back(event);
	}


	void RootForce::Menu::HostEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Host;
		event.data = p_array;

		m_event.push_back(event);
	}

	void RootForce::Menu::ConnectEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Connect;
		event.data = p_array;

		m_event.push_back(event);
	}


	void Menu::RefreshEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Refresh;
		event.data = p_array;

		m_event.push_back(event);
	}

	const Awesomium::JSValue& Menu::GetMapList(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array)
	{
		return Awesomium::JSValue();
	}

}

