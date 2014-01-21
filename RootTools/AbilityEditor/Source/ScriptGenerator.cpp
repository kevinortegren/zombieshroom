#include "ScriptGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
		
		for (int i = 0; (unsigned int)i < p_onCreate->GetEntities()->size(); i++)
		{
			std::stringstream streamustotalus;
			streamustotalus << i;

			QString test = streamustotalus.str().c_str();
			/*std::string test2 = streamustotalus.str().c_str();
			test = test2.c_str();*/
			test.append(".lua");
			
			if(i == 0)
			{
				p_filePath.chop(4);
				p_filePath.append(test);
			}
			else if(i == 10)
			{
				p_filePath.chop(5);
				p_filePath.append(test);
			}
			else
			{
				p_filePath.chop(5 + floor(i/10));
				p_filePath.append(test);
			}
			m_file.open(p_filePath.toStdString());// + ".lua");
			
			//Begin writing in the file
			m_file << m_name << " = {};\n";
			m_file << "\n";

			WriteOnCreate(p_onCreate, i);
			m_file << "\n";
			WriteAddClientComponents(p_onCreate, i);
			m_file << "\n";
			WriteOnCollide(p_onCollide, i);
			m_file << "\n";
			WriteOnDestroy(p_onDestroy, i);

			m_file.close();
		}
	}

	void ScriptGenerator::WriteOnCreate( OnCreate* p_onCreate, int i)
	{
		std::vector<AbilityEntity::Entity*>* entities = p_onCreate->GetEntities();

		m_file << "function " << m_name << ".OnCreate (self, userId, actionId)\n";
		m_file << "\tlocal playerEnt = Entity.GetEntityByTag(\"Player\");\n";
		m_file << "\tlocal posVec = Entity.GetEntityByTag(\"Player\"):GetTransformation():GetPos();\n";
		m_file << "\tlocal frontVec = Entity.GetEntityByTag(\"AimingDevice\"):GetTransformation():GetOrient():GetFront();\n";
		m_file << "\n";
		m_entityOffset = entities->size();
		
		if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::COLLISION))
		{
			m_file << "\tlocal collisionComp = Collision.New(self);\n";
			m_file << "\tlocal colRespComp = CollisionResponder.New(self);\n";
			if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::COLLISIONSHAPE))
			{
				m_file << "\tlocal physicsComp = Physics.New(self);\n";
				if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::PHYSICSCONTROLLED))
					m_file << "\tcollisionComp:CreateHandle(self:GetId(), 1, false);\n";
				else
					m_file << "\tcollisionComp:CreateHandle(self:GetId(), 1, true);\n";
			}
		}
		if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::TRANSFORM))
			m_file << "\tlocal transformComp = Transformation.New(self);\n";
		//if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::OFFENSIVEABILITY))
		//	m_file << "local "
		m_file << "\tlocal scriptComp = Script.New(self, \"" << m_name << "\");\n";
		
		// Setting values
		
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
			m_file << "\tphysicsComp:BindShape(collisionComp, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New("<<rotation.x()<<","<<rotation.y()<<","<<rotation.z()<<",1), "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
			if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::PHYSICSCONTROLLED))
				m_file << "\tphysicsComp:SetVelocity(collisionComp, Vec3.New(frontVec.x * "<<speed<<", frontVec.y * "<<speed<<", frontVec.z * "<<speed<<"));\n";
			m_file << "\tcolRespComp:SetContainer(collisionComp);\n";
		}
		
		m_file << "end\n";
	}

	void ScriptGenerator::WriteAddClientComponents( OnCreate* p_onCreate, int i )
	{
		m_file << "function " << m_name << ".AddClientComponents (self)\n";
		m_file << "\n";
		std::vector<AbilityEntity::Entity*>* entities = p_onCreate->GetEntities();
		
			if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::ABILITYMODEL))
				m_file << "\tlocal renderComp = Renderable.New(self);\n";
		for (unsigned int j = 0; j < entities->at(i)->m_components->size(); j++)
		{
			// Setting values
			if(entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::ABILITYMODEL)
			{
				m_file << "\ttransformComp:SetPos(posVec);\n";
				m_file << "\trenderComp:SetModel(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_modelName << "\");\n";
				m_file << "\trenderComp:SetMaterial(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_material << "\");\n";
				m_file << "\trenderComp:SetMaterialDiffuse(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialDiffuse << "\");\n";
				m_file << "\trenderComp:SetMaterialSpecular(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialSpecular << "\");\n";
				m_file << "\trenderComp:SetMaterialNormal(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialNormal << "\");\n";
				m_file << "\trenderComp:SetMaterialEffect(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialEffect << "\");\n";
			}
			if (entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::ABILITYPARTICLE)
			{
				// TODO : Add stuff later
			}
		}
		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnCollide( OnCollide* p_onCollide, int i )
	{
		m_file << "function " << m_name << ".OnCollide (self, entity)\n";

		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnDestroy( OnDestroy* p_onDestroy, int i )
	{
		m_file << "function " << m_name << ".OnDestroy (args)\n";
		m_file << "end\n";
	}
}

