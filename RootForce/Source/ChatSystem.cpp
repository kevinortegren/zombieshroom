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

		m_messageBuffer.push(Awesomium::ToString(p_array[0].ToString()));
	}

	std::string ChatSystem::PollMessage()
	{
		std::string temp = m_messageBuffer.back(); 
		m_messageBuffer.pop();
		return temp;
	}
	

}
