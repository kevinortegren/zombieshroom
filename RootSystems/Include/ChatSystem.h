#pragma once

#include <RootEngine/Include/GameSharedContext.h>
//#include <Awesomium/WebView.h>
#include <RootEngine/GUI/Include/WebView.h>
#include <RootEngine/GUI/Include/Dispatcher.h>
#include <RootSystems/Include/Network/ServerEvents.h>
#include <vector>
#include <mutex>
namespace RootForce
{
	class ChatSystem
	{
	public:
		void Initialize(RootEngine::GUISystem::WebView* p_view, RootEngine::GameSharedContext* p_engineContext); //Load a WebView into the chatsystem together with a gui dispatcher for the javascript

		void JSAddMessage(std::string p_message);
		void JSAddMessage(const char* p_message); //Adds a message to the hud html chatlog, not to the internal message buffer.

		bool IsFocused() { return m_hasFocus; } //Returns true if the player is typing in the chat. If this is the case all other input should be locked

		std::string PollMessage(); //Returns the last message from the internal buffer
		RootServer::EventData PollEvent(); //Returns the last server command from the internal buffer

		void InjectEvent(std::string& p_event);

	private:
		void SetFocus(const Awesomium::JSArray& p_array);
		void ProcessMessage(const Awesomium::JSArray& p_array);

		RootEngine::GUISystem::WebView* m_view;
		std::vector<std::string> m_messageBuffer;
		std::vector<RootServer::EventData> m_eventBuffer;
		std::mutex m_eventBufferMutex;
		bool m_hasFocus;
		RootEngine::GameSharedContext* m_engineContext;
	};
}