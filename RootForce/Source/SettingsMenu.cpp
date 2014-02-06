#include "SettingsMenu.h"
#include <SDL2/SDL.h>
#include <RootEngine/Include/Logging/Logging.h>
namespace RootForce
{


	SettingsMenu::SettingsMenu( RootEngine::GameSharedContext p_context )
	{
		m_context = p_context;
		m_fullscreen = m_context.m_configManager->GetConfigValueAsBool("settings-fullscreen");
	}

	SettingsMenu::~SettingsMenu()
	{

	}

	Awesomium::JSValue SettingsMenu::RequestSettingsEvent( const Awesomium::JSArray& p_array )
	{
		// Get a list of options
		std::map<std::string, std::string> valuePairs = m_context.m_configManager->GetConfigValuePairs();
		Awesomium::JSObject jsArray;
		for(auto itr = valuePairs.begin(); itr != valuePairs.end(); ++itr)
			jsArray.SetProperty(Awesomium::WSLit(itr->first.c_str()), Awesomium::WSLit(itr->second.c_str()));

		return Awesomium::JSValue(jsArray);
	}

	void SettingsMenu::SaveSettingsEvent( const Awesomium::JSArray& p_array )
	{
		if(!p_array[0].IsObject())
		{
			m_context.m_logger->LogText(LogTag::GUI, LogLevel::WARNING, "JavaScript called SaveSettings with a non-object argument.");
			return;
		}

		// Parse array, save all values
		Awesomium::JSObject map = p_array[0].ToObject();
		Awesomium::JSArray keys = map.GetPropertyNames();
		for(unsigned i = 0; i < keys.size(); i++)
		{
			if(Awesomium::ToString(keys[i].ToString()).compare("settings-fullscreen") == 0)
			{
				m_fullscreen = Awesomium::ToString(map.GetProperty(keys[i].ToString()).ToString()).compare("true") == 0 ? true : false;
			}
			m_context.m_configManager->SetConfigValue(
				Awesomium::ToString(keys[i].ToString()),
				Awesomium::ToString(map.GetProperty(keys[i].ToString()).ToString())
			);
		}

		m_context.m_configManager->StoreConfig("config.yaml"); // Hardcoding config file is not nice
	}

	void SettingsMenu::BindEvents( RootEngine::GUISystem::WebView* p_view )
	{
		p_view->RegisterJSCallback("RequestSettings", JSDelegate1WithRetval(this, &SettingsMenu::RequestSettingsEvent));
		p_view->RegisterJSCallback("SaveSettings", JSDelegate1(this, &SettingsMenu::SaveSettingsEvent));
	}

	void SettingsMenu::Update()
	{
		//Must be called from the main thread
		SDL_SetWindowFullscreen(SDL_GL_GetCurrentWindow(), m_fullscreen);
	}

}