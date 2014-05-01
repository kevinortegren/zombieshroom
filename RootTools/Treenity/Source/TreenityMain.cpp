#include "Treenity.h"
#include <RootTools/Treenity/Include/TreenityMain.h>
#include <QtWidgets/QApplication>
#include <SDL2/SDL.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootTools/Treenity/Include/EngineActions.h>

#include <RootEngine/Include/RootEngine.h>
#include <RootEngine/InputManager/Include/InputInterface.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

#include <RootSystems/Include/Components.h>

#include <RootTools/Treenity/Include/ComponentImporter.h>

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
	// Load & Initialize Root Engine.
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

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_TIMER) != 0) 
	{
		throw std::runtime_error("Failed to initialize SDL");
	}

	m_treenityEditor.CreateOpenGLContext();
	m_treenityEditor.SetEngineInterface(&m_engineActions);

	// Initialize components and preallocate memory.
	RootForce::ComponentType::Initialize();
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Renderable>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Transform>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PointLight>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Camera>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::HealthComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerControl>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::ControllerActions>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Physics>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::NetworkComponent>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::LookAtBehavior>(20);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::ThirdPersonBehavior>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Script>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Collision>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::CollisionResponder>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Animation>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::ParticleEmitter>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::TDMRuleSet>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerActionComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerPhysics>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::StateComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Shadowcaster>(1);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::DirectionalLight>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ClientComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ServerInformationComponent>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Ragdoll>(100);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::WaterCollider>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::AbilitySpawnComponent>(100);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::TryPickupComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::SoundComponent>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::TimerComponent>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::FollowComponent>(1000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::HomingComponent>(1000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::RayComponent>(1000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::DamageAndKnockback>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Scalable>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::StatChange>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::KillAnnouncement>(10);

	m_renderingSystem = new RootForce::RenderingSystem(&m_world);
	m_renderingSystem->SetRendererInterface(g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem);

	m_world.GetEntityImporter()->SetImporter(Importer);
	m_world.GetEntityImporter()->Import(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\ColorCube3.0.world");

	g_engineContext.m_renderer->SetAmbientLight(glm::vec4(1,1,1,1));

	// Display treenity editor.
	m_treenityEditor.show();
}

TreenityMain::~TreenityMain()
{
	delete m_renderingSystem;

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
	m_world.SetDelta(dt);

	HandleEvents();

	auto msgs = m_world.GetMessages();
	for(auto itr = msgs.begin(); itr != msgs.end(); ++itr) {

		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Message Type %d - Entity ID: %d - Component Type: %d", itr->m_type, itr->m_entity->GetId(), itr->m_compType);
	}

	m_world.GetEntityManager()->CleanUp();

	g_engineContext.m_renderer->Clear();

	//m_renderingSystem->Process();
	
	//g_engineContext.m_renderer->Render();
	g_engineContext.m_renderer->Swap();
}

