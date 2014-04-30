#include "Treenity.h"
#include <RootTools/Treenity/Include/TreenityMain.h>
#include <QtWidgets/QApplication>
#include <SDL2/SDL.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#undef main
int main(int argc, char *argv[])
{
	std::string path(argv[0]);
	std::string rootforcename = "Treenity.exe";
	path = path.substr(0, path.size() - rootforcename.size());

	QApplication a(argc, argv);

	Treenity w;
	w.show();

	TreenityMain m(path);

	uint64_t old = SDL_GetPerformanceCounter();
	while(w.IsRunning())
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
}

TreenityMain::~TreenityMain()
{
	SDL_Quit();

	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

void TreenityMain::Update(float dt)
{

}

