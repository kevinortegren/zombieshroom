#include "ScriptGenerator.h"
#include <iostream>
#include <fstream>

namespace AbilityEditorNameSpace
{

	ScriptGenerator::ScriptGenerator()
	{

	}

	ScriptGenerator::~ScriptGenerator()
	{

	}

	void ScriptGenerator::GenerateScript( QString p_filePath, QString p_scriptName, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy )
	{
		p_scriptName.chop(4);
		m_name = p_scriptName.toStdString();
		
		//std::ofstream file;
		m_file.open(p_filePath.toStdString());// + ".lua");
		
		//Begin writing in the file
		m_file << "ACTION_CREATE = 0;\n";
		m_file << "ACTION_COLLIDE = 1;\n";
		m_file << "ACTION_DESTROY = 2;\n";
		m_file << "\n";
		m_file << m_name << " = {};\n";
		m_file << "\n";
		m_file << "function " << m_name << ".Process (action, ...)\n";
		m_file << "\n";
		m_file << "\tlocal args = table.pack(...);\n";
		m_file << "\n";
		m_file << "\tif action == ACTION_CREATE then\n";
		m_file << "\t\t" << m_name << ".OnCreate();\n";
		m_file << "\telseif action == ACTION_COLLIDE then\n";
		m_file << "\t\t" << m_name << ".OnCollide(args);\n";
		m_file << "\telseif action == ACTION_DESTROY then\n";
		m_file << "\t\t" << m_name << ".OnDestroy(args);\n";
		m_file << "\tend";
		m_file << "\n";
		m_file << "end\n";
		m_file << "\n";

		WriteOnCreate(p_onCreate);
		m_file << "\n";
		WriteOnCollide(p_onCollide);
		m_file << "\n";
		WriteOnDestroy(p_onDestroy);
		m_file << "\n";

		m_file.close();
	}

	void ScriptGenerator::WriteOnCreate( OnCreate* p_onCreate )
	{
		std::vector<AbilityEntity::Entity*>* entities = p_onCreate->GetEntities();

		m_file << "function " << m_name << ".OnCreate ()\n";
		m_file << "\tlocal playerEnt = Entity.GetEntityByTag(\"Player\");\n";
		m_file << "\tlocal posVec = Entity.GetEntityByTag(\"Player\"):GetTransformation():GetPos();\n";
		m_file << "\tlocal frontVec = Entity.GetEntityByTag(\"AimingDevice\"):GetTransformation():GetOrient():GetFront();\n";
		m_file << "\n";
		m_entityOffset = entities->size();
		for (int i = 0; i < m_entityOffset; i++)
		{
			m_file << "\tlocal entity" << i << " = Entity.New();\n";
			m_file << "\n";
			if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::ABILITYMODEL))
				m_file << "\tlocal renderComp" << i << " = Renderable.New(entity" << i << ");\n";
			if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::COLLISION))
			{
				m_file << "\tlocal collisionComp" << i << " = Collision.New(entity" << i << ");\n";
				m_file << "\tlocal colRespComp" << i << " = CollisionResponder.New(entity" << i << ");\n";
				if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::COLLISIONSHAPE))
				{
					m_file << "\tlocal physicsComp" << i << " = Physics.New(entity" << i << ");\n";
					if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::PHYSICSCONTROLLED))
						m_file << "\tcollisionComp" << i << ":CreateHandle(entity" << i << ":GetId(), 1, false);\n";
					else
						m_file << "\tcollisionComp" << i << ":CreateHandle(entity" << i << ":GetId(), 1, true);\n";
				}
			}
			if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::TRANSFORM))
				m_file << "\tlocal transformComp" << i << " = Transformation.New(entity" << i << ");\n";
			//if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::OFFENSIVEABILITY))
			//	m_file << "local "
			m_file << "\tlocal scriptComp" << i << " = Script.New(entity" << i << ", \"" << m_name << "\");\n";
		}
		// Setting values
		for (int i = 0; i < m_entityOffset; i++)
		{
			//Collision Shape variables
			float radius = 1;
			float height = 1;
			std::string colShape = "Sphere";
			//Physics Controlled variables
			float speed = 1;
			float mass = 1;
			//Transform variables
			QVector3D rotation = QVector3D(0,0,0);
			//Collision variable
			bool colWithWorld = true;

			for (unsigned int j = 0; j < entities->at(i)->m_components->size(); j++)
			{
				if(entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::ABILITYMODEL)
				{
					m_file << "\ttransformComp" << i << ":SetPos(posVec);\n";
					m_file << "\trenderComp" << i << ":SetModel(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_modelName << "\");\n";
					m_file << "\trenderComp" << i << ":SetMaterial(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_material << "\");\n";
					m_file << "\trenderComp" << i << ":SetMaterialDiffuse(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialDiffuse << "\");\n";
					m_file << "\trenderComp" << i << ":SetMaterialSpecular(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialSpecular << "\");\n";
					m_file << "\trenderComp" << i << ":SetMaterialNormal(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialNormal << "\");\n";
					m_file << "\trenderComp" << i << ":SetMaterialEffect(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialEffect << "\");\n";
				}
				if (entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::COLLISIONSHAPE)
				{
					radius = ((AbilityComponents::CollisionShape*)entities->at(i)->m_components->at(j))->m_radius;
					height = ((AbilityComponents::CollisionShape*)entities->at(i)->m_components->at(j))->m_height;
				}
				if (entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::PHYSICSCONTROLLED)
				{
					speed = ((AbilityComponents::PhysicsControlled*)entities->at(i)->m_components->at(j))->m_speed;
					mass = ((AbilityComponents::PhysicsControlled*)entities->at(i)->m_components->at(j))->m_mass;
				}
				if (entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::TRANSFORM)
				{
					rotation = ((AbilityComponents::Transform*)entities->at(i)->m_components->at(j))->m_rotation;
				}
				if (entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::ABILITYPARTICLE)
				{
					// TODO : Add stuff later
				}
				if (entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::COLLISION)
				{
					colWithWorld = ((AbilityComponents::Collision*)entities->at(i)->m_components->at(j))->m_collidesWithWorld;
				}
			}
			if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::COLLISION))
			{
				m_file << "\tphysicsComp"<< i << ":BindShape(collisionComp" << i << ", Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New("<<rotation.x()<<","<<rotation.y()<<","<<rotation.z()<<",1), "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::PHYSICSCONTROLLED))
					m_file << "\tphysicsComp"<< i << ":SetVelocity(collisionComp" << i << ", Vec3.New(frontVec.x * "<<speed<<", frontVec.y * "<<speed<<", frontVec.z * "<<speed<<"));\n";
				m_file << "\tcolRespComp" << i << ":SetContainer(collisionComp" << i << ");\n";
			}
		}
		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnCollide( OnCollide* p_onCollide )
	{
		m_file << "function " << m_name << ".OnCollide (args)\n";
		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnDestroy( OnDestroy* p_onDestroy )
	{
		m_file << "function " << m_name << ".OnDestroy (args)\n";
		m_file << "end\n";
	}

}

