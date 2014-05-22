#include <RootSystems/Include/ParticleSystem.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootSystems/Include/RenderingSystem.h>
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
		Transform* trans = m_transforms.Get(p_entity);

		for(unsigned i = 0; i < emitter->m_particleSystems.size(); i++)
		{
			emitter->m_particleSystems[i]->m_params[Render::Semantic::TRANSPOSITION] = &trans->m_position;
			emitter->m_particleSystems[i]->m_params[Render::Semantic::ALIVE] = &emitter->m_alive;

			auto updateTechnique = emitter->m_particleSystems[i]->m_material->m_effect->GetTechniques()[0];

			g_engineContext.m_renderer->SetParticleUniforms(updateTechnique.get(), emitter->m_particleSystems[i]->m_params);

			updateTechnique->Apply();

			for(unsigned j = 0; j < updateTechnique->GetPrograms().size(); ++j)
			{
				updateTechnique->GetPrograms()[j]->Apply();

				emitter->m_systems[i]->Update();
			}

			// Simple sort of particle based on water height.
			unsigned pass = 0;
			ECS::Entity* waterEnt = m_world->GetTagManager()->GetEntityByTag("Water");
			if(waterEnt)
			{
				RootForce::Transform* waterTrans = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(waterEnt);

				if(trans->m_position.y > waterTrans->m_position.y)
					pass = RenderPass::RENDERPASS_PARTICLES1;
				else
					pass = RenderPass::RENDERPASS_PARTICLES0;
			}
			else
			{
				pass = RenderPass::RENDERPASS_PARTICLES1;
			}


			Render::RenderJob job;
			job.m_mesh = emitter->m_systems[i]->GetMesh();
			job.m_material = emitter->m_particleSystems[i]->m_material;
			job.m_renderPass = pass;
			job.m_forward = true;
			job.m_params[Render::Semantic::POSITION] = &trans->m_position;
			job.m_position = trans->m_position;

			g_engineContext.m_renderer->AddRenderJob(job);
		}
	}

	void ParticleSystem::End()
	{
		g_engineContext.m_renderer->EndTransform();
	}
}