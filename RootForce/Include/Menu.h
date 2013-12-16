#pragma once
#include <Awesomium/WebCore.h>
#include <vector>

namespace RootForce
{
	namespace MenuEvent
	{
		enum EventType
		{
			Host,
			Connect,
			NoEvent
		};

		struct MenuEvent
		{
			EventType type;
			Awesomium::JSArray data;
		};
	}

	class Menu
	{
	public:
		Menu(Awesomium::WebView* p_view);
		MenuEvent::MenuEvent PollEvent();
	private:
		void HostEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void ConnectEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);

		Awesomium::WebView* m_view;
		std::vector<MenuEvent::MenuEvent> m_event;
	};
}