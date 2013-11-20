#pragma once

#include <memory>
#include <SDL2/SDL.h>

#include <ECS/World.h>

struct TestComponent : public ECS::Component<TestComponent>
{
	float Data;
};

struct TestComponentSystem : public ECS::ComponentSystem
{
	// Constructor.
	TestComponentSystem(ECS::World* p_world)
		: ECS::ComponentSystem(p_world) {}

	// Component Mappers.
	ECS::ComponentMapper<TestComponent> m_mapper;

	void Init()
	{
		std::cout << "Init ComponentSystem" << std::endl;

		m_mapper.Init(m_world->GetEntityManager());
	}

	void Begin()
	{
		std::cout << "Begin ComponentSystem" << std::endl;
	}

	void ProcessEntity(std::shared_ptr<ECS::Entity> p_entity)
	{
		std::cout << "Component Data Fetch " << m_mapper.Get(p_entity)->Data << std::endl;
	}

	void End()
	{
		std::cout << "End ComponentSystem\n" << std::endl;
	}
};

class Main {
public:
	Main();
	~Main();

	void Start();
private:

	void HandleEvents();

	bool m_running;
	std::shared_ptr<SDL_Window> m_window;

	ECS::World world;
};