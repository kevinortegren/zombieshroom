#include "UnitTesting.h"
#include <Awesomium/WebCore.h>
#include <RootSystems/Include/ChatSystem.h>

TEST(Dispatcher, EmptyBind)
{
	RootEngine::GUISystem::Dispatcher* dispatcher = g_engineContext.m_gui->GetDispatcher();

	Awesomium::JSObject fakeObject;
	Awesomium::WebString fakeName;
	dispatcher->Bind(fakeObject, fakeName, JSDelegate());
	dispatcher->BindWithRetVal(fakeObject, fakeName, JSDelegateWithRetval());
}

TEST(Dispatcher, EmptyCall)
{
	RootEngine::GUISystem::Dispatcher* dispatcher = g_engineContext.m_gui->GetDispatcher();

	Awesomium::WebView* caller = nullptr;
	unsigned int remote_object_id = 0;
	const Awesomium::WebString method_name;
	const Awesomium::JSArray args;

	dispatcher->OnMethodCall(caller, remote_object_id, method_name, args);
	dispatcher->OnMethodCallWithReturnValue(caller,  remote_object_id, method_name, args);
}

