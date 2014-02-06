/*The MIT License (MIT)

	Copyright (c) 2013 Awesomium

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
	the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#include "Dispatcher.h"
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <Awesomium/STLHelpers.h>
namespace RootEngine
{
	namespace GUISystem
	{
		extern RootEngine::SubsystemSharedContext g_context;
		void RootEngine::GUISystem::Dispatcher::Bind( Awesomium::JSObject& p_object, const Awesomium::WebString& p_name, JSDelegate1 p_callback )
		{
			// We can't bind methods to local JSObjects
			if (p_object.type() == Awesomium::kJSObjectType_Local)
			{
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "Cannot bind methods to local JSObjects");
				return;
			}

			p_object.SetCustomMethod(p_name, false);

			ObjectMethodKey key(p_object.remote_id(), p_name);
			m_boundMethods[key] = p_callback;
		}

		void RootEngine::GUISystem::Dispatcher::BindWithRetVal( Awesomium::JSObject& p_object, const Awesomium::WebString& p_name, JSDelegate1WithRetval p_callback )
		{
			// We can't bind methods to local JSObjects
			if (p_object.type() == Awesomium::kJSObjectType_Local)
			{
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "Cannot bind methods to local JSObjects");
				return;
			}

			p_object.SetCustomMethod(p_name, true);

			ObjectMethodKey key(p_object.remote_id(), p_name);
			m_boundMethodsWithRetVal[key] = p_callback;
		}

		void RootEngine::GUISystem::Dispatcher::OnMethodCall( Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args )
		{
			// Find the method that matches the object id + method name
			std::map<ObjectMethodKey, JSDelegate1>::iterator i =
				m_boundMethods.find(ObjectMethodKey(remote_object_id, method_name));

			// Call the method
			if (i != m_boundMethods.end())
				i->second(args);
			else
			{
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "Callback method not found: %s", method_name);
			}

		}

		Awesomium::JSValue RootEngine::GUISystem::Dispatcher::OnMethodCallWithReturnValue( Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args )
		{

			// Find the method that matches the object id + method name
			std::map<ObjectMethodKey, JSDelegate1WithRetval>::iterator i =
				m_boundMethodsWithRetVal.find(ObjectMethodKey(remote_object_id, method_name));

			// Call the method
			if (i != m_boundMethodsWithRetVal.end())
				return i->second(args);
			else
			{
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "Callback method not found: %s", method_name);
				return Awesomium::JSValue::Undefined();
			}
			
		}

	}
}