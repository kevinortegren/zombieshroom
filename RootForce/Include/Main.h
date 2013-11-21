#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <RootEngine/Include/RootEngine.h>

#include "RootEngine/Include/ECS/World.h"
#include "RootEngine/Include/Logging/Logging.h"

struct Transform : public ECS::Component<Transform>
{
	float m_x;
	float m_y;
};

struct Player : public ECS::Component<Player>
{
	std::string m_name;
	float m_health;
};

struct GameLogicSystem : public ECS::ComponentSystem
{
	GameLogicSystem(ECS::World* p_world)
		: ECS::ComponentSystem(p_world) 
	{
		// Enabling use of Player/Transform components in the system.
		SetUsage<Player>();
		SetUsage<Transform>();
	}

	// Component mappers map player data to entities.
	ECS::ComponentMapper<Player> m_players;
	ECS::ComponentMapper<Transform> m_transforms;

	void Init()
	{
		// Init will fill the map with entity data.
		m_players.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void Begin()
	{
		// Will be executed before any processing occurs in each frame.
	}

	void ProcessEntity(ECS::Entity* p_entity)
	{
		Player* player = m_players.Get(p_entity);
		Transform* transform = m_transforms.Get(p_entity);

		if( transform->m_y < 0)
		{
			player->m_health -= 10.0f;
		}

		if(player->m_health <= 0.0f)
		{
			Logging::GetInstance()->LogTextToConsole("Player %s is dead", player->m_name.c_str());
	
			m_world->GetEntityManager()->RemoveEntity(p_entity);
		}
	}

	void End()
	{
		// Will be executed after any processing occurs in each frame.
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
	RootEngine::Context m_engineContext;
};