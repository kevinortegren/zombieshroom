#pragma once

#include <memory>
#include <SDL2/SDL.h>

#include <ECS/ECS.h>

enum ComponentType
{
	TYPE_TEST
};

struct TestComponent : public ECS::Component<TestComponent>
{
	float data;
};

struct TestComponentTwo : public ECS::Component<TestComponentTwo>
{
	float data;
};

struct TestComponentSystem : public ECS::ComponentSystem
{
	TestComponentSystem(ECS::World* p_world)
		: ECS::ComponentSystem(p_world)
	{}

	ECS::ComponentMapper<TestComponent> m_testComps;

	void Init()
	{
		std::cout << "Init TestComponentSystem" << std::endl;

		m_testComps.Init(m_world->GetEntityManager());
	}

	void Begin()
	{
		std::cout << "Begin TestComponentSystem" << std::endl;
	}

	void ProcessEntity(std::shared_ptr<ECS::Entity> p_entity)
	{
		std::cout << "Entity " << m_testComps.Get(p_entity)->data << std::endl;
	}

	void End()
	{
		std::cout << "End TestComponentSystem" << std::endl;
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

	std::shared_ptr<ECS::ComponentSystem> testSystem;
	std::shared_ptr<ECS::ComponentSystem> testSystemTwo;


};