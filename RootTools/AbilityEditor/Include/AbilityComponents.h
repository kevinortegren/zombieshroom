#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <QtCore/QStringList>
namespace AbilityEditorNameSpace
{


	namespace AbilityComponents
	{
		namespace ComponentType
		{
			enum ComponentType
			{
				TRANSFORM,
				COLLISION,
				ABILITYMODEL,
				COLLISIONSHAPE,
				ABILITYPARTICLE,
				PHYSICSCONTROLLED,
				OFFENSIVEABILITY,
				EXPLOSIVE,
				END_OF_ENUM
			};
		}
		const struct ComponentNameList
		{
			QStringList m_compNames;
			ComponentNameList()
			{
				m_compNames.append("Transform");
				m_compNames.append("Collision");
				m_compNames.append("Ability Model");
				m_compNames.append("Collision Shape");
				m_compNames.append("Ability Particle");
				m_compNames.append("Physics Controlled");
				m_compNames.append("Offensive Ability");
				m_compNames.append("Explosive");
			}
		} static g_componentNameList;
		//All components should inherit from the MainComponent struct
		struct MainComponent
		{
			ComponentType::ComponentType m_type;
			MainComponent(ComponentType::ComponentType p_type)
			{
				m_type = p_type;
			}
			

		};
	
		////////////////////////////////////////////////////////On create components ////////////////////////////////////////////////////////
		struct Transform: MainComponent
		{ 
			glm::vec3 m_rotation;
			glm::vec3 m_scale;
			Transform(glm::vec3 p_rotation = glm::vec3(0.0f), glm::vec3 p_scale = glm::vec3(1.0f)): MainComponent(ComponentType::TRANSFORM)
			{
				m_rotation = p_rotation;
				m_scale = p_scale;
			}
		};

		struct Collision : MainComponent
		{
			//bool m_externallyControlled;
			Collision() : MainComponent(ComponentType::COLLISION)
			{

			}
		};


		struct AbilityModel : MainComponent
		{
			std::string m_modelName;
			std::string m_material;
			std::string m_materialDiffuse;
			std::string m_materialSpecular;
			std::string m_materialNormal;
			std::string m_materialEffect;
			AbilityModel(std::string p_modelName = "", std::string p_material = "",
						std::string p_materialDiffuse = "", std::string p_materialSpecular = "",
						std::string p_materialNormal = "", std::string p_materialEffect = "") 
						: MainComponent(ComponentType::ABILITYMODEL)
			{
				m_modelName = p_modelName;
				m_material = p_material;
				m_materialDiffuse = p_materialDiffuse;
				m_materialSpecular = p_materialSpecular;
				m_materialNormal = p_materialNormal;
				m_materialEffect = p_materialEffect;
			}
		};

		struct CollisionShape : MainComponent
		{
			enum shape
			{
				CONE,
				CYLINDER,
				SPHERE,
				MESH
			};
			
			shape m_CollisionShape;
			float m_radius;
			float m_height;
			std::string m_collisionModelShapeName;
			CollisionShape(shape p_collisionShape = shape::SPHERE, float p_radius = 0.0f, float p_height = 0.0f, std::string p_collisionModelShapeName = "") : MainComponent(ComponentType::COLLISIONSHAPE)
			{
				m_CollisionShape = p_collisionShape;
				m_radius = p_radius;
				m_height = p_height;
				m_collisionModelShapeName = p_collisionModelShapeName;
			}

		};

		struct AbilityParticle : MainComponent
		{
			std::string m_particleName;
			float m_size;
			AbilityParticle(float p_size = 1.0f) : MainComponent(ComponentType::ABILITYPARTICLE)
			{
				m_size = p_size;
			}
		};

		struct PhysicsControlled : MainComponent
		{
			float m_speed;
			float m_mass;
			PhysicsControlled(float p_speed = 0.0f, float p_mass = 1.0f) : MainComponent(ComponentType::PHYSICSCONTROLLED)
			{
				m_speed = p_speed;
				m_mass = p_mass;
			}
		};

		//////////////////////////////////////////////On collide components////////////////////////////////////////////////////////
		struct OffensiveAbility: MainComponent
		{
			float m_damage;
			float m_knockbackPower;
			OffensiveAbility(float p_damage = 0.0f, float p_knockbackPower = 0.0f) : MainComponent(ComponentType::OFFENSIVEABILITY)
			{
				m_damage = p_damage;
				m_knockbackPower = p_knockbackPower;
			}
		};

		struct Explosive : MainComponent
		{
			float m_radius;
			Explosive(float p_radius = 0.0f) : MainComponent(ComponentType::EXPLOSIVE)
			{
				m_radius = p_radius;
			}
		};	
	}
	namespace AbilityEntity
	{
		//The one struct to rule them all
		struct Entity
		{
			QString m_name;
			std::vector<AbilityComponents::MainComponent*>* m_components;
			std::map<QString, int> m_nameToEnumMapper;
			Entity(QString p_name)
			{
				for(int i = 0 ; i < AbilityComponents::ComponentType::END_OF_ENUM; i++)
				{
					m_nameToEnumMapper[AbilityComponents::g_componentNameList.m_compNames.at(i)] = i;
				}
				m_name = p_name;
				m_components = new std::vector<AbilityComponents::MainComponent*>();
			}
			~Entity()
			{
				if(m_components->size() > 0)
				{
					for(unsigned int i = m_components->size() -1 ; i > 0; i --)
					{
						delete m_components->at(i);
						m_components->pop_back();
					}
				}
				delete m_components;
			}
			void AddComponent(QString p_componentName)
			{
				AddComponent((AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper.at(p_componentName));
			}
			void AddComponent(AbilityComponents::ComponentType::ComponentType p_type)
			{
				switch (p_type)
				{
					
				case AbilityComponents::ComponentType::TRANSFORM:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::Transform();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::COLLISION:			
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::Collision();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::ABILITYMODEL:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::AbilityModel();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::COLLISIONSHAPE:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::CollisionShape();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::PHYSICSCONTROLLED:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::PhysicsControlled();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::ABILITYPARTICLE:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::AbilityParticle();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::OFFENSIVEABILITY:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::OffensiveAbility();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::EXPLOSIVE:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::Explosive();
						AddComponent(temp);
					}
					break;
				default:
					break; 
				}
			}
			void AddComponent(AbilityComponents::MainComponent* p_component)
			{
				m_components->push_back(p_component);
			}
			void RemoveComponent(QString p_componentName)
			{
				RemoveComponent((AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper.at(p_componentName));
			}
			void RemoveComponent(AbilityComponents::ComponentType::ComponentType p_type)
			{
				for(unsigned int i = 0; i < m_components->size(); i++)
					if(m_components->at(i)->m_type == p_type)
					{
						delete m_components->at(i);
						m_components->erase(m_components->begin() + i);
						break;
					}
			}
			bool DoesComponentExist(QString p_componentName)
			{
				return DoesComponentExist((AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper.at(p_componentName));
			}
			bool DoesComponentExist(AbilityComponents::ComponentType::ComponentType p_type)
			{
				for(unsigned int i = 0; i < m_components->size(); i++)
				{
					if(m_components->at(i)->m_type == p_type)
						return true;
				}
				return false;
			}
		};
	}
}