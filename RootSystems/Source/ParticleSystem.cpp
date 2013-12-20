#include <RootSystems\Include\ParticleSystem.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	ParticleSystem::ParticleSystem(ECS::World* p_world)
		: ECS::EntitySystem(p_world)
	{
		SetUsage<ParticleEmitter>();
		SetUsage<Transform>();
	}

	void ParticleSystem::Init()
	{
		m_emitters.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void ParticleSystem::Begin()
	{
		dt = m_world->GetDelta();
	}

	void ParticleSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		ParticleEmitter* emitter = m_emitters.Get(p_entity);

		std::shared_ptr<Render::TechniqueInterface> updateTech = emitter->m_material->m_effect->GetTechniques()[0];	
		updateTech->GetUniformBuffer()->BufferData(1, sizeof(float), &dt);
		updateTech->Apply();

		updateTech->GetPrograms()[0]->Apply();

		emitter->m_system->Update(m_world->GetDelta());
	}

	void ParticleSystem::End()
	{

	}
}