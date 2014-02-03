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

			//Name
			//const YAML::Node& name = doc[0]["Name"];
			std::string abilityName;
			doc[0]["Name"] >> abilityName;
			//Entity* entity = new Entity(abilityName.c_str());
			
			//Components
			const YAML::Node& comps = doc[1]["Components"];
			for(unsigned int i = 0; i < comps.size(); i++)
			{
				int type;
				comps[i]["Type"] >> type;
				const YAML::Node& data = comps[i]["Data"];
				for(unsigned int j = 0; j < data.size(); j++)
					p_entity->AddComponent(ImportComponents(data[j], type));
			}

			//OnCreate
			const YAML::Node& onCreate = doc[2]["OnCreate"];
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

			//OnCollide
			const YAML::Node& onCollide = doc[3]["OnCollide"];
			for(unsigned int i = 0; i < onCollide.size(); i++)
			{
				std::string condName;
				onCreate[i]["Condition"] >> condName;
				std::string condCode;
				onCreate[i]["Code"] >> condCode;
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

			
			//OnDestroy
			const YAML::Node& onDestroy = doc[4]["OnDestroy"];
			for(unsigned int i = 0; i < onDestroy.size(); i++)
			{
				std::string condName;
				onCreate[i]["Condition"] >> condName;
				std::string condCode;
				onCreate[i]["Code"] >> condCode;
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
		catch(YAML::ParserException& e)
		{
			std::cout << e.what() << "\n";
		}
	}

	AbilityComponents::MainComponent* Importer::ImportComponents(const YAML::Node& p_node, unsigned int p_type)
	{
		//Todo, to make it slightly more simple, aka easier to put data into the components, fix their constructors to take arguments and that cool stuff

		
		switch (p_type)
		{
		case AbilityComponents::ComponentType::TRANSFORM:
			{
				AbilityComponents::Transform* tempcomp = new AbilityComponents::Transform();
				if(p_node.FindValue("Rotation"))
				{
					float x,y,z;
					p_node["Rotation"][0] >> x;
					p_node["Rotation"][1] >> y;
					p_node["Rotation"][2] >> z;
					tempcomp->m_rotation = QVector3D(x,y,z);
				}
				if(p_node.FindValue("scale"))
				{
					float x,y,z;
					p_node["scale"][0] >> x;
					p_node["scale"][1] >> y;
					p_node["scale"][2] >> z;
					tempcomp->m_scale = QVector3D(x,y,z);
				}
				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::COLLISION:			
			{
				AbilityComponents::Collision* tempcomp = new AbilityComponents::Collision();
				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
				//Nothing in this one yet, might never be, might be removed, who knows
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
		case AbilityComponents::ComponentType::PHYSICSCONTROLLED:
			{
				AbilityComponents::PhysicsControlled* tempcomp = new AbilityComponents::PhysicsControlled();
				if(p_node.FindValue("Speed"))
					p_node["Speed"] >> tempcomp->m_speed;
				if(p_node.FindValue("Mass"))
					p_node["Mass"] >> tempcomp->m_mass;
				if(p_node.FindValue("Gravity"))
				{
					float x,y,z;
					p_node["Gravity"][0] >> x;
					p_node["Gravity"][1] >> y;
					p_node["Gravity"][2] >> z;
					tempcomp->m_gravity = QVector3D(x,y,z);
				}

				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;		
		case AbilityComponents::ComponentType::OFFENSIVEABILITY:
			{
				AbilityComponents::OffensiveAbility* tempcomp = new AbilityComponents::OffensiveAbility();
				if(p_node.FindValue("Damage"))
					p_node["Damage"] >> tempcomp->m_damage;
				if(p_node.FindValue("KnockbackPower"))
					p_node["KnockbackPower"] >> tempcomp->m_knockbackPower;
				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		case AbilityComponents::ComponentType::EXPLOSIVE:
			{
				AbilityComponents::Explosive* tempcomp = new AbilityComponents::Explosive();
				if(p_node.FindValue("Radius"))
					p_node["Radius"] >> tempcomp->m_radius;

				return static_cast<AbilityComponents::MainComponent*>(tempcomp);
			}
			break;
		default:
			break;
		}
		return nullptr;
	}

}