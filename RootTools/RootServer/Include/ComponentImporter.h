#pragma once

#include <Utility\ECS\Include\World.h>
#include <RootSystems\Include\Components.h>

#include <yaml-cpp\yaml.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

static void Importer(ECS::World* p_world, int p_type, ECS::Entity* p_entity, const YAML::Node& p_node)
{
	switch(p_type)
	{
		case RootForce::ComponentType::TRANSFORM:
			{
				RootForce::Transform* transform = p_world->GetEntityManager()->CreateComponent<RootForce::Transform>(p_entity);

				glm::vec3 position;
				p_node["Position"][0] >> position.x;
				p_node["Position"][1] >> position.y;
				p_node["Position"][2] >> position.z;

				transform->m_position = position;

				glm::vec3 rotation;
				p_node["Rotation"][0] >> rotation.x;
				p_node["Rotation"][1] >> rotation.y;
				p_node["Rotation"][2] >> rotation.z;

				transform->m_orientation.SetOrientation(rotation.x, rotation.y, rotation.z);

				glm::vec3 scale;
				p_node["Scale"][0] >> scale.x;
				p_node["Scale"][1] >> scale.y;
				p_node["Scale"][2] >> scale.z;

				transform->m_scale = scale;
			}
			break;
		case RootForce::ComponentType::PLAYERCONTROL:
			{
				RootForce::PlayerPhysics* playphys = p_world->GetEntityManager()->CreateComponent<RootForce::PlayerPhysics>(p_entity);
				p_node["Speed"] >> playphys->MovementSpeed;
			}
			break;
		case RootForce::ComponentType::PHYSICS:
			{
				RootForce::Physics* physics = p_world->GetEntityManager()->CreateComponent<RootForce::Physics>(p_entity);
				p_node["Mass"] >> physics->m_mass;

				/*RootForce::PhysicsAccessor* physaccessor = p_world->GetEntityManager()->CreateComponent<RootForce::PhysicsAccessor>(p_entity);
				float mass, maxSpeed, stepHeight, modelHeight;
				int type;
				std::string modelHandle;
				RootForce::Transform* temp = p_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
				glm::quat rotation =  temp->m_orientation.GetQuaternion();
				p_node["Type"] >> type;
				p_node["Mass"] >> mass;
				p_node["ModelHandle"] >> modelHandle;
				if(type == RootEngine::Physics::PhysicsType::TYPE_PLAYER) 
				{
					p_node["StepHeight"] >> stepHeight;
					p_node["ModelHeight"] >> modelHeight;
					p_node["MaxSpeed"] >> maxSpeed;
					physaccessor->m_handle = g_engineContext.m_physics->AddPlayerObjectToWorld(modelHandle, p_entity->GetId(), temp->m_position, rotation, mass, maxSpeed, modelHeight, stepHeight, nullptr);
				}
				else if (type == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC)
				{
					physaccessor->m_handle = g_engineContext.m_physics->AddDynamicObjectToWorld(modelHandle, p_entity->GetId(), temp->m_position, rotation, mass);
				}
				else if (type == RootEngine::Physics::PhysicsType::TYPE_STATIC)
				{
					physaccessor->m_handle = g_engineContext.m_physics->AddStaticObjectToWorld(p_entity->GetId());
					g_engineContext.m_physics->BindMeshShape(*(physaccessor->m_handle), modelHandle, temp->m_position, temp->m_orientation.GetQuaternion(), 0);
				}*/
			}
			break;
		case RootForce::ComponentType::COLLISION:
			{
				RootForce::Collision* collision = p_world->GetEntityManager()->CreateComponent<RootForce::Collision>(p_entity);
				std::string meshHandle;
				
				p_node["MeshHandle"] >> meshHandle;
				collision->m_meshHandle = meshHandle;
			}
			break;
		default:
			break;
	}
}