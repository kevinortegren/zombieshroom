#include "UnitTesting.h"
#include <Awesomium/WebCore.h>
#include <RootEngine/GUI/Include/guiInstance.h>

TEST(GUITest, LoadURLTest) 
{
	RootEngine::GUISystem::WebView* temp = g_engineContext.m_gui->LoadURL("testfile", "testfile.html");
	temp->WaitLoading();
	EXPECT_FALSE(temp->GetView()->IsCrashed());
	g_engineContext.m_gui->DestroyView(temp);
}

TEST(GUITest, FunctionBindings)
{
	//RootEngine::GUISystem::Dispatcher* dispatcher = g_engineContext.m_gui->GetDispatcher();
	RootEngine::GUISystem::WebView* view = g_engineContext.m_gui->LoadURL("fakeObject", "testfile.html");

	view->RegisterJSCallback("fakeEvent", JSDelegate1());
	view->RegisterJSCallback("fakeEvent2", JSDelegate1WithRetval());
	g_engineContext.m_gui->DestroyView(view);
}

TEST(GUITest, NonExistingFile)
{
	//RootEngine::GUISystem::Dispatcher* dispatcher = g_engineContext.m_gui->GetDispatcher();
	RootEngine::GUISystem::WebView* view = g_engineContext.m_gui->LoadURL("N/A", "nonexistingfile.html");
	view->WaitLoading();
	EXPECT_FALSE(view->GetView()->IsCrashed());
	g_engineContext.m_gui->DestroyView(view);
}