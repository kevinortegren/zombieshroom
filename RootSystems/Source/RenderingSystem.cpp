#include <RenderingSystem.h>
#include <RootEngine/Render/Include/Renderer.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	Renderable::~Renderable()
	{
		
	}

	void RenderingSystem::Init()
	{
		m_renderables.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void RenderingSystem::Begin()
	{

	}

	void RenderingSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transforms.Get(p_entity);
		Renderable* renderable = m_renderables.Get(p_entity);

		if(p_entity == m_world->GetTagManager()->GetEntityByTag("Player"))
			int o = 0;

		if(!renderable->m_material)
		{
			m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Renderable of Entity %i has no material", p_entity->GetId());
			return;
		}
		if(renderable->m_material->m_effect == nullptr)
		{
			m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Renderable of Entity %i has no effect", p_entity->GetId());
			return;
		}
		if(renderable->m_model == nullptr)
		{
			m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Renderable of Entity %i has no model", p_entity->GetId());
			return;
		}


		//m_matrices[p_entity].m_model = glm::translate(glm::mat4(1.0f), transform->m_position);

#ifdef COMPILE_LEVEL_EDITOR
		transform->m_interpolatedPosition = transform->m_position;
#endif

		m_matrices[p_entity].m_model = glm::translate(glm::mat4(1.0f), transform->m_interpolatedPosition);
		m_matrices[p_entity].m_model = glm::rotate(m_matrices[p_entity].m_model, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
		m_matrices[p_entity].m_model = glm::scale(m_matrices[p_entity].m_model, transform->m_scale);
		m_matrices[p_entity].m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(m_matrices[p_entity].m_model))));

		
		Render::RenderJob job;	
		job.m_mesh = renderable->m_model->m_meshes[0];
		job.m_shadowMesh = renderable->m_model->m_meshes[0];
		job.m_material = renderable->m_material;	
		job.m_params = renderable->m_params;
		job.m_forward = renderable->m_forward;
		job.m_refractive = renderable->m_refractive;
		job.m_params[Render::Semantic::MODEL] = &m_matrices[p_entity].m_model;
		job.m_params[Render::Semantic::NORMAL] = &m_matrices[p_entity].m_normal;
		job.m_renderPass = renderable->m_pass;
		job.m_position = transform->m_interpolatedPosition; //Added this for quicker sorting, as particles dont have a modelmatrix
		//Put forward object above or under water
		if((job.m_material->m_effect->GetTechniques().at(0)->m_flags & Render::TechniqueFlags::RENDER_DEFERRED1) == Render::TechniqueFlags::RENDER_DEFERRED1 || job.m_forward)
		{
			float waterHeight = m_world->GetStorage()->GetValueAsFloat("WaterHeight");
			if(transform->m_interpolatedPosition.y > waterHeight)
				job.m_renderPass = RenderPass::RENDERPASS_PARTICLES1;
			else if(transform->m_interpolatedPosition.y < waterHeight)
				job.m_renderPass = RenderPass::RENDERPASS_PARTICLES0;
		}

		m_renderer->AddRenderJob(job);

		if(renderable->m_shadowTech != Render::ShadowTechnique::SHADOW_NONE)
		{
			Render::ShadowJob sjob;
			sjob.m_technique = (Render::ShadowTechnique::ShadowTechnique)renderable->m_shadowTech;
			sjob.m_mesh = renderable->m_model->m_meshes[0];
			sjob.m_params = job.m_params;

			if(renderable->m_shadowTech == Render::ShadowTechnique::SHADOW_TRANSPARENT)
			{
				sjob.m_textures[Render::TextureSemantic::TextureSemantic::DIFFUSE] = renderable->m_material->m_textures[Render::TextureSemantic::TextureSemantic::DIFFUSE];
			}

			if(renderable->m_shadowTech == Render::ShadowTechnique::SHADOW_ANIMATED)
			{
				sjob.m_mesh = renderable->m_model->m_meshes[0];
			}

			m_renderer->AddShadowJob(sjob);			
		}
	}

	void RenderingSystem::End()
	{

	}

	void RenderingSystem::SetRendererInterface(Render::RendererInterface* p_renderer)
	{
		m_renderer = p_renderer;
	}

	void RenderingSystem::SetLoggingInterface(Logging::LoggingInterface* p_logger)
	{
		m_logger = p_logger;
	}
}