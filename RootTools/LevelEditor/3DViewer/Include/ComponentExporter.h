#pragma once

#include <yaml-cpp\yaml.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

static void Exporter(YAML::Emitter& p_emitter, ECS::ComponentInterface* p_component, int p_type)
{
	switch(p_type)
	{
		case 0:
			{
				RootForce::Renderable* renderable = static_cast<RootForce::Renderable*>(p_component);	
				if(renderable->m_model != nullptr)
				{
					std::string s = g_engineContext.m_resourceManager->ResolveStringFromModel(renderable->m_model);
					p_emitter << YAML::Key << "Model" << YAML::Value << s;
				}
				if(renderable->m_material.m_effect != nullptr)
				{
					std::string s = g_engineContext.m_resourceManager->ResolveStringFromEffect(renderable->m_material.m_effect);
					p_emitter << YAML::Key << "Effect" << YAML::Value << s;
				}			
				if(renderable->m_material.m_diffuseMap != nullptr)
				{				
					std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material.m_diffuseMap);
					p_emitter << YAML::Key << "Diffuse" << YAML::Value << s;
				}	
				if(renderable->m_material.m_specularMap != nullptr)
				{
					std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material.m_specularMap);
					p_emitter << YAML::Key << "Specular" << YAML::Value << s;
				}
				if(renderable->m_material.m_normalMap != nullptr)
				{
					std::string s = g_engineContext.m_resourceManager->ResolveStringFromTexture(renderable->m_material.m_normalMap);
					p_emitter << YAML::Key << "Normal" << YAML::Value << s;
				}	
			}
			break;
		case 1:
			{
				RootForce::Transform* transform = static_cast<RootForce::Transform*>(p_component);
				glm::vec3 position = transform->m_position;
				glm::vec3 scale = transform->m_scale;
				glm::vec3 rotation = transform->m_orientation.GetAxis();

				p_emitter << YAML::Key << "Position" << YAML::Value << YAML::Flow << YAML::BeginSeq << position.x << position.y << position.z << YAML::EndSeq;
				p_emitter << YAML::Key << "Rotation" << YAML::Value << YAML::Flow << YAML::BeginSeq << rotation.x << rotation.y << rotation.z << YAML::EndSeq;
				p_emitter << YAML::Key << "Scale" << YAML::Value << YAML::Flow << YAML::BeginSeq << scale.x << scale.y << scale.z << YAML::EndSeq;
			}
			break;
		case 2:
			{
				RootForce::PointLight* pointLight = static_cast<RootForce::PointLight*>(p_component);
				glm::vec3 attenuation = pointLight->m_attenuation;
				glm::vec4 color = pointLight->m_color;
				float range = pointLight->m_range;

				p_emitter << YAML::Key << "Attenuation" << YAML::Value << YAML::Flow << YAML::BeginSeq << attenuation.x << attenuation.y << attenuation.z << YAML::EndSeq;
				p_emitter << YAML::Key << "Color" << YAML::Value << YAML::Flow << YAML::BeginSeq << color.x << color.y << color.z << color.w << YAML::EndSeq;
				p_emitter << YAML::Key << "Range" << YAML::Value << range;
			}
			break;
		case 3:
			{
				RootForce::PlayerControl* input = static_cast<RootForce::PlayerControl*>(p_component);
				p_emitter << YAML::Key << "Speed" << YAML::Value << input->m_speed;
			}
			break;
		case 4:
			{
				RootForce::PhysicsAccessor* accessor = static_cast<RootForce::PhysicsAccessor*>(p_component);
				int type = g_engineContext.m_physics->GetType(*(accessor->m_handle));
				float mass = g_engineContext.m_physics->GetMass(*(accessor->m_handle));
				std::string modelHandle = g_engineContext.m_physics->GetPhysicsModelHandle(*(accessor->m_handle));
				p_emitter << YAML::Key << "Type" << YAML::Value << type;
				p_emitter << YAML::Key << "Mass" << YAML::Value << mass;
				p_emitter << YAML::Key << "ModelHandle" << YAML::Value << modelHandle;
				
				if (type == RootEngine::Physics::PhysicsType::TYPE_PLAYER) 
				{
					float stepHeight = g_engineContext.m_physics->GetStepHeight(*(accessor->m_handle));
					float modelHeight =g_engineContext.m_physics->GetModelHeight(*(accessor->m_handle));
					float maxSpeed = g_engineContext.m_physics->GetMaxSpeed(*(accessor->m_handle));
					p_emitter << YAML::Key << "StepHeight" << YAML::Value << stepHeight;
					p_emitter << YAML::Key << "ModelHeight" << YAML::Value << modelHeight;
					p_emitter << YAML::Key << "MaxSpeed" << YAML::Value << maxSpeed;
				}

			}
			break;
		default:
			break;
	}
}