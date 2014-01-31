#include "Exporter.h"
#include "AbilityComponents.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace AbilityEditorNameSpace
{
	Exporter::Exporter()
	{

	}

	Exporter::~Exporter()
	{

	}

	void Exporter::Export(const std::string& p_filepath, Entity* p_entity, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy )
	{
		
		/*NEW FORMAT WOOOO!!
		-Name
		-Components
			--Values
		-OnClass
			--Condition
				---Entities
		-OnClass
			--Condition
				---Entities
		...
		*/

		YAML::Emitter out;
		out << YAML::BeginSeq;

		//Name
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << p_entity->GetName().toStdString(); 
		out << YAML::EndMap;

		//Components
		out << YAML::BeginMap;
		out << YAML::Key << "Components";
		out << YAML::Value << YAML::BeginSeq;
		for (unsigned int i = 0; i < p_entity->GetComponents()->size(); i++)
		{
			ExportComponent(out, p_entity->GetComponents()->at(i), p_entity->GetComponents()->at(i)->m_type);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;


		//Oncreate class
		out << YAML::BeginMap;
		out << YAML::Key << "OnCreate";
		out << YAML::Value << YAML::BeginSeq; 
		if(p_onCreate != nullptr)
		{
			for(unsigned int i = 0 ; i < p_onCreate->GetConditions()->size(); i++)
			{
				out << YAML::BeginMap;
				ExportCondition(out, p_onCreate->GetConditions()->at(i));
				out << YAML::EndMap;
			}
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		//OnCollide
		out << YAML::BeginMap;
		out << YAML::Key << "OnCollide";
		out << YAML::Value << YAML::BeginSeq; 
		if(p_onCollide != nullptr)
		{
			
			for(unsigned int i = 0 ; i < p_onCollide->GetConditions()->size(); i++)
			{
				out << YAML::BeginMap;
				ExportCondition(out, p_onCollide->GetConditions()->at(i));
				out << YAML::EndMap;
			}
			
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		//OnDestroy
		out << YAML::BeginMap;
		out << YAML::Key << "OnDestroy";
		out << YAML::Value << YAML::BeginSeq; 
		if(p_onDestroy != nullptr)
		{
			for(unsigned int i = 0 ; i < p_onDestroy->GetConditions()->size(); i++)
			{
				out << YAML::BeginMap;
				ExportCondition(out, p_onDestroy->GetConditions()->at(i));
				out << YAML::EndMap;
			}
	
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		//File done, prepare to write
		out << YAML::EndSeq; //Remove?
		out << YAML::EndMap; //Remove?
		out << YAML::EndSeq;

		std::ofstream file;
		file.open(p_filepath);
		file << out.c_str();
		file.close();
	}

	void Exporter::ExportCondition( YAML::Emitter& p_emitter, Condition* p_cond)
	{
		p_emitter << YAML::Key << "Condition" << YAML::Value << p_cond->GetText().toStdString();
		p_emitter << YAML::Key << "Entities";
		p_emitter << YAML::Value << YAML::BeginSeq;
		for (unsigned int i = 0; i < p_cond->GetEntities().size(); i++)
		{
			p_emitter << YAML::BeginMap;
			p_emitter << YAML::Key << "Entity" << YAML::Value << p_cond->GetEntities().at(i).toStdString();
			p_emitter << YAML::EndMap;
		}
		p_emitter << YAML::EndSeq;
	}

	void Exporter::ExportComponent( YAML::Emitter& p_emitter, AbilityComponents::MainComponent* p_component, unsigned int p_type )
	{
		
		switch (p_type)
		{
			case AbilityComponents::ComponentType::TRANSFORM:
				{
					AbilityComponents::Transform* transform = static_cast<AbilityComponents::Transform*>(p_component);
					glm::vec3 rotation = glm::vec3(transform->m_rotation.x(), transform->m_rotation.y(), transform->m_rotation.z());
					glm::vec3 scale = glm::vec3(transform->m_scale.x(), transform->m_scale.y(), transform->m_scale.z());
					p_emitter << YAML::Key << "Rotation" << YAML::Value << YAML::Flow << YAML::BeginSeq << rotation.x << rotation.y << rotation.z << YAML::EndSeq;
					p_emitter << YAML::Key << "scale" << YAML::Value << YAML::Flow << YAML::BeginSeq << scale.x << scale.y << scale.z << YAML::EndSeq;
					
				}
				break;
			case AbilityComponents::ComponentType::COLLISION:			
				{
					//Nothing in this one yet, might never be, might be removed, who knows
				}
				break;
			case AbilityComponents::ComponentType::ABILITYMODEL:
				{
					AbilityComponents::AbilityModel* model = static_cast<AbilityComponents::AbilityModel*>(p_component);
					p_emitter << YAML::Key << "Model" << YAML::Value << model->m_modelName;
					p_emitter << YAML::Key << "Material" << YAML::Value << model->m_material;
					p_emitter << YAML::Key << "Effect" << YAML::Value << model->m_materialEffect;
					p_emitter << YAML::Key << "Diffuse" << YAML::Value << model->m_materialDiffuse;
					p_emitter << YAML::Key << "Specular" << YAML::Value << model->m_materialSpecular;
					p_emitter << YAML::Key << "Normal" << YAML::Value << model->m_materialNormal;
				}
				break;
			case AbilityComponents::ComponentType::COLLISIONSHAPE:
				{
					AbilityComponents::CollisionShape* shape = static_cast<AbilityComponents::CollisionShape*>(p_component);
					p_emitter << YAML::Key << "CollisionShape" << YAML::Value << shape->m_CollisionShape;
					p_emitter << YAML::Key << "CollisionModelName" << YAML::Value << shape->m_collisionModelShapeName;
					p_emitter << YAML::Key << "Radius" << YAML::Value << shape->m_radius;
					p_emitter << YAML::Key << "Height" << YAML::Value << shape->m_height;
				}
				break;
			case AbilityComponents::ComponentType::ABILITYPARTICLE:
				{
					AbilityComponents::AbilityParticle* particle = static_cast<AbilityComponents::AbilityParticle*>(p_component);
					p_emitter << YAML::Key << "ParticleName" << YAML::Value << particle->m_particleName;
					p_emitter << YAML::Key << "Size" << YAML::Value << particle->m_size;
				}
				break;
			case AbilityComponents::ComponentType::PHYSICSCONTROLLED:
				{
					AbilityComponents::PhysicsControlled* physCon = static_cast<AbilityComponents::PhysicsControlled*>(p_component);
					p_emitter << YAML::Key << "Speed" << YAML::Value << physCon->m_speed;
					p_emitter << YAML::Key << "Mass" << YAML::Value << physCon->m_mass;
					glm::vec3 grav = glm::vec3(physCon->m_gravity.x(), physCon->m_gravity.y(), physCon->m_gravity.z());
					p_emitter << YAML::Key << "Gravity" << YAML::Value << YAML::Flow << YAML::BeginSeq <<  grav.x << grav.y << grav.z << YAML::EndSeq;
				}
				break;
			case AbilityComponents::ComponentType::OFFENSIVEABILITY:
				{
					AbilityComponents::OffensiveAbility* offensive = static_cast<AbilityComponents::OffensiveAbility*>(p_component);
					p_emitter << YAML::Key << "Damage" << YAML::Value << offensive->m_damage;
					p_emitter << YAML::Key << "KnockbackPower" << YAML::Value << offensive->m_knockbackPower;
				}
				break;
			case AbilityComponents::ComponentType::EXPLOSIVE:
				{
					AbilityComponents::Explosive* explosive = static_cast<AbilityComponents::Explosive*>(p_component);
					p_emitter << YAML::Key << "Radius" << explosive->m_radius;
				}
				break;
			default:
				break;
		}
		
	}

}