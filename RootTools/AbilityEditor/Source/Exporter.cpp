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
		

		YAML::Emitter out;
		out << YAML::BeginSeq;

		//Name
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << p_entity->GetName().toStdString(); 
		out << YAML::EndMap;

		//Charges
		out << YAML::BeginMap;
		out << YAML::Key << "Charges" << YAML::Value << p_entity->GetCharges(); 
		out << YAML::EndMap;

		//Charge Time
		out << YAML::BeginMap;
		out << YAML::Key << "ChargeTime" << YAML::Value << p_entity->GetChargeTime(); 
		out << YAML::EndMap;

		//Channeling Time
		out << YAML::BeginMap;
		out << YAML::Key << "ChannelingTime" << YAML::Value << p_entity->GetChannelingTime(); 
		out << YAML::EndMap;

		//Duration
		out << YAML::BeginMap;
		out << YAML::Key << "Duration" << YAML::Value << p_entity->GetDuration(); 
		out << YAML::EndMap;

		//Cooldown
		out << YAML::BeginMap;
		out << YAML::Key << "Cooldown" << YAML::Value << p_entity->GetCooldown(); 
		out << YAML::EndMap;

		//Components
		out << YAML::BeginMap;
		out << YAML::Key << "Components";
		out << YAML::Value << YAML::BeginSeq;
		for (unsigned int i = 0; i < p_entity->GetComponents()->size(); i++)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Type" << YAML::Value << p_entity->GetComponents()->at(i)->m_type;
			out << YAML::Key << "Data" << YAML::Value << YAML::BeginSeq;
			out << YAML::BeginMap;
			ExportComponent(out, p_entity->GetComponents()->at(i), p_entity->GetComponents()->at(i)->m_type);
			out << YAML::EndMap;
			out << YAML::EndSeq;
			out << YAML::EndMap;
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
		out << YAML::EndSeq;

		std::ofstream file;
		file.open(p_filepath);
		file << out.c_str();
		file.close();
	}

	void Exporter::ExportCondition( YAML::Emitter& p_emitter, Condition* p_cond)
	{
		p_emitter << YAML::Key << "Condition" << YAML::Value << p_cond->GetName().toStdString();
		p_emitter << YAML::Key << "Code" << YAML::Value << p_cond->GetCode().toStdString();
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
			case AbilityComponents::ComponentType::STARTPOS:
				{
					AbilityComponents::StartPos* startPos = static_cast<AbilityComponents::StartPos*>(p_component);
					glm::vec3 pos = glm::vec3(startPos->m_absolutePos.x(), startPos->m_absolutePos.y(), startPos->m_absolutePos.z());
					p_emitter << YAML::Key << "StartPos" << YAML::Value << startPos->m_startPos;
					p_emitter << YAML::Key << "AbsoluteStartPos" << YAML::Value << YAML::Flow << YAML::BeginSeq << pos.x << pos.y << pos.z << YAML::EndSeq;
				}
				break;
			case AbilityComponents::ComponentType::TARGPOS:
				{
					AbilityComponents::TargetPos* targetPos = static_cast<AbilityComponents::TargetPos*>(p_component);
					glm::vec3 pos = glm::vec3(targetPos->m_absolutePos.x(), targetPos->m_absolutePos.y(), targetPos->m_absolutePos.z());
					p_emitter << YAML::Key << "TargetPos" << YAML::Value << targetPos->m_targPos;
					p_emitter << YAML::Key << "AbsoluteTargetPos" << YAML::Value << YAML::Flow << YAML::BeginSeq << pos.x << pos.y << pos.z << YAML::EndSeq;
				}
				break;
			case AbilityComponents::ComponentType::VELOCITY:
				{
					AbilityComponents::Velocity* velocity = static_cast<AbilityComponents::Velocity*>(p_component);
					p_emitter << YAML::Key << "Direction" << YAML::Value << velocity->m_direction;
					p_emitter << YAML::Key << "Speed" << YAML::Value << velocity->m_speed;
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
			case AbilityComponents::ComponentType::DAMAGE:
				{
					AbilityComponents::Damage* damage = static_cast<AbilityComponents::Damage*>(p_component);
					p_emitter << YAML::Key << "DamagePlayers" << YAML::Value << damage->m_affectedPlayers;
					p_emitter << YAML::Key << "Damage" << YAML::Value << damage->m_damage;
				}
				break;
			case AbilityComponents::ComponentType::KNOCKBACK:
				{
					AbilityComponents::Knockback* knockback = static_cast<AbilityComponents::Knockback*>(p_component);
					p_emitter << YAML::Key << "KnockbackPlayers" << YAML::Value << knockback->m_affectedPlayers;
					p_emitter << YAML::Key << "Knockback" << YAML::Value << knockback->m_knockback;
				}
				break;
			case AbilityComponents::ComponentType::STATCHANGECASTER:
				{
					AbilityComponents::StatChangeCaster* statCaster = static_cast<AbilityComponents::StatChangeCaster*>(p_component);
					p_emitter << YAML::Key << "CasterSpeed" << YAML::Value << statCaster->m_speed;
					p_emitter << YAML::Key << "CasterJumpHeight" << YAML::Value << statCaster->m_jumpHeight;
					p_emitter << YAML::Key << "CasterKnockbackResistance" << YAML::Value << statCaster->m_knockbackResistance;
				}
				break;
			case AbilityComponents::ComponentType::STATCHANGETARGET:
				{
					AbilityComponents::StatChangeTarget* statTarget = static_cast<AbilityComponents::StatChangeTarget*>(p_component);
					p_emitter << YAML::Key << "TargetSpeed" << YAML::Value << statTarget->m_speed;
					p_emitter << YAML::Key << "TargetJumpHeight" << YAML::Value << statTarget->m_jumpHeight;
					p_emitter << YAML::Key << "TargetKnockbackResistance" << YAML::Value << statTarget->m_knockbackResistance;
				}
				break;
			case AbilityComponents::ComponentType::PHYSICS:
				{
					AbilityComponents::Physics* physCon = static_cast<AbilityComponents::Physics*>(p_component);
					p_emitter << YAML::Key << "PhysicsMass" << YAML::Value << physCon->m_mass;
					glm::vec3 grav = glm::vec3(physCon->m_gravity.x(), physCon->m_gravity.y(), physCon->m_gravity.z());
					p_emitter << YAML::Key << "PhysicsGravity" << YAML::Value << YAML::Flow << YAML::BeginSeq <<  grav.x << grav.y << grav.z << YAML::EndSeq;
					p_emitter << YAML::Key << "PhysicsCollide" << YAML::Value << physCon->m_collide;
					p_emitter << YAML::Key << "PhysicsCollideStatic" << YAML::Value << physCon->m_colWStatic;
					p_emitter << YAML::Key << "PhysicsExternally" << YAML::Value << physCon->m_externally;
				}
				break;
			case AbilityComponents::ComponentType::CHARGEVARIABLES:
				{
					AbilityComponents::ChargeVariables* chargeVar = static_cast<AbilityComponents::ChargeVariables*>(p_component);
					p_emitter << YAML::Key << "ChargeRequired" << YAML::Value << chargeVar->m_chargeReq;
					p_emitter << YAML::Key << "ChargeFactor" << YAML::Value << chargeVar->m_chargeFactor;
				}
				break;
			case AbilityComponents::ComponentType::WATER:
				{
					AbilityComponents::Water* water = static_cast<AbilityComponents::Water*>(p_component);
					p_emitter << YAML::Key << "WaterPower" << YAML::Value << water->m_power;
					p_emitter << YAML::Key << "WaterInterval" << YAML::Value << water->m_interval;
					p_emitter << YAML::Key << "WaterRadius" << YAML::Value << water->m_radius;
				}
				break;
			case AbilityComponents::ComponentType::SOUND:
				{
					AbilityComponents::Sound* sound = static_cast<AbilityComponents::Sound*>(p_component);
					p_emitter << YAML::Key << "SoundName" << YAML::Value << sound->m_soundName;
					p_emitter << YAML::Key << "SoundVolume" << YAML::Value << sound->m_volume;
					p_emitter << YAML::Key << "SoundRangeMin" << YAML::Value << sound->m_rangeMin;
					p_emitter << YAML::Key << "SoundRangeMax" << YAML::Value << sound->m_rangeMax;
					p_emitter << YAML::Key << "SoundLoop" << YAML::Value << sound->m_loop;
				}
				break;
			case AbilityComponents::ComponentType::FOLLOW:
				{
					AbilityComponents::Follow* follow = static_cast<AbilityComponents::Follow*>(p_component);
					p_emitter << YAML::Key << "FollowOffset" << YAML::Value << follow->m_offset;
				}
				break;
			default:
				break;
		}
		
	}

}