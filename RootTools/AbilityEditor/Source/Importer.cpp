#include "Importer.h"
#include <iostream>
#include <fstream>
#include <sstream>
namespace AbilityEditorNameSpace
{
	Importer::Importer()
	{

	}

	Importer::~Importer()
	{

	}

	void Importer::Import(const std::string &p_filename, Entity* p_entity, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy )
	{
		try
		{
			std::ifstream file(p_filename, std::ifstream::in);
			if(!file.good())
			{
				//TODO, add a real error message
				throw("File is bad, get another file");
			}
			YAML::Parser parser(file);
			
			YAML::Node doc;
			parser.GetNextDocument(doc);

			unsigned int counter = 0;

			//Name
			//const YAML::Node& name = doc[0]["Name"];
			std::string abilityName;
			doc[counter]["Name"] >> abilityName;
			counter++;
			p_entity->SetName(QString::fromStdString(abilityName));
			//Entity* entity = new Entity(abilityName.c_str());
			
			//Charges
			int charges;
			doc[counter]["Charges"] >> charges;
			counter++;
			p_entity->SetCharges(charges);

			//Charging Time
			float chargingTime;
			doc[counter]["ChargeTime"] >> chargingTime;
			counter++;
			p_entity->SetChargeTime(chargingTime);

			//Channeling Time
			float channelingTime;
			doc[counter]["ChannelingTime"] >> channelingTime;
			counter++;
			p_entity->SetChannelingTime(channelingTime);

			//Duration
			float duration;
			doc[counter]["Duration"] >> duration;
			counter++;
			p_entity->SetDuration(duration);

			//Cooldown
			float cooldown;
			doc[counter]["Cooldown"] >> cooldown;
			counter++;
			p_entity->SetCooldown(cooldown);

			//Components
			const YAML::Node& comps = doc[counter]["Components"];
			counter++;
			for(unsigned int i = 0; i < comps.size(); i++)
			{
				int type;
				comps[i]["Type"] >> type;
				const YAML::Node& data = comps[i]["Data"];
				for(unsigned int j = 0; j < data.size(); j++)
					p_entity->AddComponent(ImportComponents(data[j], type));
			}

			//OnCreate
			if(doc.FindValue("OnCreate"))
			{
				const YAML::Node& onCreate = doc[counter]["OnCreate"];
				counter++;
				for(unsigned int i = 0; i < onCreate.size(); i++)
				{
					std::string condName;
					onCreate[i]["Condition"] >> condName;
					std::string condCode;
					onCreate[i]["Code"] >> condCode;
					Condition* cond = new Condition(condName.c_str(), condCode.c_str());
					const YAML::Node& entities = onCreate[i]["Entities"];
					for(unsigned int j = 0; j < entities.size(); j++)
					{
						std::string entName;
						entities[j]["Entity"] >> entName;
						cond->AddEntity(entName.c_str());
					}
					p_onCreate->AddCondition(cond);
				}
			}

			//OnCollide
			if(doc.FindValue("OnCollide"))
			{
				const YAML::Node& onCollide = doc[counter]["OnCollide"];
				counter++;
				for(unsigned int i = 0; i < onCollide.size(); i++)
				{
					std::string condName;
					onCollide[i]["Condition"] >> condName;
					std::string condCode;
					onCollide[i]["Code"] >> condCode;
					Condition* cond = new Condition(condName.c_str(), condCode.c_str());
					const YAML::Node& entities = onCollide[i]["Entities"];
					for(unsigned int j = 0; j < entities.size(); j++)
					{
						std::string entName;
						entities[j]["Entity"] >> entName;
						cond->AddEntity(entName.c_str());
					}
					p_onCollide->AddCondition(cond);
				}
			}
			
			//OnDestroy
			if(doc.FindValue("OnDestroy"))
			{
				const YAML::Node& onDestroy = doc[counter]["OnDestroy"];
				counter++;
				for(unsigned int i = 0; i < onDestroy.size(); i++)
				{
					std::string condName;
					onDestroy[i]["Condition"] >> condName;
					std::string condCode;
					onDestroy[i]["Code"] >> condCode;
					Condition* cond = new Condition(condName.c_str(), condCode.c_str());
					const YAML::Node& entities = onDestroy[i]["Entities"];
					for(unsigned int j = 0; j < entities.size(); j++)
					{
						std::string entName;
						entities[j]["Entity"] >> entName;
						cond->AddEntity(entName.c_str());
					}
					p_onDestroy->AddCondition(cond);
				}
			}
		}
		catch(YAML::ParserException& e)
		{
			std::cout << e.what() << "\n";
		}
	}

	AbilityComponents::MainComponent* Importer::ImportComponents(const YAML::Node& p_node, unsigned int p_type)
	{

		switch (p_type)
		{
		case AbilityComponents::ComponentType::STARTPOS:
			{
				AbilityComponents::StartPos* tempcomp = new AbilityComponents::StartPos();
				if(p_node.FindValue("StartPos"))
				{
					int temp;
					p_node["StartPos"] >> temp;
					tempcomp->m_startPos = (AbilityComponents::StartPos::pos)temp;
				}
				if(p_node.FindValue("AbsoluteStartPos"))
				{
					float x,y,z;
					p_node["AbsoluteStartPos"][0] >> x;
					p_node["AbsoluteStartPos"][1] >> y;
					p_node["AbsoluteStartPos"][2] >> z;
					tempcomp->m_absolutePos = QVector3D(x,y,z);
				}
				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::TARGPOS:
			{
				AbilityComponents::TargetPos* tempcomp = new AbilityComponents::TargetPos();
				if(p_node.FindValue("TargetPos"))
				{
					int temp;
					p_node["TargetPos"] >> temp;
					tempcomp->m_targPos = (AbilityComponents::TargetPos::pos)temp;
				}
				if(p_node.FindValue("AbsoluteTargetPos"))
				{
					float x,y,z;
					p_node["AbsoluteTargetPos"][0] >> x;
					p_node["AbsoluteTargetPos"][1] >> y;
					p_node["AbsoluteTargetPos"][2] >> z;
					tempcomp->m_absolutePos = QVector3D(x,y,z);
				}
				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::VELOCITY:
			{
				AbilityComponents::Velocity* tempcomp = new AbilityComponents::Velocity();
				if(p_node.FindValue("Direction"))
				{
					int temp;
					p_node["Direction"] >> temp;
					tempcomp->m_direction = (AbilityComponents::Velocity::dir)temp;
				}
				if(p_node.FindValue("Speed"))
				{
					p_node["Speed"] >> tempcomp->m_speed;
				}
				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::ABILITYMODEL:
			{
				AbilityComponents::AbilityModel* tempcomp = new AbilityComponents::AbilityModel();
				if(p_node.FindValue("Model"))
					p_node["Model"] >> tempcomp->m_modelName;
				if(p_node.FindValue("Material"))
					p_node["Material"] >> tempcomp->m_material;
				if(p_node.FindValue("Effect"))
					p_node["Effect"] >> tempcomp->m_materialEffect;
				if(p_node.FindValue("Diffuse"))
					p_node["Diffuse"] >> tempcomp->m_materialDiffuse;
				if(p_node.FindValue("Specular"))
					p_node["Specular"] >> tempcomp->m_materialSpecular;
				if(p_node.FindValue("Normal"))
					p_node["Normal"] >> tempcomp->m_materialNormal;
				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::COLLISIONSHAPE:
			{
				AbilityComponents::CollisionShape* tempcomp = new AbilityComponents::CollisionShape();
				if(p_node.FindValue("CollisionShape"))
				{
					int temp;
					p_node["CollisionShape"] >> temp;
					tempcomp->m_CollisionShape = (AbilityComponents::CollisionShape::shape)temp;
				}
				if(p_node.FindValue("CollisionModelName"))
					p_node["CollisionModelName"] >> tempcomp->m_collisionModelShapeName;
				if(p_node.FindValue("Radius"))
					p_node["Radius"] >> tempcomp->m_radius;
				if(p_node.FindValue("Height"))
					p_node["Height"] >> tempcomp->m_height;

				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::ABILITYPARTICLE:
			{
				AbilityComponents::AbilityParticle* tempcomp = new AbilityComponents::AbilityParticle();
				if(p_node.FindValue("ParticleName"))
					p_node["ParticleName"] >> tempcomp->m_particleName;
				if(p_node.FindValue("Size"))
					p_node["Size"] >> tempcomp->m_size;
				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::DAMAGE:
			{
				AbilityComponents::Damage* tempcomp = new AbilityComponents::Damage();
				if(p_node.FindValue("DamagePlayers"))
				{
					int temp;
					p_node["DamagePlayers"] >> temp;
					tempcomp->m_affectedPlayers = (AbilityComponents::Damage::affected)temp;
				}
				if(p_node.FindValue("Damage"))
					p_node["Damage"] >> tempcomp->m_damage;

				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::KNOCKBACK:
			{
				AbilityComponents::Knockback* tempcomp = new AbilityComponents::Knockback();
				if(p_node.FindValue("KnockbackPlayers"))
				{
					int temp;
					p_node["KnockbackPlayers"] >> temp;
					tempcomp->m_affectedPlayers = (AbilityComponents::Knockback::affected)temp;
				}
				if(p_node.FindValue("Knockback"))
					p_node["Knockback"] >> tempcomp->m_knockback;

				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::STATCHANGECASTER:
			{
				AbilityComponents::StatChangeCaster* tempcomp = new AbilityComponents::StatChangeCaster();
				if(p_node.FindValue("CasterSpeed"))
					p_node["CasterSpeed"] >> tempcomp->m_speed;
				if(p_node.FindValue("CasterJumpHeight"))
					p_node["CasterJumpHeight"] >> tempcomp->m_jumpHeight;
				if(p_node.FindValue("CasterKnockbackResistance"))
					p_node["CasterKnockbackResistance"] >> tempcomp->m_knockbackResistance;
				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::STATCHANGETARGET:
			{
				AbilityComponents::StatChangeTarget* tempcomp = new AbilityComponents::StatChangeTarget();
				if(p_node.FindValue("TargetSpeed"))
					p_node["TargetSpeed"] >> tempcomp->m_speed;
				if(p_node.FindValue("TargetJumpHeight"))
					p_node["TargetJumpHeight"] >> tempcomp->m_jumpHeight;
				if(p_node.FindValue("TargetKnockbackResistance"))
					p_node["TargetKnockbackResistance"] >> tempcomp->m_knockbackResistance;
				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::PHYSICS:
			{
				AbilityComponents::Physics* tempcomp = new AbilityComponents::Physics();
				if(p_node.FindValue("PhysicsMass"))
					p_node["PhysicsMass"] >> tempcomp->m_mass;
				if(p_node.FindValue("PhysicsGravity"))
				{
					float x,y,z;
					p_node["PhysicsGravity"][0] >> x;
					p_node["PhysicsGravity"][1] >> y;
					p_node["PhysicsGravity"][2] >> z;
					tempcomp->m_gravity = QVector3D(x,y,z);
				}
				if(p_node.FindValue("PhysicsCollide"))
					p_node["PhysicsCollide"] >> tempcomp->m_collide;

				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;	
		case AbilityComponents::ComponentType::CHARGEVARIABLES:
			{
				AbilityComponents::ChargeVariables* tempcomp = new AbilityComponents::ChargeVariables();
				if(p_node.FindValue("ChargeRequired"))
					p_node["ChargeRequired"] >> tempcomp->m_chargeReq;
				if(p_node.FindValue("ChargeFactor"))
					p_node["ChargeFactor"] >> tempcomp->m_chargeFactor;

				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		default:
			break;
		}
		return nullptr;
	}

}