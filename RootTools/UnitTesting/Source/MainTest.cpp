#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <SDL2/SDL.h>
#include <stdexcept>
#include <exception>

#include <UnitTesting.h>
#include <MainTest.h>

#undef main

int main(int argc, char* argv[]) 
{
	std::string path = std::string(argv[0]);
	std::string rootforcename = "UnitTesting.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	try 
	{
		UnitTesting::Main m(path);
		testing::InitGoogleTest(&argc, argv);
		int result = RUN_ALL_TESTS();
		std::cin.get();
		return result;
	} 
	catch (std::exception& e) 
	{
		// TODO: Log exception message
		std::cout << e.what() << "\n";
		//std::cin.get();
		return 1;
	} 
	catch (...) 
	{
		// TODO: Log unknown exception message
		//std::cin.get();
		return 1;
	}
	
	return 0;
}

namespace UnitTesting
{
	Main::Main(std::string p_workingDirectory) 
		: m_running(true)
	{
		m_workingDirectory = p_workingDirectory;

		// Load the engine
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

		g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL, p_workingDirectory);

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		{
			throw std::runtime_error("Failed to initialize SDL");
		}


		// TODO: Make these parameters (even?) more configurable.
		m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
			"Root Force",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenWidth"),
			g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenHeight"),
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN),
			SDL_DestroyWindow);
		if (m_window == nullptr) 
		{
			throw std::runtime_error("Failed to create window");
		}

		// Setup the SDL context
		g_engineContext.m_renderer->SetupSDLContext(m_window.get());
		RootForce::ComponentType::Initialize();
	}

	Main::~Main() 
	{
		//m_world.GetEntityExporter()->Export(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");
		SDL_Quit();
		DynamicLoader::FreeSharedLibrary(m_engineModule);
	}

}