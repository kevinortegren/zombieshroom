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
		
		WriteVariables(m_entity);

		m_file << "\n";

		WriteOnCreate(p_onCreate);
		m_file << "\n";
		WriteOnCollide(p_onCreate, p_onCollide);
		m_file << "\n";
		WriteOnDestroy(p_onDestroy);

		m_file.close();

	}

	void ScriptGenerator::WriteOnCreate( OnCreate* p_onCreate)
	{
		//std::vector<AbilityEntity::Entity*>* entities = p_onCreate->GetEntities();

		m_file << "function " << m_name << ".OnCreate (userId, actionId)\n";
		m_file << "\tlocal self = Entity.New();\n";
		m_file << "\tlocal casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);\n";
		m_file << "\tlocal posVec = casterEnt:GetTransformation():GetPos();\n";
		m_file << "\tlocal frontVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();\n";
		m_file << "\tlocal networkEnt = Network.New(self, userId, actionId);\n";
		m_file << "\n";

		
		if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::COLLISION))
		{
			m_file << "\tlocal collisionComp = Collision.New(self);\n";
			m_file << "\tlocal colRespComp = CollisionResponder.New(self);\n";
			m_file << "\tlocal physicsComp = Physics.New(self);\n";
			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::COLLISIONSHAPE))
			{
				if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::PHYSICSCONTROLLED)) //Vadå bara physControlled?
					m_file << "\tcollisionComp:CreateHandle(self, 1, false);\n";
				else //The '1' is TYPE::ABILITY
					m_file << "\tcollisionComp:CreateHandle(self, 1, true);\n";
			}
			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::EXPLOSIVE))
			{
				//Explosive stuffs
				//m_file << "\tphysicsComp:CheckRadius(hitCol:GetHandle(), Vec3.New(posVec.x, posVec.y, posVec.z), " << m_name << ".pushback);\n";
			}
		}
		if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::TRANSFORM))
			m_file << "\tlocal transformComp = Transformation.New(self);\n";
		//if(entities->at(i)->DoesComponentExist(AbilityComponents::ComponentType::OFFENSIVEABILITY))
		//	m_file << "local "
		m_file << "\tlocal scriptComp = Script.New(self, \"" << m_name << "\");\n";
		
		// Setting values
		
		//Some standard values
		//Collision Shape variables
		float radius = 1;
		float height = 1;
		int colShape = 3;
		//Physics Controlled variables
		float speed = 1;
		float mass = 1;
		QVector3D grav = QVector3D(0,9.82f,0);
		//Transform variables
		QVector3D rotation = QVector3D(0,0,0);
		QVector3D scale = QVector3D(1.0f,1.0f,1.0f);
		//Collision variable
		bool colWithWorld = true;
		//Explosion variable
		float explosion_radius = 1.0f;

		for (unsigned int j = 0; j < m_entity->GetComponents()->size(); j++)
		{
			if (m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::COLLISIONSHAPE)
			{
				radius = ((AbilityComponents::CollisionShape*)m_entity->GetComponents()->at(j))->m_radius;
				height = ((AbilityComponents::CollisionShape*)m_entity->GetComponents()->at(j))->m_height;
				colShape = ((AbilityComponents::CollisionShape*)m_entity->GetComponents()->at(j))->m_CollisionShape;
			}
			else if (m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::PHYSICSCONTROLLED)
			{
				speed = ((AbilityComponents::PhysicsControlled*)m_entity->GetComponents()->at(j))->m_speed;
				mass = ((AbilityComponents::PhysicsControlled*)m_entity->GetComponents()->at(j))->m_mass;
				grav = ((AbilityComponents::PhysicsControlled*)m_entity->GetComponents()->at(j))->m_gravity;
			}
			else if (m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::TRANSFORM)
			{
				rotation = ((AbilityComponents::Transform*)m_entity->GetComponents()->at(j))->m_rotation;
				scale = ((AbilityComponents::Transform*)m_entity->GetComponents()->at(j))->m_scale;
				m_file << "\ttransformComp:SetPos(posVec);\n";
			}
			else if (m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::COLLISION)
			{
				colWithWorld = ((AbilityComponents::Collision*)m_entity->GetComponents()->at(j))->m_collidesWithWorld;
			}
			else if (m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::EXPLOSIVE)
			{
				explosion_radius = ((AbilityComponents::Explosive*)m_entity->GetComponents()->at(j))->m_radius;
			}
		}
		if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::COLLISION))
		{
			m_file << "\tcolRespComp:SetContainer(collisionComp);\n";
			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::COLLISIONSHAPE))
			{
				if (colShape == AbilityComponents::CollisionShape::CYLINDER)
				{
					m_file << "\tphysicsComp:BindCylinderShape(collisionComp, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New("<<rotation.x()<<","<<rotation.y()<<","<<rotation.z()<<",1), "<<height<<", "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::CONE)
				{
					m_file << "\tphysicsComp:BindConeShape(collisionComp, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New("<<rotation.x()<<","<<rotation.y()<<","<<rotation.z()<<",1), "<<height<<", "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::SPHERE)
				{
					m_file << "\tphysicsComp:BindSphereShape(collisionComp, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New("<<rotation.x()<<","<<rotation.y()<<","<<rotation.z()<<",1), "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::RAY)
				{
					m_file << "\tphysicsComp:ShootRay(collisionComp:GetHandle(), Vec3.New(posVec.x, posVec.y, posVec.z), Vec3.New(frontVec.x, frontVec.y, frontVec.z), "<<height<<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::MESH)
				{
					m_file << "\tphysicsComp:BindMeshShape(collisionComp, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New("<<rotation.x()<<","<<rotation.y()<<","<<rotation.z()<<",1), Vec3.New(" << scale.x() << ", " << scale.y() << ", " << scale.z() << "), "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
			}
			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::EXPLOSIVE))
			{
				m_file << "\tphysicsComp:CheckRadius(collisionComp:GetHandle(), Vec3.New(posVec.x, posVec.y, posVec.z), " << explosion_radius << ");\n";
			}
			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::PHYSICSCONTROLLED))
			{
				m_file << "\tphysicsComp:SetVelocity(collisionComp, Vec3.New(frontVec.x * "<<speed<<", frontVec.y * "<<speed<<", frontVec.z * "<<speed<<"));\n";
				m_file << "\tphysicsComp:SetGravity(collisionComp, Vec3.New(" << grav.x() << ", " << grav.y() << ", " << grav.z() << "));\n";
			}
			
		}
		
		m_file << "\tif Global.IsClient then\n"; //Client-only components

		if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::ABILITYMODEL))
			m_file << "\t\tlocal renderComp = Renderable.New(self);\n";
		for (unsigned int j = 0; j < m_entity->GetComponents()->size(); j++)
		{
			// Setting values
			if(m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::ABILITYMODEL)
			{
				
				m_file << "\t\trenderComp:SetModel(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_modelName << "\");\n";
				m_file << "\t\trenderComp:SetMaterial(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_material << "\");\n";
				m_file << "\t\trenderComp:SetMaterialDiffuse(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialDiffuse << "\");\n";
				m_file << "\t\trenderComp:SetMaterialSpecular(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialSpecular << "\");\n";
				m_file << "\t\trenderComp:SetMaterialNormal(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialNormal << "\");\n";
				m_file << "\t\trenderComp:SetMaterialEffect(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialEffect << "\");\n";
			}
			if (m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::ABILITYPARTICLE)
			{
				m_file << "\t\tlocal particleComp = ParticleEmitter.New(self, " << ((AbilityComponents::AbilityParticle*)m_entity->GetComponents()->at(j))->m_particleName << ");\n";
			}
		}

		m_file << "\tend\n";

		//Set cooldown if any
		if (m_entity->GetCooldown() > 0)
		{
			m_file << "\tlocal playerComponent = playerEnt:GetPlayerComponent();\n";
			m_file << "\tplayerComponent:StartCooldown(playerComponent:GetSelectedAbility(), " << m_name << ".cooldown);\n";
		}
		//Create new abilities
		for (unsigned int i = 0; i < p_onCreate->GetConditions()->size(); i++)
		{
			m_file << "\t" << p_onCreate->GetConditions()->at(i)->GetCode().toStdString() << "\n";
			std::vector<QString> scriptNames = p_onCreate->GetEntityNames(i);
			for (unsigned int j = 0; j < scriptNames.size(); j++)
			{
				scriptNames.at(j).chop(4);
				m_file << "\t\t" << scriptNames.at(j).toStdString() << ".OnCreate(networkEnt:GetUserId(), networkEnt:GetActionId());\n";
			}
			m_file << "\tend\n";
		}
		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnCollide( OnCreate* p_onCreate, OnCollide* p_onCollide)
	{

		m_file << "function " << m_name << ".OnCollide (self, entity)\n";
		for (unsigned int j = 0; j < m_entity->GetComponents()->size(); j++)
		{
			if(m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::OFFENSIVEABILITY) //TODO Fix here
			{

				m_file << "\tlocal hitCol = entity:GetCollision();\n";
				m_file << "\tlocal hitPhys = entity:GetPhysics();\n";
				m_file << "\tlocal type = hitPhys:GetType(hitCol);\n";
				m_file << "\tlocal network = self:GetNetwork();\n";

				m_file << "\tlocal targetPlayerComponent = entity:GetPlayerComponent();\n";
				m_file << "\tlocal abilityOwnerNetwork = self:GetNetwork();\n";
				m_file << "\tlocal abilityOwnerId = abilityOwnerNetwork:GetUserId();\n";
				m_file << "\tlocal abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);\n";
				m_file << "\tlocal abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();\n";
				if(m_damage >= 0) // TODO Maybe change this to not hard coded
					m_file << "\tif type == PhysicsType.TYPE_PLAYER and abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then\n"; 
				else //If negative damage (healing) only heal friendly players
					m_file << "\tif type == PhysicsType.TYPE_PLAYER and abilityOwnerPlayerComponent:GetTeamId() == targetPlayerComponent:GetTeamId() then\n"; 
				if(m_knockback > 0.0f)
				{
							   
					m_file << "\t\tlocal hitPos = entity:GetTransformation():GetPos();\n";
					m_file << "\t\tlocal selfPos = self:GetTransformation():GetPos();\n";
					//The '2' in the following line is SuperMegaDunderUberHyper guessed, maybe think about it?
					m_file << "\t\thitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), " << m_name << ".pushback);\n";
					
				}
				if(m_damage > 0.0f)
				{
					m_file << "\t\tlocal health = entity:GetHealth();\n";
					m_file << "\t\tif not health:IsDead() then\n";
						
						m_file << "\t\t\tlocal receiverId = network:GetUserId();\n";
						m_file << "\t\t\thealth:Damage(abilityOwnerId, " << m_name << ".damage, receiverId);\n";
					m_file << "\t\tend\n";
				}
				m_file << "\tend\n";
			}
			
		}
		//Create new abilities
		for (unsigned int i = 0; i < p_onCollide->GetConditions()->size(); i++)
		{
			m_file << "\t" << p_onCollide->GetConditions()->at(i)->GetCode().toStdString() << "\n";
			std::vector<QString> scriptNames = p_onCollide->GetEntityNames(i);
			for (unsigned int j = 0; j < scriptNames.size(); j++)
			{
				scriptNames.at(j).chop(4); 
				m_file << "\t\trequire(" << scriptNames.at(j).toStdString() << ");\n";
				m_file << "\t\t" << scriptNames.at(j).toStdString() << ".OnCreate(network:GetUserId(), network:GetActionId());\n";
			}
			m_file << "\tend\n";
		}
		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnDestroy( OnDestroy* p_onDestroy)
	{
		m_file << "function " << m_name << ".OnDestroy (self)\n";

		m_file << "\tlocal network = self:GetNetwork();\n";
		//Create new abilities
		for (unsigned int i = 0; i < p_onDestroy->GetConditions()->size(); i++)
		{
			m_file << "\t" << p_onDestroy->GetConditions()->at(i)->GetCode().toStdString() << "\n";
			std::vector<QString> scriptNames = p_onDestroy->GetEntityNames(i);
			for (unsigned int j = 0; j < scriptNames.size(); j++)
			{
				scriptNames.at(j).chop(4); 
				m_file << "\t\t" << scriptNames.at(j).toStdString() << ".OnCreate(network:GetUserId(), network:GetActionId());\n";
			}
			m_file << "\tend\n";
		}
		m_file << "\tlocal collision = self:GetCollision();\n";
		m_file << "\tCollision.RemoveObjectFromWorld(collision);\n"; //Should this be here?
		m_file << "end\n";
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
				m_file << m_name << ".pushback = " << m_knockback << ";\n";
			}
		}
		m_file << m_name << ".cooldown = " << m_entity->GetCooldown() << ";\n";
		m_file << m_name << ".charges = " << m_entity->GetCharges() << ";\n";
		m_file << m_name << ".chargeTime = " << m_entity->GetChargeTime() << ";\n";
		m_file << m_name << ".channelingTime = " << m_entity->GetChannelingTime() << ";\n";
		m_file << m_name << ".duration = " << m_entity->GetDuration() << ";\n";
	}

}

