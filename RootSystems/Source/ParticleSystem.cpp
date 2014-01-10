#include <RootSystems/Include/ParticleSystem.h>

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
		g_engineContext.m_renderer->BeginTransform(m_world->GetDelta());
	}

	void ParticleSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		ParticleEmitter* emitter = m_emitters.Get(p_entity);

		auto updateTechnique = emitter->m_material->m_effect->GetTechniques()[0];
		updateTechnique->Apply();

		for(unsigned i = 0; i <updateTechnique->GetPrograms().size(); ++i)
		{
			updateTechnique->GetPrograms()[i]->Apply();

			emitter->m_system->Update();
		}

		Render::RenderJob job;
		job.m_mesh = emitter->m_system->GetMesh();
		job.m_material = emitter->m_material;
		job.m_flags = Render::RenderFlags::RENDER_TRANSFORMFEEDBACK;
		
		g_engineContext.m_renderer->AddRenderJob(job);
	}

	void ParticleSystem::End()
	{
		g_engineContext.m_renderer->EndTransform();
	}
}