#pragma once

#include <Utility\ECS\Include\World.h>

#include <yaml-cpp\yaml.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

static void Importer(ECS::World* p_world, int p_type, ECS::Entity* p_entity, const YAML::Node& p_node)
{
	switch(p_type)
	{
		case 0:
			{
				RootForce::Renderable* renderable = p_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(p_entity);
				
				 const YAML::Node* effectNode = p_node.FindValue("Effect");
				 if(effectNode != nullptr)
				 {
					 // Load effect.
					 std::string effect;
					 p_node["Effect"] >> effect;
					 g_engineContext.m_resourceManager->LoadEffect(effect);
					 renderable->m_material.m_effect = g_engineContext.m_resourceManager->GetEffect(effect);

					 // Allocate memory for this renderables uniforms.
					renderable->m_material.m_params = g_engineContext.m_renderer->CreateEffectParams();
					renderable->m_material.m_params->AllocateParams(renderable->m_material.m_effect);
				 }

				 const YAML::Node* modelNode = p_node.FindValue("Model");
				 if(modelNode != nullptr)
				 {
					std::string model;
					 p_node["Model"] >> model;
					 g_engineContext.m_resourceManager->LoadCollada(model);
					 renderable->m_model = g_engineContext.m_resourceManager->GetModel(model);
				 }
				 const YAML::Node* diffuseNode = p_node.FindValue("Diffuse");
				 if(diffuseNode != nullptr)
				 {
					 std::string diffuse;
					 p_node["Diffuse"] >> diffuse;
					 renderable->m_material.m_diffuseMap = g_engineContext.m_resourceManager->GetTexture(diffuse);
				 }
			}
			break;
		case 1:
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
		case 2:
			{
				RootForce::PointLight* pointLight = p_world->GetEntityManager()->CreateComponent<RootForce::PointLight>(p_entity);

				glm::vec3 attenuation;
				p_node["Attenuation"][0] >> attenuation.x;
				p_node["Attenuation"][1] >> attenuation.y;
				p_node["Attenuation"][2] >> attenuation.z;

				pointLight->m_attenuation = attenuation;

				glm::vec4 color;
				p_node["Color"][0] >> color.x;
				p_node["Color"][1] >> color.y;
				p_node["Color"][2] >> color.z;
				p_node["Color"][3] >> color.w;

				pointLight->m_color = color;

				float range;
				p_node["Range"] >> range;

				pointLight->m_range = range;
			}
			break;
		case 3:
			{
				RootForce::PlayerInputControlComponent* input = p_world->GetEntityManager()->CreateComponent<RootForce::PlayerInputControlComponent>(p_entity);
				p_node["Speed"] >> input->m_speed;
			}
			break;
		case 4:
			{
				RootForce::PhysicsAccessor* physaccessor = p_world->GetEntityManager()->CreateComponent<RootForce::PhysicsAccessor>(p_entity);
				float mass, maxSpeed, stepHeight, modelHeight;
				int type;
				std::string modelHandle;
				RootForce::Transform* temp = p_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
				glm::quat rotation =  temp->m_orientation.GetQuaterion();
				p_node["Type"] >> type;
				p_node["Mass"] >> mass;
				p_node["ModelHandle"] >> modelHandle;
				if(type == RootEngine::Physics::PhysicsType::TYPE_PLAYER) 
				{
					p_node["StepHeight"] >> stepHeight;
					p_node["ModelHeight"] >> modelHeight;
					p_node["MaxSpeed"] >> maxSpeed;
					physaccessor->m_handle = g_engineContext.m_physics->AddPlayerObjectToWorld(modelHandle, &(temp->m_position.x), &(rotation.x), mass, maxSpeed, modelHeight, stepHeight);
				}
				else if (type == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC)
				{
					physaccessor->m_handle = g_engineContext.m_physics->AddDynamicObjectToWorld(modelHandle, &(temp->m_position.x), &(rotation.x), mass);
				}
				else if (type == RootEngine::Physics::PhysicsType::TYPE_STATIC)
				{
					g_engineContext.m_physics->AddStaticObjectToWorld(modelHandle, &(temp->m_position.x), &(rotation.x));
					physaccessor->m_handle[0] = -1;
				}

			}
			break;
		default:
			break;
	}
}