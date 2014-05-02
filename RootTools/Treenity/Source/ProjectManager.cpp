#include <RootTools/Treenity/Include/ProjectManager.h>
#include <RootTools/Treenity/Include/Log.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QFileInfo>

extern RootEngine::GameSharedContext g_engineContext;

ProjectManager::ProjectManager(ECS::World* p_world)
	: m_world(p_world)
	, m_projectName("Untitled")
{
}

ProjectManager::~ProjectManager()
{

}

const QString& ProjectManager::GetCurrentProjectName() const
{
	return m_projectName;
}

QString ProjectManager::GetEntityName(ECS::Entity* p_entity) const
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

void ProjectManager::EntityAdded(ECS::Entity* p_entity)
{
	if (m_entityNames.find(p_entity) == m_entityNames.end())
	{
		m_entityNames[p_entity] = "Unnamed";
	}
}

void ProjectManager::EntityRemoved(ECS::Entity* p_entity)
{
	m_entityNames.erase(p_entity);
}


void ProjectManager::Export(const QString& p_name)
{
	if (p_name == "") return;

	m_world->GetEntityExporter()->Export(p_name.toStdString(), &m_entityNames);

	QFileInfo info(p_name);
	m_projectName = info.baseName();

	Log::Write("Project saved as " + info.fileName());
	g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Project saved as %s", p_name.toStdString().c_str());
}

void ProjectManager::Import(const QString& p_name)
{
	if (p_name == "") return;

	m_world->GetEntityImporter()->Import(p_name.toStdString(), &m_entityNames);

	QFileInfo info(p_name);
	m_projectName = info.baseName();

	Log::Write("Project loaded from " + info.fileName());
	g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Project loaded from %s", p_name.toStdString().c_str());
}