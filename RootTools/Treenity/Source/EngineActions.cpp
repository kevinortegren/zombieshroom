#include <RootTools/Treenity/Include/EngineActions.h>

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

}

// Transform
void EngineActions::TranslateEntity(ECS::Entity* p_entity, const glm::vec3& p_translation)
{

}
