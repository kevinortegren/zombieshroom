#include "Treenity.h"
#include <RootTools/Treenity/Include/TreenityMain.h>
#include <QtWidgets/QApplication>
#include <SDL2/SDL.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>
#include <RootEngine/InputManager/Include/InputInterface.h>
#include <RootTools/Treenity/Include/EngineActions.h>

#undef main
int main(int argc, char *argv[])
{
	std::string path(argv[0]);
	std::string rootforcename = "Treenity.exe";
	path = path.substr(0, path.size() - rootforcename.size());

	QApplication a(argc, argv);
	TreenityMain m(path);

	uint64_t old = SDL_GetPerformanceCounter();
	while(m.IsRunning())
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;

		//w.Update(dt);
		a.processEvents(QEventLoop::AllEvents);
		m.Update(dt);
	}

	return 0;
}

TreenityMain::TreenityMain(const std::string& p_path)
	: m_engineActions(&m_world)
{
	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	if (m_engineModule == nullptr)
	{
		throw std::runtime_error("Failed to load RootEngine - please check your installation");
	}

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

	if (libInitializeEngine == nullptr)
	{
		throw std::runtime_error("Failed to load RootEngine - please check your installation");
	}

	g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_INPUT | RootEngine::SubsystemInit::INIT_RENDER, p_path);

	if (SDL_Init(SDL_INIT_TIMER) != 0) 
	{
		throw std::runtime_error("Failed to initialize SDL");
	}

	m_treenityEditor.show();
	m_treenityEditor.SetEngineInterface(&m_engineActions);
}

TreenityMain::~TreenityMain()
{

	SDL_Quit();

	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

bool TreenityMain::IsRunning()
{
	return m_treenityEditor.IsRunning();
}

void TreenityMain::HandleEvents()
{
	if (g_engineContext.m_inputSys != nullptr)
		g_engineContext.m_inputSys->Reset();

	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if (g_engineContext.m_inputSys != nullptr)
			g_engineContext.m_inputSys->HandleInput(event);
	}
}

void TreenityMain::Update(float dt)
{
	//g_world->SetDelta(p_delta);

	//g_engineContext.m_renderer->Clear();

	HandleEvents();

	//g_engineContext.m_renderer->Render();
	//g_engineContext.m_renderer->Swap();

}

