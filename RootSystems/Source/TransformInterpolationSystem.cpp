#include <RootSystems/Include/TransformInterpolationSystem.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/PlayerSystem.h>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	TransformInterpolationSystem::TransformInterpolationSystem(ECS::World* p_world)
		: ECS::EntitySystem(p_world)
	{
		SetUsage<Transform>();
		SetUsage<Renderable>();
		SetUsage<PlayerComponent>();
	}

	void TransformInterpolationSystem::Init()
	{
		m_transforms.Init(m_world->GetEntityManager());
	}

	void TransformInterpolationSystem::Begin()
	{

	}

	void TransformInterpolationSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transforms.Get(p_entity);
		
		// Ignore the local player.
		if (p_entity == m_world->GetTagManager()->GetEntityByTag("Player"))
		{
			transform->m_renderingPosition = transform->m_position;
			return;
		}

		
		const float HIGH = 4.0f;
		const float LOW = 0.01f;
		const float BASE = 0.10f;

		// If the distance between rendering position and actual position is too large, snap to the position instantly.
		// If the distance is too small, also snap to the position.
		// Otherwise, exponentially decrease the distance each frame.
		glm::vec3 displacement = transform->m_position - transform->m_renderingPosition;
		glm::mediump_float sqDistance = glm::dot(displacement, displacement);
		
		transform->m_renderingPosition += BASE * displacement;
		/*
		if (sqDistance > HIGH)
			transform->m_renderingPosition = transform->m_position;
		else if (sqDistance > LOW)
			transform->m_renderingPosition += BASE * displacement;
		*/


		if (p_entity == m_world->GetTagManager()->GetEntityByTag("Player"))
		{
			g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Displacement: (%f, %f, %f) : %f", displacement.x, displacement.y, displacement.z, std::sqrt(sqDistance));

			if (sqDistance >= HIGH)
				g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Snap: Too big");
			else if (sqDistance <= LOW)
				g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Snap: Too small");
			else
				g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Interpolating distance: %f", BASE * displacement);
		}
	}

	void TransformInterpolationSystem::End()
	{
		
	}
}