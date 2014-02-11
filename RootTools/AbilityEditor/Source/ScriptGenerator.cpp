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
				m_file << m_name << ".pushback = " << m_knockback << ";\n";
			}
		}
		m_file << m_name << ".cooldown = " << m_entity->GetCooldown() << ";\n";
		m_file << m_name << ".charges = " << m_entity->GetCharges() << ";\n";
		m_file << m_name << ".chargeTime = " << m_entity->GetChargeTime() << ";\n";
		m_file << m_name << ".channelingTime = " << m_entity->GetChannelingTime() << ";\n";
		m_file << m_name << ".duration = " << m_entity->GetDuration() << ";\n";
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

		

		m_file << "end\n";
	}

	void ScriptGenerator::WriteChargeDone()
	{

	}

	void ScriptGenerator::WriteOnCollide( OnCreate* p_onCreate, OnCollide* p_onCollide)
	{

		m_file << "function " << m_name << ".OnCollide (self, entity)\n";
		

		m_file << "end\n";
	}

	void ScriptGenerator::WriteChannelingDone()
	{

	}

	void ScriptGenerator::WriteOnDestroy( OnDestroy* p_onDestroy)
	{
		m_file << "function " << m_name << ".OnDestroy (self)\n";


		m_file << "end\n";
	}

}

