#pragma once

#include <RootEngine/Render/Include/Renderer.h>
#include <Utility/ECS/Include/World.h>
#include <Camera.h>
#include <Utility/ECS/Include/Shared/Transform.h>

namespace RootForce
{
	class CameraSystem : public ECS::EntitySystem
	{
	public:
		CameraSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world) 
		{
			SetUsage<Camera>();
			SetUsage<Transform>();
			m_renderer = nullptr;
		}

		void Init()
		{

		}

		void Begin()
		{

		}

		void ProcessEntity(ECS::Entity* p_entity)
		{
			if(m_renderer)
			{
				Transform* transform = m_transforms.Get(p_entity);
				Camera* camera = m_cameras.Get(p_entity);

				Orientation tempOrientation = transform->m_orientation;
				tempOrientation.Yaw(180.0f);
				glm::mat4 tempWorldMatrix;
				tempWorldMatrix = glm::translate(glm::mat4(1.0f), transform->m_position);
				tempWorldMatrix = glm::rotate(tempWorldMatrix, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
				tempWorldMatrix = glm::scale(tempWorldMatrix, transform->m_scale);
				glm::mat4 viewMatrix = glm::inverse(tempWorldMatrix);

				m_renderer->SetViewMatrix(viewMatrix);
				glm::mat4 projectionMatrix = glm::perspectiveFov<float>(camera->m_fov, m_renderer->GetWidth(), m_renderer->GetHeight(), camera->m_near, camera->m_far);
				m_renderer->SetProjectionMatrix(projectionMatrix);
			}
		}

		void End()
		{

		}

		void SetRendererInterface(Render::RendererInterface* p_renderer)
		{
			m_renderer = p_renderer;
		}

	private:
		Render::RendererInterface* m_renderer;
		ECS::ComponentMapper<Camera> m_cameras;
		ECS::ComponentMapper<Transform> m_transforms;
	};
}