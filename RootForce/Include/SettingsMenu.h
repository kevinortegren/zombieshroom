#pragma once
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <RootEngine/GUI/Include/Dispatcher.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootEngine/Include/ConfigManager.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <SDL2/SDL.h>

namespace RootForce
{
	class SettingsMenu
	{
	public:
		SettingsMenu(RootEngine::GameSharedContext p_context);
		~SettingsMenu();
		void BindEvents(Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, Awesomium::JSValue p_result);
	private:
		Awesomium::JSValue RequestSettingsEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void SaveSettingsEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);


		RootEngine::GameSharedContext m_context;
		std::string m_workingDir;
		SDL_Window* m_window;

	};
}