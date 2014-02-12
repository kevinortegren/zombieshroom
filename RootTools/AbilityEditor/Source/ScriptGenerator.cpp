#include "ScriptGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QtCore/QDir>

namespace AbilityEditorNameSpace
{

	ScriptGenerator::ScriptGenerator()
	{
		m_damage = 0.0f;
		m_knockback = 0.0f;
	}

	ScriptGenerator::~ScriptGenerator()
	{

	}

	void ScriptGenerator::GenerateScript( QString p_filePath, QString p_name, Entity* p_entity, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy )
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
		m_entity = p_entity;

		p_filePath.chop(p_filePath.size() - p_filePath.lastIndexOf("/") - 1);

		m_name = p_name.toStdString();
		m_file.open(p_filePath.toStdString() + m_name + ".lua");

		//Begin writing in the file
		m_file << m_name << " = {};\n";
		
		WriteVariables();
		m_file << "\n";
		WriteOnCreate(p_onCreate);
		m_file << "\n";
		WriteChargeDone();
		m_file << "\n";
		WriteChannelingDone();
		m_file << "\n";
		WriteOnCollide(p_onCreate, p_onCollide);
		m_file << "\n";
		WriteOnDestroy(p_onDestroy);

		m_file.close();

	}

	void ScriptGenerator::WriteVariables()
	{
		for (unsigned int j = 0; j < m_entity->GetComponents()->size(); j++)
		{
			if(m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::DAMAGE)
			{
				m_damage = ((AbilityComponents::Damage*)m_entity->GetComponents()->at(j))->m_damage;
				m_file << m_name << ".damage = " << m_damage << ";\n";
			}
			else if(m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::KNOCKBACK)
			{
				m_knockback = ((AbilityComponents::Knockback*)m_entity->GetComponents()->at(j))->m_knockback;
				m_file << m_name << ".knockback = " << m_knockback << ";\n";
			}
		}
		m_file << m_name << ".cooldown = " << m_entity->GetCooldown() << ";\n";
		m_file << m_name << ".charges = " << m_entity->GetCharges() << ";\n";
		m_file << m_name << ".chargeTime = " << m_entity->GetChargeTime() << ";\n";
		m_file << m_name << ".channelingTime = " << m_entity->GetChannelingTime() << ";\n";
		m_file << m_name << ".duration = " << m_entity->GetDuration() << ";\n";
	}

	void ScriptGenerator::WriteChargeDone()
	{
		float chargeReq = 0.0f, chargeFac = 1.0f;

		m_file << "function " << m_name << ".ChargeDone (time, UserID, ActionID)\n";
		for (unsigned int i = 0; i < m_entity->GetComponents()->size(); i++)
		{
			if(m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::CHARGEVARIABLES)
			{
				chargeReq = ((AbilityComponents::ChargeVariables*)m_entity->GetComponents()->at(i))->m_chargeReq;
				chargeFac = ((AbilityComponents::ChargeVariables*)m_entity->GetComponents()->at(i))->m_chargeFactor;
			}
		}
		if (chargeFac > 1.0f)
		{
			m_file << "\t" << m_name << ".damage = " << m_name << ".damage * ((time * " << chargeFac << ") / " << m_name << ".chargeTime);\n";
			m_file << "\t" << m_name << ".knockback = " << m_name << ".knockback * ((time * " << chargeFac << ") / " << m_name << ".chargeTime);\n";
		}
		if (chargeReq > 0.0f)
		{
			m_file << "\tif time >= " << m_name << ".chargeTime * " << chargeReq << "then\n\t";
		}
		m_file << "\t" << m_name << ".OnCreate(userId, actionId);\n";
		if (chargeReq > 0.0f)
		{
			m_file << "\tend\n";
		}
		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnCreate( OnCreate* p_onCreate)
	{
		m_file << "function " << m_name << ".OnCreate (userId, actionId)\n";

			m_file << "\t--Entities\n";
			m_file << "\tlocal self = Entity.New();\n";
			m_file << "\tlocal casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);\n";
			m_file << "\tlocal networkEnt = Network.New(self, userId, actionId);\n";

			m_file << "\t--Components\n";
			m_file << "\tlocal transformComp = Transformation.New(self);\n";
			m_file << "\tlocal collisionComp = Collision.New(self);\n";
			m_file << "\tlocal colRespComp = CollisionResponder.New(self);\n";
			m_file << "\tlocal physicsComp = Physics.New(self);\n";
			m_file << "\tlocal scriptComp = Script.New(self, \"" << m_name << "\");\n";

			m_file << "\t--Setting stuff\n";
			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::PHYSICS))
				m_file << "\tcollisionComp:CreateHandle(self, 1, false);\n";
			else //The '1' is Type::ABILITY
				m_file << "\tcollisionComp:CreateHandle(self, 1, true);\n";
			m_file << "\tcolRespComp:SetContainer(collisionComp);\n";

			//Some standard values
			//Collision Shape variables
			float radius = 1.0f;
			float height = 1.0f;
			int colShape = 3;
			//Physics variables
			float speed = 1.0f;
			float mass = 1.0f;
			QVector3D grav = QVector3D(0,9.82f,0);

			QVector3D startPos = QVector3D(0,0,0);
			QVector3D targetPos = QVector3D(0,0,0);
			QVector3D velocity = QVector3D(0,0,0);
			//Collision variable
			bool colWithWorld = false;

			for (unsigned int i = 0; i < m_entity->GetComponents()->size(); i++)
			{
				if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::COLLISIONSHAPE)
				{
					radius = ((AbilityComponents::CollisionShape*)m_entity->GetComponents()->at(i))->m_radius;
					height = ((AbilityComponents::CollisionShape*)m_entity->GetComponents()->at(i))->m_height;
					colShape = ((AbilityComponents::CollisionShape*)m_entity->GetComponents()->at(i))->m_CollisionShape;
				}
				else if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::PHYSICS)
				{
					speed = ((AbilityComponents::Physics*)m_entity->GetComponents()->at(i))->m_speed;
					mass = ((AbilityComponents::Physics*)m_entity->GetComponents()->at(i))->m_mass;
					grav = ((AbilityComponents::Physics*)m_entity->GetComponents()->at(i))->m_gravity;
					//colWithWorld
				}
				else if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::STARTPOS)
				{
					//startPos = ((AbilityComponents::StartPos*)m_entity->GetComponents()->at(i))->m_startPos; //This is probably gonna change
				}
				else if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::TARGPOS)
				{
					targetPos = ((AbilityComponents::TargetPos*)m_entity->GetComponents()->at(i))->m_targetPos;
				}
				else if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::VELOCITY)
				{
					velocity = ((AbilityComponents::Velocity*)m_entity->GetComponents()->at(i))->m_velocity;
				}
				//Rotation and stuff will be set by the followPlayer System
			}


			m_file << "\tlocal startPos = Vec3.New(" << startPos.x() << ", " << startPos.y() << ", " << startPos.z() << ");\n"; //Change this soon plz
			m_file << "\tlocal frontVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();\n";

			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::COLLISIONSHAPE))
			{
				if (colShape == AbilityComponents::CollisionShape::CYLINDER)
				{
					m_file << "\tphysicsComp:BindCylinderShape(collisionComp, Vec3.New((startPos.x + frontVec.x * 3), (4 + startPos.y + frontVec.y * 3), (startPos.z + frontVec.z * 3)), Quat.New(0,0,0,1), "<<height<<", "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::CONE)
				{
					m_file << "\tphysicsComp:BindConeShape(collisionComp, Vec3.New((startPos.x + frontVec.x * 3), (4 + startPos.y + frontVec.y * 3), (startPos.z + frontVec.z * 3)), Quat.New(0,0,0,1), "<<height<<", "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::SPHERE)
				{
					m_file << "\tphysicsComp:BindSphereShape(collisionComp, Vec3.New((startPos.x + frontVec.x * 3), (4 + startPos.y + frontVec.y * 3), (startPos.z + frontVec.z * 3)), Quat.New(0,0,0,1), "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::RAY)
				{
					m_file << "\tphysicsComp:BindNoShape(collisionComp:GetHandle(), startPos, Quat.New(0,0,0,1));\n";
					m_file << "\tphysicsComp:ShootRay(collisionComp:GetHandle(), startPos, frontVec, "<< height <<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::MESH)
				{
					m_file << "\tphysicsComp:BindMeshShape(collisionComp, Vec3.New((startPos.x + frontVec.x * 3), (4 + startPos.y + frontVec.y * 3), (startPos.z + frontVec.z * 3)), Quat.New(0,0,0,1), Vec3.New(1,1,1), "/*<-- scale, what do?*/<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
			}

			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::PHYSICS))
			{
				m_file << "\tphysicsComp:SetVelocity(collisionComp, Vec3.New(frontVec.x * " << speed << ", frontVec.y * " << speed << ", frontVec.z * " << speed << ")\n";
				m_file << "\tphysicsComp:SetGravity(collisionComp, Vec3.New(" << grav.x() << ", " << grav.y() << ", " << grav.z() << ")\n";
			}
			
			m_file << "\ttransformComp:SetPos(startPos);\n";

			m_file << "\n";



		//Client only components:
		for (unsigned int j = 0; j < m_entity->GetComponents()->size(); j++)
		{
			// Setting values
			if(m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::ABILITYMODEL)
			{
				m_file << "\t\tlocal renderComp = Renderable.New(self);\n";
				m_file << "\t\trenderComp:SetModel(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_modelName << "\");\n";
				m_file << "\t\trenderComp:SetMaterial(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_material << "\");\n";
				m_file << "\t\trenderComp:SetMaterialDiffuse(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialDiffuse << "\");\n";
				m_file << "\t\trenderComp:SetMaterialSpecular(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialSpecular << "\");\n";
				m_file << "\t\trenderComp:SetMaterialNormal(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialNormal << "\");\n";
				m_file << "\t\trenderComp:SetMaterialEffect(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialEffect << "\");\n";
			}
			else if (m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::ABILITYPARTICLE)
			{
				m_file << "\t\tlocal particleComp = ParticleEmitter.New(self, " << ((AbilityComponents::AbilityParticle*)m_entity->GetComponents()->at(j))->m_particleName << ");\n";
			}
		}

		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnCollide( OnCreate* p_onCreate, OnCollide* p_onCollide)
	{
		m_file << "function " << m_name << ".OnCollide (self, entity)\n";
		
		//Knockback, do damage, change stat

		m_file << "end\n";
	}

	void ScriptGenerator::WriteChannelingDone()
	{
		m_file << "function " << m_name << ".ChannelingDone (time, UserID, ActionID)\n";

		//Remove stuff, or create other stuff

		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnDestroy( OnDestroy* p_onDestroy)
	{
		m_file << "function " << m_name << ".OnDestroy (self)\n";

		//Remove stuff

		m_file << "end\n";
	}

}

