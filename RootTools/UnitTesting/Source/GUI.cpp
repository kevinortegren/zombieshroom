#include "UnitTesting.h"
#include <Awesomium/WebCore.h>
#include <RootEngine/GUI/Include/guiInstance.h>

TEST(GUITest, InitTest) 
{
}

TEST(GUITest, LoadURLTest) 
{
	// Initialize GUI
	g_engineContext.m_gui->Initialize(g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenWidth"),
		g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenHeight"));

	RootEngine::GUISystem::WebView* temp = g_engineContext.m_gui->LoadURL("HUD", "hud.html");
	temp->WaitLoading();
	EXPECT_FALSE(temp->GetView()->IsCrashed());
}

TEST(GUITest, EventTest) 
{
}