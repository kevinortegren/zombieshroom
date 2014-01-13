#pragma once
#include <string>
#include <glm/glm.hpp>
namespace Components
{
	//All components should inherit from the MainComponent struct
	struct MainComponent
	{
		std::string m_type;
		MainComponent(std::string p_type)
		{
			m_type = p_type;
		}
	};
	namespace ComponentType
	{
		enum ComponentType
		{
			COLLISION,
			ABILITYMODEL,
			COLLISIONSHAPE,
			ABILITYPARTICLE,
			PHYSICSCONTROLLED,
			OFFENSIVEABILITY,
			EXPLOSIVE
		};
	}
	////////////////////////////////////////////////////////On create components ////////////////////////////////////////////////////////
	struct Transform: MainComponent
	{ 
		glm::vec3 m_position;
		glm::vec3 m_scale;
		Transform(): MainComponent("Transform")
		{
			m_position = glm::vec3(0.0f);
			m_scale = glm::vec3(1.0f);
		}
	};

	struct Collision : MainComponent
	{
		//bool m_externallyControlled;
		Collision() : MainComponent("Collision")
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
		AbilityModel() : MainComponent("AbilityModel")
		{

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
		std::string m_collisionModelShape;
		CollisionShape() : MainComponent("CollisionShape")
		{

		}

	};

	struct AbilityParticle : MainComponent
	{
		std::string m_particleName;
		float m_size;
		AbilityParticle() : MainComponent("AbilityParticle")
		{
			m_size = 1;
		}
	};

	struct PhysicsControlled : MainComponent
	{
		float m_speed;
		float m_mass;
		PhysicsControlled() : MainComponent("PhysicsControlled")
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
		OffensiveAbility() : MainComponent("OffensiveAbility")
		{
			m_damage = 0;
			m_knockbackPower = 0;
		}
	};

	struct Explosive : MainComponent
	{
		float m_radius;
		Explosive() : MainComponent("Explosive")
		{

		}
	};
}