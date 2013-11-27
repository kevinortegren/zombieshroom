#include <ECS/World.h>
#include <RootForce/Include/Transform.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <gtest/gtest.h>

using RootForce::Transform;

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

	void Init();

	void Begin()
	{
		// Will be executed before any processing occurs in each frame.
	}

	void ProcessEntity(ECS::Entity* p_entity)
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

class ECSTest : public testing::Test
{
protected:
	void SetUp()
	{
		a = 1;
	}

	void TearDown()
	{

	}

	ECS::World m_world;
	int a;
	ECS::Entity* m_entity1;
};

TEST_F(ECSTest, CreateEntity) 
{
	ECS::Entity* m_entity1 = m_world.GetEntityManager()->CreateEntity();

	ASSERT_TRUE(m_entity1 != NULL);
	EXPECT_EQ(0, m_entity1 ->GetId());
	EXPECT_EQ(1, this->m_world.GetEntityManager()->GetNumEntities());

	ECS::Entity*m_entity2 = m_world.GetEntityManager()->CreateEntity();

	ASSERT_TRUE(m_entity2 != NULL);
	EXPECT_EQ(1, m_entity2->GetId());
	EXPECT_EQ(2, this->m_world.GetEntityManager()->GetNumEntities());

	EXPECT_EQ(1, this->a);
}

TEST_F(ECSTest, RemoveEntity) 
{
	ECS::Entity* m_entity1 = m_world.GetEntityManager()->CreateEntity();

	this->m_world.GetEntityManager()->RemoveEntity(m_entity1);
	EXPECT_EQ(0, this->m_world.GetEntityManager()->GetNumEntities());
}

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
