#include <Main.h>
#include <exception>

#undef main

int main(int argc, char* argv[]) 
{
	try 
	{
		Main m;
		m.Start();
	} 
	catch (std::exception& e) 
	{
		// TODO: Log exception message
		return 1;
	} 
	catch (...) 
	{
		// TODO: Log unknown exception message
		return 1;
	}
	
	return 0;
}



Main::Main() 
	: m_running(true) 
{

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) 
	{
		// TODO: Log error and throw exception (?)
	}

	// TODO: Set OpenGL attributes? This needs to be done before a window is opened, but responsibility suits the engine/renderer better.

	// TODO: Make these parameters more configurable.
	m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
			"Root Force",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			1280,
			720,
			SDL_WINDOW_OPENGL),
		SDL_DestroyWindow);
	if (m_window == nullptr) 
	{
		// TODO: Log error and throw exception (?)
	}

	// CreateSystem allocates and stores a system with a string handler.
	std::shared_ptr<ECS::ComponentSystem> system = world.GetSystemManager()->CreateSystem<TestComponentSystem>("System_Identifier");

	// Set usage enables use of the specified components in processing.
	system->SetUsage<TestComponent>(true);

	// CreateEntity allocates and stores a entity.
	std::shared_ptr<ECS::Entity> entity = world.GetEntityManager()->CreateEntity();
	std::shared_ptr<ECS::Entity> entity2 = world.GetEntityManager()->CreateEntity();
	
	// CreateComponent allocates and stores a specified component belonging to a entity.
	std::shared_ptr<TestComponent> testComp1 = world.GetEntityManager()->CreateComponent<TestComponent>(entity);
	testComp1->Data = 57.0f;

	// Initialize system sets up all the system for processing.
	world.GetSystemManager()->InitializeSystems();

	// Process will execute the logic flow.
	system->Process();

	// Removal methods.
	world.GetEntityManager()->RemoveComponent<TestComponent>(entity);
	world.GetEntityManager()->RemoveAllComponents(entity);
	world.GetEntityManager()->RemoveAllComponentsOfType<TestComponent>();

	system->Process();

	// Processing by requesting the system from the system manager.
	world.GetSystemManager()->GetSystem<TestComponentSystem>("System_Identifier")->Process();
}

Main::~Main() 
{
	SDL_Quit();
}

void Main::Start() 
{

	uint64_t old = SDL_GetPerformanceCounter();
	while (m_running)
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;

		// TODO: Poll and handle events
		// TODO: Update game state
		// TODO: Render and present game
		HandleEvents();

		//testSystem->Process();
		//testSystemTwo->Process();
	}
}

void Main::HandleEvents()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
   
	switch(event.type) {
   
		case SDL_QUIT:
			m_running = false;
			break;

		case SDL_KEYDOWN:
			break;

		case SDL_KEYUP:
			break;

		default:
			break;
		}
    }
}
