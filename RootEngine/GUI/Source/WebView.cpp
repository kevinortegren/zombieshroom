#include "WebView.h"
#include "Logging/Logging.h"
#include <Awesomium/STLHelpers.h>
#include <RootEngine/Include/SubsystemSharedContext.h>

namespace RootEngine
{
	namespace GUISystem
	{
		extern RootEngine::SubsystemSharedContext g_context;

		WebViewImpl::~WebViewImpl()
		{
			m_webView->Stop();
			//m_webView->Destroy();
		}

		void WebViewImpl::BufferJavascript(std::string p_script)
		{
			m_jsBufferMutex.lock();
			m_jsBuffer.push_back(p_script);
			m_jsBufferMutex.unlock();
		}
		void WebViewImpl::RegisterJSCallback(std::string p_event, JSDelegate1 callback)
		{
			m_callbackMutex.lock();
			m_callback[p_event] = callback;
			m_callbackMutex.unlock();
		}
		void WebViewImpl::RegisterJSCallback(std::string p_event, JSDelegate1WithRetval callback)
		{
			m_callbackMutex.lock();
			m_callbackWithRetval[p_event] = callback;
			m_callbackMutex.unlock();
		}
		void WebViewImpl::WaitLoading()
		{
			while(!m_webView || m_webView->IsLoading())
			{
				Sleep(100);
			}
		}
		void WebViewImpl::Focus()
		{
			m_webView->Focus();
		}

		void WebViewImpl::Update()
		{
			// Don't do anything if view is still loading
			if(!m_webView || m_webView->IsLoading())
				return;

			// Bind callback functions
			m_callbackMutex.lock();
			if(!m_callbackObject)
			{
				Awesomium::JSValue result = m_webView->CreateGlobalJavascriptObject(Awesomium::WSLit(m_callbackObjectName.c_str()));
				if(result.IsObject() && result.ToObject().type() != Awesomium::kJSObjectType_Local)
					m_callbackObject = new Awesomium::JSObject(result.ToObject());
				else
				{
					g_context.m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "Failed to create a JS object to bind events to");
					return;
				}
				m_webView->set_js_method_handler(m_dispatcher);
			}
			for(auto callback : m_callback)
				m_dispatcher->Bind(*m_callbackObject, Awesomium::WSLit(callback.first.c_str()), callback.second);
			m_callback.clear();
			for(auto callback : m_callbackWithRetval)
				m_dispatcher->BindWithRetVal(*m_callbackObject, Awesomium::WSLit(callback.first.c_str()), callback.second);
			m_callbackWithRetval.clear();
			m_callbackMutex.unlock();

			// Process buffered commands
			m_jsBufferMutex.lock();
			for(auto command : m_jsBuffer)
				m_webView->ExecuteJavascript(Awesomium::WSLit(command.c_str()),Awesomium::WebString());
			m_jsBuffer.clear();
			m_jsBufferMutex.unlock();
			
			m_injectMutex.lock();
			for(auto event : m_injectKeyboard)
				m_webView->InjectKeyboardEvent(event);
			m_injectKeyboard.clear();
			for(auto event : m_injectMouseDown)
				m_webView->InjectMouseDown(event);
			m_injectMouseDown.clear();
			for(auto event : m_injectMouseUp)
				m_webView->InjectMouseUp(event);
			m_injectMouseUp.clear();
			for(auto event : m_injectMouseMove)
				m_webView->InjectMouseMove(event.first, event.second);
			m_injectMouseMove.clear();
			for(auto event : m_injectMouseWheel)
				m_webView->InjectMouseWheel(event.second*12, event.first*12);
			m_injectMouseWheel.clear();
			m_injectMutex.unlock();
		}

		void WebViewImpl::InjectKeyboardEvent(Awesomium::WebKeyboardEvent p_event)
		{
			m_injectMutex.lock();
			m_injectKeyboard.push_back(p_event);
			m_injectMutex.unlock();
		}
		void WebViewImpl::InjectMouseDown(Awesomium::MouseButton p_mousebutton)
		{
			m_injectMutex.lock();
			m_injectMouseDown.push_back(p_mousebutton);
			m_injectMutex.unlock();
		}
		void WebViewImpl::InjectMouseUp(Awesomium::MouseButton p_mousebutton)
		{
			m_injectMutex.lock();
			m_injectMouseUp.push_back(p_mousebutton);
			m_injectMutex.unlock();
		}
		void WebViewImpl::InjectMouseMove(int p_x, int p_y)
		{
			m_injectMutex.lock();
			m_injectMouseMove.push_back(std::pair<int,int>(p_x,p_y));
			m_injectMutex.unlock();
		}
		void WebViewImpl::InjectMouseWheel(int p_x, int p_y)
		{
			m_injectMutex.lock();
			m_injectMouseWheel.push_back(std::pair<int,int>(p_x,p_y));
			m_injectMutex.unlock();
		}
	}
}
