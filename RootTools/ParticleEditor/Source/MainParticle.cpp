#include <RootTools/ParticleEditor/Include/MainParticle.h>
#include <QtWidgets/QApplication>
#include <stdexcept>
#include <exception>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <glm/glm.hpp>
#include <RootEngine/Include/RootEngine.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_syswm.h> 
#undef main
int main(int argc, char *argv[])
{
	std::string path(argv[0]);
	std::string rootforcename = "ParticleEditor.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	
	try 
	{
		QApplication a(argc, argv);
		ParticleEditor w;
		w.show();
		MainParticle m(path, &w);

		uint64_t old = SDL_GetPerformanceCounter();
		bool notExit = true;
		while(notExit)
		{
			notExit = w.CheckExit();
			
			a.processEvents(QEventLoop::AllEvents);

			uint64_t now = SDL_GetPerformanceCounter();
			float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
			old = now;
			m.Update(dt);
			m.HandleEvents();
		}
	} 
	catch (std::exception& e) 
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		return 1;
	} 
	catch (...) 
	{
		std::cout << "Unknown exception" << std::endl;
		std::cin.get();
		return 1;
	}
	return 0;
}

MainParticle::MainParticle( std::string p_workingDirectory, ParticleEditor* p_particleEditorQt )
{
	m_workingDirectory = p_workingDirectory;
	g_world = &m_world;

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

	g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_INPUT | RootEngine::SubsystemInit::INIT_RENDER, p_workingDirectory);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		throw std::runtime_error("Failed to initialize SDL");
	}


	// TODO: Make these parameters (even?) more configurable.
	m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
		"Root Particle",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		p_particleEditorQt->ui.frame->width(),
		p_particleEditorQt->ui.frame->height(),
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS),
		SDL_DestroyWindow);

	//////////////////////////////////////////////////////////////////////////
	//Get HWND from SDL window and change its position
	//////////////////////////////////////////////////////////////////////////
	SDL_SysWMinfo windowInfo;
	SDL_VERSION(&windowInfo.version);

	if(SDL_GetWindowWMInfo(m_window.get(), &windowInfo))
	{
		HWND handle = windowInfo.info.win.window;
		int x = p_particleEditorQt->ui.frame->pos().x();
		int y = p_particleEditorQt->ui.frame->pos().y() + p_particleEditorQt->ui.menuBar->height();
		BOOL result = SetWindowPos(handle, NULL, x, y, 0, 0, 
			SWP_NOREPOSITION|SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

		SetParent(handle, (HWND)p_particleEditorQt->winId());
	}
	//////////////////////////////////////////////////////////////////////////
	if (m_window == nullptr) 
	{
		throw std::runtime_error("Failed to create window");
	}
	// Setup the SDL context
	g_engineContext.m_renderer->SetupSDLContext(m_window.get());

	RootForce::Renderable::SetTypeId(RootForce::ComponentType::RENDERABLE);
	RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
	RootForce::PointLight::SetTypeId(RootForce::ComponentType::POINTLIGHT);
	RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
	RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
	RootForce::ThirdPersonBehavior::SetTypeId(RootForce::ComponentType::THIRDPERSONBEHAVIOR);
	RootForce::ParticleEmitter::SetTypeId(RootForce::ComponentType::PARTICLE);


	// Initialize render and point light system.
	m_renderingSystem = new RootForce::RenderingSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem, "RenderingSystem");

	m_renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
	m_renderingSystem->SetRendererInterface(g_engineContext.m_renderer);

	m_pointLightSystem = new RootForce::PointLightSystem(g_world, g_engineContext.m_renderer);
	g_world->GetSystemManager()->AddSystem<RootForce::PointLightSystem>(m_pointLightSystem, "PointLightSystem");

	m_particleSystem = new RootForce::ParticleSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::ParticleSystem>(m_particleSystem, "ParticleSystem");

	// Initialize camera systems.
	m_cameraSystem = new RootForce::CameraSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem, "CameraSystem");
	m_lookAtSystem = new RootForce::LookAtSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::LookAtSystem>(m_lookAtSystem, "LookAtSystem");
	// Setup the importer and exporter
	//m_world.GetEntityImporter()->SetImporter(Importer);
	//m_world.GetEntityExporter()->SetExporter(Exporter);
	g_engineContext.m_inputSys->LockMouseToCenter(false);
}

MainParticle::~MainParticle()
{
	SDL_Quit();
	DynamicLoader::FreeSharedLibrary(m_engineModule);

}

void MainParticle::HandleEvents()
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

void MainParticle::Update( float p_delta )
{
	g_world->SetDelta(p_delta);
	g_engineContext.m_renderer->Clear();

	m_lookAtSystem->Process();
	m_cameraSystem->Process();
	m_particleSystem->Process();
	m_pointLightSystem->Process();
	m_renderingSystem->Process();
	g_engineContext.m_renderer->Render();
	g_engineContext.m_renderer->Swap();
}
