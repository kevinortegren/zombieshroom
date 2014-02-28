#include <RootSystems/Include/CameraSystem.h>
#include <RootEngine/Physics/Include/RootPhysics.h>

namespace RootForce
{
	void CameraSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transforms.Get(p_entity);
		Camera* camera = m_cameras.Get(p_entity);

		Orientation tempOrientation = transform->m_orientation;
		tempOrientation.Yaw(180.0f);
		glm::mat4 tempWorldMatrix;
		/*#ifndef COMPILE_LEVEL_EDITOR
		if(transform->m_position.y <= m_world->GetStorage()->GetValueAsFloat("WaterHeight") + 8.0f)
			transform->m_position.y = m_world->GetStorage()->GetValueAsFloat("WaterHeight") + 8.0f;
		#endif*/
		glm::mat4 translation = glm::translate(glm::mat4(1), transform->m_position);
		glm::mat4 rotation = glm::rotate(translation, tempOrientation.GetAngle(), tempOrientation.GetAxis());
		glm::mat4 viewMatrix = glm::inverse(rotation);

		glm::mat4 projectionMatrix = glm::perspectiveFov<float>(camera->m_frustum.m_fov, (float)m_engineContext->m_renderer->GetWidth(), (float)m_engineContext->m_renderer->GetHeight(), camera->m_frustum.m_near, camera->m_frustum.m_far);

		camera->m_frustum.RecalculatePlanesEx(viewMatrix, projectionMatrix);
		camera->m_viewMatrix = viewMatrix;

		if(m_world->GetTagManager()->GetEntityByTag("Camera") == p_entity)
		{
			ECS::Entity* skybox = m_world->GetTagManager()->GetEntityByTag("Skybox");
			if(skybox)
			{
				m_world->GetEntityManager()->GetComponent<RootForce::Transform>(skybox)->m_position = transform->m_position;
			}
			m_engineContext->m_renderer->SetViewMatrix(viewMatrix);
			m_engineContext->m_renderer->SetProjectionMatrix(projectionMatrix);
		}
	}
#ifndef COMPILE_LEVEL_EDITOR
	void ThirdPersonBehaviorSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transforms.Get(p_entity);
		ThirdPersonBehavior* thirdPersonBehavior = m_thirdPersonBehaviors.Get(p_entity);

		if(thirdPersonBehavior->m_targetTag != "")
		{
			ECS::Entity* target = m_world->GetTagManager()->GetEntityByTag(thirdPersonBehavior->m_targetTag);
			if(target)
			{
				Transform* targetTransform = m_world->GetEntityManager()->GetComponent<Transform>(target);
				if(targetTransform)
				{
					//Move the entity
					glm::vec3 targetPosition = targetTransform->m_position;
					if(thirdPersonBehavior->m_rotateWithTarget)
					{
						Orientation tOrientation = targetTransform->m_orientation;
						glm::vec3 localDisplacement(0.0f);
						localDisplacement.z = -thirdPersonBehavior->m_distance;
						glm::vec3 worldDisplacement;
						worldDisplacement = tOrientation.GetRight() * -localDisplacement.x + tOrientation.GetUp() * localDisplacement.y + tOrientation.GetFront() * localDisplacement.z;
						float distFrac = 1.0f;
						if(thirdPersonBehavior->m_checkWithRay)
							distFrac = m_engineContext->m_physics->RayTest(targetPosition, targetPosition + worldDisplacement);
						transform->m_position = targetPosition + worldDisplacement*distFrac;
						
					}
					else
					{
						transform->m_position = targetPosition + glm::vec3(0,25.0f,0.0f);
					}
				}
				else
				{
					m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "ThirdPersonBehavior target is missing a transform component!");
				}
			}
		}
	}
#endif
}
