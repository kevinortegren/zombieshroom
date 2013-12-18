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
namespace RootEngine
{
	namespace GUISystem
	{

		void RootEngine::GUISystem::Dispatcher::Bind( Awesomium::JSObject& p_object, const Awesomium::WebString& p_name, JSDelegate p_callback )
		{
			// We can't bind methods to local JSObjects
			if (p_object.type() == Awesomium::kJSObjectType_Local)
				return;

			p_object.SetCustomMethod(p_name, false);

			ObjectMethodKey key(p_object.remote_id(), p_name);
			m_boundMethods[key] = p_callback;
		}

		void RootEngine::GUISystem::Dispatcher::OnMethodCall( Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args )
		{
			// Find the method that matches the object id + method name
			std::map<ObjectMethodKey, JSDelegate>::iterator i =
				m_boundMethods.find(ObjectMethodKey(remote_object_id, method_name));

			// Call the method
			if (i != m_boundMethods.end())
				i->second(caller, args);
		}

		Awesomium::JSValue RootEngine::GUISystem::Dispatcher::OnMethodCallWithReturnValue( Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args )
		{
			return Awesomium::JSValue::Undefined();
		}

	}
}