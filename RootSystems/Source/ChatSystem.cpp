#include "ChatSystem.h"
#include <Awesomium/STLHelpers.h>
#include <Awesomium/WebCore.h>
#include <string>
#include <iostream>
namespace RootForce
{

	void ChatSystem::Initialize( Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher )
	{
		m_view = p_view;
		m_hasFocus = false;
		m_view->Focus();

		Awesomium::JSValue result = m_view->CreateGlobalJavascriptObject(Awesomium::WSLit("ChatSystem"));

		if(result.IsObject() && result.ToObject().type() != Awesomium::kJSObjectType_Local)
		{
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Send"), JSDelegate(this, &ChatSystem::ProcessMessage));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("SetFocus"), JSDelegate(this, &ChatSystem::SetFocus));
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
