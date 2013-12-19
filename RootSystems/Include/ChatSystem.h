#pragma once

#include <Awesomium/WebView.h>
#include <RootEngine/GUI/Include/Dispatcher.h>
#include <vector>
namespace RootForce
{
	class ChatSystem
	{
	public:
		void Initialize(Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher); //Load a WebView into the chatsystem together with a gui dispatcher for the javascript

		void JSAddMessage(std::string p_message);
		void JSAddMessage(const char* p_message); //Adds a message to the hud html chatlog, not to the internal message buffer.

		bool IsFocused() { return m_hasFocus; } //Returns true if the player is typing in the chat. If this is the case all other input should be locked

		std::string PollMessage(); //Returns the last message from the internal buffer

	private:
		void SetFocus(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		void ProcessMessage(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array);
		Awesomium::WebView* m_view;
		std::vector<std::string> m_messageBuffer;
		bool m_hasFocus;
	};
}