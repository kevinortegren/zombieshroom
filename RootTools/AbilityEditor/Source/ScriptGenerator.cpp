#include "ScriptGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QtCore/QDir>

namespace AbilityEditorNameSpace
{

	ScriptGenerator::ScriptGenerator()
	{

	}

	ScriptGenerator::~ScriptGenerator()
	{

	}

	void ScriptGenerator::GenerateScript( QString p_filePath, QString p_name, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy )
	{
		//p_scriptName.chop(4);
		//m_name = p_folderName.toStdString();

		/* THIS IS IF WE WANT TO PUT SCRIPTS IN A FOLDER
		p_filePath.chop(p_filePath.size() - p_filePath.lastIndexOf("/") - 1);
		QDir thedir;
		thedir.setCurrent(p_filePath);
		//QDir().setCurrent(p_filePath);
		if(!thedir.exists(p_name))
		{
			thedir.mkdir(p_name);
		}*/

		p_filePath.chop(p_filePath.size() - p_filePath.lastIndexOf("/") - 1);

		for (int i = 0; (unsigned int)i < p_onCreate->GetEntities()->size(); i++)
		{
			//Folder only
			//m_name = p_onCreate->GetEntities()->at(i)->m_name.toStdString();
			//m_file.open(p_filePath.toStdString() + p_name.toStdString() + "/" + m_name + ".lua");
			
			//No folder
			m_name = p_onCreate->GetEntities()->at(i)->m_name.toStdString();
			m_file.open(p_filePath.toStdString() + m_name + ".lua");

			//Begin writing in the file
			m_file << m_name << " = {};\n";
			m_file << "\n";

			WriteOnCreate(p_onCreate, i);
			m_file << "\n";
			WriteOnCollide(p_onCreate, p_onCollide, i);
			m_file << "\n";
			WriteOnDestroy(p_onDestroy, i);

			m_file.close();
		}

		for (int i = 0; (unsigned int)i < p_onCollide->GetEntities()->size(); i++)
		{
			m_name = p_onCollide->GetEntities()->at(i)->m_name.toStdString();
			m_file.open(p_filePath.toStdString() + p_name.toStdString() + "/" + m_name + ".lua");

			//Begin writing in the file
			m_file << m_name << " = {};\n";
			m_file << "\n";

			WriteOnCreate(p_onCreate, i);
			m_file << "\n";
			WriteOnCollide(p_onCreate, p_onCollide, i);
			m_file << "\n";
			WriteOnDestroy(p_onDestroy, i);

			m_file.close();
		}

		for (int i = 0; (unsigned int)i < p_onDestroy->GetEntities()->size(); i++)
		{
			m_name = p_onDestroy->GetEntities()->at(i)->m_name.toStdString();
			m_file.open(p_filePath.toStdString() + p_name.toStdString() + "/" + m_name + ".lua");

			//Begin writing in the file
			m_file << m_name << " = {};\n";
			m_file << "\n";

			WriteOnCreate(p_onCreate, i);
			m_file << "\n";
			WriteOnCollide(p_onCreate, p_onCollide, i);
			m_file << "\n";
			WriteOnDestroy(p_onDestroy, i);

			m_file.close();
		}
	}

	void ScriptGenerator::WriteOnCreate( OnCreate* p_onCreate, int i)
	{
		std::vector<AbilityEntity::Entity*>* entities = p_onCreate->GetEntities();

		m_file << "function " << m_name << ".OnCreate (userId, actionId)\n";
		m_file << "\tlocal self = Entity.New();\n";
		m_file << "\tlocal playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);\n";
		m_file << "\tlocal posVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetTransformation():GetPos();\n";
		m_file << "\tlocal frontVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();\n";
		m_file << "\tlocal networkEnt = Network.New(self, userId, actionId);\n";
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
					m_file << "\tcollisionComp:CreateHandle(self, 1, false);\n";
				else
					m_file << "\tcollisionComp:CreateHandle(self, 1, true);\n";
			}
			if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::EXPLOSIVE))
			{
				// call physics explosion
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
		QVector3D grav = QVector3D(0,9.82f,0);
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
				grav = ((AbilityComponents::PhysicsControlled*)entities->at(i)->m_components->at(j))->m_gravity;
			}
			if (entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::TRANSFORM)
			{
				rotation = ((AbilityComponents::Transform*)entities->at(i)->m_components->at(j))->m_rotation;
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
			{
				m_file << "\tphysicsComp:SetVelocity(collisionComp, Vec3.New(frontVec.x * "<<speed<<", frontVec.y * "<<speed<<", frontVec.z * "<<speed<<"));\n";
				m_file << "\tphysicsComp:SetGravity(collisionComp, Vec3.New(" << grav.x() << ", " << grav.y() << ", " << grav.z() << "));\n";
			}
			m_file << "\tcolRespComp:SetContainer(collisionComp);\n";
		}
		m_file << "\ttransformComp:SetPos(posVec);\n";
		m_file << "\tif Global.IsClient then\n";

		if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::ABILITYMODEL))
			m_file << "\t\tlocal renderComp = Renderable.New(self);\n";
		for (unsigned int j = 0; j < entities->at(i)->m_components->size(); j++)
		{
			// Setting values
			if(entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::ABILITYMODEL)
			{
				
				m_file << "\t\trenderComp:SetModel(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_modelName << "\");\n";
				m_file << "\t\trenderComp:SetMaterial(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_material << "\");\n";
				m_file << "\t\trenderComp:SetMaterialDiffuse(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialDiffuse << "\");\n";
				m_file << "\t\trenderComp:SetMaterialSpecular(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialSpecular << "\");\n";
				m_file << "\t\trenderComp:SetMaterialNormal(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialNormal << "\");\n";
				m_file << "\t\trenderComp:SetMaterialEffect(\"" << ((AbilityComponents::AbilityModel*)entities->at(i)->m_components->at(j))->m_materialEffect << "\");\n";
			}
			if (entities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::ABILITYPARTICLE)
			{
				// TODO : Add stuff later
			}
		}

		m_file << "\tend\n";

		std::vector<QString> scriptNames = p_onCreate->GetScriptNames();
		for (unsigned int j = 0; j < scriptNames.size(); j++)
		{
			scriptNames.at(j).chop(4);
			m_file << "\t" << scriptNames.at(j).toStdString() << ".OnCreate();\n";
		}
		
		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnCollide( OnCreate* p_onCreate, OnCollide* p_onCollide, int i )
	{
		std::vector<AbilityEntity::Entity*>* OCEntities = p_onCreate->GetEntities();

		m_file << "function " << m_name << ".OnCollide (self, entity)\n";
		for (unsigned int j = 0; j < OCEntities->at(i)->m_components->size(); j++)
		{
			if(OCEntities->at(i)->m_components->at(j)->m_type == AbilityComponents::ComponentType::OFFENSIVEABILITY)
			{
				m_file << "\tlocal hitCol = entity:GetCollision();\n";
				m_file << "\tlocal hitPhys = entity:GetPhysics();\n";
				m_file << "\tlocal type = hitPhys:GetType(hitCol);\n";
				m_file << "\tif type == 3 then\n";
				m_file << "\t\tlocal hitPos = entity:GetTransformation():GetPos();\n";
				m_file << "\t\tlocal selfPos = self:GetTransformation():GetPos();\n";
				//The 2 in the following line is SuperMegaDunderUberHyper guessed, maybe think about it?
				m_file << "\t\thitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), " << ((AbilityComponents::OffensiveAbility*)OCEntities->at(i)->m_components->at(j))->m_knockbackPower << ");\n";
				m_file << "\tend\n";
			}
			
		}

		std::vector<QString> scriptNames = p_onCollide->GetScriptNames();
		for (unsigned int j = 0; j < scriptNames.size(); j++)
		{
			scriptNames.at(j).chop(4);
			m_file << "\t" << scriptNames.at(j).toStdString() << ".OnCreate();\n";
		}
		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnDestroy( OnDestroy* p_onDestroy, int i )
	{
		m_file << "function " << m_name << ".OnDestroy (self)\n";
		std::vector<QString> scriptNames = p_onDestroy->GetScriptNames();
		for (unsigned int j = 0; j < scriptNames.size(); j++)
		{
			scriptNames.at(j).chop(4);
			m_file << "\t" << scriptNames.at(j).toStdString() << ".OnCreate();\n";
		}
		m_file << "end\n";
	}
}

