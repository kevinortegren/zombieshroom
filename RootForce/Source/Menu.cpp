#include "Menu.h"

#include <External/Include/dirent/dirent.h>

namespace RootForce
{

	RootForce::Menu::Menu( Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, RootEngine::GameSharedContext p_context )
	{
		m_view = p_view;
		m_context = p_context;
		/*while(m_view->IsLoading())
		{

		}*/

		Awesomium::JSValue result = m_view->CreateGlobalJavascriptObject(Awesomium::WSLit("Menu"));

		if(result.IsObject() && result.ToObject().type() != Awesomium::kJSObjectType_Local)
		{
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Exit"), JSDelegate(this, &Menu::ExitEvent));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Host"), JSDelegate(this, &Menu::HostEvent));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Connect"), JSDelegate(this, &Menu::ConnectEvent));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Refresh"), JSDelegate(this, &Menu::RefreshEvent));
			p_dispatcher->BindWithRetVal(result.ToObject(), Awesomium::WSLit("RequestSettings"), JSDelegateWithRetval(this, &Menu::RequestSettingsEvent));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("SaveSettings"), JSDelegate(this, &Menu::SaveSettingsEvent));
		}
		m_view->set_js_method_handler(p_dispatcher);
		m_view->Focus();
	}

	Menu::~Menu()
	{
	}

	Awesomium::WebView* RootForce::Menu::GetView()
	{
		return m_view;
	}
	void RootForce::Menu::AddServer(const std::pair<uint64_t, RootSystems::ServerInfoInternal>& p_serverInfo)
	{
		std::string command = "AddServer(";
		command = command + "'" + p_serverInfo.second.IP + ":" + std::to_string(p_serverInfo.second.Port) + "',";
		command = command + "'" + p_serverInfo.second.Information.ServerName.C_String() + "',";
		command = command + "'" + p_serverInfo.second.Information.MapName.C_String() + "',";
		command = command + std::to_string(p_serverInfo.second.Information.CurrentPlayers) + ",";
		command = command + std::to_string(p_serverInfo.second.Information.MaxPlayers) + ",";
		command = command + std::to_string(p_serverInfo.first/1000) + ",";
		command = command + "'" + (p_serverInfo.second.Information.PasswordProtected ? "Yes" : "No") + "');";

		m_view->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WebString());
	}

	void RootForce::Menu::LoadDefaults(RootEngine::ConfigManagerInterface* p_configMan, std::string p_workingDir)
	{
		m_workingDir = p_workingDir;
		std::string command = "SetDefaults(";
		command = command + "'" + p_configMan->GetConfigValueAsString("ServerName") + "',";
		command = command + "'" + p_configMan->GetConfigValueAsString("ServerMapfile") + "',";
		command = command + std::to_string(p_configMan->GetConfigValueAsInteger("ServerPort")) + ",";
		command = command + "'" + p_configMan->GetConfigValueAsString("ServerPassword") + "',";
		command = command + std::to_string(p_configMan->GetConfigValueAsInteger("ServerMaxPlayers")) + ",";
		command = command + std::to_string(p_configMan->GetConfigValueAsInteger("ServerMatchLength")) + ",";
		command = command + std::to_string(p_configMan->GetConfigValueAsInteger("ServerKillVictory")) + ",";

		command += GetMapList();

		command += ");";

		m_view->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WebString());
	}
	
    void Menu::ShowError(std::string p_errorMessage, std::string p_errorTitle)
	{
		std::string command = "ShowError(";
		command += "'" + p_errorMessage + "',";
		command += "'" + p_errorTitle + "'";
		command += ");";

		m_view->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WebString());
	}

	MenuEvent::MenuEvent RootForce::Menu::PollEvent()
	{
		MenuEvent::MenuEvent event;
		if(m_event.size() < 1)
		{
			event.type = MenuEvent::EventType::NoEvent;
			return event;
		}
		event = m_event.at(0); 
		m_event.erase(m_event.begin());
		return event;
	}


	void Menu::ExitEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Exit;

		m_event.push_back(event);
	}


	void RootForce::Menu::HostEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Host;
		event.data = p_array;

		m_event.push_back(event);
	}

	void RootForce::Menu::ConnectEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Connect;
		event.data = p_array;

		m_event.push_back(event);
	}


	void Menu::RefreshEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Refresh;
		event.data = p_array;

		m_event.push_back(event);
	}

	Awesomium::JSValue Menu::GetMapListEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array)
	{
		return Awesomium::JSValue(Awesomium::WSLit(GetMapList().c_str()));
	}
	
	Awesomium::JSValue Menu::RequestSettingsEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array)
	{
		// Get a list of options
		std::map<std::string, std::string> valuePairs = m_context.m_configManager->GetConfigValuePairs();
		Awesomium::JSObject jsArray;
		for(auto itr = valuePairs.begin(); itr != valuePairs.end(); ++itr)
			jsArray.SetProperty(Awesomium::WSLit(itr->first.c_str()), Awesomium::WSLit(itr->second.c_str()));

		return Awesomium::JSValue(jsArray);
	}

	void Menu::SaveSettingsEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array)
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
			m_context.m_configManager->SetConfigValue(
				Awesomium::ToString(keys[i].ToString()),
				Awesomium::ToString(map.GetProperty(keys[i].ToString()).ToString())
			);

		m_context.m_configManager->StoreConfig("config.yaml"); // Hardcoding config file is not nice
	}

	std::string Menu::GetMapList()
	{
		// Create a list of available maps
		DIR *dir;
		struct dirent *ent;
		std::string command;
		command += "[";
		if ((dir = opendir((m_workingDir+"Assets/Levels/").c_str())) != NULL)
		{
			bool first = true;
			/* print all the files and directories within directory */
			while ((ent = readdir(dir)) != NULL)
			{
				if (ent->d_type != DT_REG) // Not a regular file
					continue;
				std::string fname = ent->d_name;
				std::string extension = ".world";
				if(fname.find(extension, (fname.length() - extension.length())) == std::string::npos) // Look for extension
					continue; // Mismatching extension, skip file
				if(!first)
					command += ",";
				else
					first = !first;
				command = command + "'" + fname.substr(0,fname.length() - extension.length()) + "'";
			}
			closedir (dir);
		}
		else
			m_context.m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "Could not open level directory");

		command += "]";
		return command;
	}

}

