#include <LightSystem.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/RenderingSystem.h>

namespace RootForce
{	
	void PointLightSystem::Init()
	{
		m_plights.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
		m_test = false;
	}	

	void PointLightSystem::Begin()
	{
		//LightStressTest();

		m_lightCount = 0;
		
		// Fetch camera frustum.
		auto entity = m_world->GetTagManager()->GetEntityByTag("Camera");
		m_frustum = &m_world->GetEntityManager()->GetComponent<RootForce::Camera>(entity)->m_frustum;
	}

	void PointLightSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transforms.Get(p_entity);
		PointLight* pointLight = m_plights.Get(p_entity);

		float range = CalculateScale(pointLight);

		// Frustum cull lights.
		if(m_frustum->CheckSphere(transform->m_position, range))
		{
			glm::mat4 mt;		
			mt = glm::translate(glm::mat4(1.0f), transform->m_position);
			mt = glm::scale(mt, glm::vec3(range));

			Render::PointLight pl;
			pl.m_position = transform->m_position;
			pl.m_color = pointLight->m_color;
			pl.m_attenuation = pointLight->m_attenuation;
			pl.m_range = range;
			pl.m_transform = mt;

			m_context->m_renderer->AddPointLight(pl, m_lightCount);

			m_lightCount++;
		}	
	}

	float PointLightSystem::CalculateScale(PointLight* p_pl)
	{
		float sigma = 0.1f;
		float L = glm::max(glm::max(p_pl->m_color.x, p_pl->m_color.y), p_pl->m_color.z);
		
		return (L/p_pl->m_attenuation.y)/sigma;
	}

	void PointLightSystem::LightStressTest()
	{
		if(!m_test)
		{
			for (int i = 0; i < 64; i++) {

				float x = -sinf((float)i * ((8.0f * 3.14f) / 64.0f));
				float z = cosf((float)i * ((8.0f * 3.14f) / 64.0f));
				float y = 10.0f;
			  
				ECS::Entity* e = m_world->GetEntityManager()->CreateEntity();
				PointLight* pl = m_world->GetEntityManager()->CreateComponent<PointLight>(e);
				Transform* t = m_world->GetEntityManager()->CreateComponent<Transform>(e);

				pl->m_color = glm::vec4((i % 1 == 0) ? 1.0f : 0.0f, (i % 2 == 0) ? 1.0f : 0.0f, (i % 3 == 0) ? 1.0f : 0.0f, 1.0f);
				pl->m_attenuation = glm::vec3(0.0f, 0.1f, 0.02f);
				t->m_position = glm::vec3(x * i * 10.0f, y, z * i * 10.0f);

				m_world->GetEntityExporter()->Export("LightTest");
			}
			m_test = true;
		}
	}

	void PointLightSystem::End() 
	{
		
	}

	void DirectionalLightSystem::Init()
	{
		m_dlights.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void DirectionalLightSystem::Begin() 
	{ 
		m_lightCount = 0; 
	}

	void DirectionalLightSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transforms.Get(p_entity);
		DirectionalLight* directionalLigt = m_dlights.Get(p_entity);

		Render::DirectionalLight dl;
		dl.m_color = directionalLigt->m_color;
		dl.m_direction = transform->m_orientation.GetFront();

		m_context->m_renderer->AddDirectionalLight(dl, m_lightCount);

		m_lightCount++;
	}

	void DirectionalLightSystem::End() {}
}