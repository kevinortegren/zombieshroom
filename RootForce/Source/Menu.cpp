#include "Menu.h"

#include <External/Include/dirent/dirent.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/GUI/Include/guiInstance.h>
#include <RootEngine/Sound/Include/SoundManager.h>

namespace RootForce
{

	RootForce::Menu::Menu( RootEngine::GUISystem::WebView* p_view, RootEngine::GameSharedContext p_context )
	{
		m_view = p_view;
		m_context = p_context;
		m_settingsMenu = new SettingsMenu(p_context);
		m_view->WaitLoading();

		m_view->RegisterJSCallback("Exit", JSDelegate1(this, &Menu::ExitEvent));
		m_view->RegisterJSCallback("Host", JSDelegate1(this, &Menu::HostEvent));
		m_view->RegisterJSCallback("Connect", JSDelegate1(this, &Menu::ConnectEvent));
		m_view->RegisterJSCallback("Refresh", JSDelegate1(this, &Menu::RefreshEvent));
		
		m_settingsMenu->BindEvents(p_view);

		m_view->Focus();

		m_context.m_sound->PlayBackgroundSound();
	}

	Menu::~Menu()
	{
		delete m_settingsMenu;
	}

	RootEngine::GUISystem::WebView* RootForce::Menu::GetView()
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

		m_view->BufferJavascript(command);
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
		command = command + std::to_string(p_configMan->GetConfigValueAsFloat("ServerMatchLength")) + ",";
		command = command + std::to_string(p_configMan->GetConfigValueAsInteger("ServerKillVictory")) + ",";

		command += GetMapList();

		command += ");";

		m_view->BufferJavascript(command);
	}
	
    void Menu::ShowError(std::string p_errorMessage, std::string p_errorTitle)
	{
		std::string command = "ShowError(";
		command += "'" + p_errorMessage + "',";
		command += "'" + p_errorTitle + "'";
		command += ");";

		m_view->BufferJavascript(command);
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


	void Menu::ExitEvent( const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Exit;

		m_event.push_back(event);
	}


	void RootForce::Menu::HostEvent( const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Host;
		event.data = p_array;

		m_event.push_back(event);
	}

	void RootForce::Menu::ConnectEvent( const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Connect;
		event.data = p_array;

		m_event.push_back(event);
	}


	void Menu::RefreshEvent( const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Refresh;
		event.data = p_array;

		m_event.push_back(event);
	}

	Awesomium::JSValue Menu::GetMapListEvent(const Awesomium::JSArray& p_array)
	{
		return Awesomium::JSValue(Awesomium::WSLit(GetMapList().c_str()));
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

	void Menu::Update()
	{
		m_settingsMenu->Update();
		m_context.m_sound->Update();
	}

}

