#include <RootTools/Treenity/Include/ProjectManager.h>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

ProjectManager::ProjectManager(ECS::World* p_world)
	: m_world(p_world)
{
	
}

ProjectManager::~ProjectManager()
{

}

const QString& ProjectManager::GetCurrentProjectName() const
{

}

const QString& ProjectManager::GetEntityName(ECS::Entity* p_entity) const
{
	auto it = m_entityNames.find(p_entity);
	if (it != m_entityNames.end())
		return QString(it->second.c_str());
	else
		return "";
}


void ProjectManager::SetEntityName(ECS::Entity* p_entity, const QString& p_name)
{
	auto it = m_entityNames.find(p_entity);
	if (it != m_entityNames.end())
	{
		it->second = p_name.toStdString();
	}
	else
	{
		assert(false);
	}
}


void ProjectManager::Export(const QString& p_name)
{
	m_world->GetEntityExporter()->Export(p_name.toStdString(), &m_entityNames);
}

void ProjectManager::Import(const QString& p_name)
{

}