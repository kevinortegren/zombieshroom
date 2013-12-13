#include <PhysicsSystem.h>


namespace RootForce
{
	void PhysicsSystem::Init()
	{
		m_physicsAccessor.Init(m_world->GetEntityManager());
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
			PhysicsAccessor* accessor = m_physicsAccessor.Get(p_entity);
			if((accessor->m_handle) == nullptr || *(accessor->m_handle) == -1)
			{
				//m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "Entity %i has no physicsaccessor", p_entity->GetId());
				return;
			}
		

			transform->m_position = m_physics->GetPos(*(accessor->m_handle));
			transform->m_orientation.SetOrientation(m_physics->GetOrientation(*(accessor->m_handle)));
			
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
}