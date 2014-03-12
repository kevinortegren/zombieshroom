#include "SettingsMenu.h"
#include <SDL2/SDL.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/GUI/Include/guiInstance.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootSystems/Include/PlayerSystem.h>

extern ECS::World* g_world;
namespace RootForce
{


	SettingsMenu::SettingsMenu( RootEngine::GameSharedContext p_context, Keymapper* p_keymapper, ChatSystem* p_chatSystem)
		: m_keymapper(p_keymapper)
		, m_chatSystem(p_chatSystem)
	{
		m_context = p_context;
		m_workingDir = m_context.m_resourceManager->GetWorkingDirectory();
		m_fullscreen = m_context.m_configManager->GetConfigValueAsBool("settings-fullscreen");
		std::string resolutionString = m_context.m_configManager->GetConfigValueAsString("settings-resolution");
		int splitPos = resolutionString.find('x');
		m_screenWidth = std::stoi(resolutionString.substr(0, splitPos));
		m_screenHeight = std::stoi(resolutionString.substr(splitPos+1));
		m_shouldUpdate = true;
	}

	SettingsMenu::~SettingsMenu()
	{

	}

	Awesomium::JSValue SettingsMenu::RequestSettingsEvent( const Awesomium::JSArray& p_array )
	{
		// Reload config
		m_context.m_configManager->LoadConfig( m_workingDir + "config.yaml");
		// Update keybinding manager
		auto map = m_context.m_configManager->GetConfigValuePairs();
		for(auto pair : map)
			m_keymapper->SetActionBinding(pair.first, (SDL_Scancode)m_context.m_configManager->GetConfigValueAsInteger(pair.first));

		// Get a list of options
		std::map<std::string, std::string> valuePairs = m_context.m_configManager->GetConfigValuePairs();
		Awesomium::JSObject jsArray;
		for(auto itr = valuePairs.begin(); itr != valuePairs.end(); ++itr)
			jsArray.SetProperty(Awesomium::WSLit(itr->first.c_str()), Awesomium::WSLit(itr->second.c_str()));

		std::string displayModes = "[";
		SDL_DisplayMode displayMode;
		for(int i = 0; i < SDL_GetNumDisplayModes(0); i++)
		{
			SDL_GetDisplayMode(0, i, &displayMode);
			if(displayMode.format == SDL_PIXELFORMAT_RGB888 && displayMode.refresh_rate == 60)
				displayModes = displayModes + (displayModes.size()>1?",":"") + "'" + std::to_string(displayMode.w) + "x" + std::to_string(displayMode.h) + "'";
		}
		displayModes += "]";
		jsArray.SetProperty(Awesomium::WSLit("settings-resolution-list"), Awesomium::WSLit(displayModes.c_str()));

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
			SetValue(Awesomium::ToString(keys[i].ToString()), Awesomium::ToString(map.GetProperty(keys[i].ToString()).ToString()));
		}

		m_context.m_configManager->StoreConfig(m_workingDir + "config.yaml"); // Hardcoding config file is not nice
	}

	void SettingsMenu::FocusBindEvent(const Awesomium::JSArray& p_array)
	{
		m_keymapper->FocusBindAction(Awesomium::ToString(p_array[0].ToString()));
	}

	void SettingsMenu::UnfocusBindEvent(const Awesomium::JSArray& p_array)
	{
		m_keymapper->UnfocusEvent();
	}

	void SettingsMenu::BindEvents( RootEngine::GUISystem::WebView* p_view )
	{
		p_view->RegisterJSCallback("RequestSettings", JSDelegate1WithRetval(this, &SettingsMenu::RequestSettingsEvent));
		p_view->RegisterJSCallback("SaveSettings", JSDelegate1(this, &SettingsMenu::SaveSettingsEvent));
		p_view->RegisterJSCallback("FocusBind", JSDelegate1(this, &SettingsMenu::FocusBindEvent));
		p_view->RegisterJSCallback("UnfocusBind", JSDelegate1(this, &SettingsMenu::UnfocusBindEvent));
	}

	void SettingsMenu::Update()
	{
		if(m_shouldUpdate)
		{
			//Must be called from the main thread
			//SDL_SetWindowFullscreen(SDL_GL_GetCurrentWindow(), false);  //Force disable fullscreen to get the correct resolution setting
			m_context.m_renderer->SetResolution(m_fullscreen, m_screenWidth, m_screenHeight);
			m_context.m_gui->ResizeAllViews(m_context.m_renderer->GetWidth(), m_context.m_renderer->GetHeight());
			m_shouldUpdate = false;
		}
	}

	void SettingsMenu::SetValue(std::string p_key, std::string p_value)
	{
		if(p_key.compare("settings-glow") == 0)
		{
			if(m_chatSystem)
				m_chatSystem->InjectEvent("render glow display " + std::to_string(p_value.compare("true")==0));
		}
		else if(p_key.compare("settings-grass") == 0)
		{
			float factor = 1;
			if(p_value.compare("low") == 0)
				factor = 0.25f;
			else if(p_value.compare("medium") == 0)
				factor = 0.5f;
			if(m_chatSystem)
				m_chatSystem->InjectEvent("botany factor " + std::to_string(factor));
		}
		else if(p_key.compare("settings-shadows") == 0)
		{
			float factor = 1;
			if(p_value.compare("low") == 0)
				factor = 0.25f;
			else if(p_value.compare("medium") == 0)
				factor = 0.5f;
			if(m_chatSystem)
				m_chatSystem->InjectEvent("render shadow " + std::to_string(factor==1));
		}
		else if(p_key.compare("settings-water") == 0)
		{
			float factor = 1;
			if(p_value.compare("low") == 0)
				factor = 0.25f;
			else if(p_value.compare("medium") == 0)
				factor = 0.5f;
			if(m_chatSystem)
				m_chatSystem->InjectEvent("water quality " + std::to_string(factor));
		}
		else if(p_key.compare("settings-fullscreen") == 0)
		{
			m_fullscreen = p_value.compare("true") == 0 ? true : false;
			m_shouldUpdate = true;
		}
		else if(p_key.compare("settings-resolution") == 0)
		{
			int splitPos = p_value.find('x');
			m_screenWidth = std::stoi(p_value.substr(0, splitPos));
			m_screenHeight = std::stoi(p_value.substr(splitPos+1));
			m_shouldUpdate = true;
		}

		m_context.m_configManager->SetConfigValue(
			RootEngine::GUISystem::PreventHTMLInjections(p_key),
			RootEngine::GUISystem::PreventHTMLInjections(p_value)
		);
	}
}