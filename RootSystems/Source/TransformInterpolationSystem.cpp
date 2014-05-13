#ifndef COMPILE_LEVEL_EDITOR

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
		
		// Do not interpolate the local player.
		if (p_entity == m_world->GetTagManager()->GetEntityByTag("Player"))
		{
			transform->m_interpolatedPosition = transform->m_position;
			return;
		}
		// Do not interpolate non-players.
		if (m_world->GetEntityManager()->GetComponent<PlayerComponent>(p_entity) == nullptr)
		{
			transform->m_interpolatedPosition = transform->m_position;
			return;
		}
		
		// Exponentially decrease the distance between actual and interpolated position every frame.
		const float BASE = 0.50f;
		
		glm::vec3 displacement = transform->m_position - transform->m_interpolatedPosition;
		glm::mediump_float sqDistance = glm::dot(displacement, displacement);
		
		transform->m_interpolatedPosition += BASE * displacement;
	}

	void TransformInterpolationSystem::End()
	{
		
	}
}

#endif