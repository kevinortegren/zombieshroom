#pragma once

#include <Awesomium/WebCore.h>
#include <mutex>
#include <queue>
#include <vector>
#include "Dispatcher.h"

namespace RootEngine
{
	namespace GUISystem
	{
		inline std::string PreventHTMLInjections(std::string p_source)
		{
			size_t pos = 0;
			// & - &amp;
			/*while ((pos = p_source.find("&", pos)) != std::string::npos) {
				p_source.replace(pos, 1, "&amp;");
				pos += 5;
			}*/
			// < - &lt;
			pos = 0;
			while ((pos = p_source.find("<", pos)) != std::string::npos) {
				p_source.replace(pos, 1, "&lt;");
				pos += 4;
			}
			// > - &gt;
			pos = 0;
			while ((pos = p_source.find(">", pos)) != std::string::npos) {
				p_source.replace(pos, 1, "&gt;");
				pos += 4;
			}
			// ` - &#96;
			pos = 0;
			while ((pos = p_source.find("`", pos)) != std::string::npos) {
				p_source.replace(pos, 1, "&#96;");
				pos += 5;
			}
			// ' - &#39;
			pos = 0;
			while ((pos = p_source.find("'", pos)) != std::string::npos) {
				p_source.replace(pos, 1, "&#39;");
				pos += 5;
			}
			// " - &quot;
			pos = 0;
			while ((pos = p_source.find("\"", pos)) != std::string::npos) {
				p_source.replace(pos, 1, "&quot;");
				pos += 6;
			}

			return p_source;
		}

		class WebView abstract
		{
		public:
			// Buffer a Javascript to be executed on the next view update
			virtual void BufferJavascript(std::string p_script) = 0;
			// Registers an event in form of a callable method on an object within the JavaScript of the view
			// Currently only one object is supported and reused
			virtual void RegisterJSCallback(std::string p_event, JSDelegate1 callback) = 0;
			virtual void RegisterJSCallback(std::string p_event, JSDelegate1WithRetval callback) = 0;

			// Wait until the view has finished loading
			virtual void WaitLoading() = 0;
			// Give the view the focus
			virtual void Focus() = 0;

			virtual Awesomium::WebView* GetView() = 0;

			virtual void SetActive(bool p_active) = 0;

			virtual void InjectKeyboardEvent(Awesomium::WebKeyboardEvent p_event) = 0;
			virtual void InjectMouseDown(Awesomium::MouseButton p_mousebutton) = 0;
			virtual void InjectMouseUp(Awesomium::MouseButton p_mousebutton) = 0;
			virtual void InjectMouseMove(int p_x, int p_y) = 0;
			virtual void InjectMouseWheel(int p_x, int p_y) = 0;
		};

		// Used by the guiSystem to establish communications between Awesomium::WebView and the program
		// Use mutexes when implementing callback functions to avoid race conditions
		class WebViewImpl : public WebView
		{
			// This class is closely related to guiInstance, therefore give full access to it
			friend class guiInstance;
		private:
			WebViewImpl(std::string p_callbackObjectName, GUISystem::DispatcherInterface* p_dispatcher)
				: m_webView(nullptr),
				m_callbackObject(nullptr),
				m_dispatcher(p_dispatcher),
				m_callbackObjectName(p_callbackObjectName)
			{
				m_isActive = true;
			}
			~WebViewImpl();
		public:

			// Buffer a Javascript to be executed on the next view update
			void BufferJavascript(std::string p_script);
			// Registers an event in form of a callable method on the callback object within the JavaScript of the view
			// Currently only one object is supported and reused
			void RegisterJSCallback(std::string p_event, JSDelegate1 callback);
			void RegisterJSCallback(std::string p_event, JSDelegate1WithRetval callback);

			// Wait until the view has finished loading
			void WaitLoading();
			// Give the view the focus
			void Focus();

			Awesomium::WebView* GetView()
			{
				return m_webView;
			}

			void SetActive(bool p_active)
			{
				m_isActive = p_active;
			}

		private:
			void Update();
			//void PushEvent(std::pair<std::string,Awesomium::JSArray> p_event);

			void ProcessEvent(Awesomium::WebView* p_caller, std::string p_event, const Awesomium::JSArray& p_array);

			void InjectKeyboardEvent(Awesomium::WebKeyboardEvent p_event);
			void InjectMouseDown(Awesomium::MouseButton p_mousebutton);
			void InjectMouseUp(Awesomium::MouseButton p_mousebutton);
			void InjectMouseMove(int p_x, int p_y);
			void InjectMouseWheel(int p_x, int p_y);

			Awesomium::WebView* m_webView;
			Awesomium::JSObject* m_callbackObject;
			std::mutex m_jsBufferMutex;
			std::vector<std::string> m_jsBuffer;
			std::mutex m_callbackMutex;
			std::map<std::string, JSDelegate1> m_callback;
			std::map<std::string, JSDelegate1WithRetval> m_callbackWithRetval;
			GUISystem::DispatcherInterface* m_dispatcher;
			std::string m_callbackObjectName;
			std::mutex m_injectMutex;
			std::vector<Awesomium::WebKeyboardEvent> m_injectKeyboard;
			std::vector<Awesomium::MouseButton> m_injectMouseDown;
			std::vector<Awesomium::MouseButton> m_injectMouseUp;
			std::vector<std::pair<int,int>> m_injectMouseMove;
			std::vector<std::pair<int,int>> m_injectMouseWheel;
			bool m_isActive;
		};
	}
}
