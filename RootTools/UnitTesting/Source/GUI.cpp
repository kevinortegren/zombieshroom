#include "UnitTesting.h"
#include <Awesomium/WebCore.h>

TEST(GUITest, InitTest) 
{
}

TEST(GUITest, LoadURLTest) 
{
	g_engineContext.m_gui->Initialize(1028,1028);
	Awesomium::WebView* temp = g_engineContext.m_gui->LoadURL("hud.html");
	EXPECT_FALSE(temp->IsCrashed());
}

TEST(GUITest, EventTest) 
{
}