#include <RootSystems/Include/ScaleSystem.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Include/Logging/Logging.h>

extern RootEngine::GameSharedContext g_engineContext;
namespace RootForce
{
	ScaleSystem::ScaleSystem(ECS::World* p_world) 
		: ECS::EntitySystem(p_world), m_world(p_world)
	{
		SetUsage<RootForce::Transform>();
		SetUsage<RootForce::Scalable>();
	}

	ScaleSystem::~ScaleSystem()
	{
	}

	void ScaleSystem::Init()
	{
		m_transform.Init(m_world->GetEntityManager());
		m_scalable.Init(m_world->GetEntityManager());
	}

	void ScaleSystem::Begin()
	{
	}

	void ScaleSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		RootForce::Transform* transform = m_transform.Get(p_entity);
		RootForce::Scalable* scalable = m_scalable.Get(p_entity);

		float dt = m_world->GetDelta();
		float dtScale = scalable->ScaleSpeed * dt;
		
		if(scalable->CurScale < scalable->MaxScale)
		{
			transform->m_scale += glm::vec3(dtScale);
			scalable->CurScale += dtScale;
		}
	}

	void ScaleSystem::End()
	{
	}
}
