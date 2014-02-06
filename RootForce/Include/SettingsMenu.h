#pragma once
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <RootEngine/GUI/Include/Dispatcher.h>
#include <RootEngine/GUI/Include/WebView.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootEngine/Include/ConfigManager.h>
#include <RootEngine/Include/GameSharedContext.h>


namespace RootForce
{
	class SettingsMenu
	{
	public:
		SettingsMenu(RootEngine::GameSharedContext p_context);
		~SettingsMenu();
		void BindEvents(RootEngine::GUISystem::WebView* p_view);
		void Update();
	private:
		Awesomium::JSValue RequestSettingsEvent(const Awesomium::JSArray& p_array);
		void SaveSettingsEvent(const Awesomium::JSArray& p_array);

		RootEngine::GameSharedContext m_context;
		std::string m_workingDir;
		bool m_fullscreen;

	};
}