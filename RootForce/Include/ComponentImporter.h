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
		case RootForce::ComponentType::RENDERABLE:
			{
				RootForce::Renderable* renderable = p_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(p_entity);
				
				const YAML::Node* modelNode = p_node.FindValue("Model");
				if(modelNode != nullptr)
				{
					std::string model;
					p_node["Model"] >> model;
					g_engineContext.m_resourceManager->LoadCollada(model);
					renderable->m_model = g_engineContext.m_resourceManager->GetModel(model);
				}

				const YAML::Node* materialNode = p_node.FindValue("Material");
				if(materialNode != nullptr)
				{
					const YAML::Node* materialNameNode = materialNode->FindValue("Name");
					if(materialNameNode != nullptr)
					{
						std::string materialName;
						p_node["Material"]["Name"] >> materialName;
						renderable->m_material = g_engineContext.m_resourceManager->GetMaterial(materialName);
					}

					const YAML::Node* effectNode = materialNode->FindValue("Effect");
					if(effectNode != nullptr)
					{
						// Load effect.
						std::string effect;
						p_node["Material"]["Effect"] >> effect;
						g_engineContext.m_resourceManager->LoadEffect(effect);
						if(renderable->m_material)
						{
							renderable->m_material->m_effect = g_engineContext.m_resourceManager->GetEffect(effect);

							// Allocate memory for this renderable's uniforms.
							//renderable->m_material->m_params = g_engineContext.m_renderer->CreateEffectParams();
							//renderable->m_material->m_params->AllocateParams(renderable->m_material->m_effect);
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Trying to set effect on a renderable that doesn't have a material!");
						}
					}

					const YAML::Node* diffuseNode = materialNode->FindValue("Diffuse");
					if(diffuseNode != nullptr)
					{
						std::string diffuse;
						p_node["Material"]["Diffuse"] >> diffuse;
						if(renderable->m_material)
						{
							renderable->m_material->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture(diffuse, Render::TextureType::TEXTURE_2D);
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Trying to set diffuse texture on a renderable that doesn't have a material!");
						}
					}
					const YAML::Node* specularNode = materialNode->FindValue("Specular");
					if(specularNode != nullptr)
					{
						std::string specular;
						p_node["Material"]["Specular"] >> specular;
						renderable->m_material->m_specularMap = g_engineContext.m_resourceManager->LoadTexture(specular, Render::TextureType::TEXTURE_2D);
					}
					const YAML::Node* normalNode = materialNode->FindValue("Normal");
					if(normalNode != nullptr)
					{
						std::string normal;
						p_node["Material"]["Normal"] >> normal;
						renderable->m_material->m_normalMap = g_engineContext.m_resourceManager->LoadTexture(normal, Render::TextureType::TEXTURE_2D);
					}
				}

				
			}
			break;
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
		case RootForce::ComponentType::POINTLIGHT:
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
		case RootForce::ComponentType::PLAYERCONTROL:
			{
				RootForce::PlayerControl* input = p_world->GetEntityManager()->CreateComponent<RootForce::PlayerControl>(p_entity);
				p_node["Speed"] >> input->m_speed;
			}
			break;
		case RootForce::ComponentType::PHYSICS:
			{
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