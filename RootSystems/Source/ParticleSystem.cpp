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

		m_updateEffect = g_engineContext.m_resourceManager->LoadEffect("ParticleUpdate");
	}

	void ParticleSystem::Init()
	{
		m_emitters.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void ParticleSystem::Begin()
	{
		auto technique = m_updateEffect->GetTechniques()[0];

		// Update delta time in the gpu update technique.

		m_perUpdateVariables.dt = m_world->GetDelta();
		technique->BufferUniforms(1, sizeof(m_perUpdateVariables), &m_perUpdateVariables);

		glBindBufferBase(GL_UNIFORM_BUFFER, 3, technique->GetUniformBuffer()->GetBufferId());

		technique->GetPrograms()[0]->Apply();
	}

	void ParticleSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		auto emitter = m_emitters.Get(p_entity);

		emitter->m_system->Update(m_world->GetDelta());

		//TODO: Use a new system for more effiecent processing.
		//Render::RenderJob job;
		//job.m_mesh = emitter->m_system->GetMesh();
		//job.m_material = emitter->m_material;

		//TODO: Add flag for transform feedback.

		//TODO: Set uniforms for the emitter job from transform when moving an emitter.
		//job.m_uniforms = transform.

		//g_engineContext.m_renderer->AddRenderJob(job);
	}

	void ParticleSystem::End()
	{

	}
}