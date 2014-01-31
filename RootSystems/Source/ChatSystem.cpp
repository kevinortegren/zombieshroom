#ifndef COMPILE_LEVEL_EDITOR

#include "ChatSystem.h"
//#include <Awesomium/STLHelpers.h>
//#include <Awesomium/WebCore.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <string>
#include <iostream>
#include <RootEngine/GUI/Include/guiInstance.h>
#include <Awesomium/STLHelpers.h>

namespace RootForce
{

	void ChatSystem::Initialize( RootEngine::GUISystem::WebView* p_view, RootEngine::GameSharedContext* p_engineContext)
	{
		m_view = p_view;
		m_hasFocus = false;
		m_engineContext = p_engineContext;
		
		m_view->WaitLoading();
		m_view->Focus(); // Give the view the focus, necessary for JavaScript to catch the input events


		m_view->RegisterJSCallback("Send", JSDelegate1(this, &ChatSystem::ProcessMessage));
		m_view->RegisterJSCallback("SetFocus", JSDelegate1(this, &ChatSystem::SetFocus));
	}

	void RootForce::ChatSystem::JSAddMessage( std::string p_message )
	{
		JSAddMessage(p_message.c_str());
	}

	void RootForce::ChatSystem::JSAddMessage( const char* p_message )
	{
		std::string command; // Compile a string-function call in form of AddMessage(p_message);
		command = "AddMessage(\"";
		command += p_message;
		command += "\");";
		m_view->BufferJavascript(command.c_str());
	}

	void RootForce::ChatSystem::SetFocus( const Awesomium::JSArray& p_array )
	{
		m_hasFocus = p_array[0].ToBoolean();
	}

	void RootForce::ChatSystem::ProcessMessage( const Awesomium::JSArray& p_array)
	{
		//JSAddMessage(Awesomium::ToString(p_array[0].ToString()).c_str()); //This line of code sounds retardeded :/

		//TODO: check the first char for a / sign, if found it is a server event and should be treated differently
		std::string temp = Awesomium::ToString(p_array[0].ToString());
		if(temp[0] == '/')
		{
			RootServer::EventData ev = RootServer::EventFromString(temp.substr(1));
			m_eventBuffer.push_back(ev);
		}
		else
			m_messageBuffer.push_back(Awesomium::ToString(p_array[0].ToString()));
	}

	std::string ChatSystem::PollMessage()
	{
		if(m_messageBuffer.size() < 1)
			return "";
		std::string temp = m_messageBuffer.at(0); 
		m_messageBuffer.erase(m_messageBuffer.begin());
		return temp;
	}

	RootServer::EventData ChatSystem::PollEvent()
	{
		if(m_eventBuffer.size() < 1)
			return RootServer::EventData();
		RootServer::EventData temp = m_eventBuffer.at(0); 
		m_eventBuffer.erase(m_eventBuffer.begin());
		return temp;
	}



}

#endif