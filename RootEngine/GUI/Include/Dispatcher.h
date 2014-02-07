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

#pragma once
#include <Awesomium/JSObject.h>
#include "js_delegate.h"
#include <map>
namespace RootEngine
{
	namespace GUISystem
	{
		class DispatcherInterface abstract : public Awesomium::JSMethodHandler
		{
		public:
			virtual void Bind(Awesomium::JSObject& p_object,
				const Awesomium::WebString& p_name,
				JSDelegate1 p_callback) = 0;

			virtual void BindWithRetVal(Awesomium::JSObject& p_object,
				const Awesomium::WebString& p_name,
				JSDelegate1WithRetval p_callback) = 0;

			// Inherited from JSMethodHandler
			virtual void OnMethodCall(Awesomium::WebView* caller,
				unsigned int remote_object_id,
				const Awesomium::WebString& method_name,
				const Awesomium::JSArray& args) = 0;

			// Inherited from JSMethodHandler
			virtual Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView* caller,
				unsigned int remote_object_id,
				const Awesomium::WebString& method_name,
				const Awesomium::JSArray& args) = 0;
		};

		typedef std::pair<int, Awesomium::WebString> ObjectMethodKey;
		class Dispatcher : public DispatcherInterface
		{
		public:
			void Bind(Awesomium::JSObject& p_object,
				const Awesomium::WebString& p_name,
				JSDelegate1 p_callback);

			void BindWithRetVal(Awesomium::JSObject& p_object,
				const Awesomium::WebString& p_name,
				JSDelegate1WithRetval p_callback);

			// Inherited from JSMethodHandler
			void OnMethodCall(Awesomium::WebView* caller,
				unsigned int remote_object_id,
				const Awesomium::WebString& method_name,
				const Awesomium::JSArray& args);

			// Inherited from JSMethodHandler
			Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView* caller,
				unsigned int remote_object_id,
				const Awesomium::WebString& method_name,
				const Awesomium::JSArray& args);

		private:
			std::map<ObjectMethodKey, JSDelegate1> m_boundMethods;
			std::map<ObjectMethodKey, JSDelegate1WithRetval> m_boundMethodsWithRetVal;
		};
	}
}