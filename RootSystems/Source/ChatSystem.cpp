#include "ChatSystem.h"
#include <Awesomium/STLHelpers.h>
#include <Awesomium/WebCore.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <string>
#include <iostream>
#include <RootEngine/GUI/Include/guiInstance.h>

namespace RootForce
{

	void ChatSystem::Initialize( Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, RootEngine::GameSharedContext* p_engineContext)
	{
		m_view = p_view;
		m_hasFocus = false;
		m_engineContext = p_engineContext;
		
		while(m_view->IsLoading())
		{
			m_engineContext->m_gui->Update();
		}
		m_view->Focus(); // Give the view the focus, necessary for JavaScript to catch the input events

		Awesomium::JSValue result = m_view->CreateGlobalJavascriptObject(Awesomium::WSLit("ChatSystem"));

		if(result.IsObject())
		{
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Send"), JSDelegate(this, &ChatSystem::ProcessMessage));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("SetFocus"), JSDelegate(this, &ChatSystem::SetFocus));
		}
		else
		{
			m_engineContext->m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "Could not create ChatSystem object in Javascript");
			return;
		}
		m_view->set_js_method_handler(p_dispatcher);
		m_view->Focus();
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
		m_view->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WSLit(""));
	}

	void RootForce::ChatSystem::SetFocus( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		m_hasFocus = p_array[0].ToBoolean();
	}

	void RootForce::ChatSystem::ProcessMessage( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array)
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
