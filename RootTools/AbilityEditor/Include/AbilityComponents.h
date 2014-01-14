#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
namespace AbilityEditor
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
				EXPLOSIVE
			};
		}
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
			Transform(): MainComponent(ComponentType::TRANSFORM)
			{
				m_rotation = glm::vec3(0.0f);
				m_scale = glm::vec3(1.0f);
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
			AbilityModel() : MainComponent(ComponentType::ABILITYMODEL)
			{
				m_modelName = "";
				m_material = "";
				m_materialDiffuse = "";
				m_materialSpecular = "";
				m_materialNormal = "";
				m_materialEffect = "";
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
			CollisionShape() : MainComponent(ComponentType::COLLISIONSHAPE)
			{
				m_collisionModelShapeName = "";
				m_radius = 0;
				m_height = 0;
			}

		};

		struct AbilityParticle : MainComponent
		{
			std::string m_particleName;
			float m_size;
			AbilityParticle() : MainComponent(ComponentType::ABILITYPARTICLE)
			{
				m_size = 1;
			}
		};

		struct PhysicsControlled : MainComponent
		{
			float m_speed;
			float m_mass;
			PhysicsControlled() : MainComponent(ComponentType::PHYSICSCONTROLLED)
			{
				m_speed = 1;
				m_mass = 1;
			}
		};

		//////////////////////////////////////////////On collide components////////////////////////////////////////////////////////
		struct OffensiveAbility: MainComponent
		{
			float m_damage;
			float m_knockbackPower;
			OffensiveAbility() : MainComponent(ComponentType::OFFENSIVEABILITY)
			{
				m_damage = 0;
				m_knockbackPower = 0;
			}
		};

		struct Explosive : MainComponent
		{
			float m_radius;
			Explosive() : MainComponent(ComponentType::EXPLOSIVE)
			{

			}
		};

	}
	namespace AbilityEntity
	{
		struct Entity
		{
			std::string m_name;
			std::vector<AbilityComponents::MainComponent*>* m_components;
			Entity(std::string p_name)
			{
				m_name = p_name;
				m_components = new std::vector<AbilityComponents::MainComponent*>();
			}
			~Entity()
			{
				for(unsigned int i = m_components->size() -1; i > 0; i --)
				{
					delete m_components->at(i);
					m_components->pop_back();
				}
				delete m_components;
			}
		};
	}
}