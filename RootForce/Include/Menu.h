#pragma once
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <vector>
#include <RootEngine/GUI/Include/WebView.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootEngine/Include/ConfigManager.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootForce/Include/SettingsMenu.h>

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
		Menu(RootEngine::GUISystem::WebView* p_view, RootEngine::GameSharedContext p_context);
		~Menu();
		MenuEvent::MenuEvent PollEvent();
		RootEngine::GUISystem::WebView* GetView();

		void AddServer(const std::pair<uint64_t,RootSystems::ServerInfoInternal>& p_serverInfo);
        void LoadDefaults(RootEngine::ConfigManagerInterface* p_configMan, std::string p_workingDir);
        void ShowError(std::string p_errorMessage, std::string p_errorTitle = "Error");
	private:
		void ExitEvent(const Awesomium::JSArray& p_array);
		void HostEvent(const Awesomium::JSArray& p_array);
		void ConnectEvent(const Awesomium::JSArray& p_array);
		void RefreshEvent(const Awesomium::JSArray& p_array);
		Awesomium::JSValue GetMapListEvent(const Awesomium::JSArray& p_array);
		void SaveSettingsEvent(const Awesomium::JSArray& p_array);

		std::string GetMapList();
		SettingsMenu* m_settingsMenu;

		RootEngine::GUISystem::WebView* m_view;
		std::vector<MenuEvent::MenuEvent> m_event;
		RootEngine::GameSharedContext m_context;
		std::string m_workingDir;
	};
}