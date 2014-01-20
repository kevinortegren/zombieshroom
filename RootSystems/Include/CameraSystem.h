#pragma once

#include <RootEngine/Render/Include/Renderer.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Camera.h>
#include <RootSystems/Include/Transform.h>

#include <RootEngine/Include/GameSharedContext.h>

namespace RootForce
{
	class CameraSystem : public ECS::EntitySystem
	{
	public:
		CameraSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world), m_engineContext(p_engineContext)
		{
			SetUsage<Camera>();
			SetUsage<Transform>();
		}

		void Init()
		{
			m_cameras.Init(m_world->GetEntityManager());
			m_transforms.Init(m_world->GetEntityManager());
		}

		void ProcessEntity(ECS::Entity* p_entity)
		{
			if(m_world->GetTagManager()->GetEntityByTag("Camera") == p_entity)
			{
				Transform* transform = m_transforms.Get(p_entity);
				Camera* camera = m_cameras.Get(p_entity);

				ECS::Entity* skybox = m_world->GetTagManager()->GetEntityByTag("Skybox");
				if(skybox)
				{
					m_world->GetEntityManager()->GetComponent<RootForce::Transform>(skybox)->m_position = transform->m_position;
				}

				Orientation tempOrientation = transform->m_orientation;
				tempOrientation.Yaw(180.0f);
				glm::mat4 tempWorldMatrix;
				tempWorldMatrix = glm::translate(glm::mat4(1.0f), transform->m_position);
				tempWorldMatrix = glm::rotate(tempWorldMatrix, tempOrientation.GetAngle(), tempOrientation.GetAxis());
				tempWorldMatrix = glm::scale(tempWorldMatrix, transform->m_scale);
				glm::mat4 viewMatrix = glm::inverse(tempWorldMatrix);

				glm::mat4 projectionMatrix = glm::perspectiveFov<float>(camera->m_fov, (float)m_engineContext->m_renderer->GetWidth(), (float)m_engineContext->m_renderer->GetHeight(), camera->m_near, camera->m_far);

				m_engineContext->m_renderer->SetViewMatrix(viewMatrix);
				m_engineContext->m_renderer->SetProjectionMatrix(projectionMatrix);
			}
		}

	private:
		RootEngine::GameSharedContext* m_engineContext;
		ECS::ComponentMapper<Camera> m_cameras;
		ECS::ComponentMapper<Transform> m_transforms;
	};

	struct LookAtBehavior : public ECS::Component<LookAtBehavior>
	{
		LookAtBehavior(){ m_targetTag = ""; m_displacement = glm::vec3(0.0f); }
		std::string m_targetTag;
		glm::vec3 m_displacement;
	};

	class LookAtSystem : public ECS::EntitySystem
	{
		//This system reorients all entities that have lookat behaviors
		//so that they always point to their respective targets
	public:
		LookAtSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world) , m_engineContext(p_engineContext)
		{
			SetUsage<Transform>();
			SetUsage<LookAtBehavior>();
		}

		void Init()
		{
			m_lookAtBehaviors.Init(m_world->GetEntityManager());
			m_transforms.Init(m_world->GetEntityManager());
		}

		void ProcessEntity(ECS::Entity* p_entity)
		{
			Transform* transform = m_transforms.Get(p_entity);
			LookAtBehavior* lookAtBehavior = m_lookAtBehaviors.Get(p_entity);

			if(lookAtBehavior->m_targetTag != "")
			{
				ECS::Entity* target = m_world->GetTagManager()->GetEntityByTag(lookAtBehavior->m_targetTag);
				if(target)
				{
					Transform* targetTransform = m_world->GetEntityManager()->GetComponent<Transform>(target);
					if(targetTransform)
					{
						glm::vec3 targetPosition;
						Orientation* tOr = &targetTransform->m_orientation;
						targetPosition = targetTransform->m_position;
						targetPosition += tOr->GetRight() * -lookAtBehavior->m_displacement.x + tOr->GetUp() * lookAtBehavior->m_displacement.y + tOr->GetFront() * lookAtBehavior->m_displacement.z;
						transform->m_orientation.LookAt(targetPosition - transform->m_position, tOr->GetUp());
					}
					else
					{
						m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "LookAtBehavior target is missing a transform component!");
					}
				}
			}
			else
			{
				transform->m_orientation.LookAt(glm::vec3(0.0f) - transform->m_position, transform->m_orientation.GetUp());
			}
		}

	private:
		RootEngine::GameSharedContext* m_engineContext;
		ECS::ComponentMapper<Transform> m_transforms;
		ECS::ComponentMapper<LookAtBehavior> m_lookAtBehaviors;
	};
	
	struct ThirdPersonBehavior : ECS::Component<ThirdPersonBehavior>
	{
		ThirdPersonBehavior(){ m_targetTag = ""; m_distance = 12.0f;}
		std::string m_targetTag;
		glm::vec3 m_displacement;
		float m_distance;
	};

	class ThirdPersonBehaviorSystem : public ECS::EntitySystem
	{
		//This system positions an entity so that it follows another entity,
		//this is used for the camera.
	public:
		ThirdPersonBehaviorSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world), m_engineContext(p_engineContext)
		{
			SetUsage<Transform>();
			SetUsage<ThirdPersonBehavior>();
		}

		void Init()
		{
			m_thirdPersonBehaviors.Init(m_world->GetEntityManager());
			m_transforms.Init(m_world->GetEntityManager());
		}

		void ProcessEntity(ECS::Entity* p_entity)
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
						Orientation tOrientation = targetTransform->m_orientation;
						glm::vec3 localDisplacement(0.0f);
						localDisplacement.z = -thirdPersonBehavior->m_distance;
						glm::vec3 worldDisplacement;
						worldDisplacement = tOrientation.GetRight() * -localDisplacement.x + tOrientation.GetUp() * localDisplacement.y + tOrientation.GetFront() * localDisplacement.z;
						float distFrac = m_engineContext->m_physics->RayTest(targetPosition, targetPosition + worldDisplacement);
						transform->m_position = targetPosition + worldDisplacement*distFrac;
					}
					else
					{
						m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "ThirdPersonBehavior target is missing a transform component!");
					}
				}
			}
		}

	private:
		RootEngine::GameSharedContext* m_engineContext;
		ECS::ComponentMapper<Transform> m_transforms;
		ECS::ComponentMapper<ThirdPersonBehavior> m_thirdPersonBehaviors;
		glm::ivec2 m_deltaMouseMovement;
	};
}