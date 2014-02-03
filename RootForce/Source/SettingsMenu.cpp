#include "SettingsMenu.h"

namespace RootForce
{


	SettingsMenu::SettingsMenu( Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, RootEngine::GameSharedContext p_context )
	{
		m_view = p_view;
		m_context = p_context;
	}

	SettingsMenu::~SettingsMenu()
	{

	}

	Awesomium::JSValue SettingsMenu::RequestSettingsEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		// Get a list of options
		std::map<std::string, std::string> valuePairs = m_context.m_configManager->GetConfigValuePairs();
		Awesomium::JSObject jsArray;
		for(auto itr = valuePairs.begin(); itr != valuePairs.end(); ++itr)
			jsArray.SetProperty(Awesomium::WSLit(itr->first.c_str()), Awesomium::WSLit(itr->second.c_str()));

		return Awesomium::JSValue(jsArray);
	}

	void SettingsMenu::SaveSettingsEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		if(!p_array[0].IsObject())
		{
			//m_context.m_logger->LogText(LogTag::GUI, LogLevel::WARNING, "JavaScript called SaveSettings with a non-object argument.");
			return;
		}

		// Parse array, save all values
		Awesomium::JSObject map = p_array[0].ToObject();
		Awesomium::JSArray keys = map.GetPropertyNames();
		for(unsigned i = 0; i < keys.size(); i++)
			m_context.m_configManager->SetConfigValue(
			Awesomium::ToString(keys[i].ToString()),
			Awesomium::ToString(map.GetProperty(keys[i].ToString()).ToString())
			);

		m_context.m_configManager->StoreConfig("config.yaml"); // Hardcoding config file is not nice
	}

}