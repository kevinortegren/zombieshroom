#include <WinSock2.h>
#include <Windows.h>
#include <RootEngine/Include/RootEngine.h>
#include <DLLMain.h>

namespace RootEngine
{
	EngineMain* g_engineMain = nullptr;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		RootEngine::g_engineMain = new RootEngine::EngineMain;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		delete RootEngine::g_engineMain;
		break;
	}

	return TRUE;
}