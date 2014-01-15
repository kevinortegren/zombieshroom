#include "UnitTesting.h"
#include <Awesomium/WebCore.h>

TEST(GUITest, InitTest) 
{
}

TEST(GUITest, LoadURLTest) 
{
	// Initialize GUI
	g_engineContext.m_gui->Initialize(g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenWidth"),
		g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenHeight"));

	Awesomium::WebView* temp = g_engineContext.m_gui->LoadURL("hud.html");
	while(temp->IsLoading())
		g_engineContext.m_gui->Update();
	EXPECT_FALSE(temp->IsCrashed());
}

TEST(GUITest, EventTest) 
{
}