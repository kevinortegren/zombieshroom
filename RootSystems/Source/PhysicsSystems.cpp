#ifndef COMPILE_LEVEL_EDITOR

#include <PhysicsSystem.h>
#include <RootEngine/Include/GameSharedContext.h>

extern RootEngine::GameSharedContext g_engineContext;

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

			if(collision->m_handle)
			{
				transform->m_position = m_physics->GetPos(*(collision->m_handle));
				transform->m_orientation.SetOrientation(m_physics->GetOrientation(*(collision->m_handle)));
				if (m_physics->GetType(*collision->m_handle) != RootEngine::Physics::PhysicsType::TYPE_PLAYER)
				{
					// Players do not have a current velocity.
					accessor->m_velocity = m_physics->GetVelocity(*collision->m_handle);
				}
			}
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

	}



	void PhysicsTransformCorrectionSystem::Init()
	{
		m_physicsAccessor.Init(m_world->GetEntityManager());
		m_collisions.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void PhysicsTransformCorrectionSystem::Begin()
	{

	}

	void PhysicsTransformCorrectionSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		if(g_engineContext.m_physics && g_engineContext.m_logger)
		{
			Transform* transform = m_transforms.Get(p_entity);
			Collision* collision = m_collisions.Get(p_entity);

			Physics* accessor = m_physicsAccessor.Get(p_entity);

			if(collision->m_handle)
			{
				g_engineContext.m_physics->SetPosition(*collision->m_handle, transform->m_position);
				g_engineContext.m_physics->SetOrientation(*collision->m_handle, transform->m_orientation.GetQuaternion());
				if (g_engineContext.m_physics->GetType(*collision->m_handle) != RootEngine::Physics::PhysicsType::TYPE_PLAYER)
				{
					g_engineContext.m_physics->SetVelocity(*collision->m_handle, accessor->m_velocity);
				}
			}
		}
	}

	void PhysicsTransformCorrectionSystem:: End()
	{

	}

}

#endif