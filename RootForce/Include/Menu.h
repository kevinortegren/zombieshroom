#pragma once
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <vector>
#include <RootEngine/GUI/Include/Dispatcher.h>
#include <RootSystems/Include/Network/ServerInfo.h>

namespace RootForce
{
	namespace MenuEvent
	{
		enum EventType
		{
			Exit,
			Host,
			Connect,
			Refresh,
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
		Menu(Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher);
		MenuEvent::MenuEvent PollEvent();
		void Hide();
		void Unhide();
		void AddServer(std::pair<uint64_t,RootSystems::ServerInfoInternal*> p_serverInfo);
	private:
		void ExitEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void HostEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void ConnectEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void RefreshEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		const Awesomium::JSValue& GetMapList(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);

		Awesomium::WebView* m_view;
		std::vector<MenuEvent::MenuEvent> m_event;
	};
}