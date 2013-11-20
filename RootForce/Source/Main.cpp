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


	// ECS Test
	
	testSystem = world.GetSystemManager()->CreateSystem<TestComponentSystem>("test");
	testSystem->SetUsage<TestComponent>(true);

	testSystemTwo = world.GetSystemManager()->CreateSystem<TestComponentSystem>("test2");
	testSystemTwo->SetUsage<TestComponent>(true);
	testSystemTwo->SetUsage<TestComponentTwo>(true);

	std::shared_ptr<ECS::Entity> entity = world.GetEntityManager()->CreateEntity();
	std::shared_ptr<ECS::Entity> entity2 = world.GetEntityManager()->CreateEntity();
	
	std::shared_ptr<TestComponent> testComp1e1 = world.GetEntityManager()->CreateComponent<TestComponent>(entity);
	testComp1e1->data = 57.0f;

	std::shared_ptr<TestComponent> testComp1e2 = world.GetEntityManager()->CreateComponent<TestComponent>(entity2);
	testComp1e2->data = 333333.0f;

	world.GetSystemManager()->InitializeSystems();

	testSystem->Process();

	//world.GetEntityManager()->RemoveComponent<TestComponent>(entity);
	//world.GetEntityManager()->RemoveAllComponents(entity);
	world.GetEntityManager()->RemoveAllComponentsOfType<TestComponent>();

	testSystem->Process();

	int a = 0;
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
