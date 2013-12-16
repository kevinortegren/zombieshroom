#pragma once

#include <yaml-cpp\yaml.h>
#include <RootSystems\Include\Components.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

static void Exporter(YAML::Emitter& p_emitter, ECS::ComponentInterface* p_component, int p_type)
{
	switch(p_type)
	{
		case RootForce::ComponentType::RENDERABLE:
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
		case RootForce::ComponentType::TRANSFORM:
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
		case RootForce::ComponentType::POINTLIGHT:
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
		case RootForce::ComponentType::PLAYERCONTROL:
			{
				RootForce::PlayerControl* input = static_cast<RootForce::PlayerControl*>(p_component);
				p_emitter << YAML::Key << "Speed" << YAML::Value << input->m_speed;
			}
			break;
		case RootForce::ComponentType::PHYSICS:
			{
				RootForce::Physics* physics = static_cast<RootForce::Physics*>(p_component);
				p_emitter << YAML::Key << "Mass" << YAML::Value << physics->m_mass;
			}
			break;
		default:
			break;
	}
}