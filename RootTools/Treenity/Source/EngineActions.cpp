#include <RootTools/Treenity/Include/EngineActions.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/RenderingSystem.h>

EngineActions::EngineActions(ECS::World* p_world)
	: m_world(p_world)
{

}

// Entity
ECS::Entity* EngineActions::CreateEntity()
{
	ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
	RootForce::Transform* transform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(entity);

	return entity;
}

void EngineActions::DeleteEntity(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->RemoveEntity(p_entity);
}

// Transform
void EngineActions::SetPosition(ECS::Entity* p_entity, const glm::vec3& p_position)
{
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	transform->m_position = p_position;
}

void EngineActions::TranslateEntity(ECS::Entity* p_entity, const glm::vec3& p_translation)
{
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	transform->m_position += p_translation;
}

void EngineActions::SetOrientation(ECS::Entity* p_entity, const RootForce::Orientation& p_orientation)
{
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	transform->m_orientation = p_orientation;
}

void EngineActions::SetScale(ECS::Entity* p_entity, const glm::vec3& p_scale)
{
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	transform->m_scale = p_scale;
}

const glm::vec3& EngineActions::GetPosition(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity)->m_position;
}

RootForce::Orientation& EngineActions::GetOrientation(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity)->m_orientation;
}

const glm::vec3& EngineActions::GetScale(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity)->m_scale;
}


// Renderable
void EngineActions::AddRenderable(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(p_entity);
}

void EngineActions::RemoveRenderable(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::Renderable>(p_entity);
}
