#pragma once

#include <yaml-cpp\yaml.h>
#include <RootSystems\Include\Components.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

static void Exporter(YAML::Emitter& p_emitter, ECS::ComponentInterface* p_component, int p_type)
{
	switch(p_type)
	{
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
		case RootForce::ComponentType::PLAYERCONTROL:
			{
				RootForce::PlayerPhysics* playphys = static_cast<RootForce::PlayerPhysics*>(p_component);
				p_emitter << YAML::Key << "Speed" << YAML::Value << playphys->MovementSpeed;
			}
			break;
		case RootForce::ComponentType::PHYSICS:
			{
				RootForce::Physics* physics = static_cast<RootForce::Physics*>(p_component);
				p_emitter << YAML::Key << "Mass" << YAML::Value << physics->m_mass;
			}
		case RootForce::ComponentType::COLLISION:
			{
				RootForce::Collision* collision = static_cast<RootForce::Collision*>(p_component);
				p_emitter << YAML::Key << "MeshHandle" << YAML::Value << collision->m_meshHandle;
			}
			break;
		default:
			break;
	}
}