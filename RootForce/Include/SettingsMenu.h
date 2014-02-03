#pragma once
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <RootEngine/GUI/Include/Dispatcher.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootEngine/Include/ConfigManager.h>
#include <RootEngine/Include/GameSharedContext.h>

namespace RootForce
{
	class SettingsMenu
	{
	public:
		SettingsMenu(Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, RootEngine::GameSharedContext p_context);
		~SettingsMenu();
		Awesomium::WebView* GetView() { return m_view; }
	private:
		Awesomium::JSValue RequestSettingsEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void SaveSettingsEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);

		Awesomium::WebView* m_view;
		RootEngine::GameSharedContext m_context;
		std::string m_workingDir;

	};
}