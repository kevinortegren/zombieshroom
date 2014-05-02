#include <RootTools/Treenity/Include/EngineActions.h>
#include <RootSystems/Include/RenderingSystem.h>

EngineActions::EngineActions(ECS::World* p_world)
	: m_world(p_world)
{

}

ECS::Entity* EngineActions::CreateEntity()
{
	return m_world->GetEntityManager()->CreateEntity();
}

void EngineActions::DeleteEntity(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->RemoveEntity(p_entity);
}

// Transform
void EngineActions::TranslateEntity(ECS::Entity* p_entity, const glm::vec3& p_translation)
{

}

void EngineActions::AddRenderable(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(p_entity);
}

void EngineActions::RemoveRenderable(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::Renderable>(p_entity);
}
