#include <PhysicsSystem.h>

namespace RootForce
{
	void PhysicsSystem::Init()
	{
		m_physicsAccessor.Init(m_world->GetEntityManager());
		m_collisions.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void PhysicsSystem::Begin()
	{

	}

	void PhysicsSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		if(m_physics && m_logger)
		{
			Transform* transform = m_transforms.Get(p_entity);
			Collision* collision = m_collisions.Get(p_entity);

			Physics* accessor = m_physicsAccessor.Get(p_entity);

			transform->m_position = m_physics->GetPos(*(collision->m_handle));
			transform->m_orientation.SetOrientation(m_physics->GetOrientation(*(collision->m_handle)));
		}
	}

	void PhysicsSystem::End()
	{

	}

	void PhysicsSystem::SetPhysicsInterface(RootEngine::Physics::PhysicsInterface* p_physics)
	{
		m_physics = p_physics;
	}

	void PhysicsSystem::SetLoggingInterface(Logging::LoggingInterface* p_logger)
	{
		m_logger = p_logger;
	}

	void PhysicsSystem::AddStaticEntities()
	{
		ECS::GroupManager::GroupRange range = m_world->GetGroupManager()->GetEntitiesInGroup("Static");
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = itr->second;
			Collision* c = m_world->GetEntityManager()->GetComponent<Collision>(entity);
			Transform* t = m_world->GetEntityManager()->GetComponent<Transform>(entity);

			c->m_handle = m_physics->AddStaticObjectToWorld(c->m_meshHandle, entity->GetId(), t->m_position, t->m_orientation.GetQuaternion());
		}
	}
}