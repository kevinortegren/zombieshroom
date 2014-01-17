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

	void Importer::Import(const std::string &p_filename, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy )
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
			//OnCreate

			const YAML::Node& onCreate = doc[0]["OnCreate"];
			for(unsigned int i = 0; i < onCreate.size() ; i++)
			{
				std::string entityName;
				onCreate[i]["EntityName"] >> entityName;
				QString temp;
				temp.append(entityName.c_str());
				AbilityEntity::Entity* entity = new AbilityEntity::Entity(temp);
				const YAML::Node& comps = onCreate[i]["Components"];
				for(unsigned int j = 0; j < comps.size(); j++)
				{
					int type;
					comps[j]["Type"] >> type;
					const YAML::Node& data = comps[j]["Data"];
					for(unsigned int k = 0; k < data.size(); k++)
						entity->AddComponent(ImportComponents(data[k], type));
				}
				p_onCreate->AddEntity(entity);
			}

		//onCollide data
			
				
			const YAML::Node& onCollide = doc[1]["OnCollide"];
			for(unsigned int i = 0; i < onCollide.size() ; i++)
			{
				std::string entityName;
				onCollide[i]["EntityName"] >> entityName;
				QString temp;
				temp.append(entityName.c_str());
				AbilityEntity::Entity* entity = new AbilityEntity::Entity(temp);
				const YAML::Node& comps = onCollide[i]["Components"];
				for(unsigned int j = 0; j < comps.size(); j++)
				{
					int type;
					comps[j]["Type"] >> type;
					const YAML::Node& data = comps[j]["Data"];
					for(unsigned int k = 0; k < data.size(); k++)
						entity->AddComponent(ImportComponents(data[k], type));
				}
				p_onCreate->AddEntity(entity);
			}
			
		//OnDestroy data
			
			
			
			const YAML::Node& onDestroy = doc[2]["OnDestroy"];
			for(unsigned int i = 0; i < onDestroy.size() ; i++)
			{
				std::string entityName;
				onDestroy[i]["EntityName"] >> entityName;
				QString temp;
				temp.append(entityName.c_str());
				AbilityEntity::Entity* entity = new AbilityEntity::Entity(temp);
				const YAML::Node& comps = onDestroy[i]["Components"];
				for(unsigned int j = 0; j < comps.size(); j++)
				{
					int type;
					comps[j]["Type"] >> type;
					const YAML::Node& data = comps[j]["Data"];
					for(unsigned int k = 0; k < data.size(); k++)
						entity->AddComponent(ImportComponents(data[k], type));
				}
				p_onCreate->AddEntity(entity);
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
					p_node["Rotation"][0] >> tempcomp->m_rotation.x;
					p_node["Rotation"][1] >> tempcomp->m_rotation.y;
					p_node["Rotation"][2] >> tempcomp->m_rotation.z;
				}
				if(p_node.FindValue("scale"))
				{
					p_node["scale"][0] >> tempcomp->m_scale.x;
					p_node["scale"][1] >> tempcomp->m_scale.y;
					p_node["scale"][2] >> tempcomp->m_scale.z;
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