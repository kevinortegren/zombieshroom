#pragma once
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <RootEngine/GUI/Include/Dispatcher.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootEngine/Include/ConfigManager.h>
#include <RootEngine\Include\GameSharedContext.h>
#include <RootForce/Include/SettingsMenu.h>

namespace RootForce
{
	class IngameMenu
	{
	public:
		IngameMenu(Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, RootEngine::GameSharedContext p_context);
		~IngameMenu();
		Awesomium::WebView* GetView() { return m_view; }
		bool GetReturn() { return m_return; }
		bool GetExit() { return m_exit; }
		void Reset();
	private:
		void Return(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void Exit(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);

		SettingsMenu* m_settingsMenu;
		Awesomium::WebView* m_view;

		bool m_return;
		bool m_exit;
	};
}