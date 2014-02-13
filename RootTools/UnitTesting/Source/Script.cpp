#include <UnitTesting.h>
#include <RootEngine/Script/Include/RootScript.h>

TEST(Script, SetGlobals)
{
	g_engineContext.m_resourceManager->LoadScript("TestScript");
	g_engineContext.m_resourceManager->LoadScript("Global");

	g_engineContext.m_script->SetGlobalNumber("TestVariable", 5, "TestTable");

	g_engineContext.m_script->SetFunction("TestTable", "TestSetVariable");
	g_engineContext.m_script->ExecuteScript();
}

TEST(Script, GetGlobals)
{
	g_engineContext.m_resourceManager->LoadScript("TestScript");

	g_engineContext.m_script->SetFunction("TestTable", "TestGetVariable");
	g_engineContext.m_script->ExecuteScript();

	int value = (int) g_engineContext.m_script->GetGlobalNumber("TestVariable", "TestTable");
	ASSERT_EQ(value, 25);
}