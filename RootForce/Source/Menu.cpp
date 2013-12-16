#include "Menu.h"

namespace RootForce
{

	RootForce::Menu::Menu( Awesomium::WebView* p_view )
	{
		m_view = p_view;
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

}

