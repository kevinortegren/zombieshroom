#pragma once
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <vector>
#include <RootEngine/GUI/Include/Dispatcher.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootEngine/Include/ConfigManager.h>
#include <RootEngine\Include\GameSharedContext.h>

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
		Menu(Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, RootEngine::GameSharedContext p_context);
		~Menu();
		MenuEvent::MenuEvent PollEvent();
		Awesomium::WebView* GetView();

		void AddServer(const std::pair<uint64_t,RootSystems::ServerInfoInternal>& p_serverInfo);
        void LoadDefaults(RootEngine::ConfigManagerInterface* p_configMan, std::string p_workingDir);
	private:
		void ExitEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void HostEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void ConnectEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void RefreshEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		Awesomium::JSValue GetMapListEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);

		std::string GetMapList();

		Awesomium::WebView* m_view;
		std::vector<MenuEvent::MenuEvent> m_event;
		RootEngine::GameSharedContext m_context;
		std::string m_workingDir;
	};
}