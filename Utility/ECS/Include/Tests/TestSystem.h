#pragma once

#include <Utility/ECS/Include/EntitySystem.h>
#include <Utility\ECS\Include\Shared\Transform.h>
#include <RootEngine/Include/Logging/Logging.h>

using RootForce::Transform;

struct Player : public ECS::Component<Player>
{
	std::string m_name;
	float m_health;
};

struct GameLogicSystem : public ECS::EntitySystem
{
	GameLogicSystem(ECS::World* p_world)
		: ECS::EntitySystem(p_world) 
	{
		// Enabling use of Player/Transform components in the system.
		SetUsage<Player>();
		SetUsage<Transform>();
	}

	// Component mappers map player data to entities.
	ECS::ComponentMapper<Player> m_players;
	ECS::ComponentMapper<Transform> m_transforms;

	void Init();

	void Begin()
	{
		// Will be executed before any processing occurs in each frame.
	}

	void ProcessEntity(ECS::Entity* p_entity, float dt)
	{
		Player* player = m_players.Get(p_entity);
		Transform* transform = m_transforms.Get(p_entity);

		if( transform->m_position.y < 0)
		{
			player->m_health -= 10.0f;
		}

		if(player->m_health <= 0.0f)
		{
			//Logging::GetInstance()->LogTextToConsole("Player %s is dead", player->m_name.c_str());
	
			m_world->GetEntityManager()->RemoveEntity(p_entity);
		}
	}

	void End()
	{
		// Will be executed after any processing occurs in each frame.
	}
};



/*
//ECS::ComponentSystem* gameLogic = m_engineContext->GetWorld()->GetSystemManager()->CreateSystem<GameLogicSystem>("GameLogic");

	//E

	// CreateSystem allocates and stores a system with a string handler.

	// CreateEntity allocates and stores a entity.

	// CreateComponent allocates and stores a specified component belonging to a entity.
	Player* playerData = m_engineContext->GetWorld()->GetEntityManager()->CreateComponent<Player>(rolf);
	playerData->m_health = 10.0f;
	playerData->m_name = "Rolf";

	Transform* transformData = m_engineContext->GetWorld()->GetEntityManager()->CreateComponent<Transform>(rolf);
	transformData->m_x = 0.0f;
	transformData->m_y = -5.0f;

	// Initialize system sets up all the system for processing.
	m_engineContext->GetWorld()->GetSystemManager()->InitializeSystems();

	// Process will execute the logic flow.
	gameLogic->Process();
	*/
